// api.js
import request from './request'

export function fetchCsdnRss() {
  return request.get('http://127.0.0.1:8000/blog/csdn/rss')
}