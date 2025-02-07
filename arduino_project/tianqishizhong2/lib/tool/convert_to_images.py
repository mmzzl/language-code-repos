import tkinter as tk
from tkinter import filedialog
from PIL import Image
import os
import subprocess


def convert_gif_frames():
    try:
        # 获取用户输入的宽度和高度
        width = int(width_entry.get())
        height = int(height_entry.get())

        # 让用户选择要转换的 GIF 文件
        gif_path = filedialog.askopenfilename(filetypes=[("GIF files", "*.gif")])
        if not gif_path:
            return

        # 让用户选择输出目录
        output_dir = filedialog.askdirectory()
        if not output_dir:
            return

        # 打开 GIF 图片
        gif = Image.open(gif_path)

        frame_num = 0
        try:
            while True:
                # 调整当前帧的大小
                resized_frame = gif.resize((width, height), Image.Resampling.LANCZOS)

                # 生成输出文件路径
                output_path = os.path.join(output_dir, f"frame_{frame_num}.bmp")

                # 保存当前帧为 BMP 格式
                resized_frame.save(output_path, "BMP")

                frame_num += 1
                # 移动到下一帧
                gif.seek(frame_num)
        except EOFError:
            pass

        result_label.config(text="转换成功！")

        # 打开输出目录
        if os.name == 'nt':  # Windows 系统
            os.startfile(output_dir)
        elif os.name == 'posix':  # Linux 或 macOS 系统
            subprocess.call(['open', output_dir])

    except ValueError:
        result_label.config(text="请输入有效的长度和宽度。")
    except Exception as e:
        result_label.config(text=f"转换失败：{str(e)}")


# 创建主窗口
root = tk.Tk()
root.title("GIF 帧转 BMP")

# 创建宽度输入框及标签
width_label = tk.Label(root, text="宽度:")
width_label.pack()
width_entry = tk.Entry(root)
width_entry.pack()

# 创建高度输入框及标签
height_label = tk.Label(root, text="高度:")
height_label.pack()
height_entry = tk.Entry(root)
height_entry.pack()

# 创建转换按钮
convert_button = tk.Button(root, text="转换", command=convert_gif_frames)
convert_button.pack()

# 创建结果显示标签
result_label = tk.Label(root, text="")
result_label.pack()

# 运行主循环
root.mainloop()