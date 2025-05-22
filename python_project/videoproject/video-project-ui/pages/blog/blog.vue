<template>
  <view class="container">
    <!-- RSS 文章列表 -->
    <view class="post-list">
      <view v-for="(entry, index) in entries" :key="index" class="post" @click="showArticle(entry)">
        <text class="title">{{ entry.title }}</text>
        <p>{{ entry.summary }}</p>
        <small>{{ entry.published }}</small>
      </view>
    </view>

    <!-- 文章预览区域 -->
	<!-- <view v-if="selectedArticleHtml" class="article-preview">
	  <rich-text :nodes="selectedArticleHtml"></rich-text>
	</view> -->
	<view v-if="selectedArticleHtml" class="article-preview" v-html="selectedArticleHtml"></view>
  </view>
</template>

<script>
export default {
  data() {
    return {
      entries: [],
      selectedArticleHtml: null
    };
  },
  onLoad() {
    this.fetchData();
  },
  methods: {
	handleContentClick(e) {
	      // 阻止所有子元素的点击冒泡导致的页面跳转
	      e.preventDefault();
	      e.stopPropagation();
	  
	      const target = e.target;
	      if (target.tagName === 'A') {
	        uni.showToast({ title: '此链接已在应用内展示', icon: 'none' });
	        return false;
	      }
	},
    fetchData() {
      uni.request({
        url: 'http://127.0.0.1:8000/blog/csdn/rss',
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
    showArticle(entry) {
      const encodedUrl = encodeURIComponent(entry.link);
      uni.request({
        url: `http://127.0.0.1:8000/blog/api/csdn-article/?url=${encodedUrl}`,
        success: (res) => {
          if (res.statusCode === 200 && res.data.content) {
            this.selectedArticleHtml = res.data.content;
			// 	const parser = new DOMParser();
			// 	const doc = parser.parseFromString(res.data.content, 'text/html');
			// 	// 提取 body 中的 HTML 内容
			// 	const bodyContent = doc.body.innerHTML;
			
			// 	this.selectedArticleHtml = bodyContent;
				
				// 将 HTML 转为 JSON 节点树（需自己实现或使用库如 htmlparser2）
			 // 打印看是否是合法 HTML
          } else {
            this.selectedArticleHtml = '<p>加载文章内容失败</p>';
          }
        },
        fail: (err) => {
          console.error('文章内容请求失败:', err);
          this.selectedArticleHtml = '<p>网络错误，请重试</p>';
        }
      });
    }
  }
};
</script>

<style scoped>
.container {
  padding: 20px;
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

.article-preview {
  margin-top: 20px;
  width: 100%;
  height: 500px;
  border: 1px solid #ddd;
}

.article-preview img {
  max-width: 100%;
  height: auto;
}

.article-preview img {
  max-width: 100%;
  height: auto;
}
.article-preview p,
.article-preview div,
.article-preview span {
  font-size: 14px;
  line-height: 1.6;
}

</style>