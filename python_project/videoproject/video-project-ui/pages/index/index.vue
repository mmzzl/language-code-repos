<template>
	<view class="container">
	  <view v-for="(video, index) in videos" :key="index" class="video-item">
		<!-- 缩略图 -->
		<image
		  :src="video.thumbnail"
		  mode="aspectFill"
		  class="thumbnail"
		  @click="goToDetail(video.id)"
		></image>
		<!-- 标题 -->
		<text class="video-title">{{ video.title }}</text>
	  </view>
	  <view class="load-more" @click="fetchVideos">
  		{{ isLoading ? '加载中...' : (hasNextPage ? '加载更多' : '没有更多了') }}
	  </view>
	</view>
</template>

<script lang="ts">
import Vue from 'vue';

interface VideoItem {
  url?: string
  title: string
  thumbnail: string
}

interface ApiResponse {
  results: Array<{
    id: number
    title: string
    cover_image: string
  }>
  next: string | null
}

export default Vue.extend({
  data() {
		return {
			videos: [] as Array<{ url?: string; title: string; thumbnail: string; id: number}>,
			currentPage: 1,
			hasNextPage: true,
			isLoading: false
		};
    },
  onLoad() {
    this.fetchVideos();
  },
  onReachBottom() {
	if (this.hasNextPage) {
		this.fetchVideos();
	}
  },
  methods: {
    async fetchVideos() {
      try {
        const res = await uni.request({
          url:  '/api/videos/series' , //'https://your-api.com/api/videos', // 替换为你的接口地址
          method: 'GET'
        });
		const data = res.data as ApiResponse;
		
        this.videos = [
			...this.videos,
			...data.results.map(item=>({
				title: item.title,
				thumbnail: item.cover_image,
				id: item.id
			}))
		];
		this.hasNextPage = !!data.next;
		this.currentPage += 1;
      } catch (error) {
        console.error('Failed to fetch videos:', error);
      } finally {
        this.isLoading = false;
      }
    },
	goToDetail(id: number) {
		uni.navigateTo({
			url: `/pages/detail/detail?id=${id}`
		});
	}
}
});
</script>

<style>
.container {
  display: flex;
  flex-direction: column;
  align-items: center;
  padding: 20px;
}

.video-item {
  width: 100%;
  max-width: 600px;
  margin-bottom: 20px;
  text-align: center;
}

.video {
  width: 100%;
  height: auto;
  border-radius: 8px;
}

.video-title {
  font-size: 16px;
  margin-top: 10px;
}

.load-more {
  padding: 15px;
  text-align: center;
  font-size: 14px;
  color: #666;
}

</style>
