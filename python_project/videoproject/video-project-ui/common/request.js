// common/request.js

const BASE_URL = 'http://121.37.47.63'; // 后端地址
let isRefreshing = false; // 防止重复刷新 token
let failedQueue = []; // 存储等待刷新 token 后再执行的请求

// 刷新 token 的请求函数
function refreshToken() {
    return new Promise((resolve, reject) => {
        const refreshToken = uni.getStorageSync('refresh_token');
        if (!refreshToken) {
            onRefreshTokenError();
            return reject('No refresh token');
        }

        uni.request({
            url: `${BASE_URL}/api/token/refresh/`,
            method: 'POST',
            data: {
                refresh: refreshToken
            },
            success: res => {
                if (res.statusCode === 200) {
                    const { access } = res.data;
                    uni.setStorageSync('token', access); // 更新 access_token
                    resolve(access);
                } else {
                    onRefreshTokenError();
                    reject('Refresh token failed');
                }
            },
            fail: () => {
                onRefreshTokenError();
                reject('Refresh token request failed');
            }
        });
    });
}

function onRefreshTokenError() {
    isRefreshing = false;
    uni.removeStorageSync('token');
    uni.removeStorageSync('refresh_token');
    uni.showToast({ title: '登录已过期，请重新登录', icon: 'none' });
    uni.navigateTo({ url: '/pages/login/login' });
}

export default function request(url, method, data, showLoading = true) {
    const token = uni.getStorageSync('token');

    return new Promise((resolve, reject) => {
        if (showLoading) {
            uni.showLoading({ title: '加载中', mask: true });
        }

        function makeRequest(tokenToUse) {
            uni.request({
                url: BASE_URL + url,
                method,
                data,
                header: {
                    'Authorization': tokenToUse ? `Bearer ${tokenToUse}` : ''
                },
                success: res => {
                    if (res.statusCode === 200) {
                        resolve(res.data);
                    } else if (res.statusCode === 401) {
                        handleTokenExpired(url, method, data, resolve, reject);
                    } else {
                        uni.showToast({ title: '网络异常', icon: 'none' });
                        reject(new Error(`HTTP error! status: ${res.statusCode}`));
                    }
                },
                fail: err => {
                    uni.showToast({ title: '请求失败', icon: 'none' });
                    reject(err);
                },
                complete: () => {
                    if (showLoading) uni.hideLoading();
                }
            });
        }

        makeRequest(token);
    });
}

// 处理 token 过期
function handleTokenExpired(originalUrl, originalMethod, originalData, resolve, reject) {
    if (!isRefreshing) {
        isRefreshing = true;

        refreshToken()
            .then(newToken => {
                isRefreshing = false;
                // 重试所有排队的请求
                failedQueue.forEach(cb => cb(newToken));
                failedQueue = [];

                // 重试当前请求
                resolveAfterRefresh(originalUrl, originalMethod, originalData, resolve, reject);
            })
            .catch(err => {
                isRefreshing = false;
                failedQueue = [];
                reject(err);
            });
    } else {
        // 暂存请求到队列，等刷新完成后执行
        failedQueue.push(() => {
            resolveAfterRefresh(originalUrl, originalMethod, originalData, resolve, reject);
        });
    }
}

function resolveAfterRefresh(url, method, data, resolve, reject) {
    uni.request({
        url: BASE_URL + url,
        method,
        data,
        header: {
            'Authorization': `Bearer ${uni.getStorageSync('token')}`
        },
        success: res => {
            if (res.statusCode === 200) {
                resolve(res.data);
            } else {
                reject(new Error('Retry request failed'));
            }
        },
        fail: err => {
            reject(err);
        }
    });
}