import tkinter as tk
from tkinter import filedialog
from PIL import Image
import os
import subprocess


def convert_image():
    try:
        # 获取输入的长度和宽度
        width = int(width_entry.get())
        height = int(height_entry.get())

        # 选择 GIF 图片文件
        file_path = filedialog.askopenfilename(filetypes=[("GIF files", "*.gif")])
        if file_path:
            # 打开 GIF 图片
            img = Image.open(file_path)

            # 调整图片大小
            resized_img = img.resize((width, height), Image.Resampling.LANCZOS)

            # 选择输出目录
            output_dir = filedialog.askdirectory()
            if output_dir:
                # 生成输出文件名
                base_name = os.path.basename(file_path).split('.')[0]
                output_path = os.path.join(output_dir, f"{base_name}.bmp")

                # 保存为 BMP 格式
                resized_img.save(output_path, "BMP")
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
root.title("GIF 转 BMP")

# 创建长度和宽度输入框
width_label = tk.Label(root, text="宽度:")
width_label.pack()
width_entry = tk.Entry(root)
width_entry.pack()

height_label = tk.Label(root, text="高度:")
height_label.pack()
height_entry = tk.Entry(root)
height_entry.pack()

# 创建转换按钮
convert_button = tk.Button(root, text="转换", command=convert_image)
convert_button.pack()

# 创建结果显示标签
result_label = tk.Label(root, text="")
result_label.pack()

# 运行主循环
root.mainloop()