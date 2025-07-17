<template>
  <view class="container">
    <!-- RSS 文章列表 -->
    <view class="post-list">
      <view v-for="(entry, index) in entries" :key="index" class="post" @click="navigateToArticleDetail(entry)">
        <text class="title">{{ entry.title }}</text>
        <p>{{ truncateSummary(entry.summary) }}</p>
        <small>{{ entry.published }}</small>
      </view>
    </view>
    <custom-tab-bar></custom-tab-bar>
  </view>
</template>

<script>
import Vue from 'vue';
const baseUrl = Vue.prototype.$BASE_URL; // 全局变量

export default {
  data() {
    return {
      entries: []
    };
  },
  onLoad() {
    this.fetchData();
  },
  methods: {
    fetchData() {
      const url = baseUrl + '/blog/csdn/rss';
      uni.request({
        url: url,
        method: 'GET',
        success: (res) => {
          if (res.statusCode === 200) {
            this.entries = res.data.entries;
          } else {
            console.error('获取 RSS 数据失败，状态码：', res.statusCode);
          }
        },
        fail: (err) => {
          console.error('网络请求失败：', err);
        }
      });
    },
    navigateToArticleDetail(entry) {
      // 使用 uni.navigateTo 跳转到 articleDetail 页面并传递参数
      uni.navigateTo({
        url: `/pages/blog/articleDetail?url=${encodeURIComponent(entry.link)}&title=${encodeURIComponent(entry.title)}`
      });
    },
    truncateSummary(summary) {
      if (summary.length > 100) {
        return summary.substring(0, 100) + '...';
      }
      return summary;
    }
  }
};
</script>

<style scoped>
.container {
  padding: 20px;
  padding-bottom: env(safe-area-inset-bottom); /* 自动调整底部内边距 */
  padding-bottom: calc(60px + env(safe-area-inset-bottom)); /* 结合固定值和安全区域值 */
}

.post {
  margin-bottom: 20px;
  padding: 10px;
  border-bottom: 1px solid #eee;
}

.title {
  font-size: 16px;
  font-weight: bold;
  color: #007AFF;
}

.summary {
  display: -webkit-box;
  -webkit-box-orient: vertical;
  -webkit-line-clamp: 3; /* 控制显示的行数 */
  overflow: hidden;
  text-overflow: ellipsis;
}
</style>