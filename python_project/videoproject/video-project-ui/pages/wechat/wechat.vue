<template>
  <view class="webview-container">
    <web-view :src="url"></web-view>
  </view>
</template>

<script>
export default {
  data() {
    return {
      url: 'http://121.37.47.63:8065/'
    };
  }
};
</script>

<style scoped>
.webview-container {
  width: 100%;
  height: 100%;
}
</style>