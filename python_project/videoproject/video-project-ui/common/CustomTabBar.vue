<template>
  <view class="custom-tabbar">
    <view 
      v-for="(tab, index) in tabs" 
      :key="index" 
      @click="switchTab(tab)"
      :class="['tab-item', activeTab === tab.pagePath ? 'active' : '']"
    >
      <image :src="activeTab === tab.pagePath ? tab.selectedIconPath : tab.iconPath"></image>
      <text>{{ tab.text }}</text>
    </view>
  </view>
</template>

<script>
const baseUrl = "http://121.37.47.63"; // 全局变量
export default {
  data() {
    return {
      tabs: [],
      activeTab: ''
    };
  },
  mounted() {
    this.fetchTabs();
    this.activeTab = this.getCurrentPageUrl();
  },
  methods: {
    fetchTabs() {
	  const url = baseUrl + '/api/tabbar/';
      uni.request({
        url: url,
        success: (res) => {
          if (res.statusCode === 200 && res.data.tabs) {
            this.tabs = res.data.tabs;
          }
        },
        fail: (err) => {
          console.error('请求 tab 失败:', err);
        }
      });
    },
    switchTab(tab) {
      this.activeTab = tab.pagePath;
      uni.switchTab({
        url: '/' + tab.pagePath,
        fail: () => {
          uni.reLaunch({ url: '/' + tab.pagePath }); // fallback
        }
      });
    },
    getCurrentPageUrl() {
      const pages = getCurrentPages();
      const currentPage = pages[pages.length - 1];
      return currentPage.route;
    }
  }
};
</script>

<style>
.custom-tabbar {
  position: fixed;
  bottom: 0;
  left: 0;
  width: 100%;
  height: 60px;
  background: linear-gradient(180deg, #ffffff, #f0f0f0); /* 渐变背景 */
  display: flex;
  justify-content: space-around;
  align-items: center;
  z-index: 9999;
  box-shadow: 0 -4px 8px rgba(0, 0, 0, 0.1), 0 -2px 4px rgba(0, 0, 0, 0.05); /* 增强阴影效果 */
}

.tab-item {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  flex: 1;
}

.tab-item image {
  width: 26px;
  height: 26px;
  margin-bottom: 2px;
  transition: transform 0.3s ease, filter 0.3s ease;
}

.tab-item.active image {
  transform: scale(1.1); /* 点击放大动画 */
  filter: brightness(1.2);
}

.tab-item text {
  font-size: 14px;
  color: #777;
  transition: color 0.3s ease, font-weight 0.3s ease;
}

.tab-item.active text {
  color: #0056b3; /* 选中时的颜色（蓝色） */
  font-weight: 600;
}
</style>