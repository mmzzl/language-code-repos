import os
from PIL import Image

def bmp_to_rgb565(bmp_path):
    # 打开 BMP 图像

    image = Image.open(bmp_path)
    # 确保图像为 RGB 模式
    image = image.convert('RGB')
    width, height = image.size
    rgb565_data = bytearray()

    for y in range(height):
        for x in range(width):
            r, g, b = image.getpixel((x, y))
            # 将 RGB 颜色转换为 RGB565 格式
            rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
            # 将 16 位的 RGB565 拆分为两个 8 位的字节
            high_byte = (rgb565 >> 8) & 0xFF
            low_byte = rgb565 & 0xFF
            rgb565_data.extend([high_byte, low_byte])

    return rgb565_data

def bpm_to_rgb565_h(bmp_data, output_path):
    # 生成 .h 文件内容
    # header_content = f"#ifndef IMAGE_DATA_H\n#define IMAGE_DATA_H\n\n"
    data_list = []
    # 存储每个图片的名称
    image_name = []
    # 存储每个图片的数据长度
    image_length = []
    for index, rgb565_data in enumerate(bmp_data):
        name = "taikongren_%s" % index
        image_name.append(name)
        image_length.append(len(rgb565_data))
        header_content = ""
        header_content += f"const uint8_t %s[] = {{\n" % name
        for i, byte in enumerate(rgb565_data):
            header_content += f"0x{byte:02X}"
            if i < len(rgb565_data) - 1:
                header_content += ", "
            if (i + 1) % 16 == 0:
                header_content += "\n"
        header_content += "\n};"
        data_list.append(header_content)
    
    data_list.insert(0,  f"#ifndef IMAGE_DATA_H\n#define IMAGE_DATA_H\n\n")
    data_list.append('static const uint8_t  *astronaut[%s] PROGMEM = {%s};' % (len(bmp_data), ",".join(image_name)))
    data_list.append('static const uint32_t astronaut_size[%s] PROGMEM = {%s};' %(len(bmp_data), ",".join([str(i) for i in image_length])))
    data_list.append("#endif")
    # print(data_list)

    # 将内容写入 .h 文件
    if not os.path.exists(output_path):
        os.makedirs(output_path)
    output = os.path.join(output_path, 'image_data.h')
    with open(output, 'w') as f:
        f.write("\n".join(data_list))

# 示例用法
bmp_path = 'C:\\Users\\life8\\Pictures\\taikongren\\'
output_path = 'C:\\Users\\life8\\Pictures\\taikongren\\h\\'
rgb565_data = []
for bmp_picture_name in os.listdir(bmp_path):
    bmp_path_file = os.path.join(bmp_path, bmp_picture_name)
    if os.path.isfile(bmp_path_file):
        output_path_file = os.path.join(output_path, bmp_picture_name.split('.')[0] + '.h')
        rgb565_data.append(bmp_to_rgb565(bmp_path_file))
print(len(rgb565_data))
bpm_to_rgb565_h(rgb565_data, output_path)
# output_path = 'image_data.h'
# bmp_to_rgb565_h(bmp_path, output_path)
