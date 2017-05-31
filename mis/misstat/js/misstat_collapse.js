$(document).ready(function() {
  $('div.spi-more').click(function(e) {
    $(this)
      .closest('.spi-item')
      .children('ul')
      .slideToggle('slow', function() {
        var $more = $(this).siblings().children('.spi-more');
        if ($(this).is(':visible')) {
          $more.removeClass('arrow-right').addClass('arrow-down');
        }
        else {
          $more.removeClass('arrow-down').addClass('arrow-right');
        }
      });
  });
});
