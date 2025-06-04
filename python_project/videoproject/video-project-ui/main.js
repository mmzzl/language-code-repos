import App from './App'
// #ifndef VUE3
import Vue from 'vue'
import './uni.promisify.adaptor'
import CustomTabBar from '@/common/CustomTabBar.vue'
import http from '@/common/request.js'
Vue.component('custom-tab-bar', CustomTabBar);
// main.js
Vue.config.productionTip = false;
Vue.prototype.$http = http;
Vue.prototype.$BASE_URL = 'https://www.life233.top';
App.mpType = 'app'
const app = new Vue({
  ...App
})
app.$mount()
// #endif

// #ifdef VUE3
import { createSSRApp } from 'vue'
export function createApp() {
  const app = createSSRApp(App)
  return {
    app
  }
}
// #endif