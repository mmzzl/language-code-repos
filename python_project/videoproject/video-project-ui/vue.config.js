module.exports = {
    devServer: {
        proxy: {
            '/api': {
                target: 'http://127.0.0.1:8000', // 目标服务器地址
                changeOrigin: true,
                // pathRewrite: {
                //     '^/api': '' // 重写路径，去掉/api前缀
                // }
            }
        }
    }
}