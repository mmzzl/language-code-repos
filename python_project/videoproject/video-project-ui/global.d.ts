// global.d.ts
import Vue from 'vue'

declare module 'vue/types/vue' {
  interface Vue {
    $api: {
      baseUrl: string
    }
  }
}