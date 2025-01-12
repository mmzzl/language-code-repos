from PIL import Image

def pixels_to_rgb565(pixels):
    rgb565_data = []
    for pixel in pixels:
        r, g, b = pixel
        # 提取红色通道的高5位
        red = (r >> 3) & 0x1F
        # 提取绿色通道的高6位
        green = ((g >> 2) & 0x3F)
        # 提取蓝色通道的高5位
        blue = (b >> 3) & 0x1F
        # 组合成RGB565值
        rgb565_value = (red << 11) | (green << 5) | blue
        rgb565_data.append(rgb565_value)
    return rgb565_data


def rgb565_to_c_array(rgb565_data, array_name):
    c_array = ""
    for value in rgb565_data:
        c_array += f"0x{value:04X}, "
    # 生成C数组代码
    c_code = f"unsigned short {array_name}[] = {{{c_array}}};".replace(", }", "}")
    return c_code


if __name__ == "__main__":
    file_path = 'jpg/bg.jpg'  # 替换为实际的图像文件路径
    # 读取图像文件
    image = Image.open(file_path)
    # 获取图像的像素数据
    pixels = list(image.getdata())
    # 将像素数据转换为RGB565格式
    rgb565_data = pixels_to_rgb565(pixels)
    # 将RGB565数据转换为C数组格式
    array_name = "image_data"
    c_array_code = rgb565_to_c_array(rgb565_data, array_name)
    print(c_array_code)