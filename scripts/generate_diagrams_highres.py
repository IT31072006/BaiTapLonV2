#!/usr/bin/env python3
import os
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from PIL import Image

os.makedirs('assets/presentation_images', exist_ok=True)

# -------------------------------------------------------------
# 1. CROP & ENHANCE UI SCREENSHOTS
# -------------------------------------------------------------
print("Cropping and enhancing UI screenshots...")

# Slide 9: Concurrency Lab Workstation
im1 = Image.open('assets/presentation_images/media_1794470338449_2beaa0c4.jpg')
# Crop the main white workstation window tightly
box1 = (46, 105, 975, 510)
crop1 = im1.crop(box1)
crop1.save('assets/presentation_images/crop_concurrency_lab.png')
print("Saved crop_concurrency_lab.png")

# Slide 10: Download Session History & Integrity Audit
im2 = Image.open('assets/presentation_images/media_1794469653925_b03440fc.jpg')
# Crop the dark audit window tightly
box2 = (35, 98, 970, 485)
crop2 = im2.crop(box2)
crop2.save('assets/presentation_images/crop_session_history.png')
print("Saved crop_session_history.png")

# Slide 10: Gemini Direct Range Test Links
im3 = Image.open('assets/presentation_images/media_1794469091268_5ab5ccfb.jpg')
# Crop the test links area
box3 = (380, 40, 800, 420)
crop3 = im3.crop(box3)
crop3.save('assets/presentation_images/crop_gemini_links.png')
print("Saved crop_gemini_links.png")


# -------------------------------------------------------------
# 2. GENERATE HIGH-RES DIAGRAM 1: HTTP Range Request (Slide 3)
# -------------------------------------------------------------
print("Generating high-res fig_http_range.png...")
fig, ax = plt.subplots(figsize=(10, 5.5), dpi=300)
ax.set_facecolor('#F8FAFC')
fig.patch.set_facecolor('#F8FAFC')

# Remote Web Server
ax.add_patch(patches.FancyBboxPatch((0.5, 3.4), 3.4, 1.4, boxstyle='round,pad=0.15', facecolor='#091E42', edgecolor='#0C66E4', lw=2.5))
ax.text(2.2, 4.25, 'MÁY CHỦ WEB TỪ XA\nHTTP / NGINX / GITHUB CDN', color='#FFFFFF', ha='center', va='center', fontsize=12, fontweight='bold')
ax.text(2.2, 3.75, 'Header: Accept-Ranges: bytes', color='#38BDF8', ha='center', va='center', fontsize=11, fontweight='bold')

# File on Server
ax.add_patch(patches.Rectangle((0.5, 1.0), 3.4, 1.7, facecolor='#FFFFFF', edgecolor='#64748B', lw=2))
ax.text(2.2, 2.35, 'Tệp Dữ Liệu Gốc (100 MB)', color='#0F172A', ha='center', va='center', fontsize=12, fontweight='bold')
colors = ['#0284C7', '#16A34A', '#D97706', '#DB2777']
bg_colors = ['#E0F2FE', '#DCFCE7', '#FEF3C7', '#FCE7F3']
for i in range(4):
    ax.add_patch(patches.Rectangle((0.55 + i*0.82, 1.15), 0.76, 0.85, facecolor=bg_colors[i], edgecolor=colors[i], lw=2))
    ax.text(0.93 + i*0.82, 1.68, f'Chunk #{i+1}', color=colors[i], ha='center', va='center', fontsize=10, fontweight='bold')
    ax.text(0.93 + i*0.82, 1.38, f'{i*25}-{(i+1)*25}M', color='#1E293B', ha='center', va='center', fontsize=9, fontweight='bold')

# 4 Parallel Producer Threads on Client
for i in range(4):
    y_pos = 4.2 - i*1.0
    # Connection line / Arrow
    ax.annotate('', xy=(5.4, y_pos), xytext=(4.0, 2.4),
                arrowprops=dict(arrowstyle='->', color=colors[i], lw=3, connectionstyle='arc3,rad=' + str(0.18 - i*0.12)))
    # Producer thread box
    ax.add_patch(patches.FancyBboxPatch((5.4, y_pos-0.35), 4.1, 0.75, boxstyle='round,pad=0.1', facecolor=bg_colors[i], edgecolor=colors[i], lw=2))
    ax.text(7.45, y_pos+0.08, f'Producer Thread #{i+1}', color=colors[i], ha='center', va='center', fontsize=12, fontweight='bold')
    ax.text(7.45, y_pos-0.18, f'GET Range: bytes={i*25}M-{(i+1)*25}M', color='#0F172A', ha='center', va='center', fontsize=10.5, fontweight='bold')

ax.set_xlim(0.2, 9.8)
ax.set_ylim(0.6, 5.2)
ax.axis('off')
plt.tight_layout()
plt.savefig('assets/presentation_images/fig_http_range.png', bbox_inches='tight')
plt.close()
print("Saved fig_http_range.png")


# -------------------------------------------------------------
# 3. GENERATE HIGH-RES DIAGRAM 2: Architecture & Bounded Buffer (Slide 5)
# -------------------------------------------------------------
print("Generating high-res fig_architecture.png...")
fig, ax = plt.subplots(figsize=(10, 5.5), dpi=300)
ax.set_facecolor('#F8FAFC')
fig.patch.set_facecolor('#F8FAFC')

# Producers Column
ax.add_patch(patches.FancyBboxPatch((0.3, 0.5), 2.7, 4.4, boxstyle='round,pad=0.15', facecolor='#EFF6FF', edgecolor='#2563EB', lw=2))
ax.text(1.65, 4.55, 'NETWORK PRODUCERS\n(4 Luồng Tải Mạng)', color='#1E40AF', ha='center', va='center', fontsize=12, fontweight='bold')
for i in range(4):
    ax.add_patch(patches.FancyBboxPatch((0.5, 3.4 - i*0.9), 2.3, 0.68, boxstyle='round,pad=0.08', facecolor='#FFFFFF', edgecolor='#3B82F6', lw=1.8))
    ax.text(1.65, 3.82 - i*0.9, f'Producer #{i+1}', color='#1D4ED8', ha='center', va='center', fontsize=11, fontweight='bold')
    ax.text(1.65, 3.52 - i*0.9, 'libcurl HTTP Range', color='#475569', ha='center', va='center', fontsize=10)

# Circular Bounded Buffer Column
ax.add_patch(patches.FancyBboxPatch((3.5, 0.5), 3.2, 4.4, boxstyle='round,pad=0.15', facecolor='#FEF3C7', edgecolor='#D97706', lw=2))
ax.text(5.1, 4.55, 'RAM BOUNDED BUFFER\n(32 Slots Circular Map)', color='#92400E', ha='center', va='center', fontsize=12, fontweight='bold')
for i in range(4):
    ax.add_patch(patches.Rectangle((3.75, 3.55 - i*0.55), 2.7, 0.45, facecolor='#FFFFFF', edgecolor='#B45309', lw=1.2))
    ax.text(5.1, 3.77 - i*0.55, f'Slot [{i}]: 64KB Chunk Buffer', color='#78350F', ha='center', va='center', fontsize=10.5, fontweight='bold')
ax.text(5.1, 1.65, '... (Tổng cộng 32 Slots = 2.0 MB) ...', color='#92400E', ha='center', va='center', fontsize=10, fontstyle='italic')

# Sync Primitives Badge
ax.add_patch(patches.FancyBboxPatch((3.7, 0.65), 2.8, 0.8, boxstyle='round,pad=0.08', facecolor='#FDE68A', edgecolor='#B45309', lw=1.8))
ax.text(5.1, 1.15, 'ĐỒNG BỘ HÓA ĐỘC QUYỀN', color='#78350F', ha='center', va='center', fontsize=10.5, fontweight='bold')
ax.text(5.1, 0.85, 'sem_empty / sem_full / Mutex', color='#92400E', ha='center', va='center', fontsize=10, fontweight='bold')

# Consumers Column
ax.add_patch(patches.FancyBboxPatch((7.2, 2.2), 2.5, 2.7, boxstyle='round,pad=0.15', facecolor='#F0FDF4', edgecolor='#16A34A', lw=2))
ax.text(8.45, 4.55, 'STORAGE CONSUMERS\n(2 Luồng Ghi Đĩa)', color='#166534', ha='center', va='center', fontsize=12, fontweight='bold')
for i in range(2):
    ax.add_patch(patches.FancyBboxPatch((7.4, 3.4 - i*1.0), 2.1, 0.75, boxstyle='round,pad=0.08', facecolor='#FFFFFF', edgecolor='#15803D', lw=1.8))
    ax.text(8.45, 3.85 - i*1.0, f'Consumer #{i+1}', color='#15803D', ha='center', va='center', fontsize=11, fontweight='bold')
    ax.text(8.45, 3.55 - i*1.0, 'pwrite64() Direct I/O', color='#475569', ha='center', va='center', fontsize=10)

# Disk Box
ax.add_patch(patches.FancyBboxPatch((7.2, 0.5), 2.5, 1.3, boxstyle='round,pad=0.12', facecolor='#F1F5F9', edgecolor='#475569', lw=2))
ax.text(8.45, 1.3, 'Ổ ĐĨA ĐÍCH (NVMe/SSD)', color='#0F172A', ha='center', va='center', fontsize=11.5, fontweight='bold')
ax.text(8.45, 0.9, 'Tệp Đích Hoàn Chỉnh\n(Xác thực mã SHA-256)', color='#334155', ha='center', va='center', fontsize=9.5)

# Connecting Arrows
ax.annotate('', xy=(3.5, 2.9), xytext=(3.0, 2.9), arrowprops=dict(arrowstyle='->', color='#2563EB', lw=3.5))
ax.annotate('', xy=(7.2, 3.3), xytext=(6.7, 3.3), arrowprops=dict(arrowstyle='->', color='#16A34A', lw=3.5))
ax.annotate('', xy=(8.45, 1.8), xytext=(8.45, 2.2), arrowprops=dict(arrowstyle='->', color='#15803D', lw=3))

ax.set_xlim(0.1, 9.9)
ax.set_ylim(0.3, 5.2)
ax.axis('off')
plt.tight_layout()
plt.savefig('assets/presentation_images/fig_architecture.png', bbox_inches='tight')
plt.close()
print("Saved fig_architecture.png")


# -------------------------------------------------------------
# 4. GENERATE HIGH-RES DIAGRAM 3: Safe Sync vs Race Condition (Slide 7)
# -------------------------------------------------------------
print("Generating high-res fig_race_condition.png...")
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 5.5), dpi=300)
for ax in (ax1, ax2):
    ax.set_facecolor('#F8FAFC')
fig.patch.set_facecolor('#F8FAFC')

# Left: Safe Sync
ax1.set_title('CHẾ ĐỘ CÓ KHÓA (SAFE SYNCHRONIZED)', fontsize=13, fontweight='bold', color='#166534', pad=14)

ax1.add_patch(patches.FancyBboxPatch((0.4, 3.3), 3.2, 1.0, boxstyle='round,pad=0.1', facecolor='#DCFCE7', edgecolor='#16A34A', lw=2))
ax1.text(2.0, 3.95, 'BẢO VỆ BẰNG KHÓA', color='#14532D', ha='center', va='center', fontsize=12, fontweight='bold')
ax1.text(2.0, 3.55, 'sem_wait() + Mutex Lock\nTruy cập RAM tuần tự', color='#166534', ha='center', va='center', fontsize=10.5)

ax1.add_patch(patches.FancyBboxPatch((0.4, 1.9), 3.2, 1.0, boxstyle='round,pad=0.1', facecolor='#EFF6FF', edgecolor='#2563EB', lw=2))
ax1.text(2.0, 2.55, 'VÙNG TRANH CHẤP AN TOÀN', color='#1E40AF', ha='center', va='center', fontsize=12, fontweight='bold')
ax1.text(2.0, 2.15, 'Mỗi slot chỉ 1 luồng can thiệp\n0 biến cố va chạm (0 Collisions)', color='#1E3A8A', ha='center', va='center', fontsize=10)

ax1.add_patch(patches.FancyBboxPatch((0.4, 0.4), 3.2, 1.1, boxstyle='round,pad=0.1', facecolor='#DCFCE7', edgecolor='#16A34A', lw=2.5))
ax1.text(2.0, 1.1, 'KẾT QUẢ: TOÀN VẸN 100%', color='#14532D', ha='center', va='center', fontsize=13, fontweight='bold')
ax1.text(2.0, 0.65, 'Mã SHA-256 Khớp Hoàn Toàn\n256/256 Khối Đĩa Ghi Đúng', color='#15803D', ha='center', va='center', fontsize=11, fontweight='bold')

ax1.annotate('', xy=(2.0, 2.9), xytext=(2.0, 3.3), arrowprops=dict(arrowstyle='->', color='#16A34A', lw=3))
ax1.annotate('', xy=(2.0, 1.5), xytext=(2.0, 1.9), arrowprops=dict(arrowstyle='->', color='#16A34A', lw=3))
ax1.set_xlim(0.1, 3.9)
ax1.set_ylim(0.2, 4.6)
ax1.axis('off')

# Right: Race Condition
ax2.set_title('CHẾ ĐỘ KHÔNG KHÓA (RACE CONDITION)', fontsize=13, fontweight='bold', color='#991B1B', pad=14)

ax2.add_patch(patches.FancyBboxPatch((0.4, 3.3), 3.2, 1.0, boxstyle='round,pad=0.1', facecolor='#FEE2E2', edgecolor='#DC2626', lw=2))
ax2.text(2.0, 3.95, 'BỎ QUA TOÀN BỘ KHÓA', color='#7F1D1D', ha='center', va='center', fontsize=12, fontweight='bold')
ax2.text(2.0, 3.55, 'Không Semaphore, Không Mutex\nCác luồng tự do chen lấn', color='#991B1B', ha='center', va='center', fontsize=10.5)

ax2.add_patch(patches.FancyBboxPatch((0.4, 1.9), 3.2, 1.0, boxstyle='round,pad=0.1', facecolor='#FEF2F2', edgecolor='#EF4444', lw=2.5, ls='--'))
ax2.text(2.0, 2.55, 'XUNG ĐỘT GHI ĐÈ (OVERWRITE)', color='#B91C1C', ha='center', va='center', fontsize=12, fontweight='bold')
ax2.text(2.0, 2.15, 'Nhiều luồng ghi đè cùng 1 slot RAM\nLàm mất dữ liệu các chunk khác', color='#991B1B', ha='center', va='center', fontsize=10)

ax2.add_patch(patches.FancyBboxPatch((0.4, 0.4), 3.2, 1.1, boxstyle='round,pad=0.1', facecolor='#FEE2E2', edgecolor='#DC2626', lw=2.5))
ax2.text(2.0, 1.1, 'KẾT QUẢ: HỎNG DỮ LIỆU!', color='#7F1D1D', ha='center', va='center', fontsize=13, fontweight='bold')
ax2.text(2.0, 0.65, 'MÃ HASH SHA-256 BỊ SAI LỆCH\nTệp Tin Bị Lỗi (Corrupted File)', color='#991B1B', ha='center', va='center', fontsize=11, fontweight='bold')

ax2.annotate('', xy=(2.0, 2.9), xytext=(2.0, 3.3), arrowprops=dict(arrowstyle='->', color='#DC2626', lw=3))
ax2.annotate('', xy=(2.0, 1.5), xytext=(2.0, 1.9), arrowprops=dict(arrowstyle='->', color='#DC2626', lw=3))
ax2.set_xlim(0.1, 3.9)
ax2.set_ylim(0.2, 4.6)
ax2.axis('off')

plt.tight_layout()
plt.savefig('assets/presentation_images/fig_race_condition.png', bbox_inches='tight')
plt.close()
print("Saved fig_race_condition.png")

print("All high-res diagrams and cropped screenshots generated successfully!")
