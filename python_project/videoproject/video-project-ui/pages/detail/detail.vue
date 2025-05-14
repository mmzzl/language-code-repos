<template>
  <view class="container">
    <!-- 原生 video 组件 -->
    <video
      :src="currentVideoUrl"
      :enable-play-gesture="true"
      @play="onPlay"
      @ended="handleVideoEnd"
      :autoplay="true"
      style="width: 100%; height: 500rpx;"
      :show-progress="true"
      :show-fullscreen-btn="true"
    ></video>

    <!-- 简介区域 -->
    <view class="description-box">
      <text :class="['description-text', { expanded: isDescriptionExpanded }]">
        {{ description }}
      </text>
      <text class="toggle-btn" @click="toggleDescription">
        {{ isDescriptionExpanded ? '收起' : '展开' }}
      </text>
      <view class="play-count">
        播放量: {{ playCount }}
      </view>
    </view>

    <!-- 水平滚动选集列表 -->
    <scroll-view v-if="isEpisodesVisible" scroll-x class="episode-list-horizontal">
      <block v-for="item in episodes" :key="item.id">
        <view 
          @click="playEpisode(item.id)" 
          :class="['episode-item-horizon', { active: item.id === selectedEpisodeId }]">
          {{ item.title }} - 第{{ item.episode_number }}集
        </view>
      </block>
    </scroll-view>
  </view>
</template>

<script>
export default {
  data() {
    return {
      currentVideoUrl: '', // 当前播放视频地址
      episodes: [],        // 所有剧集数据
      isEpisodesVisible: true,
      selectedEpisodeId: null, // 当前选中剧集ID
      seriesId: null,      // 系列ID
      playCount: 0,        // 当前剧集播放量
      description: "",
      isDescriptionExpanded: false,
	  apiBaseURL: "http://192.168.43.21:8000"
    };
  },
  onLoad(options) {
    if (options.id) {
      console.log('接收到的id是：', options.id);
      this.seriesId = options.id;
      this.fetchEpisodes(this.seriesId);
    }
  },
  onReady() {
    if (!this.seriesId) {
      uni.showToast({ title: "缺少系列ID", icon: 'none' });
    }
  },
  methods: {
    toggleDescription() {
      this.isDescriptionExpanded = !this.isDescriptionExpanded;
    },

    // 播放时调用播放接口
    async onPlay() {
      await this.incrementPlayCount(this.selectedEpisodeId);
    },

    // 视频结束时处理下一集
    handleVideoEnd() {
      const currentIndex = this.episodes.findIndex(e => e.id === this.selectedEpisodeId);
      if (currentIndex < this.episodes.length - 1) {
        const nextEpisodeId = this.episodes[currentIndex + 1].id;
        this.playEpisode(nextEpisodeId); // 自动播放下一集
      } else {
        console.log('已播放到最后一集');
      }
    },

    // 播放下一集
    playEpisode(videoId) {
      const episode = this.episodes.find(e => e.id === videoId);
      if (episode) {
        this.currentVideoUrl = episode.processed_video_file;
        this.selectedEpisodeId = episode.id;
        this.description = episode.description;
        // 获取当前播放量
        this.fetchPlayCount(videoId);
        // 👇 每次切换剧集都增加播放量
        this.incrementPlayCount(videoId);
      }
    },

    // 获取播放量
    async fetchPlayCount(video_id) {
      try {
        const res = await uni.request({
          url: `${this.apiBaseURL}/api/videos/playcount/${video_id}`,
          method: 'GET',
        });
        if (res.statusCode === 200 && typeof res.data.number === 'number') {
          this.playCount = res.data.number;
        } else {
          console.error('获取播放量失败:', res);
        }
      } catch (error) {
        console.error('获取播放量失败:', error);
      }
    },

    // 增加播放量
    async incrementPlayCount(video_id) {
      try {
        await uni.request({
          url: `${this.apiBaseURL}/api/videos/played/${video_id}`,
          method: 'POST',
        });
        console.log(`视频ID ${video_id} 的播放量已增加`);
      } catch (error) {
        console.error(`增加视频ID ${video_id} 的播放量失败:`, error);
      }
    },

    // 获取所有剧集
    async fetchEpisodes(series_id) {
      try {
        const res = await uni.request({
          url: `${this.apiBaseURL}/api/videos/episodes/${series_id}`,
          method: 'GET',
        });
        if (res.statusCode === 200 && Array.isArray(res.data)) {
          this.episodes = res.data;
          if (this.episodes.length > 0) {
            this.selectedEpisodeId = this.episodes[0].id;
            this.playEpisode(this.episodes[0].id);
          }
        } else {
          uni.showToast({ title: '获取选集失败', icon: 'none' });
        }
      } catch (err) {
        uni.showToast({ title: '网络异常', icon: 'none' });
        console.error(err);
      }
    },

    // 切换选集面板显示状态
    toggleEpisodes() {
      this.isEpisodesVisible = !this.isEpisodesVisible;
    }
  }
};
</script>

<style scoped>
/* 保持原有样式不变 */
.video-player {
  width: 100%;
  height: 500rpx;
}
.episode-list {
  margin-top: 20rpx;
}
.episode-item {
  padding: 20rpx;
  border-bottom: 1rpx solid #eee;
}
.episode-item.active {
  color: #007AFF;
  font-weight: bold;
}
.description-box {
  padding: 20rpx;
  background-color: #f9f9f9;
}

.description-text {
  display: -webkit-box;
  overflow: hidden;
  text-overflow: ellipsis;
  -webkit-line-clamp: 2;
  -webkit-box-orient: vertical;
  font-size: 28rpx;
  color: #666;
}

.description-text.expanded {
  -webkit-line-clamp: unset;
}

.toggle-btn {
  color: #007AFF;
  margin-top: 10rpx;
  font-size: 26rpx;
  display: inline-block;
}

.play-count {
  font-size: 24rpx;
  color: #999;
  margin-top: 10rpx;
  display: block;
}

.episode-list-horizontal {
  white-space: nowrap;
  overflow-x: auto;
  padding: 20rpx;
  border-top: 1rpx solid #eee;
  border-bottom: 1rpx solid #eee;
  margin-top: 20rpx;
}

.episode-item-horizon {
  display: inline-block;
  padding: 15rpx 30rpx;
  margin-right: 20rpx;
  font-size: 28rpx;
  color: #333;
}

.episode-item-horizon.active {
  color: red;
  font-weight: bold;
}
</style>