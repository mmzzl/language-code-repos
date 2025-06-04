<template>
  <view class="webview-container">
    <web-view :src="url" class="web-view" @load="handleLoad"></web-view>
    <custom-tab-bar class="custom-tab-bar"></custom-tab-bar>
  </view>
</template>

<script>
export default {
  data() {
    return {
      url: 'https://www.life233.top/mattermost',
      statusbar: 0,
      height: 0,
      originalHeight: 0 // 新增变量用于保存原始高度
    };
  },
  onLoad(option) {
    uni.getSystemInfo({
      success: (sysinfo) => {
        this.statusbar = sysinfo.statusBarHeight;
        this.height = sysinfo.windowHeight;
        this.originalHeight = this.height - this.statusbar - 50;
      }
    });

    let currentWebview = this.$scope.$getAppWebview();

    setTimeout(() => {
      const wv = currentWebview.children()[0];
      wv.setStyle({
        top: this.statusbar + 50,
        height: this.originalHeight
      });
    }, 200);
  },
  onReady() {
    uni.onKeyboardHeightChange(res => {
      let currentWebview = this.$scope.$getAppWebview();
      const wv = currentWebview.children()[0];

      if (res.height > 0) {
        // 输入法弹出，缩小 WebView 高度
        wv.setStyle({
          height: this.originalHeight - res.height
        });
      } else {
        // 输入法关闭，恢复原始高度
        wv.setStyle({
          height: this.originalHeight
        });
      }
    });
  },
  methods: {
    handleLoad(event) {
      const loadedUrl = event.target.url;
      console.log('Loaded URL:', loadedUrl);
    }
  }
};
</script>

<style scoped>
.webview-container {
  width: 100%;
  height: 100%; /* 使用 100% 而不是 100vh */
}

.web-view {
  width: 100%;
}

.custom-tab-bar {
  position: absolute;
  bottom: 0;
  left: 0;
  width: 100%;
  height: 50px;
}
</style>