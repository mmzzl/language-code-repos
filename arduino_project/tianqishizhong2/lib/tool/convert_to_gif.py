from PIL import Image,ImageOps
import os, traceback
from io import BytesIO
import binascii

def processImage(in_file, size, saveImg=True):
  
    im = Image.open(in_file)
    # 截取文件名
    filename = in_file.split('.')[0]
    folderName = in_file.split('.')[0]
    cacheFileName =filename 
    filename = 'taikongren'
    
    i = 0
    arr_name_all = ''  # 存取数组
    arr_size_all = ''  # 存储数组容量

    try:
        if not os.path.exists(folderName):
            os.mkdir(folderName)
        with open(cacheFileName + '.h', 'w', encoding='utf-8') as f:  # 写入文件
            f.write('#include <pgmspace.h> \n\n')
            while 1:
                print('.', end="")
                new_im = Image.new("RGB", im.size)
                new_im.paste(im)

                # 缩放图像，
                width = new_im.size[0]  # 获取原始图像宽度
                height = new_im.size[1]  # 获取原始图像高度
                # 等比例缩放后的图像高度，根据实际需要调整
                print(width, " ", height)
                if height > size[1] or width > size[0]:
                    new_im = ImageOps.fit(new_im,size,Image.LANCZOS)
                print("after", new_im.size[0], " ", new_im.size[1])
                # 获取图像字节流，转16进制格式
                img_byte = BytesIO()  # 获取字节流
                new_im.save(img_byte, format='jpeg')
                
                # 16进制字符串
                img_hex = binascii.hexlify(img_byte.getvalue()).decode('utf-8')  
                
                arr_name = filename + '_' + str(i)
                arr_size = 0  # 记录数组长度
                arr_name_all += arr_name + ','

                # 将ac --> 0xac
                f.write('const uint8_t ' + arr_name + '[] PROGMEM = { \n')  # 写前
                for index, x in zip(range(len(img_hex)), range(0, len(img_hex), 2)):
                    temp_hex = '0x' + img_hex[x:x + 2] + ', '
                    # 30个数据换行
                    if (index + 1) % 30 == 0:
                        temp_hex += '\n'

                    f.write(temp_hex)  # 写入文件
                    arr_size += 1
                f.write('\n};\n\n')  # 写结尾
                i += 1
                arr_size_all += str(arr_size) + ','

                # 保存一帧帧图像
                if saveImg:
                    if not os.path.exists('./out_img'):
                        os.mkdir('./out_img')
                    if not os.path.exists('./out_img/' + filename):
                        os.mkdir('./out_img/' + filename)
                    new_im.save('./out_img/' + filename + '/' + str(i) + '.jpg')

                try:
                    im.seek(im.tell() + 1)
                except EOFError:
                    # 动图读取结束
                    f.write('const uint8_t *' + filename + '[' + str(i) + '] PROGMEM = { ' + arr_name_all + '};\n')
                    f.write('const uint32_t ' + filename + '_size[' + str(i) + '] PROGMEM = { ' + arr_size_all + '};')
                    print("成功保存文件为：" + filename + '.h')
                    # # 再生成一个 ino arduino文件
                    # with open(cacheFileName + '.ino', 'w', encoding='utf-8') as f2:  # 写入文件
                    #     # 写个头文件
                    #     f2.write(inoInfo.replace('[gif]',filename))
                    break

    except EOFError as e:
        print(e.args)
        print(traceback.format_exc())
        pass  # end of sequence

processImage("C:\\Users\\life8\\Pictures\\xiaogou.gif", (48, 48), False)