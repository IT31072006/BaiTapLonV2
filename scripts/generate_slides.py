#!/usr/bin/env python3
import os
import io
import re
import pptx
from pptx.util import Pt
from PIL import Image

TEMPLATE_PATH = '/home/quan/.gemini/antigravity/brain/eb4ac895-87c7-4dd3-b0f1-a142ebeea400/.user_uploaded/media_1795066958979_1c3cd5d9.pptx'
OUTPUT_PATH = 'Bao_Cao_Thuyet_Trinh_LTHT_BaiTapLonV2.pptx'
ARTIFACT_OUTPUT = '/home/quan/.gemini/antigravity/brain/eb4ac895-87c7-4dd3-b0f1-a142ebeea400/Bao_Cao_Thuyet_Trinh_LTHT_BaiTapLonV2.pptx'

def set_text_preserve_style(p, new_text, font_override=None, size_override=None):
    if not p.runs:
        p.text = new_text
        return
    font_name = p.runs[0].font.name
    font_size = p.runs[0].font.size
    font_bold = p.runs[0].font.bold
    font_italic = p.runs[0].font.italic
    try:
        font_color = p.runs[0].font.color.rgb
    except Exception:
        font_color = None
    
    # Fix python-pptx font lookup bug: Arial Bold is not a valid font family
    if font_name and "Bold" in font_name:
        font_name = font_name.replace("Bold", "").strip()
        font_bold = True
    
    # Consolas and other non-unicode fonts lack Vietnamese glyphs (Đ, đ, ệ, ồ) -> fallback to Arial
    if font_override:
        font_name = font_override
    elif not font_name or 'Consolas' in font_name or font_name in ['Arial', 'Arial Bold']:
        font_name = 'Arial'
    
    if size_override:
        font_size = Pt(size_override)
    
    p.runs[0].text = new_text
    p.runs[0].font.name = font_name
    p.runs[0].font.size = font_size
    p.runs[0].font.bold = font_bold
    p.runs[0].font.italic = font_italic
    if font_color:
        p.runs[0].font.color.rgb = font_color
    
    for run in p.runs[1:]:
        run.text = ''

def update_subshape_text(shape, new_text, font_override=None, size_override=None):
    if shape.has_text_frame:
        for i, p in enumerate(shape.text_frame.paragraphs):
            if i == 0:
                set_text_preserve_style(p, new_text, font_override, size_override)
            else:
                p.text = ''

def replace_shape_image(slide, shape, image_path):
    if not os.path.exists(image_path):
        print(f"Warning: Image file not found: {image_path}")
        return
    xml = shape._element.xml
    m = re.search(r'embed=\"([^\"]+)\"', xml)
    if not m:
        print(f"No embed blip found in shape {shape.name}")
        return
    rId = m.group(1)
    part = slide.part.related_part(rId)
    
    buf = io.BytesIO()
    with Image.open(image_path) as im:
        im.convert('RGB').save(buf, format='PNG')
    part._blob = buf.getvalue()
    print(f"Replaced image in {shape.name} with {image_path}")

def main():
    print(f"Loading template: {TEMPLATE_PATH}")
    prs = pptx.Presentation(TEMPLATE_PATH)
    
    # -------------------------------------------------------------
    # SLIDE 1: BÌA ĐỀ TÀI (NGƯỜI 1)
    # -------------------------------------------------------------
    s1 = prs.slides[0]
    update_subshape_text(s1.shapes[1].shapes[1], "BÀI TẬP LỚN LẬP TRÌNH HỆ THỐNG LINUX")
    update_subshape_text(s1.shapes[2].shapes[1], "HỆ THỐNG TẢI FILE ĐA LUỒNG")
    update_subshape_text(s1.shapes[3].shapes[1], "HTTP Range Requests, Circular Bounded Buffer & Đồng Bộ Hóa POSIX")
    update_subshape_text(s1.shapes[4].shapes[1], "Trực quan hóa Bộ nhớ GTK3, Kiểm toán SHA-256 & Đấu trường Benchmark")
    update_subshape_text(s1.shapes[5].shapes[1], "Khoa Công Nghệ Thông Tin // Nhóm Sinh Viên Thực Hiện - 11/2026")
    
    # -------------------------------------------------------------
    # SLIDE 2: MỤC LỤC BÁO CÁO (NGƯỜI 1)
    # -------------------------------------------------------------
    s2 = prs.slides[1]
    update_subshape_text(s2.shapes[1].shapes[1], "MỤC LỤC BÁO CÁO (AGENDA)")
    # 01
    update_subshape_text(s2.shapes[3].shapes[1], "BỐI CẢNH VÀ MỤC TIÊU")
    update_subshape_text(s2.shapes[4].shapes[1], "Bài toán tải phân đoạn HTTP Range & giải quyết nghẽn I/O trên Linux.")
    # 02
    update_subshape_text(s2.shapes[6].shapes[1], "CƠ CHẾ ĐỒNG BỘ HĐH")
    update_subshape_text(s2.shapes[7].shapes[1], "POSIX Threads, Semaphores, Mutex & mô hình Bounded Buffer.")
    # 03
    update_subshape_text(s2.shapes[9].shapes[1], "KIẾN TRÚC VÀ GIAO THỨC CHUNK")
    update_subshape_text(s2.shapes[10].shapes[1], "Mô hình Producer-Consumer đa luồng, Header 64KB & pwrite64().")
    # 04
    update_subshape_text(s2.shapes[12].shapes[1], "TRẠM ĐIỀU KHIỂN GTK3")
    update_subshape_text(s2.shapes[13].shapes[1], "Giao diện Jira SaaS hiện đại, biểu đồ thời gian thực & Tooltip RAM.")
    # 05
    update_subshape_text(s2.shapes[15].shapes[1], "THỬ NGHIỆM & BENCHMARK")
    update_subshape_text(s2.shapes[16].shapes[1], "Đấu trường so sánh trực tiếp: Chế độ Có khóa vs Xung đột Race Condition.")
    # 06
    update_subshape_text(s2.shapes[18].shapes[1], "KẾT LUẬN & HƯỚNG PHÁT TRIỂN")
    update_subshape_text(s2.shapes[19].shapes[1], "Tổng kết kiểm toán SHA-256, bài học kinh nghiệm và nâng cấp tương lai.")
    
    # -------------------------------------------------------------
    # SLIDE 3: BỐI CẢNH & HTTP RANGE (NGƯỜI 1)
    # -------------------------------------------------------------
    s3 = prs.slides[2]
    update_subshape_text(s3.shapes[1].shapes[1], "Bối cảnh và Động lực Đề tài")
    update_subshape_text(s3.shapes[2].shapes[1], "Khai thác tối đa băng thông mạng và năng lực xử lý đa luồng trên Linux.")
    # Card 1
    update_subshape_text(s3.shapes[5].shapes[1], "Tải đơn luồng truyền thống")
    update_subshape_text(s3.shapes[6].shapes[1], "Thắt cổ chai băng thông mạng, lãng phí tài nguyên CPU đa nhân và dễ gián đoạn.")
    # Card 2
    update_subshape_text(s3.shapes[9].shapes[1], "Giải pháp HTTP Range Request")
    update_subshape_text(s3.shapes[10].shapes[1], "Tách file thành nhiều dải byte độc lập, gửi đồng thời qua nhiều kết nối song song.")
    # Card 3
    update_subshape_text(s3.shapes[13].shapes[1], "Thách thức Đồng bộ Bounded Buffer")
    update_subshape_text(s3.shapes[14].shapes[1], "Nhiều luồng ghi/đọc đồng thời đòi hỏi cơ chế Semaphore và Mutex để tránh hỏng dữ liệu.")
    # Caption & Image
    update_subshape_text(s3.shapes[17].shapes[1], "Figure 1.1. Cơ chế phân đoạn dải byte HTTP Range và tải song song")
    replace_shape_image(s3, s3.shapes[16], 'assets/presentation_images/fig_http_range.png')

    # -------------------------------------------------------------
    # SLIDE 4: NỀN TẢNG LÝ THUYẾT ĐỒNG BỘ HĐH (NGƯỜI 1)
    # -------------------------------------------------------------
    s4 = prs.slides[3]
    update_subshape_text(s4.shapes[1].shapes[1], "Nền tảng Đồng bộ Hóa Hệ Điều Hành")
    update_subshape_text(s4.shapes[2].shapes[1], "Nhân Linux quản lý luồng POSIX và điều phối tài nguyên bằng Semaphore và Mutex.")
    # Box 1
    update_subshape_text(s4.shapes[5].shapes[1], "pthread_create & Join")
    update_subshape_text(s4.shapes[6].shapes[1], "Khởi tạo và quản lý vòng đời các luồng tải mạng (Producers) và luồng ghi đĩa (Consumers).")
    # Box 2
    update_subshape_text(s4.shapes[9].shapes[1], "sem_empty & sem_full")
    update_subshape_text(s4.shapes[10].shapes[1], "Hai Semaphore đếm điều phối giới hạn 32 slots của Bounded Buffer, chặn Producer khi RAM đầy.")
    # Box 3
    update_subshape_text(s4.shapes[13].shapes[1], "pthread_mutex_t")
    update_subshape_text(s4.shapes[14].shapes[1], "Khóa loại trừ tương hỗ bảo vệ con trỏ mảng vòng head và tail trong vùng tranh chấp (Critical Section).")
    # Box 4
    update_subshape_text(s4.shapes[17].shapes[1], "Direct I/O pwrite64()")
    update_subshape_text(s4.shapes[18].shapes[1], "Ghi trực tiếp từng block 64KB vào offset tuyệt đối trên tệp đích mà không cần nối tệp tuần tự.")

    # -------------------------------------------------------------
    # SLIDE 5: KIẾN TRÚC MÔ HÌNH PRODUCER-CONSUMER (NGƯỜI 2)
    # -------------------------------------------------------------
    s5 = prs.slides[4]
    update_subshape_text(s5.shapes[1].shapes[1], "Kiến trúc Hệ thống và Phân tầng Module")
    update_subshape_text(s5.shapes[2].shapes[1], "Mô hình Producer-Consumer đồng bộ kết nối hạ tầng mạng với hệ thống tệp đĩa cứng.")
    # Card 1
    update_subshape_text(s5.shapes[5].shapes[1], "Network Producers (Luồng Mạng)")
    update_subshape_text(s5.shapes[6].shapes[1], "Tải song song các dải byte HTTP Range qua libcurl và đẩy chunk vào RAM Bounded Buffer.")
    # Card 2
    update_subshape_text(s5.shapes[9].shapes[1], "Circular Bounded Buffer (RAM 2.0 MB)")
    update_subshape_text(s5.shapes[10].shapes[1], "Vùng đệm tròn 32 slots (64KB/slot) đóng vai trò trung chuyển được bảo vệ bởi Semaphore.")
    # Card 3
    update_subshape_text(s5.shapes[13].shapes[1], "Storage Consumers (Luồng Ghi Đĩa)")
    update_subshape_text(s5.shapes[14].shapes[1], "Lấy chunk từ RAM và thực hiện ghi song song xuống đĩa bằng pwrite64() bảo toàn thứ tự.")
    # Caption & Image
    update_subshape_text(s5.shapes[17].shapes[1], "Figure 2.1. Sơ đồ kiến trúc Producer-Consumer đa luồng trên Linux")
    replace_shape_image(s5, s5.shapes[16], 'assets/presentation_images/fig_architecture.png')

    # -------------------------------------------------------------
    # SLIDE 6: CẤU TRÚC DỮ LIỆU RAM BUFFER & GIAO THỨC CHUNK (NGƯỜI 2)
    # -------------------------------------------------------------
    s6 = prs.slides[5]
    update_subshape_text(s6.shapes[1].shapes[1], "Cấu trúc Dữ liệu và Giao thức Khối Bộ Nhớ")
    update_subshape_text(s6.shapes[2].shapes[1], "Thiết kế gói tin Chunk 64KB độc lập và bản đồ ánh xạ 32 slots RAM.")
    # Field 1
    update_subshape_text(s6.shapes[4].shapes[1], "chunk_index")
    update_subshape_text(s6.shapes[5].shapes[1], "Định danh số thứ tự của khối dữ liệu (từ 0 đến N-1).")
    # Field 2
    update_subshape_text(s6.shapes[7].shapes[1], "byte_offset")
    update_subshape_text(s6.shapes[8].shapes[1], "Vị trí byte tuyệt đối trên tệp đích để pwrite64() ghi trực tiếp.")
    # Field 3
    update_subshape_text(s6.shapes[10].shapes[1], "data_size")
    update_subshape_text(s6.shapes[11].shapes[1], "Kích thước thực tế của khối (tối đa 64 KB, byte cuối của file).")
    # Field 4
    update_subshape_text(s6.shapes[13].shapes[1], "payload")
    update_subshape_text(s6.shapes[14].shapes[1], "Mảng dữ liệu thô nhị phân chứa nội dung tải về từ HTTP Range.")
    # Footer
    update_subshape_text(s6.shapes[15].shapes[1], "Mỗi slot RAM mang đầy đủ Header định tuyến, loại bỏ hoàn toàn việc tìm kiếm tuần tự.")
    # Caption & Image
    update_subshape_text(s6.shapes[18].shapes[1], "Figure 2.2. Trực quan hóa 32 slots Bounded Buffer và ma trận 256 Disk Blocks")
    replace_shape_image(s6, s6.shapes[17], 'assets/presentation_images/media_1794465971300_87eff79d.png')

    # -------------------------------------------------------------
    # SLIDE 7: VÒNG ĐỜI LUỒNG & RACE CONDITION (NGƯỜI 2)
    # -------------------------------------------------------------
    s7 = prs.slides[6]
    update_subshape_text(s7.shapes[1].shapes[1], "Vòng đời Luồng và Trực quan hóa Xung Đột")
    update_subshape_text(s7.shapes[2].shapes[1], "Phân tích cơ chế bảo vệ của Semaphore đối nghịch với hiện tượng tương tranh bộ nhớ.")
    # Card 1
    update_subshape_text(s7.shapes[5].shapes[1], "Đẩy dữ liệu vào Buffer (Produce)")
    update_subshape_text(s7.shapes[6].shapes[1], "Chờ sem_empty > 0, khóa Mutex, ghi slot tại head, mở Mutex, tăng sem_full.")
    # Card 2
    update_subshape_text(s7.shapes[9].shapes[1], "Rút dữ liệu ghi đĩa (Consume)")
    update_subshape_text(s7.shapes[10].shapes[1], "Chờ sem_full > 0, khóa Mutex, lấy slot tại tail, mở Mutex, tăng sem_empty.")
    # Card 3
    update_subshape_text(s7.shapes[13].shapes[1], "Xung đột khi Tắt Khóa (Race Overwrite)")
    update_subshape_text(s7.shapes[14].shapes[1], "Thiếu khóa khiến nhiều luồng ghi đè một slot RAM, dẫn đến mất khối và hỏng tệp tin.")
    # Caption & Image
    update_subshape_text(s7.shapes[17].shapes[1], "Figure 2.3. So sánh trực tiếp Chế độ Có khóa (Safe) và Không khóa (Race Condition)")
    replace_shape_image(s7, s7.shapes[16], 'assets/presentation_images/fig_race_condition.png')

    # -------------------------------------------------------------
    # SLIDE 8: BẢNG SO SÁNH CHẾ ĐỘ ĐỒNG BỘ (NGƯỜI 2)
    # -------------------------------------------------------------
    s8 = prs.slides[7]
    update_subshape_text(s8.shapes[1].shapes[1], "Bảng So sánh Kỹ thuật Hai Chế độ Chạy")
    update_subshape_text(s8.shapes[2].shapes[1], "Phân tích định lượng giữa Chế độ Có khóa (Synchronized) và Chế độ Bỏ khóa (Race Condition).")
    # Table Headers
    update_subshape_text(s8.shapes[4].shapes[1], "TIÊU CHÍ")
    update_subshape_text(s8.shapes[5].shapes[1], "CÓ KHÓA (SAFE SYNC)")
    update_subshape_text(s8.shapes[6].shapes[1], "KHÔNG KHÓA (RACE CONDITION)")
    update_subshape_text(s8.shapes[7].shapes[1], "Ý NGHĨA HỆ THỐNG")
    # Row 1: Đồng bộ hóa
    update_subshape_text(s8.shapes[9].shapes[1], "Cơ chế Đồng bộ")
    update_subshape_text(s8.shapes[10].shapes[1], "Mutex + 2 Semaphores (empty/full)")
    update_subshape_text(s8.shapes[11].shapes[1], "Bỏ qua toàn bộ cơ chế khóa")
    update_subshape_text(s8.shapes[12].shapes[1], "Độc quyền truy cập vùng RAM tranh chấp")
    # Row 2: Va chạm
    update_subshape_text(s8.shapes[14].shapes[1], "Xung đột RAM")
    update_subshape_text(s8.shapes[15].shapes[1], "0 biến cố (An toàn tuyệt đối)")
    update_subshape_text(s8.shapes[16].shapes[1], "Hàng chục/trăm va chạm ghi đè")
    update_subshape_text(s8.shapes[17].shapes[1], "Chứng minh trực quan hiện tượng Race")
    # Row 3: SHA-256
    update_subshape_text(s8.shapes[19].shapes[1], "Toàn vẹn SHA-256")
    update_subshape_text(s8.shapes[20].shapes[1], "Khớp 100% mã checksum gốc")
    update_subshape_text(s8.shapes[21].shapes[1], "Sai lệch hoàn toàn (Hỏng file)")
    update_subshape_text(s8.shapes[22].shapes[1], "Khẳng định giá trị của đồng bộ hóa")
    # Row 4: Khối đĩa
    update_subshape_text(s8.shapes[24].shapes[1], "Khối đĩa Commit")
    update_subshape_text(s8.shapes[25].shapes[1], "256/256 Blocks Committed")
    update_subshape_text(s8.shapes[26].shapes[1], "Nhiều blocks bị ghi đè/mất")
    update_subshape_text(s8.shapes[27].shapes[1], "Hiển thị màu đỏ trực tiếp trên Ma trận")
    # Row 5: Hiệu năng
    update_subshape_text(s8.shapes[29].shapes[1], "Tận dụng Băng thông")
    update_subshape_text(s8.shapes[30].shapes[1], "Tối ưu, mượt mà không nghẽn I/O")
    update_subshape_text(s8.shapes[31].shapes[1], "Tải nhanh giả tạo do ghi đè")
    update_subshape_text(s8.shapes[32].shapes[1], "Hiệu năng luôn phải đi liền an toàn")
    # Footer
    update_subshape_text(s8.shapes[33].shapes[1], "Trong chế độ Race Condition, giao diện phát hiện và đổi màu khối xung đột sang đỏ cảnh báo.")

    # -------------------------------------------------------------
    # SLIDE 9: TRẠM ĐIỀU KHIỂN GTK3 (NGƯỜI 3)
    # -------------------------------------------------------------
    s9 = prs.slides[8]
    update_subshape_text(s9.shapes[1].shapes[1], "Trạm Điều khiển Đồ họa GTK3 Workstation")
    update_subshape_text(s9.shapes[2].shapes[1], "Ứng dụng Desktop Native C/GTK3 giao diện Jira SaaS hiện đại, sống động và trực quan.")
    # Card 1
    update_subshape_text(s9.shapes[5].shapes[1], "Thanh tra Bộ nhớ Tương tác (Tooltips)")
    update_subshape_text(s9.shapes[6].shapes[1], "Hover vào từng ô RAM / Disk Block để xem chi tiết Thread phụ trách, byte range và trạng thái.")
    # Card 2
    update_subshape_text(s9.shapes[9].shapes[1], "Biểu đồ Sóng Đa Kênh Thời gian thực")
    update_subshape_text(s9.shapes[10].shapes[1], "Theo dõi song song 2 đường sóng: Tốc độ mạng Ingress (Xanh dương) vs Ghi đĩa Egress (Xanh lá).")
    # Card 3
    update_subshape_text(s9.shapes[13].shapes[1], "Chuyển đổi Giao diện Sáng / Tối")
    update_subshape_text(s9.shapes[14].shapes[1], "Chuyển đổi linh hoạt giữa giao diện Trắng - Xanh Jira SaaS và Dark Cyberpunk khi chiếu slide.")
    # Caption & Image (Crisp Cropped Workstation UI)
    update_subshape_text(s9.shapes[17].shapes[1], "Figure 3.1. Trạm làm việc GTK3 Workstation đang chạy tải đa luồng trên Ubuntu")
    replace_shape_image(s9, s9.shapes[16], 'assets/presentation_images/crop_concurrency_lab.png')

    # -------------------------------------------------------------
    # SLIDE 10: LỊCH SỬ TẢI & KIỂM TOÁN SHA-256 (NGƯỜI 3)
    # -------------------------------------------------------------
    s10 = prs.slides[9]
    update_subshape_text(s10.shapes[1].shapes[1], "Lịch sử Tải xuống và Kiểm toán Toàn vẹn Tệp")
    update_subshape_text(s10.shapes[2].shapes[1], "Lưu trữ nhật ký phiên tải, thống kê lưu lượng và đối chiếu mã hash SHA-256 tự động.")
    # Fig 3.2 (Top left) - Cropped test links
    update_subshape_text(s10.shapes[5].shapes[1], "Figure 3.2. Nguồn dữ liệu kiểm thử tải trực tiếp (HTTP Range Support)")
    replace_shape_image(s10, s10.shapes[4], 'assets/presentation_images/crop_gemini_links.png')
    # Card
    update_subshape_text(s10.shapes[8].shapes[1], "Kiểm toán Mật mã SHA-256")
    update_subshape_text(s10.shapes[9].shapes[1], "Sử dụng OpenSSL EVP API tính toán chuỗi hash 64 ký tự hex của file thực tế và đối chiếu với mã gốc.")
    # Fig 3.3 (Right) - Cropped session history table
    update_subshape_text(s10.shapes[12].shapes[1], "Figure 3.3. Bảng Kiểm toán Phiên tải và Xác thực Toàn vẹn Tệp (Dark Mode)")
    replace_shape_image(s10, s10.shapes[11], 'assets/presentation_images/crop_session_history.png')

    # -------------------------------------------------------------
    # SLIDE 11: KẾT QUẢ THỬ NGHIỆM TRÊN UBUNTU (NGƯỜI 3)
    # -------------------------------------------------------------
    s11 = prs.slides[10]
    update_subshape_text(s11.shapes[1].shapes[1], "Kết quả Thử nghiệm Thực tế trên Linux")
    update_subshape_text(s11.shapes[2].shapes[1], "Môi trường thử nghiệm: Ubuntu 24.04 LTS x86_64, GCC 13, POSIX pthreads & libcurl.")
    # Stat 1
    update_subshape_text(s11.shapes[4].shapes[1], "256/256")
    update_subshape_text(s11.shapes[5].shapes[1], "Khối Đĩa Toàn Vẹn")
    update_subshape_text(s11.shapes[6].shapes[1], "Cam kết ghi đĩa 100% ở chế độ Safe Sync.")
    # Stat 2
    update_subshape_text(s11.shapes[8].shapes[1], "100%")
    update_subshape_text(s11.shapes[9].shapes[1], "Khớp Mã SHA-256")
    update_subshape_text(s11.shapes[10].shapes[1], "Đối chiếu thành công với mã hash gốc GitHub.")
    # Stat 3
    update_subshape_text(s11.shapes[12].shapes[1], "3.8x")
    update_subshape_text(s11.shapes[13].shapes[1], "Tăng Tốc Độ Tải")
    update_subshape_text(s11.shapes[14].shapes[1], "Nhanh hơn gấp ~4 lần so với tải đơn luồng.")
    # Big Card
    update_subshape_text(s11.shapes[17].shapes[1], "Đấu trường So sánh Hiệu năng (Benchmark Arena)")
    update_subshape_text(s11.shapes[18].shapes[1], "Tích hợp trang thứ 3 trên GUI cho phép chạy song song cả 2 chế độ (Có khóa vs Không khóa) trên cùng tệp mẫu để trực quan hóa biểu đồ cột so sánh thời gian, xung đột và mức tải CPU.")
    update_subshape_text(s11.shapes[19].shapes[1], "Thử nghiệm ổn định trên các tệp từ nhỏ (13MB), trung bình (110MB - 145MB) đến lớn (>1GB).")

    # -------------------------------------------------------------
    # SLIDE 12: KẾT LUẬN & HƯỚNG PHÁT TRIỂN (NGƯỜI 3)
    # -------------------------------------------------------------
    s12 = prs.slides[11]
    update_subshape_text(s12.shapes[1].shapes[1], "Kết luận Đề tài và Hướng Mở rộng")
    update_subshape_text(s12.shapes[2].shapes[1], "KẾT QUẢ ĐẠT ĐƯỢC")
    # Results
    update_subshape_text(s12.shapes[5].shapes[1], "Xây dựng thành công hệ thống tải file đa luồng đồng bộ theo chuẩn POSIX hoàn chỉnh bằng C.")
    update_subshape_text(s12.shapes[8].shapes[1], "Giải quyết triệt để bài toán Bounded Buffer với Semaphore và Mutex, bảo toàn 100% dữ liệu.")
    update_subshape_text(s12.shapes[11].shapes[1], "Giao diện GTK3 Jira SaaS mượt mà, trực quan hóa từng block RAM, biểu đồ mạng và đấu trường so sánh.")
    # Future work
    update_subshape_text(s12.shapes[12].shapes[1], "HƯỚNG PHÁT TRIỂN TIẾP THEO")
    update_subshape_text(s12.shapes[15].shapes[1], "Hỗ trợ Resume / Pause thông minh dựa trên bản đồ Disk Block Matrix đã lưu.")
    update_subshape_text(s12.shapes[18].shapes[1], "Bổ sung thuật toán điều phối động (Dynamic Rate Limiting) tự động cân bằng tốc độ ghi đĩa và mạng.")
    update_subshape_text(s12.shapes[21].shapes[1], "Đóng gói daemon dịch vụ nền (systemd) kết nối qua Unix Domain Socket IPC cho ứng dụng web.")

    # -------------------------------------------------------------
    # SLIDE 13: CẢM ƠN & Q&A (NGƯỜI 3)
    # -------------------------------------------------------------
    s13 = prs.slides[12]
    update_subshape_text(s13.shapes[1].shapes[1], "Xin Chân Thành Cảm Ơn!")
    update_subshape_text(s13.shapes[2].shapes[1], "Kính chúc Quý Thầy Cô và các bạn nhiều sức khỏe // Hỏi đáp và Thảo luận (Q&A)")

    print(f"Saving generated presentation to: {OUTPUT_PATH}")
    prs.save(OUTPUT_PATH)
    prs.save(ARTIFACT_OUTPUT)
    print("Successfully built and saved presentation!")

if __name__ == '__main__':
    main()
