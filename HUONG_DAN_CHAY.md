# HƯỚNG DẪN CÀI ĐẶT VÀ KHỞI CHẠY DỰ ÁN (USER MANUAL & SETUP GUIDE)

## Đề tài: Trình Quản lý Tải tệp Đa luồng & Phòng Thí nghiệm Đồng bộ hóa (Multi-threaded Download Manager & Concurrency Lab)
**Môn học:** Lập trình Hệ thống (System Programming)  
**Ngôn ngữ & Thư viện:** C (GNU11), POSIX Threads, POSIX Semaphores, Mutexes, `libcurl`, OpenSSL EVP SHA-256, GTK+ 3, Cairo 2D Vector Engine.

---

## 1. YÊU CẦU HỆ THỐNG (SYSTEM PREREQUISITES)

* **Hệ điều hành:** Linux (Khuyến nghị **Ubuntu 20.04 / 22.04 / 24.04 LTS**, Debian 11/12, Fedora 38+, Arch Linux).
* **Kiến trúc CPU:** x86_64 hoặc ARM64.
* **Môi trường đồ họa:** X11 hoặc Wayland (để hiển thị cửa sổ Native GTK3).
* **Trình biên dịch:** `gcc` (hỗ trợ chuẩn C11 / GNU11) và công cụ `make`.

---

## 2. CÀI ĐẶT CÁC GÓI THƯ VIỆN PHỤ THUỘC (DEPENDENCIES)

Trước khi biên dịch, hãy mở Terminal trên Linux và chạy **một dòng lệnh duy nhất** tương ứng với hệ điều hành của bạn:

### Trên Ubuntu / Debian / Linux Mint:
```bash
sudo apt update && sudo apt install -y build-essential libgtk-3-dev libcurl4-openssl-dev libssl-dev pkg-config
```

### Trên Fedora / RHEL:
```bash
sudo dnf install -y gcc make gtk3-devel libcurl-devel openssl-devel pkgconf-pkg-config
```

### Trên Arch Linux / Manjaro:
```bash
sudo pacman -Syu --noconfirm base-devel gtk3 curl openssl pkgconf
```

---

## 3. TẢI VÀ BIÊN DỊCH DỰ ÁN (BUILD INSTRUCTIONS)

### Bước 1: Sao chép mã nguồn về máy
```bash
git clone https://github.com/IT31072006/BaiTapLonV2.git
cd BaiTapLonV2
```

### Bước 2: Biên dịch các thành phần

Dự án cung cấp `Makefile` tự động hóa toàn bộ quá trình liên kết thư viện:

| Lệnh biên dịch | Mục tiêu | Kết quả đầu ra |
| :--- | :--- | :--- |
| **`make gui`** | Biên dịch ứng dụng Desktop Giao diện Native hoàn chỉnh | `bin/sysp_downloader` |
| **`make benchmark`** | Biên dịch công cụ dòng lệnh (Headless CLI) kiểm thử thực nghiệm | `bin/cli_benchmark` |
| **`make test`** | Tự động chạy đối chứng cả 2 chế độ Synchronized và Race Condition | Báo cáo trực tiếp trên Terminal |
| **`make clean`** | Xóa sạch các file tạm `*.o`, binaries và file dữ liệu mẫu | Trả lại mã nguồn ban đầu |

> **Khuyến nghị:** Chạy lệnh sau để dọn dẹp và biên dịch mới toàn bộ:
> ```bash
> make clean && make gui && make benchmark
> ```

---

## 4. HƯỚNG DẪN KHỞI CHẠY GIAO DIỆN DESKTOP (`./bin/sysp_downloader`)

Khởi động ứng dụng Desktop:
```bash
./bin/sysp_downloader
```
Cửa sổ Native kích thước **1240 x 780** chuẩn công nghiệp (Anti-AI Matte Carbon & Oxide) sẽ xuất hiện trên màn hình.

### Các bước Demo trực quan trên giao diện:

#### Bước A: Khởi tạo dữ liệu kiểm thử (100% Offline)
* Bấm vào nút **`[GEN 16MB]`** (hoặc `[GEN 32MB]`).
* **Cơ chế:** Hệ thống tự động sinh ra một file nhị phân mẫu ngẫu nhiên có cấu trúc kiểm định, tính trước mã băm SHA-256 gốc, đồng thời tự động kích hoạt một **In-process Web Socket Server** tại địa chỉ `http://127.0.0.1:<cổng>/benchmark.bin` để mô phỏng tải qua mạng ngay trên máy mà không sợ rớt mạng.

#### Bước B: Thử nghiệm 1 - Chế độ ĐỒNG BỘ AN TOÀN (`SYNCHRONIZED`)
1. Đảm bảo công tắc ở trạng thái: **`MODE: SYNCHRONIZED (MUTEX/SEM)`** (dòng chữ có màu xanh lá).
2. Tùy chỉnh số luồng: `PRODUCERS: 4` và `CONSUMERS: 2`.
3. Bấm **`[START TRANSFER]`**.
4. **Hiện tượng quan sát:**
   * **Khung RAM trên:** 32 ô nhớ của Bounded Buffer sáng màu vàng đất, mũi tên `HEAD (IN)` màu xanh thép và `TAIL (OUT)` màu xanh lá di chuyển nhịp nhàng nối đuôi nhau.
   * **Khung Ma trận đĩa dưới:** 256 ô dữ liệu lần lượt chuyển từ Xám $\rightarrow$ Xanh thép (Đang tải) $\rightarrow$ Vàng (Trong RAM) $\rightarrow$ Xanh lá (Đã ghi đĩa an toàn).
   * **Sidebar trái:** Biểu đồ tốc độ vẽ đường cong thời gian thực, tốc độ vượt 300 MB/s. Số lần xung đột khóa `MUTEX LOCK COLLISIONS` ghi nhận an toàn, `RACE OVERWRITES: 0 BLOCKS`.
   * **Đáy màn hình:** Huy hiệu hiển thị màu xanh lá:  
     `[ STATUS: VERIFIED // BYTE-TO-BYTE INTEGRITY CONFIRMED ]`  
     Hai chuỗi mã băm `EXPECTED SHA-256` và `ACTUAL SHA-256` trùng khớp $100\%$.

#### Bước C: Thử nghiệm 2 - Chế độ BẪY VA CHẠM RACE CONDITION (`UNSYNCHRONIZED`)
1. Bấm nút **`[RESET]`**, sau đó bấm lại nút **`[GEN 16MB]`**.
2. Gạt công tắc sang trái: **`MODE: UNSYNCHRONIZED (RACE CONDITION)`** (dòng chữ chuyển sang màu đỏ gạch).
3. Bấm **`[START TRANSFER]`**.
4. **Hiện tượng quan sát:**
   * **Ma trận đĩa dưới:** Ngay lập tức xuất hiện hàng chục ô **MÀU ĐỎ GẠCH CÓ DẤU GẠCH CHÉO X** (`#C94A4A`) báo hiệu các khối bị ghi đè sai lệch con trỏ file do luồng khác cướp `lseek`.
   * **Sidebar trái:** Chỉ số `RACE OVERWRITES` nhảy vọt lên trên 100 khối lỗi màu đỏ rực.
   * **Bảng Console Log:** Báo lỗi dồn dập bằng các dòng đỏ:  
     `C_THREAD_01 | RACE_ERR | lseek collision detected at offset=0x...`
   * **Đáy màn hình:** Huy hiệu đổi màu đỏ thẫm:  
     `[ STATUS: CORRUPTED // DATA RACE DETECTED (SHA MISMATCH) ]`  
     Mã băm SHA-256 thực tế bị sai lệch hoàn toàn so với gốc!

---

## 5. HƯỚNG DẪN KHỞI CHẠY BỘ KIỂM THỬ DÒNG LỆNH (HEADLESS CLI)

Nếu đang kết nối qua SSH hoặc máy chủ không có giao diện đồ họa, bạn có thể chạy bộ kiểm thử CLI độc lập:

### Chạy tự động cả 2 chế độ để so sánh:
```bash
make test
```

### Hoặc chạy thủ công từng chế độ:
```bash
# 1. Chạy chế độ Đồng bộ (Mutex & Semaphore):
./bin/cli_benchmark

# 2. Chạy chế độ Xung đột Race Condition:
./bin/cli_benchmark --race
```

**Bảng kết quả đối chứng thực tế trên Terminal:**

```text
=================================================================
TRANSFER TELEMETRY REPORT
=================================================================
Chỉ số                         | Synchronized Mode | Unsynchronized Mode
-------------------------------|-------------------|---------------------
Thời gian hoàn thành (Elapsed) | 0.053 giây        | 0.579 giây
Tốc độ trung bình (Throughput) | 304.32 MB/s       | 27.62 MB/s
Mutex Lock Collisions          | 33 lần an toàn    | 0 (Do bỏ qua khóa)
Race Overwrite Blocks          | 0 khối (0%)       | 183 khối (71.5%)
Kiểm định mã băm SHA-256       | [STATUS: VERIFIED]| [STATUS: CORRUPTED]
=================================================================
```

---

## 6. Ý NGHĨA MÃ MÀU TRÊN GIAO DIỆN (ANTI-AI COLOR SYSTEM)

| Màu sắc | Mã Hex | Trạng thái hiển thị |
| :--- | :--- | :--- |
| **Xám Tro Kỹ thuật** | `#232321` | Khối dữ liệu chưa cấp phát (`UNASSIGNED`) |
| **Xanh Thép (Steel Blue)** | `#4A88C7` | Luồng Producer đang tải dải byte (`DOWNLOADING`) |
| **Vàng Đất (Warm Ochre)** | `#D4973B` | Khối đang chờ trong Bounded Buffer trên RAM (`QUEUED`) |
| **Xanh Lá Trầm (Sage Green)**| `#4E9A67` | Khối đã được Consumer ghi an toàn xuống đĩa (`COMMITTED`) |
| **Đỏ Gạch (Brick Crimson)** | `#C94A4A` | Khối bị hỏng do xung đột Race Condition (`CORRUPTED` kèm dấu `X`) |

---

## 7. XỬ LÝ SỰ CỐ THƯỜNG GẶP (TROUBLESHOOTING FAQ)

### Lỗi 1: `fatal error: gtk/gtk.h: No such file or directory`
* **Nguyên nhân:** Máy chưa cài gói phát triển giao diện GTK+ 3.
* **Cách khắc phục:** Chạy `sudo apt install -y libgtk-3-dev pkg-config`.

### Lỗi 2: `fatal error: curl/curl.h: No such file or directory`
* **Nguyên nhân:** Thiếu thư viện client URL `libcurl`.
* **Cách khắc phục:** Chạy `sudo apt install -y libcurl4-openssl-dev`. (Dự án đã tích hợp sẵn thư mục header cục bộ tại `include/curl/` để dự phòng).

### Lỗi 3: `Cannot open display` khi chạy `./bin/sysp_downloader`
* **Nguyên nhân:** Bạn đang chạy qua phiên kết nối SSH từ xa không có máy chủ X11.
* **Cách khắc phục:** 
  - Nếu kết nối qua SSH, thêm cờ `-X` hoặc `-Y`: `ssh -X user@host`.
  - Hoặc nếu chỉ cần kiểm tra logic hoạt động, hãy dùng bản CLI không cần màn hình: `./bin/cli_benchmark`.

### Lỗi 4: `Permission denied` khi chạy file trong `bin/`
* **Khắc phục:** Cấp quyền thực thi:
  ```bash
  chmod +x bin/sysp_downloader bin/cli_benchmark
  ```

---

## 8. CẤU TRÚC THƯ MỤC MÃ NGUỒN

```text
BaiTapLonV2/
├── Makefile                 # Cấu hình biên dịch tối ưu (-O2, -pthread, pkg-config)
├── HUONG_DAN_CHAY.md        # Tài liệu hướng dẫn cài đặt và sử dụng (Tệp này)
├── README.md                # Giới thiệu kiến trúc học thuật và tổng quan đồ án
├── REPORT_UPDATE_GUIDE.md   # Hướng dẫn chi tiết cập nhật báo cáo Word mẫu
├── include/
│   ├── common.h             # Cấu trúc BoundedBuffer, DataChunk, AppContext, bảng màu
│   ├── network_producer.h   # API Producer Thread Pool, Loopback Server
│   ├── storage_consumer.h   # API Consumer Thread Pool, Race Condition, SHA-256
│   └── gui_dashboard.h      # API Giao diện Desktop GTK3, Cairo 2D Visualizer
└── src/
    ├── common.c             # Khởi tạo ngữ cảnh, Logger dạng Kernel, SHA-256 EVP
    ├── network_producer.c   # HTTP Range Worker, In-process Loopback Server
    ├── storage_consumer.c   # Consumer Worker, Bẫy lỗi lseek collision
    ├── gui_dashboard.c      # Giao diện GTK3, hàm vẽ Cairo, Sparkline biểu đồ
    ├── main.c               # Điểm nhập ứng dụng Desktop
    └── cli_benchmark.c      # Trình chạy thực nghiệm độc lập dòng lệnh
```

---
*Tác giả: Quân Phạm Huỳnh (IT31072006) - Đồ án Lập trình Hệ thống VKU.*
