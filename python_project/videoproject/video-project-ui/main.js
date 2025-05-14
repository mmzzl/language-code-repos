import App from './App'

// #ifndef VUE3
import Vue from 'vue'
import './uni.promisify.adaptor'
// main.js
Vue.config.productionTip = false;

const env = process.env.NODE_ENV;

let apiBaseURL = '';
if (env === 'development') {
  apiBaseURL = 'http://192.168.43.21:8000';
} else {
  apiBaseURL = 'https://yourdomain.com';
}

Vue.prototype.$apiBaseURL = apiBaseURL;
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