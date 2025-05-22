// utils/request.js
import Vue from 'vue'

const apiClient = {
  get(url) {
    return new Promise((resolve, reject) => {
      uni.request({
        url,
        method: 'GET',
        success(res) {
          resolve(res)
        },
        fail(err) {
          reject(err)
        }
      })
    })
  }
}

export default apiClient