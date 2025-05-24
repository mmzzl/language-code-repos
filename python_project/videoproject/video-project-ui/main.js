import App from './App'
import BASE_URL from './common/api.js'
// #ifndef VUE3
import Vue from 'vue'
import './uni.promisify.adaptor'
import CustomTabBar from '@/common/CustomTabBar.vue'
Vue.component('custom-tab-bar', CustomTabBar);
// main.js
Vue.config.productionTip = false;
const base_url = 'http://121.37.47.63';
const env = process.env.NODE_ENV;
console.log("base url", base_url);
Vue.prototype.$BASE_URL = base_url;
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