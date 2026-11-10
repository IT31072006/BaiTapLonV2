# HƯỚNG DẪN CẬP NHẬT VÀ NÂNG CẤP BÁO CÁO ĐỒ ÁN LẬP TRÌNH HỆ THỐNG
## Chuyển đổi từ `final2(2).docx` sang Đồ án: "Multi-threaded Download Manager & Concurrency Lab"

> **Tài liệu tham chiếu:** `final2(2).docx` (Báo cáo mẫu cũ về Shell Script / Zenity User & Group Management).  
> **Đề tài mới:** *"Ứng dụng Mô hình Producer–Consumer trong Tối ưu hóa Tốc độ Tải xuống: Xây dựng Trình Quản lý Tải tệp Đa luồng (Multi-threaded Download Manager) trên Desktop bằng C, Phân tích Race Condition và Đồng bộ hóa bằng Mutex/Semaphore"*.  
> **Công nghệ lõi:** C (GNU11), POSIX Threads (`pthread`), POSIX Semaphores, Mutexes, `libcurl`, OpenSSL EVP (SHA-256), GTK+ 3, Cairo 2D Vector Graphics.

---

## MỤC LỤC TÀI LIỆU
1. [PROMPT MẪU ĐỂ DÙNG VỚI GEMINI](#1-prompt-mẫu-để-dùng-với-gemini)
2. [BẢNG ÁNH XẠ TOÀN DIỆN CŨ VS MỚI (MAPPING TABLE)](#2-bảng-ánh-xạ-toàn-diện-cũ-vs-mới)
3. [CHI TIẾT THAY ĐỔI THEO TỪNG PHẦN](#3-chi-tiết-thay-đổi-theo-từng-phần)
   - [Trang bìa & Thông tin chung](#31-trang-bìa--thông-tin-chung)
   - [Lời cảm ơn & Nhận xét của Giảng viên](#32-lời-cảm-ơn--nhận-xét-của-giảng-viên)
   - [Danh mục Hình ảnh (List of Figures - 32 Hình)](#33-danh-mục-hình-ảnh-list-of-figures)
   - [Chương 1: Project Overview](#34-chương-1-project-overview)
   - [Chương 2: Theoretical Basis](#35-chương-2-theoretical-basis)
   - [Chương 3: Installation & Implementation Results](#36-chương-3-installation--implementation-results)
   - [Kết luận & Hướng phát triển (Conclude)](#37-kết-luận--hướng-phát-triển-conclude)
   - [Tài liệu tham khảo (References)](#38-tài-liệu-tham-khảo-references)
4. [BẢNG DỮ LIỆU ĐỐI CHỨNG THỰC NGHIỆM ĐỂ CHÈN VÀO BÁO CÁO](#4-bảng-dữ-liệu-đối-chứng-thực-nghiệm)

---

## 1. PROMPT MẪU ĐỂ DÙNG VỚI GEMINI

*Bạn hãy sao chép đoạn prompt dưới đây vào khung chat của Gemini kèm theo 2 tệp: `final2(2).docx` và tệp `.md` này:*

```text
Chào Gemini, tôi gửi cho bạn 2 tài liệu:
1. File 'final2(2).docx': Là báo cáo đồ án mẫu của trường tôi (VKU) về môn Lập trình Hệ thống (System Programming).
2. File 'REPORT_UPDATE_GUIDE.md': Là tài liệu hướng dẫn kỹ thuật chi tiết chỉ rõ từng điểm cần sửa, nội dung cần thay thế và dữ liệu thực nghiệm mới.

NHIỆM VỤ CỦA BẠN:
- Giữ nguyên cấu trúc định dạng học thuật, bố cục đề mục (Index), phông chữ, văn phong tiếng Anh học thuật chuẩn mực của trường VKU từ file 'final2(2).docx'.
- Thay thế toàn bộ nội dung của đề tài cũ (Shell Script Zenity quản lý User/Group) bằng nội dung mới của đề tài: "Multi-threaded Download Manager in C with Producer-Consumer Architecture, POSIX Mutex/Semaphore Synchronization, and Race Condition Analysis" theo đúng hướng dẫn trong 'REPORT_UPDATE_GUIDE.md'.
- Viết lại chi tiết toàn bộ các Chương (Chapter 1, Chapter 2, Chapter 3, Conclude, References), đảm bảo đầy đủ lý thuyết kỹ thuật hệ thống Linux, giải thuật, bảng biểu đối chứng thực nghiệm (Sync vs Race Condition), mã nguồn C minh họa và danh mục 32 hình ảnh mới.
- Xuất ra nội dung hoàn chỉnh để tôi cập nhật trực tiếp vào file báo cáo Word.
```

---

## 2. BẢNG ÁNH XẠ TOÀN DIỆN CŨ VS MỚI

| STT | Vị trí trong `final2(2).docx` | Nội dung CŨ (Shell Script / Zenity) | Nội dung MỚI CẦN THAY THẾ (C / POSIX / Concurrency) |
| :--- | :--- | :--- | :--- |
| **0** | Trang bìa (Cover) | USER AND GROUP MANAGEMENT: WRITING A SHELL SCRIPT... | **MULTI-THREADED DOWNLOAD MANAGER IN C: PRODUCER–CONSUMER PATTERN, POSIX SYNCHRONIZATION AND RACE CONDITION ANALYSIS** |
| **1** | Ch. 1 - 1.1 | Giới thiệu về User & Group management trên Linux | Giới thiệu về tối ưu hóa I/O mạng, tải file đa luồng và bài toán đồng bộ hóa dữ liệu trong hệ điều hành Linux |
| **2** | Ch. 1 - 1.2 | Mục tiêu: Tự động hóa quản trị user/group bằng script | Mục tiêu: Xây dựng trình tải file đa luồng hiệu năng cao bằng C, phân tích định lượng Race Condition và cơ chế Mutex/Semaphore |
| **3** | Ch. 1 - 1.3 | Đối tượng: `/etc/passwd`, `/etc/group`, lệnh useradd... | Đối tượng: POSIX Threads (`pthreads`), Semaphores, Bounded Buffer, HTTP Range Requests (RFC 7233), File Descriptors (`lseek`, `write`) |
| **4** | Ch. 1 - 1.4 | Đóng góp: Script Zenity quản trị tài khoản | Đóng góp: Ứng dụng Desktop Native C hiệu năng cao, môi trường giả lập Race Condition trực quan, kiểm định tính toàn vẹn SHA-256 byte-to-byte |
| **5** | Ch. 2 - 2.1 | Khái niệm lập trình hệ thống, Driver, Firmware | Giữ nguyên khái niệm Lập trình hệ thống, mở rộng sâu về Quản lý Bộ nhớ, Bộ đệm vòng trên RAM (Ring Buffer) và Hệ thống I/O nhân Linux |
| **6** | Ch. 2 - 2.2 | VirtualBox, Ubuntu, Shell Script | Thay "Shell Script" bằng: **C Language (GNU11), POSIX Threads, `libcurl`, OpenSSL EVP SHA-256, GTK+ 3 & Cairo 2D Graphics Engine** |
| **7** | Ch. 2 - 2.3 | Chi tiết về User và Group (`/etc/passwd`, `usermod`...) | Thay toàn bộ bằng: **Lý thuyết Đa luồng (Multi-threading), Mô hình Producer-Consumer, Hàng đợi giới hạn (Bounded Buffer), Cơ chế Race Condition và Giải pháp Mutex/Semaphore** |
| **8** | Ch. 3 - 3.1 | Môi trường Bash, Zenity, phân quyền `chmod +x` | Môi trường biên dịch Linux GCC, `make`, thư viện `libgtk-3-dev`, `libcurl4-openssl-dev`, `libssl-dev`, Makefile tự động |
| **9** | Ch. 3 - 3.2 | Giao diện Zenity: Tạo user, xóa user, xem log | Kiến trúc 3 Phân hệ: Network Producer Pool, Storage Consumer Pool, Native Desktop Workstation GUI (Cairo 2D) & Phân tích đối chứng Thực nghiệm (Synchronized vs Unsynchronized) |
| **10**| Conclude | Kết quả quản trị user/group bằng script | Kết quả tải file đa luồng tốc độ cao (304 MB/s), đo đạc thành công 183 va chạm Race Condition, đối chiếu SHA-256 |
| **11**| References | Tài liệu về Linux shell, Zenity, lệnh `groupadd` | Tài liệu chuẩn: Kerrisk (The Linux Programming Interface), Stevens (APUE), POSIX Standard, RFC 7233, GTK3 Reference Manual |

---

## 3. CHI TIẾT THAY ĐỔI THEO TỪNG PHẦN

### 3.1. TRANG BÌA & THÔNG TIN CHUNG
* **Tên trường:** VIETNAM – KOREA UNIVERSITY OF INFORMATION AND COMMUNICATION TECHNOLOGY (VKU)
* **Khoa:** FACULTY OF COMPUTER SCIENCE
* **Môn học:** SYSTEM PROGRAMMING (LẬP TRÌNH HỆ THỐNG)
* **Tên đề tài tiếng Anh mới:**  
  `APPLICATION OF THE PRODUCER–CONSUMER PATTERN IN DOWNLOAD ACCELERATION: BUILDING A NATIVE MULTI-THREADED DOWNLOAD MANAGER IN C WITH POSIX SYNCHRONIZATION AND RACE CONDITION ANALYSIS`
* **Tên đề tài tiếng Việt:**  
  `ỨNG DỤNG MÔ HÌNH PRODUCER–CONSUMER TRONG TỐI ƯU HÓA TỐC ĐỘ TẢI XUỐNG: XÂY DỰNG TRÌNH QUẢN LÝ TẢI TỆP ĐA LUỒNG TRÊN DESKTOP BẰNG C, PHÂN TÍCH RACE CONDITION VÀ ĐỒNG BỘ HÓA BẰNG MUTEX/SEMAPHORE`
* **Giảng viên hướng dẫn:** PhD. Hoang Huu Duc (hoặc theo phân công thực tế của lớp bạn).

---

### 3.2. LỜI CẢM ƠN & NHẬN XÉT CỦA GIẢNG VIÊN
* Giữ nguyên cấu trúc trang *"INFORMER'S COMMENTS"* và *"ACKNOWLEDGEMENTS"*.
* Trong phần Acknowledgements, cập nhật nội dung cảm ơn về việc được thầy hướng dẫn nghiên cứu chuyên sâu về **kỹ thuật lập trình hệ thống Linux cấp thấp, cơ chế đồng bộ hóa tiến trình/tiểu trình và kỹ thuật đồ họa Native UI trong C**.

---

### 3.3. DANH MỤC HÌNH ẢNH (LIST OF FIGURES - 32 HÌNH)
*Trong báo cáo cũ có 32 hình về Zenity và User Management. Cần thay thế toàn bộ bằng 32 hình ảnh kỹ thuật tương ứng của dự án mới:*

1. `Figure 1 - Linux Multi-threading Architecture and POSIX Threads (pthreads) Model`
2. `Figure 2 - Ubuntu 24.04 LTS Development Environment and GCC Compiler Toolchain`
3. `Figure 3 - Producer–Consumer Architectural Pattern with Bounded Buffer`
4. `Figure 4 - HTTP Range Request Protocol (RFC 7233) and Byte-level Chunking Mechanism`
5. `Figure 5 - Bounded Buffer Circular Queue Topology in RAM (Capacity: 32 Slots)`
6. `Figure 6 - POSIX Counting Semaphore State Transitions (sem_empty and sem_full)`
7. `Figure 7 - Mutex Lock Serialization and Critical Section Protection Flowchart`
8. `Figure 8 - Shared File Descriptor Race Condition Mechanism: lseek and write Collision Window`
9. `Figure 9 - OpenSSL EVP SHA-256 Cryptographic Digest Pipeline for Byte-to-byte Verification`
10. `Figure 10 - High-Density Native Workstation GUI Architecture based on GTK+ 3 and Cairo 2D`
11. `Figure 11 - Project Directory Structure, Modular File Hierarchy, and Makefile Automation`
12. `Figure 12 - In-process Local Socket HTTP Server Architecture for 100% Offline Testing`
13. `Figure 13 - Synthetic 16MB and 32MB Deterministic Binary Dataset Generator`
14. `Figure 14 - Headless Benchmark CLI Harness Execution: Synchronized Execution Mode`
15. `Figure 15 - Headless Benchmark CLI Harness Execution: Unsynchronized Race Condition Mode`
16. `Figure 16 - High-Resolution Native Desktop Workstation Interface (1240x780 Overall View)`
17. `Figure 17 - Industrial Flat Toolbar: URL Input, Thread Spinners, and Sync Toggle Switch`
18. `Figure 18 - Real-Time Telemetry Sidebar: Transfer Rate, Semaphores, and Lock Contentions`
19. `Figure 19 - Cairo 2D Visualizer 1: RAM Circular Buffer Inspector with Real-time HEAD and TAIL Pointers`
20. `Figure 20 - RAM Circular Buffer Inspector during High-Speed Producer Enqueue Operations`
21. `Figure 21 - RAM Circular Buffer Inspector during Consumer Dequeue and Drain Operations`
22. `Figure 22 - Cairo 2D Visualizer 2: Disk Block Allocation Matrix (256 Blocks, 64KB Chunks)`
23. `Figure 23 - Disk Block Allocation Matrix: Producer Downloading State (Steel Blue #4A88C7)`
24. `Figure 24 - Disk Block Allocation Matrix: RAM Queued State (Warm Ochre #D4973B)`
25. `Figure 25 - Disk Block Allocation Matrix: Safe Disk Committed State (Sage Green #4E9A67)`
26. `Figure 26 - Disk Block Allocation Matrix: Race Corrupted Blocks Marked with Diagonal Crosses (#C94A4A)`
27. `Figure 27 - Cryptographic Integrity Verification Panel: Expected vs Actual SHA-256 Match`
28. `Figure 28 - Cryptographic Integrity Verification Panel: SHA-256 Mismatch Alert on Race Mode`
29. `Figure 29 - Monospace Technical Kernel-style Log Stream: Producer Enqueue Events`
30. `Figure 30 - Monospace Technical Kernel-style Log Stream: Consumer Commit Events`
31. `Figure 31 - Monospace Technical Kernel-style Log Stream: lseek Collision Errors (RACE_ERR)`
32. `Figure 32 - Comparative Throughput and Collision Analysis Graph (Synchronized vs Unsynchronized)`

---

### 3.4. CHƯƠNG 1: PROJECT OVERVIEW

#### 1.1. Introduction
* **Nội dung cũ:** Nói về tầm quan trọng của quản lý user và group trên hệ thống Linux.
* **Nội dung mới:**
  - Trình bày về thách thức tối ưu hóa băng thông mạng trong thời đại truyền tải dữ liệu lớn. Tải file đơn luồng truyền thống không khai thác được hết băng thông đa kết nối và gây lãng phí tài nguyên CPU đa lõi.
  - Phân tích sự cần thiết của kỹ thuật **HTTP Range Chunking (RFC 7233)** kết hợp kiến trúc đa luồng **Producer–Consumer** để tải song song các khối dữ liệu vào bộ đệm RAM (Bounded Buffer) trước khi ghi xuống đĩa cứng.
  - Đặt ra vấn đề then chốt trong lập trình hệ thống: Khi nhiều luồng cùng đọc/ghi vào bộ nhớ RAM chia sẻ và chia sẻ cùng một File Descriptor trên đĩa cứng, nếu không đồng bộ hóa sẽ dẫn đến **Race Condition**, mất mát dữ liệu và sai lệch cấu trúc file.

#### 1.2. Objectives and tasks of the topic
* **1.2.1. Main objectives:**
  - Thiết kế và cài đặt hoàn chỉnh ứng dụng Native Desktop Quản lý Tải file Đa luồng hiệu năng cao bằng ngôn ngữ C trên nền tảng Linux.
  - Xây dựng mô hình thực nghiệm đối chứng để tái hiện trực quan hiện tượng **Race Condition** trên cả RAM và File Descriptor, từ đó chứng minh vai trò sống còn của **POSIX Mutex và Semaphore** trong bảo toàn tính toàn vẹn dữ liệu (Data Integrity).
* **1.2.2. Specific tasks:**
  - *Task 1:* Xây dựng module Producer tải song song qua HTTP Range bằng `libcurl` kèm bộ sinh dữ liệu nhị phân mẫu và Local HTTP Range Server để test offline 100%.
  - *Task 2:* Xây dựng module Consumer ghi đĩa với bộ đệm vòng Bounded Buffer 32 slots và chế độ Toggle kép: `SYNCHRONIZED` (bảo vệ tuyệt đối) vs `UNSYNCHRONIZED` (cố tình gây lỗi xung đột `lseek` và ghi đè RAM).
  - *Task 3:* Tích hợp thuật toán băm mật mã học **OpenSSL EVP SHA-256** để đối chiếu tự động byte-to-byte giữa file nguồn và file đích.
  - *Task 4:* Thiết kế giao diện Desktop Native Workstation `1240x780` bằng **GTK+ 3 và Cairo 2D** theo ngôn ngữ thiết kế kỹ thuật **Anti-AI** (phẳng, sắc nét, không gradient lòe loẹt, không emoji, biểu diễn 256 khối dữ liệu và con trỏ HEAD/TAIL thời gian thực).

#### 1.3. Objects, scope and approach
* **1.3.1. Research subjects:**
  - Lập trình hệ thống Linux cấp thấp (System-level C programming, GNU11 standard).
  - Thư viện tiểu trình POSIX Threads (`<pthread.h>`) và Semaphore vô danh (`<semaphore.h>`).
  - Giao thức HTTP Range (RFC 7233) và thư viện truyền thông mạng `libcurl`.
  - Cơ chế đồng bộ hóa bộ đệm vòng (Circular Queue) và xử lý File I/O (`lseek`, `write`, `fsync`, `ftruncate`).
  - Mật mã học hàm băm với OpenSSL EVP API (`<openssl/evp.h>`).
  - Thư viện giao diện đồ họa GTK+ 3 (`<gtk/gtk.h>`) và thư viện vẽ đồ họa vector 2D Cairo (`<cairo.h>`).
* **1.3.2. Scope of research:**
  - Triển khai trên môi trường Linux Ubuntu (x86_64).
  - File tải nhị phân có dung lượng từ 16MB đến 32MB chia thành 256 đến 512 chunks (mỗi chunk 64KB).
  - Bounded Buffer cố định 32 slots trên RAM (kích thước 2.0 MB).
  - Số luồng Producer tùy chỉnh (1 đến 8 luồng), Consumer (1 đến 8 luồng).
* **1.3.3. Approach:**
  - Phương pháp phân tích kiến trúc hướng module (Modular Systems Design).
  - Phương pháp đo đạc thực nghiệm đối chứng (Empirical Benchmark Comparison): chạy cùng một tập dữ liệu nhị phân ở 2 trạng thái có khóa và không có khóa.

#### 1.4. Contribution of the topic
- Cung cấp một bộ mã nguồn C hoàn chỉnh, chuẩn công nghiệp, không phụ thuộc vào framework nặng nề, biên dịch trực tiếp bằng GCC và Makefile.
- Xây dựng thành công công cụ trực quan hóa (Visualization Tool) biến các khái niệm trừu tượng trong môn Hệ điều hành / Lập trình hệ thống (Mutex contention, Semaphore signaling, HEAD/TAIL pointer movement, Block allocation matrix) thành các đối tượng đồ họa trực quan 2D chuyển động theo thời gian thực.
- Đóng góp một báo cáo nghiên cứu định lượng rõ ràng với số liệu cụ thể về tốc độ tải (MB/s), số lần tranh chấp khóa (Contentions), và số khối dữ liệu bị phá hủy do Race Condition.

---

### 3.5. CHƯƠNG 2: THEORETICAL BASIS

*Chương này cần thay thế hoàn toàn phần lý thuyết Shell Script và User/Group cũ bằng 7 mục lý thuyết chuyên sâu:*

#### 2.1. Introduction to Systems Programming in Linux
* Giữ lại định nghĩa về lập trình hệ thống, vai trò can thiệp trực tiếp vào phần cứng và kernel space.
* Phân tích sự khác biệt giữa Process (Tiến trình - không gian địa chỉ riêng biệt) và Thread (Tiểu trình - chia sẻ chung không gian địa chỉ heap và file descriptors).

#### 2.2. Overview of Core Technologies
* **C Language (GNU11):** Ngôn ngữ nền tảng của hệ điều hành UNIX/Linux, cho phép thao tác con trỏ, bộ nhớ và lời gọi hệ thống (system calls) trực tiếp với overhead bằng 0.
* **POSIX Threads (pthreads):** Chuẩn IEEE POSIX 1003.1c cho quản lý đa luồng trên Unix/Linux.
* **libcurl (Multiprotocol File Transfer Library):** Thư viện C mạnh mẽ hỗ trợ chuẩn RFC 7233 cho phép chỉ định HTTP Range header `Range: bytes=start-end`.
* **OpenSSL Cryptography Engine (EVP API):** Thư viện mật mã chuẩn cấp hệ thống, sử dụng High-level EVP Interface để tính mã băm SHA-256 không phụ thuộc phần cứng.
* **GTK+ 3 & Cairo 2D Vector Engine:** Bộ công cụ xây dựng giao diện Native trên Linux kết hợp engine vẽ hình học vector 2D với độ chính xác sub-pixel, đảm bảo hiệu năng dựng hình cao.

#### 2.3. The Producer–Consumer Problem and Bounded Buffer
* **Khái niệm bài toán:** Mô hình đồng bộ kinh điển giữa luồng sản xuất dữ liệu (Producers - tải từ mạng) và luồng tiêu thụ dữ liệu (Consumers - ghi xuống đĩa).
* **Cấu trúc hàng đợi vòng (Circular Queue):**
  - Quản lý qua 2 con trỏ: `head` (chỉ số nạp vào) và `tail` (chỉ số lấy ra).
  - Công thức tịnh tiến modulo: `head = (head + 1) % BUFFER_CAPACITY;` và `tail = (tail + 1) % BUFFER_CAPACITY;`.
  - Trạng thái hàng đợi: Rỗng khi `count == 0`, Đầy khi `count == BUFFER_CAPACITY`.

#### 2.4. POSIX Synchronization Primitives
* **Counting Semaphores (`sem_t`):**
  - `sem_empty`: Khởi tạo bằng `BUFFER_CAPACITY = 32`, đếm số lượng ô nhớ còn trống trong RAM. Luồng Producer gọi `sem_wait(&empty)` trước khi đẩy dữ liệu.
  - `sem_full`: Khởi tạo bằng `0`, đếm số lượng ô nhớ đã nạp dữ liệu. Luồng Consumer gọi `sem_wait(&full)` trước khi lấy dữ liệu.
* **Mutexes (`pthread_mutex_t`):**
  - `buffer_mutex`: Bảo vệ tính nguyên tử (atomicity) khi nhiều luồng cùng cập nhật chỉ số `head`, `tail`, và `count`.
  - `file_io_mutex`: Bảo vệ cặp thao tác dịch chuyển con trỏ file `lseek()` và ghi đĩa `write()`.
* **Condition Variables (`pthread_cond_t`):**
  - `pause_cond` kết hợp với `control_mutex` để thực hiện tính năng Tạm dừng (Pause) và Tiếp tục (Resume) đường ống truyền tải mà không gây hiện tượng tiêu thụ CPU vô ích (Busy-waiting).

#### 2.5. Race Condition Mechanics and Concurrency Bugs
* **Định nghĩa Race Condition:** Tình huống xảy ra khi hai hoặc nhiều luồng truy cập đồng thời vào vùng nhớ chia sẻ mà không có đồng bộ, và kết quả phụ thuộc vào thứ tự thực thi của bộ lập lịch (Scheduler).
* **Va chạm trong RAM (Memory Overwrite):**
  - Xảy ra khi Producer A đọc chỉ số `head`, bị ngắt lập lịch (preemption delay `usleep(1500)`), Producer B đọc cùng chỉ số `head` đó và ghi đè dữ liệu lên ô nhớ của Producer A.
* **Va chạm File Descriptor (`lseek` vs `write` Collision):**
  - Phân tích cấu trúc bảng File Table trong Linux Kernel: Các luồng trong cùng tiến trình dùng chung 1 File Descriptor `fd` sẽ dùng chung con trỏ vị trí hiện tại của file (Current File Offset).
  - Khi Luồng 1 gọi `lseek(fd, offset_1, SEEK_SET)`, rồi tạm dừng `usleep(2500)`. Luồng 2 xen vào gọi `lseek(fd, offset_2, SEEK_SET)`. Khi Luồng 1 thức dậy gọi `write(fd, buffer_1)`, dữ liệu của Khối 1 sẽ bị ghi đè lên vị trí của Khối 2!

#### 2.6. HTTP Range Chunking Protocol (RFC 7233)
* Cơ chế chia nhỏ file thành các dải byte cố định `64 KB`.
* Định dạng Header gửi đi: `Range: bytes=0-65535`, `Range: bytes=65536-131071`...
* Phản hồi từ máy chủ: Mã trạng thái `HTTP 206 Partial Content` kèm `Content-Range: bytes 0-65535/16777216`.

#### 2.7. Cryptographic Data Verification (SHA-256 EVP)
* Nguyên lý hàm băm mật mã học một chiều SHA-256 (Secure Hash Algorithm 256-bit).
* Tính chất tuyết lở (Avalanche Effect): Chỉ cần 1 byte trong file 16MB bị ghi sai vị trí do Race Condition, toàn bộ chuỗi băm 64 ký tự hex đầu ra sẽ thay đổi hoàn toàn, giúp phát hiện lỗi xung đột với độ tin cậy $100\%$.

---

### 3.6. CHƯƠNG 3: INSTALLATION & IMPLEMENTATION RESULTS

*Chương 3 sẽ thay thế hoàn toàn các màn hình Zenity bằng cấu trúc kiến trúc C và các kết quả thực nghiệm đồ họa của ứng dụng:*

#### 3.1. Environment Setup & Build Pipeline
* **Yêu cầu hệ điều hành:** Linux Ubuntu 22.04 LTS hoặc 24.04 LTS (x86_64).
* **Các gói thư viện phát triển cần cài đặt:**
  ```bash
  sudo apt update
  sudo apt install -y build-essential libgtk-3-dev libcurl4-openssl-dev libssl-dev pkg-config
  ```
* **Cấu trúc Makefile công nghiệp:**
  - Tự động nhận diện cờ thư viện bằng `pkg-config --cflags --libs gtk+-3.0 libcurl openssl`.
  - Hỗ trợ cơ chế dự phòng (Fallback paths) nếu hệ thống dùng LAMPP hoặc đường dẫn tùy biến.
  - Các mục tiêu (Targets): `make benchmark`, `make test`, `make gui`, `make clean`.

#### 3.2. Detailed Subsystem Implementation

##### Phân hệ 1: Network Engine & Producer Pool (`network_producer.c`)
* Trình bày cấu trúc dữ liệu `struct DataChunk`:
  ```c
  struct DataChunk {
      int chunk_id;
      off_t byte_offset;
      size_t size;
      uint8_t buffer[CHUNK_SIZE]; /* 65,536 Bytes = 64 KB */
      int producer_id;
      bool is_occupied;
  };
  ```
* **Bộ sinh dữ liệu nhị phân mẫu (`generate_benchmark_file`):** Sinh file 16MB (256 chunks) hoặc 32MB (512 chunks) có đóng dấu nhận dạng kỹ thuật `magic = 0x53595350` ("SYSP"), `chunk_id` và `byte_offset` ngay trong payload để phát hiện sai lệch.
* **In-process Local HTTP Range Server (`start_local_http_server`):** Lập trình Socket POSIX (`socket`, `bind`, `listen`, `select`) lắng nghe tại `127.0.0.1:ephemeral_port`, hỗ trợ đầy đủ các yêu cầu `HEAD` và `GET` kèm `Range: bytes=start-end`, phục vụ thử nghiệm ngoại tuyến 100% không phụ thuộc Internet.
* **Producer Worker:** Luồng Producer dùng `libcurl` tải dải byte, đóng gói vào `struct DataChunk`, và đẩy vào Bounded Buffer.

##### Phân hệ 2: Storage Consumer & Race Condition Mechanics (`storage_consumer.c`)
* **Tiền cấp phát không gian đĩa (`ftruncate`):** Khởi tạo file đích với dung lượng chính xác, tạo cấu trúc file container thưa (sparse/preallocated file).
* **Chế độ Synchronized:** Áp dụng đầy đủ `sem_wait(&full_slots)`, `pthread_mutex_lock(&buffer_mutex)`, `pthread_mutex_lock(&file_io_mutex)`. Đảm bảo chuỗi `lseek + write` diễn ra nguyên tử. Áp dụng cơ chế Cascading Wakeup khi hoàn tất khối dữ liệu cuối cùng.
* **Chế độ Unsynchronized (Minh họa Race Condition):**
  - Bỏ qua khóa `buffer_mutex`, chèn độ trễ `usleep(1500)` để gây xung đột ghi đè ô nhớ trong RAM.
  - Các luồng consumer dùng chung 1 `fd`, gọi `lseek(fd, chunk.byte_offset, SEEK_SET)`, rồi cố tình trì hoãn `usleep(2500)` trước khi `write(...)` mà không có khóa bảo vệ.
  - Kiểm tra con trỏ file thực tế ngay trước khi ghi:
    ```c
    off_t actual_offset = lseek(ctx->output_fd, 0, SEEK_CUR);
    if (actual_offset != chunk.byte_offset) {
        __sync_fetch_and_add(&ctx->race_corrupted_blocks, 1);
        ctx->block_statuses[chunk.chunk_id] = BLOCK_CORRUPTED;
        app_log(ctx, thread_tag, "RACE_ERR", "lseek collision detected at offset=0x%08lX", chunk.byte_offset);
    }
    ```

##### Phân hệ 3: Native Desktop Workstation GUI (`gui_dashboard.c`, `main.c`)
* **Hệ thống thiết kế Anti-AI (Matte Carbon & Oxide):**
  - Nền ứng dụng: `#141413` (Warm Matte Black).
  - Vùng chứa: `#1C1C1A` (Dark Graphite).
  - Ô nhập liệu & Bảng log: `#10100F` (Deep Inset).
  - Viền phân cách hairline: `1px solid #2C2C29` (Radius: 3px).
  - Toàn bộ số liệu dùng phông `JetBrains Mono` với thuộc tính `font-variant-numeric: tabular-nums` chống rung lắc.
* **4 Vùng giao diện chi tiết:**
  1. *Top Toolbar:* Ô nhập URL, nút sinh file 16MB/32MB, SpinButton chọn số luồng Producers (1-8), Consumers (1-8), Switch chuyển đổi chế độ đồng bộ, nút điều khiển Start/Pause/Reset.
  2. *Left Sidebar:* Bảng đo đạc thời gian thực (Transfer Rate, `sem_empty`, `sem_full`, Mutex Contentions, Race Overwrite Errors chuyển đỏ khi > 0, thanh tiến trình).
  3. *Center Workspace:*
     - Khung Cairo trên: Vẽ trực quan 32 ô nhớ của RAM Circular Buffer với mũi tên con trỏ `HEAD (IN)` màu xanh thép `#4A88C7` và `TAIL (OUT)` màu xanh lá `#4E9A67`.
     - Khung Cairo dưới: Vẽ ma trận 256 khối dữ liệu với 5 màu trạng thái kỹ thuật (Xám: Chưa tải, Xanh thép: Đang tải, Vàng đất: Trong RAM, Xanh lá: Đã ghi đĩa an toàn, Đỏ gạch: Hỏng do Race Condition).
  4. *Bottom Panel:* Thanh đối chiếu SHA-256 (`EXPECTED` vs `ACTUAL`) kèm nhãn trạng thái phẳng `[STATUS: VERIFIED]` hoặc `[STATUS: CORRUPTED]` và bảng Log chuẩn System Log không emoji tự động cuộn.

---

### 3.7. KẾT LUẬN & HƯỚNG PHÁT TRIỂN (CONCLUDE)

#### 1. Results Achieved
* **1.1. Theoretical Knowledge:**
  - Nắm vững kiến trúc đa luồng POSIX, cơ chế cấp phát tài nguyên của hệ điều hành Linux.
  - Hiểu sâu sắc bản chất của xung đột tài nguyên chia sẻ (Race Condition) ở cả tầng bộ nhớ ảo (RAM) và tầng hệ thống tệp (VFS / File Table / File Descriptors).
* **1.2. Practical Implementation:**
  - Xây dựng thành công ứng dụng C hoàn chỉnh với hiệu năng truyền tải vượt trội (đạt trên 300 MB/s qua bộ đệm cục bộ).
  - Thiết kế thành công giao diện kỹ thuật chuyên nghiệp GTK3/Cairo đạt tiêu chuẩn Anti-AI của các phần mềm workstation hàng đầu.
* **1.3. Systems Programming Skills:**
  - Nâng cao kỹ năng quản lý bộ nhớ động không rò rỉ (no memory leaks), quản lý vòng đời luồng, tránh hiện tượng Deadlock và Starvation.

#### 2. Limitations
* Bounded Buffer hiện tại có kích thước cố định (32 slots x 64KB = 2MB); chưa hỗ trợ tự động co giãn kích thước bộ đệm (Dynamic Buffer Resizing) theo tải CPU.
* Việc kiểm thử hiện tập trung tối ưu cho giao thức HTTP/1.1 Range Requests; chưa mở rộng sang giao thức HTTP/2 hoặc HTTP/3 đa kênh (Multiplexing).

#### 3. Future Research and Development Directions
* Mở rộng hỗ trợ thuật toán tải thích ứng (Adaptive Rate Limiting & Dynamic Chunk Sizing).
* Tích hợp cơ chế Resume tải tệp sau khi bị gián đoạn nguồn hoặc mất mạng bằng cách lưu bảng Block Allocation Matrix vào file metadata `.sysp`.
* Phát triển thành thư viện dùng chung (`libsysp_download.so`) để nhúng vào các ứng dụng nhúng (Embedded Linux) và hệ thống IoT.

---

### 3.8. TÀI LIỆU THAM KHẢO (REFERENCES)

*Thay thế toàn bộ tài liệu tham khảo về Zenity và lệnh shell cũ bằng các tài liệu học thuật chuẩn mực sau:*

1. **Michael Kerrisk** (2010), *The Linux Programming Interface: A Linux and UNIX System Programming Handbook*, No Starch Press, San Francisco, CA.
2. **W. Richard Stevens, Stephen A. Rago** (2013), *Advanced Programming in the UNIX Environment (3rd Edition)*, Addison-Wesley Professional.
3. **Andrew S. Tanenbaum, Herbert Bos** (2014), *Modern Operating Systems (4th Edition)*, Pearson Education.
4. **IEEE and The Open Group** (2018), *POSIX.1-2017 Standard: Standard for Information Technology—Portable Operating System Interface (POSIX(R))*, IEEE Std 1003.1-2017.
5. **R. Fielding, J. Reschke** (2014), *Hypertext Transfer Protocol (HTTP/1.1): Range Requests*, IETF RFC 7233.
6. **OpenSSL Project Team** (2023), *OpenSSL EVP Message Digest API Documentation*, Available at: https://www.openssl.org/docs/manmaster/man3/EVP_DigestInit.html.
7. **Daniel Stenberg** (2024), *libcurl - The Multi-protocol File Transfer Library (C API Reference)*, Available at: https://curl.se/libcurl/c/.
8. **GNOME Developer Team** (2024), *GTK+ 3 Reference Manual & Cairo 2D Graphics Library Guide*, Available at: https://docs.gtk.org/gtk3/.

---

## 4. BẢNG DỮ LIỆU ĐỐI CHỨNG THỰC NGHIỆM
*(Gemini hãy sử dụng bảng số liệu thực nghiệm đo đạc thực tế dưới đây để đưa vào Mục 3.2.4 và Mục Bàn luận kết quả trong Báo cáo)*

### Bảng 1: Bảng đối chứng thực nghiệm giữa Chế độ Đồng bộ và Không đồng bộ

| Chỉ số Đo đạc Thực nghiệm | Chế độ ĐỒNG BỘ (`SYNCHRONIZED`) | Chế độ KHÔNG ĐỒNG BỘ (`UNSYNCHRONIZED`) | Ý nghĩa Hệ thống |
| :--- | :--- | :--- | :--- |
| **Dung lượng tập tin thử nghiệm** | 16,777,216 Bytes (16.0 MB) | 16,777,216 Bytes (16.0 MB) | Kích thước đồng nhất chuẩn 256 chunks |
| **Số khối dữ liệu (Chunks)** | 256 khối (64 KB / khối) | 256 khối (64 KB / khối) | Khớp ma trận 32 x 8 của Cairo |
| **Cấu hình luồng thực thi** | 4 Producers / 2 Consumers | 4 Producers / 2 Consumers | Đa luồng cạnh tranh cao |
| **Dung lượng Bounded Buffer** | 32 Slots (2.0 MB RAM) | 32 Slots (2.0 MB RAM) | Giới hạn bộ đệm cố định |
| **Cơ chế bảo vệ RAM Buffer** | `sem_empty`, `sem_full`, `buffer_mutex` | **Bỏ qua khóa** (`usleep(1500)`) | Tranh chấp vị trí `head`/`tail` |
| **Cơ chế bảo vệ File Descriptor** | `file_io_mutex` bọc `lseek + write` | **Bỏ qua khóa** (`usleep(2500)`) | Tranh chấp con trỏ file trong Kernel |
| **Thời gian truyền tải (Elapsed)** | **0.053 giây** | **0.579 giây** | Chế độ Race bị nghẽn do lệch I/O |
| **Băng thông trung bình (Throughput)**| **304.32 MB/s** | **27.62 MB/s** | Tốc độ truyền tải thực tế |
| **Số lần tranh chấp khóa (Contentions)**| **33 lần** (Xử lý an toàn) | **0 lần** (Do cố tình không khóa) | Đo đạc qua `pthread_mutex_trylock` |
| **Số khối va chạm Race Condition** | **0 khối (0.0%)** | **183 khối (71.5%)** | Phát hiện qua `lseek(fd, 0, SEEK_CUR)` |
| **Mã băm SHA-256 File Gốc** | `f69c747442d78c648f2513be03eee1ea994cfba646a55d654ef930e1e3eb44db` | `f69c747442d78c648f2513be03eee1ea994cfba646a55d654ef930e1e3eb44db` | Băm nguồn tính trước |
| **Mã băm SHA-256 File Tải về** | `f69c747442d78c648f2513be03eee1ea994cfba646a55d654ef930e1e3eb44db` | `81c0010bc67ff677d1605139fdd520073301c8aa1590decdc14a88c851902596` | Sai lệch 100% khi có Race |
| **Trạng thái toàn vẹn (Integrity)** | **STATUS: VERIFIED (Khớp 100%)** | **STATUS: CORRUPTED (Hỏng file)** | Kết luận kiểm định mật mã |

---
*Tài liệu này được biên soạn bởi Principal Linux Systems Engineer để làm căn cứ cập nhật báo cáo đồ án cho sinh viên.*

