(function($) {
    $(document).ready(function() {
        var form = $('#video_form');
        if (form.length) {
            console.log('Form found:', form);
        } else {
            console.error('Form not found!');
        }
        // 创建一个隐藏的加载指示器
        var loadingIndicator = $('<div class="loading-indicator"></div>').appendTo(document.body);

        // 监听表单提交事件
        form.on('submit', function(event) {
            loadingIndicator.show(); // 提交时显示加载指示器
        });

        // 可选：如果需要在 AJAX 请求完成后隐藏指示器，请确保在这里处理
        // 注意：对于大型文件上传，浏览器可能会在连接关闭之前保持此界面，
        // 所以可能无法及时隐藏指示器。
    });
})(django.jQuery);