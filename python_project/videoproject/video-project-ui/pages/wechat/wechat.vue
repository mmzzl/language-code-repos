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
      url: 'https://www.life233.top/mattermost'
    };
  },
  methods: {
    handleLoad(event) {
      const loadedUrl = event.target.url; // 获取加载后的 URL
      console.log('Loaded URL:', loadedUrl);
    }
  }
};
</script>

<style scoped>
.webview-container {
  position: relative; /* 使用相对定位 */
  width: 100%;
  height: 100vh; /* 设定容器高度为视口高度 */
  display: flex;
  flex-direction: column;
}

.web-view {
  flex: 1; /* 让 web-view 占据剩余空间 */
  margin-bottom: 50px; /* 留出底部菜单的空间 */
}

.custom-tab-bar {
  position: absolute; /* 使用绝对定位 */
  bottom: 0; /* 固定在底部 */
  width: 100%; /* 使菜单宽度占满 */
}
</style>