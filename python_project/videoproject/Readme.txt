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

# url 路由
获取视频
http://127.0.0.1:8000/api/videos/series/
获取详情
http://127.0.0.1:8000/api/videos/episodes/


npm install @dcloudio/uni-ui
npm install --save-dev typescript vue-tsc
# 安装celery 用于处理上传视频
1. pip install celery redis
2. 在django项目根目录（与settings.py 同级） 创建celery.py
3. 在videoproject/__init__.py 中添加
4. settings.py 配置消息代理
5. 在应用目录下创建tasks.py
6. 在视图中调用异步任务
7. 启动celery worker
python -m celery -A videoproject worker --loglevel=info --pool=solo
8. 通过任务id查询任务状态

集成celery Flower
1. pip install flower
2. 配置django url
3. 启动python -m celery -A videoproject  flower --port=5555
4. 打开浏览器访问http://localhost:5555
- 可以查看实时任务状态
- Worker活动监控
- 任务历史和统计
- 支持任务重试，取消操作

pm2 管理启动命令
npm install pm2 -g
//pm2 start "python -m celery -A videoproject worker --loglevel=info --pool=solo" --name celery-worker
//pm2 start "python -m celery -A videoproject  flower --port=5555" --name celery-flower
//pm2 save
//pm2 startup

pm2 start ecosystem.config.js
pm2 save

# 添加python虚拟环境
 python3 -m venv python3env

django 部署
1. 收集静态文件
python manage.py collectstatic
2. 安装 pip install gunicorn
3. 配置nginx
4. 创建一个符号链接 使配置生效 sudo ln -s /etc/nginx/sites-available/yourprojectname
/etc/nginx/sites-enabled
5. 检测nginx -t 配置是否正确
6. 重启nginx sudo systemctl restart nginx
7. 设置gunicorn 服务
8. 启动gunicorn 服务
sudo systemctl start gunicorn
sudo systemctl enable gunicorn

安装certbot 免费获取ssl证书
pip install certbot
ln -s /opt/cerbot/bin/certbot /usr/bin/certbot
certbot certonly --standalone

# cloudflare 和certbot
1. sudo apt install python3-certbot-dns-cloudflare
2. vi /etc/letsencrypt/cloudflare.ini
3. 设置cloudflare  api 令牌 ,选择区域， DNS, 编辑， 区域资源 包括，特定区域，369924.xyz
4. sudo certbot certonly   --dns-cloudflare   --dns-cloudflare-credentials /etc/letsencrypt/cloudflare.ini   --key-type ecdsa   --domain "369924.xyz,*.369924.xyz"
5. 申请成功后，会有两个文件
/etc/letsencrypt/live/369924.xyz/fullchain.pem
/etc/letsencrypt/live/369924.xyz/privkey.pem
6. 配置定时任务，更新ssl证书
0 0 * * * /usr/bin/certbot renew --quiet >> /var/log/letsencrypt/renew.log 2>&1
certbot renew
自签名ssl
sudo openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout /etc/nginx/ssl/nginx-selfsigned.key -out /etc/nginx/ssl/nginx-selfsigned.crt -subj "/C=US/ST=State/L=City/O=Organization/CN=localhost"
sudo nano /etc/nginx/sites-available/default
nginx -t 检查语法是否错误
sudo systemctl reload nginx 重启

生成ssh 密钥
ssh-keygen -t ed25519 -C "your_email@example.com"
gunicorn.service 放到这个目录
    /etc/systemd/system/gunicorn.service

# 安装django simpleui
pip install django-simpleui

# 在liunx 环境上部署 mattermost

sudo  apt-get update
sudo apt-get install -y wget curl unzip

sudo apt-get install -y postgresql postgresql-contrib

sudo systemctl start postgresql
sudo systemctl enable postgresql
sudo -u postgres psql

CREATE DATABASE mattermost;
CREATE USER mattermost WITH PASSWORD 'your_password';
ALTER ROLE mattermost SET client_encoding TO 'utf8';
ALTER ROLE mattermost SET default_transaction_isolation TO 'read committed';
ALTER ROLE mattermost SET timezone TO 'UTC';
GRANT ALL PRIVILEGES ON DATABASE mattermost TO mattermost;
\q
登陆postgresql, 授予mattermost权限
sudo -u postgres psql
\c mattermost
GRANT ALL PRIVILEGES ON SCHEMA public TO mattermost;
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO mattermost;
GRANT ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public TO mattermos
ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT ALL PRIVILEGES ON TABLES TO mattermost;
ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT ALL PRIVILEGES ON SEQUENCES TO mattermost;
\q

wget  https://releases.mattermost.com/10.8.1/mattermost-team-10.8.1-linux-amd64.tar.gz
tar -zxvf mattermost-team-10.8.1-linux-amd64.tar.gz  -C /home/fantom/
cd mattermost
mkdir data
vi  config/config.json, 修改
"DriverName": "postgres",
"DataSource": "postgres://xxxxx:xxxx@localhost:5432/mattermost?sslmode=disable&connect_timeout=10&binary_parameters=yes",
启动
./bin/mattermost

用pm2 管理mattermost
