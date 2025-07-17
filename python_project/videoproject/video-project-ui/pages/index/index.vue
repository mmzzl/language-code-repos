<template>
  <view class="container">
    <!-- 搜索区域 -->
    <view class="search-wrapper">
      <!-- 用户头像图标（外部） -->
      <uni-icons type="contact" size="24" color="#666" class="external-user-icon"></uni-icons>

      <!-- 搜索容器（包含搜索图标 + 输入框 + 搜索按钮） -->
      <view class="search-container">
        <!-- 搜索图标 + 输入框 -->
        <uni-icons type="search" size="20" color="#999" class="search-icon"></uni-icons>
        <input 
          type="text" 
          v-model="searchQuery" 
          placeholder="请输入关键词搜索..." 
          class="search-box"
          @confirm="handleSearch"
        />
      </view>

      <!-- 独立的搜索按钮 -->
      <button class="search-btn" @click="handleSearch">搜索</button>
    </view>

    <!-- 视频列表 -->
    <view class="videos-container">
      <view v-for="(video, index) in videos" :key="index" class="video-item" @click="goToDetail(video.id)">
        <image
          :src="video.thumbnail"
          mode="aspectFill"
          class="thumbnail"
        ></image>
        <text class="video-title">{{ video.title }}</text>
      </view>
    </view>

    <!-- 加载更多 -->
    <view v-if="!isSearching" class="load-more" @click="fetchVideos">
      {{ isLoading ? '加载中...' : (hasNextPage ? '加载更多' : '没有更多了') }}
    </view>

    <!-- 搜索提示 -->
    <view v-if="isSearching && videos.length === 0" class="no-result">
      没有找到相关视频
    </view>
	<custom-tab-bar></custom-tab-bar>
  </view>
</template>

<script lang="ts">
import Vue from 'vue';
import uniIcons from '@dcloudio/uni-ui/lib/uni-icons/uni-icons.vue';
const baseUrl = Vue.prototype.$BASE_URL; // 全局变量
console.log('baseUrl', baseUrl);
interface VideoItem {
  url?: string;
  title: string;
  thumbnail: string;
  id: number;
}

interface ApiResponse {
  results: Array<{
    id: number;
    title: string;
    cover_image: string;
  }>;
  next: string | null;
}

export default Vue.extend({
  components: { 
	uniIcons
  },
  data() {
    return {
      videos: [] as Array<VideoItem>,
      searchQuery: '', // 搜索关键词
      currentPage: 1,
      hasNextPage: true,
      isLoading: false,
      isSearching: false, // 是否正在搜索
    };
  },
  onLoad() {
    this.fetchVideos();
  },
  onReachBottom() {
    if (this.hasNextPage && !this.isSearching) {
      this.fetchVideos();
    }
  },
  methods: {
    async fetchVideos(reset = false) {
      if (this.isLoading) return;

      this.isLoading = true;

      try {
        const page = reset ? 1 : this.currentPage;
        const apiPath = "/api/videos/series";
        const params = this.isSearching
          ? `?title=${encodeURIComponent(this.searchQuery)}&page=${page}`
          : `?page=${page}`;  

        const url = baseUrl + apiPath + params;
        const res = await uni.request({
          url,
          method: 'GET',
          credentials: 'omit'
        });

        const data = res.data as ApiResponse;

        const newVideos = data.results.map(item => ({
          title: item.title,
          thumbnail: item.cover_image,
          id: item.id,
        }));

        this.videos = reset ? newVideos : [...this.videos, ...newVideos];
        this.hasNextPage = !!data.next;
        this.currentPage += 1;
      } catch (error) {
        console.error('获取视频失败:', error);
      } finally {
        this.isLoading = false;
      }
    },

    handleSearch() {
      if (!this.searchQuery.trim()) {
        uni.showToast({ title: '请输入关键词', icon: 'none' });
        return;
      }

      this.isSearching = true;
      this.currentPage = 1;
      this.videos = [];
      this.fetchVideos(true); // 重置并加载搜索结果
    },

    goToDetail(id: number) {
      uni.navigateTo({
        url: `/pages/detail/detail?id=${id}`,
      });
    },
  },
});
</script>

<style scoped>
.container {
  display: flex;
  flex-direction: column;
  align-items: center;
  padding: 20px;
  padding-bottom: env(safe-area-inset-bottom); /* 自动调整底部内边距 */
  padding-bottom: calc(60px + env(safe-area-inset-bottom)); /* 结合固定值和安全区域值 */
}

.search-wrapper {
  display: flex;
  align-items: center;
  padding: 10px;
  gap: 10px; /* 控制图标、输入框和按钮之间的间距 */
}

.external-user-icon {
  margin-right: 10px; /* 图标与搜索区域之间留出空间 */
}

.search-container {
  position: relative;
  flex: 1;
  display: flex;
  align-items: center;
  background-color: #f5f5f5;
  border-radius: 20px;
  padding: 5px 10px;
}

.search-icon {
  margin-right: 8px;
}

.search-box {
  flex: 1;
  font-size: 14px;
  height: 30px;
  line-height: 30px;
}

.search-btn {
  width: 60px;
  height: 35px;
  font-size: 14px;
  padding: 0;
}


/* 视频容器 */
.videos-container {
  display: flex;
  flex-wrap: wrap;
  justify-content: space-between;
  width: 100%;
  max-width: 600px;
}

/* 单个视频项 */
.video-item {
  width: 48%; /* 一行显示两个 */
  margin-bottom: 20px;
  text-align: center;
}

.thumbnail {
  width: 100%;
  height: 180px;
  border-radius: 8px;
  object-fit: cover;
}

.video-title {
  font-size: 14px;
  margin-top: 8px;
  color: #333;
  display: block;
}

/* 加载更多 */
.load-more {
  padding: 15px;
  text-align: center;
  font-size: 14px;
  color: #666;
}
</style>
