# BỘ SLIDE THUYẾT TRÌNH BẢO VỆ ĐỒ ÁN LẬP TRÌNH HỆ THỐNG
## CHUẨN CẤU TRÚC THEO MẪU PPTX (13 SLIDES - CHIA 3 NGƯỜI NÓI)

> **Môn học:** System Programming (Lập trình hệ thống)  
> **Đề tài:** Multi-threaded Download Manager in C: Producer–Consumer Pattern, POSIX Synchronization and Race Condition Analysis  
> **Thư mục ảnh đính kèm cho Slide:** `assets/presentation_images/`  

---

## BẢNG PHÂN CHIA NHIỆM VỤ THUYẾT TRÌNH

| Người nói | Phụ trách | Các Slide đảm nhiệm | Thời lượng |
| :--- | :--- | :--- | :--- |
| **NGƯỜI 1** | **Bối cảnh, Bài toán & Cơ chế Hệ điều hành** | Slide 1, Slide 2, Slide 3, Slide 4 | ~3 phút |
| **NGƯỜI 2** | **Kiến trúc Hệ thống, Cấu trúc Dữ liệu & Xung đột Race** | Slide 5, Slide 6, Slide 7, Slide 8 | ~3.5 phút |
| **NGƯỜI 3** | **Giao diện GTK3, Đấu trường Benchmark, Kết quả & Kết luận** | Slide 9, Slide 10, Slide 11, Slide 12, Slide 13 | ~4.5 phút (kèm Demo) |

---

# [SLIDE 1] TITLE SLIDE (BÌA BÁO CÁO)

### Nội dung trên Slide:
* **Header nhỏ:** SYSTEMS PROGRAMMING PROJECT
* **Tiêu đề lớn:** MULTI-THREADED DOWNLOAD MANAGER & CONCURRENCY LAB
* **Tiêu đề phụ 1:** High-Performance Producer–Consumer Pattern with POSIX Synchronization and Race Condition Analysis on Linux
* **Tiêu đề phụ 2:** Native GTK3 Workstation and Dual-Pass Benchmark Arena
* **Footer:** Faculty of Computer Science — Da Nang, 2026
* **Thành viên nhóm:** [Họ tên 3 bạn sinh viên]

> **Lời thoại Người 1:**  
> *"Kính thưa thầy cô trong hội đồng chấm thi và các bạn, hôm nay nhóm chúng em xin phép được trình bày đồ án môn Lập trình hệ thống với đề tài: 'Xây dựng Trình quản lý tải tệp đa luồng và Phòng thí nghiệm đồng bộ hóa Concurrency Lab trên Linux'. Mục tiêu của nhóm là ứng dụng các kiến thức cốt lõi của môn học như POSIX Threads, Semaphore, Mutex và System Calls để giải quyết bài toán tải tệp hiệu năng cao, đồng thời phân tích thực nghiệm hiện tượng Race Condition."*

---

# [SLIDE 2] PRESENTATION AGENDA

### Nội dung trên Slide:
* **01 CONTEXT AND OBJECTIVES:** Multi-threaded acceleration and Linux I/O concurrency challenges.
* **02 PRODUCER–CONSUMER & BOUNDED BUFFER:** Fixed 2.0MB RAM circular queue with HEAD and TAIL pointers.
* **03 SYSTEM ARCHITECTURE AND DATA STRUCTURE:** Modular C implementation, chunk metadata, and embedded HTTP loopback socket server.
* **04 SYNCHRONIZATION AND RACE CONDITIONS:** POSIX Semaphores, Mutex serialization, and lseek collision analysis.
* **05 CONCURRENCY LAB & BENCHMARK ARENA:** Native GTK3/Cairo 2D desktop workstation and dual-pass comparative testing.
* **06 TEST RESULTS AND CONCLUSIONS:** Empirical measurements on Ubuntu, cryptographic SHA-256 verification, and future work.

> **Lời thoại Người 1:**  
> *"Nội dung báo cáo hôm nay gồm 6 phần chính: Khởi đầu từ bối cảnh và mô hình Producer–Consumer; kế tiếp là kiến trúc dữ liệu và phân tích xung đột Race Condition; cuối cùng là trực quan hóa trên giao diện máy trạm và các kết quả đo đạc thực nghiệm trên hệ điều hành Ubuntu."*

---

# [SLIDE 3] CONTEXT AND OBJECTIVES

### Nội dung trên Slide:
* **Tiêu đề phụ:** Multi-threaded HTTP Range acceleration and shared resource coordination in Linux.
* **Khối 1: Single-Thread Bottleneck**
  - Tải tuần tự chỉ dùng 1 kết nối TCP $\rightarrow$ Không tận dụng được băng thông mạng và lãng phí CPU đa lõi.
* **Khối 2: HTTP Range Chunking (RFC 7233)**
  - Tải song song từng khối dữ liệu 64KB độc lập bằng nhiều tiểu trình để tối đa hóa thông lượng mạng.
* **Khối 3: Concurrency & Storage Challenges**
  - Tốc độ mạng vượt tốc độ đĩa $\rightarrow$ Tràn bộ nhớ RAM.
  - Nhiều luồng cùng ghi xuống 1 File Descriptor $\rightarrow$ Xung đột dữ liệu (**Race Condition**).
* **Khối 4: Demonstration and Verification**
  - Kiểm định toàn vẹn dữ liệu từng byte bằng hàm băm mật mã học **OpenSSL EVP SHA-256**.
* **Hình ảnh minh họa:**  
  `Figure 1.1: Multi-threaded Producer-Consumer Architecture and HTTP Range Requests`  
  *(Chèn ảnh: `assets/presentation_images/media_1794469091268_5ab5ccfb.jpg`)*

> **Lời thoại Người 1:**  
> *"Tải file đơn luồng truyền thống bị giới hạn bởi một kết nối TCP duy nhất. Để bứt phá tốc độ, chúng em sử dụng HTTP Range Requests để chia file thành các khối 64KB tải song song. Tuy nhiên, bài toán khó nhất của lập trình hệ thống xuất hiện: Nếu nạp vào RAM nhanh hơn ghi đĩa thì RAM sẽ tràn; còn nếu nhiều luồng cùng ghi vào một file mà không khóa, dữ liệu sẽ bị ghi đè nát nhau. Để kiểm định tệp có bị hỏng hay không, nhóm tích hợp chuẩn băm OpenSSL SHA-256."*

---

# [SLIDE 4] PRODUCER–CONSUMER & BOUNDED RING BUFFER

### Nội dung trên Slide:
* **Tiêu đề phụ:** The kernel manages threads. Threads share a fixed 2.0 MB circular queue in RAM.
* **01 Allocate Fixed Memory Geometry:**
  - Khởi tạo mảng tĩnh: **32 Slots $\times$ 64 KB = Đúng 2.0 MB RAM Footprint**.
  - Ngăn ngừa cấp phát động không kiểm soát, loại trừ hoàn toàn nguy cơ Out-Of-Memory (OOM).
* **02 Producer Enqueue Mechanism:**
  - Producer tải xong chunk 64KB, chờ `sem_wait(&empty_slots)`, nạp vào slot tại con trỏ `HEAD`.
* **03 Consumer Dequeue Mechanism:**
  - Consumer chờ `sem_wait(&full_slots)`, rút chunk tại con trỏ `TAIL` để ghi xuống đĩa, giải phóng ô nhớ.
* **04 Capacity & Pointer Protection:**
  - Con trỏ `HEAD` và `TAIL` xoay vòng theo toán tử modulo: `(index + 1) % 32`.
  - Khóa `pthread_mutex_t` bảo vệ vùng găng khi tăng con trỏ.
* **Hình ảnh minh họa:**  
  `Figure 1.2: RAM Circular Bounded Buffer with Real-time HEAD and TAIL Pointers`  
  *(Chèn ảnh: `assets/presentation_images/media_1794465971300_87eff79d.png`)*

> **Lời thoại Người 1:**  
> *"Để cân bằng giữa mạng và đĩa, hệ thống áp dụng mô hình Producer–Consumer với Bounded Ring Buffer cố định đúng 32 slots (2MB RAM). Dù file tải nặng hàng GB thì RAM sử dụng cũng không vượt quá 2MB. Con trỏ HEAD do Producer phụ trách nạp vào, con trỏ TAIL do Consumer phụ trách rút ra ghi đĩa. Hai con trỏ này dịch chuyển nhịp nhàng xoay vòng. Tiếp theo, em xin mời bạn [Tên Người 2] trình bày về kiến trúc hệ thống và cơ chế Race Condition."*

---

# [SLIDE 5] SYSTEM ARCHITECTURE AND MODULES (NGƯỜI 2)

### Nội dung trên Slide:
* **Tiêu đề phụ:** Modular separation between network ingestion, shared ring buffer, disk storage, and UI presentation.
* **Khối 1: Network Producer Pool (`network_producer.c`)**
  - Quản lý mảng tiểu trình `pthread_t` tải qua `libcurl`.
  - Tích hợp **Embedded HTTP Loopback Server** nhúng qua Socket POSIX để kiểm thử 100% offline.
* **Khối 2: Shared Buffer & Logging Engine (`common.c`)**
  - Quản lý hàng đợi xoay vòng 32 ô, đồng bộ Mutex/Semaphore và hàng đợi log cú pháp Kernel.
* **Khối 3: Storage Consumer Pool (`storage_consumer.c`)**
  - Quản lý các luồng ghi đĩa, tiền cấp phát tệp bằng `posix_fallocate`, công tắc kép Sync vs Race.
* **Khối 4: Presentation & Cairo 2D Layer (`gui_dashboard.c`)**
  - Giao diện máy trạm GTK+ 3 và động cơ vẽ vector Cairo 2D thời gian thực (30 FPS).
* **Hình ảnh minh họa:**  
  `Figure 2.1: Multi-layered System Architecture and Inter-Module Workflow`

> **Lời thoại Người 2:**  
> *"Kính thưa thầy cô, toàn bộ mã nguồn được thiết kế theo tư duy Module hóa rõ ràng: Module Producer chịu trách nhiệm tải mạng; Module Consumer chịu trách nhiệm ghi đĩa; Module Common quản lý bộ đệm và tính toán SHA-256; và Module GUI GTK3 phụ trách hiển thị trực quan. Đặc biệt, nhóm em đã tự tay xây dựng một Local HTTP Loopback Server bằng socket C để giả lập máy chủ HTTP ngay trên máy chấm thi mà không cần kết nối Internet."*

---

# [SLIDE 6] CHUNK DATA STRUCTURE & BUFFER GEOMETRY

### Nội dung trên Slide:
* **Tiêu đề phụ:** Memory layout, chunk payload encapsulation, and 256-block storage map.
* **Cấu trúc dữ liệu Chunk (`ChunkData`):**
  - `chunk_id` (int): Số thứ tự phân đoạn từ `000` đến `255` (cho file 16MB).
  - `byte_offset` (off_t): Vị trí bắt đầu ghi trong file đích (ví dụ: `0x00000000`, `0x00010000`...).
  - `size` (size_t): Kích thước chuẩn 65,536 Bytes (64 KB).
  - `producer_id` / `consumer_id` (int): ID luồng tải và luồng ghi để theo dõi vết (Traceability).
  - `data` (uint8_t[65536]): Bộ nhớ lưu trữ dữ liệu nhị phân.
* **Bản đồ phân bổ khối đĩa (Disk Block Allocation Matrix):**
  - 32 cột $\times$ 8 hàng = **256 khối**. Mỗi khối phản ánh chính xác trạng thái của 64KB trên ổ cứng:
    `UNASSIGNED` $\rightarrow$ `DOWNLOADING` $\rightarrow$ `QUEUED` $\rightarrow$ `COMMITTED` (hoặc `CORRUPTED`).
* **Hình ảnh minh họa:**  
  `Figure 2.2: Disk Block Allocation Matrix (256 Blocks Map)`  
  *(Chèn ảnh: `assets/presentation_images/media_1794465971300_87eff79d.png`)*

> **Lời thoại Người 2:**  
> *"Mỗi khối dữ liệu 64KB đều được đóng gói đầy đủ thông tin: ID khối, offset byte trên đĩa, ID của Producer tải và ID của Consumer ghi đĩa. Trên giao diện, 256 khối này được thể hiện bằng ma trận 32 cột x 8 hàng. Khi một khối chuyển trạng thái từ đang tải sang nạp RAM và ghi đĩa an toàn, màu sắc sẽ biến đổi theo thời gian thực, giúp ta giám sát được dòng dữ liệu ở cấp độ vi mô."*

---

# [SLIDE 7] THREAD LIFECYCLE & RACE CONDITION MECHANICS

### Nội dung trên Slide:
* **Tiêu đề phụ:** Shared file descriptor race condition and the interleaving vulnerability window.
* **Khối 1: Producer Lifecycle Flow**
  - `pthread_create` $\rightarrow$ Fetch HTTP Range $\rightarrow$ `sem_wait(&empty)` $\rightarrow$ Mutex Lock $\rightarrow$ Enqueue $\rightarrow$ Mutex Unlock $\rightarrow$ `sem_post(&full)`.
* **Khối 2: Consumer Lifecycle Flow (Synchronized)**
  - `sem_wait(&full)` $\rightarrow$ Mutex Lock $\rightarrow$ Dequeue $\rightarrow$ Mutex Unlock $\rightarrow$ `sem_post(&empty)` $\rightarrow$ Mutex Lock FD $\rightarrow$ `pwrite()` $\rightarrow$ Mutex Unlock FD.
* **Khối 3: Xung đột con trỏ tệp (Unsynchronized Race Mode)**
  - Dùng chung `output_fd`. Thao tác ghi gồm 2 System Calls riêng biệt:
    $$\text{1. } \texttt{lseek(output\_fd, offset, SEEK\_SET)} \quad \longrightarrow \quad \text{2. } \texttt{write(output\_fd, buffer, 65536)}$$
  - **Xung đột:** Luồng 1 `lseek` tới `0x10000`, Hệ điều hành Context Switch sang Luồng 2 `lseek` tới `0x50000`. Khi Luồng 1 quay lại `write()`, nó ghi nhầm vào `0x50000` $\rightarrow$ **Ghi đè chéo, hỏng cấu trúc tệp!**
* **Hình ảnh minh họa:**  
  `Figure 2.3: Shared File Descriptor lseek Collision Sequence`

> **Lời thoại Người 2:**  
> *"Điểm mấu chốt của đồ án là phân tích nguyên nhân gây ra Race Condition ở tầng Kernel: Khi nhiều luồng cùng chia sẻ 1 File Descriptor, hệ điều hành chỉ lưu duy nhất 1 con trỏ offset chung. Nếu không có Mutex bảo vệ, một luồng vừa gọi `lseek` xong thì luồng khác nhảy vào đổi con trỏ, dẫn đến việc ghi nhầm vị trí của nhau. Nhóm em đã cài đặt một công tắc để tắt Mutex và cố tình tạo ra va chạm này. Tiếp theo, em xin mời bạn [Tên Người 3] trình bày về giao diện và số liệu thực nghiệm."*

---

# [SLIDE 8] CONCURRENCY MODES AND PRIMITIVES

### Nội dung trên Slide:
* **Tiêu đề phụ:** Comparative analysis of system calls and synchronization guards.

| Chế độ / Tính năng | Cơ chế Đồng bộ (Primitives) | Bảo vệ Vùng găng (Critical Section) | Kết quả Dữ liệu (Integrity) |
| :--- | :--- | :--- | :--- |
| **SYNCHRONIZED** | `sem_empty`, `sem_full`, `pthread_mutex_t` | Khóa bảo vệ con trỏ RAM và lệnh ghi đĩa (`pwrite`) | **SHA-256 Khớp 100% (An toàn)** |
| **UNSYNCHRONIZED** | Tháo bỏ toàn bộ Mutex | Không khóa $\rightarrow$ Xuất hiện khe hở giữa `lseek` và `write` | **Mã băm SAI LỆCH (Hỏng tệp)** |
| **AUTO-TUNE** | Adaptive Heuristic Engine | Tự động đo đạc dung lượng và độ trễ mạng (RTT Ping) | Tối ưu hóa số luồng $P/C$ tự động |
| **OFFLINE LOOPBACK** | Socket API (`bind`, `listen`, `send`) | In-process Server giả lập HTTP Range RFC 7233 | Hoạt động độc lập không cần Internet |

* **Đo lường va chạm:** Hệ thống đếm chính xác số sự kiện tranh chấp (`mutex_contentions`) và số khối bị ghi đè (`race_corrupted_blocks`).

> **Lời thoại Người 2:**  
> *"Bảng trên tóm tắt 4 cơ chế cốt lõi của đồ án: Ở chế độ Có khóa, Mutex và Semaphore bảo vệ toàn diện, file tải về đạt độ toàn vẹn 100%. Ở chế độ Không khóa, các luồng tranh chấp tự do làm phát sinh hàng trăm lỗi va chạm con trỏ. Ngoài ra, tính năng Auto-tune tự động đo đạc mạng để điều chỉnh số luồng tối ưu."*

---

# [SLIDE 9] CONCURRENCY LAB WORKSTATION INTERFACE (NGƯỜI 3)

### Nội dung trên Slide:
* **Tiêu đề phụ:** High-density Native GTK+ 3 & Cairo 2D user interface designed for systems programming demonstration.
* **Khối 1: Thanh công cụ điều khiển (Top Toolbar)**
  - Ô nhập URL thông minh, nút `[TỰ SINH FILE TEST]`, nút `[AUTO-TUNE]`, bộ điều chỉnh số luồng và công tắc gạt bật/tắt Đồng bộ.
* **Khối 2: Hệ thống đo lường & Đồ họa Cairo 2D (Telemetry & Visualizers)**
  - Đồ thị sóng 2 kênh: Tốc độ mạng (**Net Ingress**) vs Tốc độ ghi đĩa (**Disk Write**).
  - Thanh tra RAM Bounded Buffer (32 ô) với con trỏ `HEAD` và `TAIL`.
  - Ma trận 256 khối đĩa với **Tooltip thanh tra vi mô khi rê chuột (Hover Inspector)**.
* **Khối 3: Bảng kiểm định SHA-256 & Console Log chuẩn Kernel**
  - Đối chiếu mã băm thực tế vs mã băm nguồn; Nhật ký sự kiện thời gian thực có gắn màu cú pháp.
* **Hình ảnh minh họa:**  
  `Figure 3.1: Concurrency Lab Native Desktop Workstation Running on Linux`  
  *(Chèn ảnh: `assets/presentation_images/media_1794470338449_2beaa0c4.jpg`)*

> **Lời thoại Người 3:**  
> *"Kính thưa thầy cô, em xin phép tiếp tục với phần trực quan hóa và thực nghiệm. Đây là giao diện chính của Concurrency Lab được viết bằng Native GTK3 và Cairo 2D với kích thước chuẩn 1140x740. Mọi biến động của hệ thống từ 2 con trỏ HEAD/TAIL trên RAM, biểu đồ 2 đường sóng tốc độ cho đến ma trận 256 khối đĩa đều hiển thị thời gian thực. Đặc biệt, khi rê chuột vào bất kỳ ô nào, giao diện sẽ bật popup hiển thị dải byte và luồng đang phụ trách."*

---

# [SLIDE 10] BENCHMARK ARENA AND SESSION HISTORY

### Nội dung trên Slide:
* **Tiêu đề phụ:** Dual-pass automated benchmarking and historical audit inspection.
* **Trang 3: Đấu trường so sánh hiệu năng kép (Benchmark Arena)**
  - Bấm nút `[⚡ BẮT ĐẦU CHẠY SO SÁNH TỰ ĐỘNG (DUAL-PASS)]`:
    - Chạy Lượt 1 (Có Khóa - Sync): Ghi nhận thời gian, tốc độ, 0 lỗi, SHA khớp.
    - Chạy Lượt 2 (Không Khóa - Race): Ghi nhận va chạm `lseek`, số khối hỏng và SHA sai lệch.
  - Tự động vẽ **Biểu đồ cột so sánh trực quan (Comparative Bar Chart)** bằng động cơ Cairo 2D.
* **Trang 2: Quản lý lịch sử phiên tải (Download Session History & Audit)**
  - Lưu trữ chi tiết các phiên tải thành công/thất bại, nút mở thư mục lưu trữ `test_data/` và sao chép mã SHA-256.
* **Hình ảnh minh họa:**  
  `Figure 3.2: Download History & Integrity Audit View`  
  *(Chèn ảnh: `assets/presentation_images/media_1794469653925_b03440fc.jpg`)*

> **Lời thoại Người 3:**  
> *"Để chứng minh sự khác biệt một cách thuyết phục nhất, nhóm em thiết kế Trang thứ 3 - Đấu trường Benchmark. Khi bấm nút, hệ thống tự động chạy liên tiếp 2 lượt trên cùng 1 file 16MB để đo đạc và vẽ biểu đồ cột đối chiếu trực tiếp. Bên cạnh đó, Trang thứ 2 lưu lại toàn bộ lịch sử các lần tải kèm mã băm SHA-256 để kiểm tra lại bất cứ lúc nào."*

---

# [SLIDE 11] TEST RESULTS ON UBUNTU LINUX

### Nội dung trên Slide:
* **Tiêu đề phụ:** Test environment: Ubuntu 24.04 / 22.04 LTS (x86_64), GCC Compiler Toolchain.
* **4 Khối chỉ số đo lường nổi bật:**
  - **256 / 256** Chunks ghi đĩa an toàn tuyệt đối ở chế độ Có Khóa (0 lỗi).
  - **107 / 256** Khối bị hỏng do xung đột Race Condition khi bỏ khóa (~41.8%).
  - **141.64 MB/s** Tốc độ truyền tải ghi đĩa đo được trên tệp 16 MB.
  - **100% MATCH** Mã băm SHA-256 trùng khớp hoàn toàn giữa file nguồn và đích.
* **Bản chất hệ thống:**
  - Khi không có khóa, lệnh `ls -lh` vẫn báo đủ 16MB dung lượng, nhưng mã SHA-256 bị sai lệch hoàn toàn. File đích chứa nhiều khoảng trống và dữ liệu bị đè nát.
  - $\rightarrow$ Chứng minh chi phí khóa của Mutex là cực kỳ nhỏ nhưng là điều kiện sống còn của dữ liệu!

> **Lời thoại Người 3:**  
> *"Đây là bảng kết quả thực nghiệm trên Ubuntu: Ở chế độ Có khóa, toàn bộ 256 khối dữ liệu an toàn 100%, mã SHA-256 trùng khớp hoàn hảo. Nhưng khi bỏ khóa, có tới 107 khối bị ghi đè và phá hỏng. Điều thú vị là file tải về vẫn báo đủ dung lượng 16MB, nhưng giải nén ra sẽ bị lỗi ngay lập tức vì cấu trúc bên trong đã bị phá hủy. Điều này chứng minh tốc độ tải chỉ có ý nghĩa khi đi kèm với tính toàn vẹn dữ liệu."*

---

# [SLIDE 12] CONCLUSIONS AND FUTURE WORK

### Nội dung trên Slide:
* **PROJECT RESULTS (Kết quả đạt được):**
  - Làm chủ hoàn toàn kiến trúc đa luồng POSIX Threads, Semaphore và Mutex trên Linux.
  - Xây dựng thành công ứng dụng tải tệp đa luồng hiệu năng cao bằng C với giao diện đồ họa Native GTK3/Cairo 2D mượt mà.
  - Tái hiện, đo lường và trực quan hóa thành công hiện tượng Race Condition đến từng byte bộ nhớ.
  - Tích hợp chuẩn băm OpenSSL SHA-256 kiểm định toàn vẹn dữ liệu byte-to-byte.
* **LIMITATIONS AND FUTURE WORK (Hạn chế & Hướng phát triển):**
  - Mở rộng tính năng Tạm dừng và Tiếp tục tải (HTTP Range Resume) khi mất kết nối mạng.
  - Nghiên cứu cơ chế ghép nhiều đường mạng song song (Multi-interface Bandwidth Bonding: vừa chạy Wifi vừa chạy mạng dây LAN).

> **Lời thoại Người 3:**  
> *"Đồ án đã giúp nhóm em không chỉ nắm vững lý thuyết trong giáo trình mà đã thực sự nhìn thấy, đo lường được và làm chủ được các hành vi của luồng và bộ nhớ trong nhân Linux. Nhóm đã chứng minh được tính đúng đắn của giải thuật và xây dựng được một công cụ trực quan hữu ích cho môn học."*

---

# [SLIDE 13] THANK YOU! QUESTIONS AND DISCUSSION

### Nội dung trên Slide:
* **Tiêu đề lớn:** THANK YOU!
* **Dòng chữ:** Questions & Discussion (Hỏi đáp & Trao đổi)
* **Thông tin dự án:**
  - GitHub Repository: `https://github.com/IT31072006/BaiTapLonV2`
  - Ngôn ngữ: C (GNU11), POSIX APIs, GTK+ 3, Cairo 2D, OpenSSL, libcurl.

> **Lời thoại Người 3:**  
> *"Nhóm chúng em xin chân thành cảm ơn quý thầy cô trong hội đồng đã chú ý lắng nghe. Sau đây, nhóm em xin phép được bắt đầu phần Demo trực tiếp ứng dụng trên máy tính và rất mong nhận được những câu hỏi, đóng góp quý báu từ thầy cô ạ!"*
