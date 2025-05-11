<template>
  <view class="container">
    <!-- 视频播放器 -->
	<!-- <video :src="currentVideoUrl" class="video-player"></video> -->
	<view>
		<div id="mui-player"></div>
	</view>
    <!-- 切换按钮 -->
    <button @click="toggleEpisodes">{{ isEpisodesVisible ? '隐藏选集' : '显示选集' }}</button>

    <!-- 选集列表 -->
	<scroll-view v-if="isEpisodesVisible" scroll-y class="episode-list">
	  <block v-for="item in episodes" :key="item.id">
	    <view 
	      @click="playEpisode(item.id)" 
	      :class="['episode-item', { active: item.id === selectedEpisodeId }]">
	      {{ item.title }} - 第{{ item.episode_number }}集
	    </view>
	  </block>
	</scroll-view>
  </view>
</template>

<script>
import 'mui-player/dist/mui-player.min.css';
import MuiPlayer from 'mui-player';
import Hls from 'hls.js';

export default {
  data() {
    return {
      currentVideoUrl: '', // 当前播放视频的URL
      episodes: [],        // 存储选集数据
      isEpisodesVisible: false, // 控制选集显示隐藏
      selectedEpisodeId: null   ,// 当前选中的剧集ID
	  mp: null   
    };
  },
  onLoad(options) {
	  // options 就是跳转时传入的参数对象
	  if (options.id) {
	  	console.log('接收到的id是：', options.id);
		this.seriesId = options.id;
	  }
  },
  onUnload() {
    if (this.hlsInstance) {
      this.hlsInstance.destroy();
      this.hlsInstance = null;
    }
  },
  onReady() {
	if (this.seriesId) {
		this.fetchEpisodes(this.seriesId);
	} else {
		uni.showToast({
			title: "缺少系列ID",
			icon: 'none'
		})
	}
  },
 
  beforeDestroy() {
	this.mp.destroy();
  },
  methods: {
	initMuiPlayer() {
	    const container = document.getElementById("mui-player");
	    if (!container) {
	      console.error("播放器容器未找到");
	      return;
	    }
	
	    if (!this.currentVideoUrl) {
	      console.warn("视频地址为空，无法初始化播放器");
	      return;
	    }
	
	    this.mp = new MuiPlayer({
	      container: container,
	      src: this.currentVideoUrl,
	      parse: {
	        type: 'hls',
	        loader: Hls, // 确保 Hls 已挂载到 window
	        config: { debug: false },
	      },
	      pageHead: false,
	    });
	  },
    // 获取选集列表
    async fetchEpisodes(series_id) {
      try {
        const res = await uni.request({
            url: `/api/videos/episodes?series_id=${series_id}`,
            method: 'GET',
          });
        console.log("res: ", res);
		console.log('statuscode: ', res.statusCode);
		console.log(Array.isArray(res.data));
        if (res.statusCode === 200 && Array.isArray(res.data)) {
          this.episodes = res.data;
          if (this.episodes.length > 0) {
            this.selectedEpisodeId = this.episodes[0].id;
            this.playEpisode(this.episodes[0].id);
          } else {
			  console.log('小于0');
		  }
        } else {
          uni.showToast({ title: '获取选集失败', icon: 'none' });
        }
      } catch (err) {
        uni.showToast({ title: '网络异常', icon: 'none' });
        console.error(err);
      }
    },
	playEpisode(videoId) {
	  const episode = this.episodes.find(e => e.id === videoId);
	  if (episode) {
	    this.currentVideoUrl = episode.processed_video_file; // 使用 original_video_file 播放
	    this.selectedEpisodeId = episode.id;
		console.log('current_videourl:', this.currentVideoUrl);
		// 使用 $nextTick 确保 DOM 更新后再初始化播放器
		this.$nextTick(() => {
			if (!this.mp) {
			  this.initMuiPlayer(); // 第一次播放时初始化播放器
			} else {
			  this.mp.changeSrc(this.currentVideoUrl); // 切换剧集时更新地址
			}
		});
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
</style>