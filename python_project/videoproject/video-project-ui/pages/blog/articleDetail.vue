<<template>
  <view class="article-detail">
    <!-- 文章详情内容 -->
    <view class="article-tile">
      <text class='article-tile-text'>{{ articleTitle }}</text>
    </view>
    
    <!-- 字体大小选择器 -->
    <!-- <view class="font-size-selector">
      <picker @change="bindPickerChange" :value="fontSizeIndex" :range="fontSizeOptions">
        <view class="picker">
          当前字体大小: {{ fontSizeOptions[fontSizeIndex] }}
        </view>
      </picker>
    </view> -->

    <rich-text :nodes="articleContent"></rich-text>
  </view>
</template>

<script>
import Vue from 'vue';
const baseUrl = Vue.prototype.$BASE_URL; // 全局变量

export default {
  data() {
    return {
      articleTitle: '',
      articleContent: '',
      fontSizeOptions: ['小', '中', '大'], // 字体大小选项
      fontSizeIndex: 1, // 默认选中 '中' 大小
      fontSizeMap: {
        '小': 'small',
        '中': 'medium',
        '大': 'large'
      }
    };
  },
  onLoad(options) {
    // 从 options 中获取传递过来的参数
    this.articleTitle = decodeURIComponent(options.title);
    // 根据 options 获取更多文章详情数据
    if (options.url) {
      const encodedUrl = encodeURIComponent(decodeURIComponent(options.url));
      const url = baseUrl + '/blog/api/csdn-article/?url=' + encodedUrl;
      uni.request({
        url: url,
        success: (res) => {
          if (res.statusCode === 200 && res.data.content) {
            this.articleContent = res.data.content;
          } else {
            this.articleContent = '<p>加载文章内容失败</p>';
          }
        },
        fail: (err) => {
          console.error('文章内容请求失败:', err);
          this.articleContent = '<p>网络错误，请重试</p>';
        }
      });
    }
  },
  methods: {
    bindPickerChange(e) {
      this.fontSizeIndex = e.target.value;
      const selectedFontSize = this.fontSizeMap[this.fontSizeOptions[this.fontSizeIndex]];
      this.setFontSize(selectedFontSize);
    },
    setFontSize(size) {
      this.fontSize = size;
    }
  }
};
</script>

<style scoped>
.article-detail {
  padding: 20px;
  background-color: #f0f0f0; /* 修改背景颜色 */
}

.article-tile {
  text-align: center; /* 标题居中 */
  margin-bottom: 10px; /* 添加一些间距 */
  margin-top: 0px;
}

.article-tile-text {
  font-size: 24px; /* 设置字体大小 */
  color: #007AFF; /* 蓝色文字 */
}

/* 字体大小选择器 */
.font-size-selector {
  display: flex;
  justify-content: center;
  margin-bottom: 10px;
}

.picker {
  padding: 5px 10px;
  border: 1px solid #ccc;
  border-radius: 5px;
  font-size: 16px;
  cursor: pointer;
}

/* 动态字体大小 */
.rich-text {
  color: #333; /* 修改正文文字颜色 */
  text-align: center; /* 文本居中 */
}

.rich-text * {
  font-size: 28px;
}
</style>