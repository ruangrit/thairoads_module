$(document).ready(function() {
  var $trs = $('table#spi-list > tbody > tr');
  $.each($trs, function(i, tr) {
    var $td = $('td:first', tr);
	var $indents = $('.indentation', $td);
	$.each($indents, function(i, indent) {
	  if (i == 0) {
	    $(indent).addClass('tree-child');
	  }
	  else {
        $(indent).addClass('tree-child-horizontal');
      }
    });
  });
});
