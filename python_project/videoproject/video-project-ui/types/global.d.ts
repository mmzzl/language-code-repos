import Vue from 'vue';

declare module 'vue/types/vue' {
  interface Vue {
    $apiBaseURL: string; // 声明全局变量
  }
}