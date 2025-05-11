(function($) {
    $(document).ready(function() {
        var form = $('#content-main form');
        var submitBtn = form.find('input[type="submit"]');
        var loadingIndicator = $('<div class="loading-indicator"></div>').appendTo(document.body);

        submitBtn.click(function() {
            loadingIndicator.show();
        });

        // 如果需要，在表单提交后隐藏指示器（根据实际情况调整）
        // 注意：对于大型文件上传，浏览器可能会在连接关闭之前保持此界面，
        // 所以可能无法及时隐藏指示器。
    });
})(django.jQuery);