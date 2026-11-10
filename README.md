# CONCURRENCY LAB: MULTI-THREADED DOWNLOAD MANAGER & RACE CONDITION DEMONSTRATOR

Đồ án Lập trình Hệ thống (Linux Systems Programming): **"Ứng dụng Mô hình Producer–Consumer trong Tối ưu hóa Tốc độ Tải xuống: Xây dựng Trình Quản lý Tải tệp Đa luồng (Multi-threaded Download Manager) trên Desktop bằng C, Phân tích Race Condition và Đồng bộ hóa bằng Mutex/Semaphore"**.

- **Tài liệu hướng dẫn chi tiết & Kịch bản bảo vệ đồ án:** Xem [HUONG_DAN_CHAY.md](HUONG_DAN_CHAY.md)
- **Tài liệu hướng dẫn cập nhật báo cáo nghiệm thu:** Xem [REPORT_UPDATE_GUIDE.md](REPORT_UPDATE_GUIDE.md)

---

## 1. TỔNG QUAN KIẾN TRÚC & PHÂN HỆ

Ứng dụng được viết bằng ngôn ngữ **C nguyên bản (C11 / GNU11)**, chia làm 3 phân hệ độc lập:

1. **Phân hệ 1: Network Engine & Producer Pool (`network_producer.c`)**:
   - Chia nhỏ dữ liệu theo tiêu chuẩn HTTP Range RFC (64 KB / chunk).
   - Thread pool Producer (1 đến 8 luồng) tải dải byte song song bằng `libcurl`.
   - Tích hợp **In-process Local HTTP Range Server** trên loopback socket `127.0.0.1` và bộ sinh dữ liệu nhị phân mẫu (16MB / 32MB) cho phép kiểm thử ngoại tuyến 100% không phụ thuộc Internet.

2. **Phân hệ 2: Storage Consumer & Trọng tâm Race Condition (`storage_consumer.c`)**:
   - Bounded Buffer (Hàng đợi vòng) 32 slots trên RAM được quản lý bằng `sem_t empty_slots`, `sem_t full_slots`, `pthread_mutex_t buffer_mutex`.
   - Lưu trữ song song dùng chung 1 File Descriptor `output_fd`.
   - **Chế độ Synchronized**: Khóa `file_io_mutex` tuần tự hóa cặp thao tác `lseek` và `write`, đảm bảo 0 byte sai lệch, SHA-256 khớp 100%.
   - **Chế độ Unsynchronized (Minh họa Race Condition)**: Bỏ qua `file_io_mutex`, chèn độ trễ vi mô `usleep(2500)` giữa `lseek` và `write`. Khi luồng khác dịch chuyển con trỏ file, va chạm được phát hiện bằng `lseek(fd, 0, SEEK_CUR)`, tăng bộ đếm `race_corrupted_blocks`, ghi hỏng file và làm lệch mã băm SHA-256.

3. **Phân hệ 3: Native Desktop Workstation GUI (`gui_dashboard.c`, `main.c`)**:
   - Cửa sổ Native kích thước `1240x780` dựa trên **GTK+ 3** và **Cairo 2D Vector Canvas**.
   - Bảng màu **Matte Carbon & Oxide** kỹ thuật chuyên nghiệp (chuẩn Anti-AI: cấm emoji, cấm viền phát sáng, cấm gradient lòe loẹt, chỉ dùng font tabular monospace cho số liệu).
   - Hai khung vẽ Cairo thời gian thực:
     + **RAM Bounded Buffer Inspector**: 32 ô nhớ trực quan kèm mũi tên con trỏ `HEAD (IN)` và `TAIL (OUT)`.
     + **Disk Block Allocation Matrix**: 256 khối dữ liệu tô màu theo trạng thái (Chưa tải, Đang tải, Trong RAM, Đã ghi đĩa, Hỏng do Race Condition).

---

## 2. HƯỚNG DẪN CÀI ĐẶT THƯ VIỆN & BIÊN DỊCH

### Bước 1: Cài đặt gói phát triển GTK+ 3 và OpenSSL
Chạy lệnh sau trên terminal Linux (Ubuntu/Debian):
```bash
sudo apt update
sudo apt install -y build-essential libgtk-3-dev libcurl4-openssl-dev libssl-dev pkg-config
```

### Bước 2: Biên dịch ứng dụng

1. **Biên dịch bộ kiểm thử hiệu năng và đối chứng Race Condition (Headless CLI):**
   ```bash
   make benchmark
   ```

2. **Chạy kiểm thử thực nghiệm đối chứng tự động (Sync vs Race):**
   ```bash
   make test
   ```

3. **Biên dịch ứng dụng Desktop Giao diện Native:**
   ```bash
   make gui
   ```

4. **Chạy ứng dụng Desktop:**
   ```bash
   ./bin/sysp_downloader
   ```

---

## 3. THIẾT KẾ ĐỐI CHỨNG THỰC NGHIỆM

| Tiêu chí | Chế độ SYNCHRONIZED | Chế độ UNSYNCHRONIZED (Race Condition) |
| :--- | :--- | :--- |
| **Bảo vệ RAM Bounded Buffer** | `sem_wait(&empty/full)` + `pthread_mutex_lock` | Bỏ qua khóa, `usleep(1500)` ghi đè ô nhớ |
| **Bảo vệ File Descriptor** | `file_io_mutex` bọc `lseek` + `write` | Bỏ qua khóa, `usleep(2500)` giữa `lseek` và `write` |
| **Va chạm con trỏ File (`lseek`)** | **0 khối** | **90 ~ 180 khối va chạm** (Phát hiện tức thời) |
| **Kiểm định mã băm SHA-256** | **STATUS: VERIFIED** (Khớp 100% byte-to-byte) | **STATUS: CORRUPTED - DATA RACE DETECTED** |
| **Trực quan hóa trên UI Cairo** | Toàn bộ 256 ô chuyển màu xanh lá `#4E9A67` | Hàng chục ô chuyển đỏ gạch `#C94A4A` kèm dấu `X` |

