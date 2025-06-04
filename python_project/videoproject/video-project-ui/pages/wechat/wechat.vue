<template>
  <view class="webview-container">
    <!-- 注意：web-view 不再设置 margin-bottom -->
    <web-view :src="url" class="web-view" @load="handleLoad"></web-view>
    <custom-tab-bar class="custom-tab-bar"></custom-tab-bar>
  </view>
</template>

<script>
export default {
  data() {
    return {
      url: 'https://www.life233.top/mattermost'
    };
  },
  onLoad(option) {
    let statusbar = 0;
    let height = 0;

    uni.getSystemInfo({
      success: (sysinfo) => {
        statusbar = sysinfo.statusBarHeight;
        height = sysinfo.windowHeight;
      }
    });

    let currentWebview = this.$scope.$getAppWebview();

    setTimeout(() => {
      const wv = currentWebview.children()[0];

      // 设置 WebView 的 top 和 height，避开状态栏 + 底部菜单
      wv.setStyle({
        top: statusbar + 50, // 避开状态栏
        height: height - statusbar - 50, // 屏幕高 - 状态栏 - 底部菜单
      });
    }, 200);
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
  /* 移除 flex 和 margin-bottom，由 JS 动态控制 */
}

.custom-tab-bar {
  position: absolute;
  bottom: 0;
  left: 0;
  width: 100%;
  height: 50px;
}
</style>