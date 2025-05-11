# 数据库迁移
python manage.py makemigrations
python manage.py migrate
# 创建管理员
python manage.py createsuperuser
# 安装 django_restframework
pip install djangorestframework
djangorestframework 要有seralizers.py 文件
# 安装mpe 视频转换，用于提取缩略图
pip install moviepy
# 使用moviepy 把mp4转换为m3u8
from moviepy.editor import VideoFileClip
input_file = 'input.mp4'
output_m3u8 = 'output.m3u8'
clip = VideoFileClip(input_file)
options = {'hls_time': 10, 'hls_list_size': 0}
clip.write_videofile(output_m3u8, **options)

# ffmpeg
# 1.视频整体转码ts
ffmpeg -y -i music.mp4  -vcodec copy -acodec copy -vbsf h264_mp4toannexb out\music.ts
# 2. ts 文件切片
ffmpeg -i music.ts -c copy -map 0 -f segment -segment_list out\music.m3u8 -segment_time 10 out\15s_%3d.ts
ffmpeg -i demo.mp4 -profile:v baseline -level 3.0 -start_number 0 -hls_time 10 -hls_list_size 0 -f hls demo.m3u8
# vue uni-app vscode开发
1. 下载nodejs
2. 安装vue  npm install -g @vue/cli
3. 安装cli-init  npm install -g @vue/cli-init
4. 创建vue项目 vue init dcloudio/uni-preset-vue video-project
    1. npm init -y 生成package.json
    2. 把下面的复制到package.json中
    {
      "name": "video-project",
      "version": "1.0.0",
      "private": true,
      "scripts": {
        "serve": "uni serve",
        "build": "uni build",
        "info": "uni info"
      },
      "devDependencies": {
        "@dcloudio/uni-app-plus": "^3.0.0",
        "@dcloudio/uni-cli-i18n": "^1.0.0",
        "@dcloudio/uni-cli-shared": "^3.0.0",
        "@dcloudio/uni-h5": "^3.0.0",
        "@dcloudio/uni-mp-alipay": "^3.0.0",
        "@dcloudio/uni-mp-baidu": "^3.0.0",
        "@dcloudio/uni-mp-jd": "^3.0.0",
        "@dcloudio/uni-mp-kuaishou": "^3.0.0",
        "@dcloudio/uni-mp-lark": "^3.0.0",
        "@dcloudio/uni-mp-qq": "^3.0.0",
        "@dcloudio/uni-mp-toutiao": "^3.0.0",
        "@dcloudio/uni-mp-weixin": "^3.0.0",
        "@dcloudio/uni-pages-json": "^3.0.0",
        "@dcloudio/uni-rollup-plugin-json": "^1.0.0",
        "@dcloudio/uni-sass-transformer": "^1.0.0",
        "@dcloudio/uni-serverless-runner": "^1.0.0",
        "@dcloudio/uni-template-compiler": "^3.0.0",
        "@dcloudio/vue3-babel-proxy": "^1.0.0",
        "@dcloudio/vue3-compiler-uni-app": "^3.0.0",
        "@dcloudio/vue3-runtime-uni-app": "^3.0.0",
        "@dcloudio/vite-plugin-uni": "^3.0.0",
        "@vitejs/plugin-vue": "^4.0.0",
        "autoprefixer": "^10.4.0",
        "babel-eslint": "^10.1.0",
        "eslint": "^8.9.0",
        "eslint-config-prettier": "^8.3.0",
        "eslint-plugin-vue": "^9.0.0",
        "postcss": "^8.4.0",
        "prettier": "^2.6.0",
        "sass": "^1.62.1",
        "typescript": "^4.9.4",
        "unocss": "^0.54.0",
        "vite": "^4.1.0",
        "vue": "^3.2.0",
        "vue-loader": "^17.0.0",
        "webpack": "^5.76.3"
      }
    }
    3. npm install
    4. npm run serve
5. vscode 打开项目
6， 安装必要的扩展， Vetur, 高亮语法, ESLint, 帮助保持代码风格， Prettier , 代码格式化， uni-app Snippets,
提供Uni-app 相关的代码片段，加快开发速度
7. 运行和调试
npm run serve
8. 编译
npm run build
npm i mui-player --save
npm install hls.js --save
步骤1：安装MuiPlayer和hls.js

npm i mui-player --save
npm install hls.js --save
步骤2：引入MuiPlayer

import 'mui-player/dist/mui-player.min.css';
import MuiPlayer from 'mui-player';
import Hls from 'hls.js';
步骤3：在页面中使用MuiPlayer

<template>
<view>
<div id="mui-player"></div>
</view>
</template>

<script>
export default {
data() {
return {
mp: null,
};
},
mounted() {
this.$nextTick(() => {
this.mp = new MuiPlayer({
container: document.getElementById("mui-player"),
src: 'https://test-streams.mux.dev/x36xhzz/x36xhzz.m3u8',
parse: {
type: 'hls',
loader: Hls,
config: { debug: false },
},
pageHead: false,
});
});
},
beforeDestroy() {
this.mp.destroy();
},
};
</script>
后面改用uni-app 退出的工具进行开发了
django 解决跨域
1. pip install django-cors-headers
2. settings.py 中INSTALLED_APPS 添加 corsheaders
3. settings.py 中MIDDLEWARE 添加 corsheaders.middleware.CorsMiddleware
4. settings.py CORS_ALLOW_ALL_ORIGINS = True  允许所有域名都可以访问
5. CORS_ALLOWED_ORIGINS = [
"http://127.0.0.1:8000"
]  # 添加指定的域名才能访问
# 是否允许发送 Cookie 等凭据信息
CORS_ALLOW_CREDENTIALS = True

# 可以根据需要调整允许的方法和头部
CORS_ALLOW_METHODS = [
    "DELETE",
    "GET",
    "OPTIONS",
    "PATCH",
    "POST",
    "PUT",
]

CORS_ALLOW_HEADERS = [
    "accept",
    "accept-encoding",
    "authorization",
    "content-type",
    "dnt",
    "origin",
    "user-agent",
    "x-csrftoken",
    "x-requested-with",
]