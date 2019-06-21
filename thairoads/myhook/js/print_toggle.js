$(document).ready(function() {
  var mapLeft = $('#svgmap-info');
  var mapRight = $('#svgmap2-info');
  $('#print-left').click(function() {
    map_print_description('filter1');
    mapLeft.addClass('show'); 
    $('.map-right').addClass('hide'); 
    $('.map-left').removeClass('hide'); 
    mapRight.removeClass('show'); 
    setTimeout(function() {
      window.print();
    }, 1000);
  });
  $('#print-right').click(function() {
    map_print_description('filter2');
    mapRight.addClass('show'); 
    $('.map-left').addClass('hide'); 
    $('.map-right').removeClass('hide'); 
    mapLeft.removeClass('show');
    setTimeout(function() {
      window.print();
    }, 1000);
  });

  function map_print_description(filterId) {
    $('.map-add-title').remove();
    if ($('#'+filterId+'year').attr('checked')) {
      var year = $('#'+filterId).val();
      $('.map-title h2').append('<span class="map-add-title"> ปี '+year+'</span>');
    }
    else {
      var yearStart = $('#'+filterId+'_start').val();
      var yearEnd= $('#'+filterId+'_end').val();
      $('.map-title h2').append('<span class="map-add-title"> เฉลี่ยระหว่างปี '+yearStart+' - '+yearEnd+'</span>');
    }
  }

});
