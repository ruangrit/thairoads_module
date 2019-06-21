$(document).ready(function () {
  var height = $('.node-condition .node-situation').height();
  if (height < $('.node-condition .node-rule').height()) {
    height = $('.node-condition .node-rule').height();
  }
  $('.node-condition .node-situation').css({'min-height':height});
  $('.node-condition .node-rule').css({'min-height':height});
});
