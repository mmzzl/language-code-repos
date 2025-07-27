import tkinter as tk
from tkinter import ttk, messagebox
import time
import winsound
from datetime import datetime, timedelta

class PomodoroTimer:
    def __init__(self, root):
        self.root = root
        self.root.title("番茄钟")
        self.root.overrideredirect(True)  # 无边框窗口
        self.root.attributes('-topmost', True)  # 窗口置顶
        self.root.configure(bg='#2c3e50')
        self.root.resizable(False, False)

        # 设置窗口大小和初始位置（屏幕右下角）
        self.window_width = 250
        self.window_height = 150
        self.screen_width = self.root.winfo_screenwidth()
        self.screen_height = self.root.winfo_screenheight()
        self.x_pos = self.screen_width - self.window_width - 20
        self.y_pos = self.screen_height - self.window_height - 80
        self.root.geometry(f'{self.window_width}x{self.window_height}+{self.x_pos}+{self.y_pos}')

        # 拖动相关变量
        self.dragging = False
        self.drag_start_x = 0
        self.drag_start_y = 0

        # 番茄钟状态
        self.work_time = 25 * 60  # 25分钟工作
        self.break_time = 5 * 60  # 5分钟休息
        self.current_time = self.work_time
        self.is_working = True
        self.is_running = False
        self.timer_id = None

        # 创建UI元素
        self.create_widgets()

        # 绑定拖动事件
        self.bind_drag_events()

    def create_widgets(self):
        # 创建标题栏（用于拖动）
        self.title_bar = tk.Frame(self.root, bg='#34495e', height=25)
        self.title_bar.pack(fill=tk.X)

        # 关闭按钮
        self.close_btn = tk.Label(self.title_bar, text='✕', bg='#34495e', fg='white', cursor='hand2')
        self.close_btn.pack(side=tk.RIGHT, padx=5)
        self.close_btn.bind('<Button-1>', lambda e: self.root.destroy())

        # 时间显示
        self.time_label = tk.Label(
            self.root, 
            text=self.format_time(self.current_time), 
            font=('Helvetica', 40, 'bold'), 
            bg='#2c3e50', 
            fg='white'
        )
        self.time_label.pack(pady=10)

        # 按钮框架
        self.btn_frame = tk.Frame(self.root, bg='#2c3e50')
        self.btn_frame.pack(fill=tk.X, padx=20)

        # 开始/暂停按钮
        self.start_btn = ttk.Button(
            self.btn_frame, 
            text='开始', 
            command=self.toggle_timer
        )
        self.start_btn.pack(side=tk.LEFT, padx=5)

        # 重置按钮
        self.reset_btn = ttk.Button(
            self.btn_frame, 
            text='重置', 
            command=self.reset_timer
        )
        self.reset_btn.pack(side=tk.RIGHT, padx=5)

        # 状态标签
        self.status_label = tk.Label(
            self.root, 
            text='工作时间', 
            bg='#2c3e50', 
            fg='#3498db'
        )
        self.status_label.pack(pady=5)

    def bind_drag_events(self):
        # 绑定标题栏拖动事件
        self.title_bar.bind('<Button-1>', self.start_drag)
        self.title_bar.bind('<B1-Motion>', self.on_drag)
        self.title_bar.bind('<ButtonRelease-1>', self.stop_drag)

        # 鼠标样式
        self.title_bar.config(cursor='fleur')

    def start_drag(self, event):
        self.dragging = True
        self.drag_start_x = event.x
        self.drag_start_y = event.y

    def on_drag(self, event):
        if self.dragging:
            x = self.root.winfo_x() + event.x - self.drag_start_x
            y = self.root.winfo_y() + event.y - self.drag_start_y
            self.root.geometry(f'+{x}+{y}')

    def stop_drag(self, event):
        self.dragging = False

    def format_time(self, seconds):
        mins, secs = divmod(seconds, 60)
        return f'{mins:02d}:{secs:02d}'

    def update_timer(self):
        if self.current_time > 0 and self.is_running:
            self.current_time -= 1
            self.time_label.config(text=self.format_time(self.current_time))
            self.timer_id = self.root.after(1000, self.update_timer)
        elif self.current_time == 0:
            # 时间到，切换状态
            self.switch_mode()
            # 播放提示音
            winsound.Beep(1000, 500)
            messagebox.showinfo('时间到', f'{"休息" if self.is_working else "工作"}时间结束！')

    def toggle_timer(self):
        if self.is_running:
            self.root.after_cancel(self.timer_id)
            self.start_btn.config(text='继续')
            self.is_running = False
        else:
            self.is_running = True
            self.update_timer()
            self.start_btn.config(text='暂停')

    def reset_timer(self):
        if self.timer_id:
            self.root.after_cancel(self.timer_id)
        self.current_time = self.work_time if self.is_working else self.break_time
        self.time_label.config(text=self.format_time(self.current_time))
        self.is_running = False
        self.start_btn.config(text='开始')

    def switch_mode(self):
        self.is_working = not self.is_working
        self.current_time = self.work_time if self.is_working else self.break_time
        self.time_label.config(text=self.format_time(self.current_time))
        self.status_label.config(
            text='工作时间' if self.is_working else '休息时间',
            fg='#3498db' if self.is_working else '#2ecc71'
        )
        # 切换窗口颜色
        self.root.configure(bg='#2c3e50' if self.is_working else '#1abc9c')
        self.time_label.configure(bg='#2c3e50' if self.is_working else '#1abc9c')
        self.btn_frame.configure(bg='#2c3e50' if self.is_working else '#1abc9c')
        self.status_label.configure(bg='#2c3e50' if self.is_working else '#1abc9c')
        # 自动启动计时器
        self.is_running = True
        self.update_timer()
        self.start_btn.config(text='暂停')

if __name__ == '__main__':
    root = tk.Tk()
    app = PomodoroTimer(root)
    root.mainloop()