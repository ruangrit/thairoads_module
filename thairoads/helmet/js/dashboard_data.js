$ = jQuery;
$(document).ready(function () {

  $('#dashboard_filter_btn').click(function() {
    var year = $('.year-filter').val();
    var area = $('.area-filter').val();
    load_dashboard_data(year, area);

  });  
  // Todo remove hardcode; 
  var provinceId = 300;
  $.ajax({
    type: 'POST',
    url: Drupal.settings.basePath + 'helmet/yearlist',
    dataType: 'json',
    success: function(data) {
      data = data.reverse();
      var yearOption = '';
      data.forEach(function(entry) {
        yearOption += '<option value="'+entry+'">'+entry+'</option>';
      });
      $('.year-filter').html(yearOption);
      load_dashboard_data(data[0], provinceId);


    },
    data: 'code=C1_01-C2_01'
  });
  function load_dashboard_data(year, provinceId) {
    // ajax call
    console.log(Drupal.settings.basePath);
    $.ajax({
      type: 'POST',
      url: Drupal.settings.basePath + 'helmet/dashboard_data',
      dataType: 'json',
      success: function(data) {
        var position = $('.position');
        var age = $('.age');
        var area = $('.area');
        
        $('.area-info').html($(".area-filter option:selected").text());
        $('.year-info').html($(".year-filter option:selected").text());

        position.find('.all > .value').html(data['position'][0] +'%');
        position.find('.driver > .value').html(data['position'][1] +'%');
        position.find('.passenger > .value').html(data['position'][2] +'%');

        age.find('.adult > .block-percent').find('.value').attr({"value": data['age'][0]});
        age.find('.child > .block-percent').find('.value').attr({"value": data['age'][1]});
        age.find('.teen > .block-percent').find('.value').attr({"value": data['age'][2]});

        area.find('.all').find('td:eq(1)').find('.value').attr({"value": data['area']['all'][0]});
        area.find('.all').find('td:eq(2)').find('.value').attr({"value": data['area']['all'][1]});
        area.find('.all').find('td:eq(3)').find('.value').attr({"value": data['area']['all'][2]});
        area.find('.all').find('td:eq(4)').find('.value').html(data['area']['all'][3]+'%');

        area.find('.driver').find('td:eq(1)').find('.value').attr({"value": data['area']['driver'][0]});
        area.find('.driver').find('td:eq(2)').find('.value').attr({"value": data['area']['driver'][1]});
        area.find('.driver').find('td:eq(3)').find('.value').attr({"value": data['area']['driver'][2]});
        area.find('.driver').find('td:eq(4)').find('.value').html(data['area']['driver'][3]+'%');

        area.find('.passenger').find('td:eq(1)').find('.value').attr({"value": data['area']['passenger'][0]});
        area.find('.passenger').find('td:eq(2)').find('.value').attr({"value": data['area']['passenger'][1]});
        area.find('.passenger').find('td:eq(3)').find('.value').attr({"value": data['area']['passenger'][2]});
        area.find('.passenger').find('td:eq(4)').find('.value').html(data['area']['passenger'][3]+'%');

        // Tack circle progress
        var circleValue;
        $( ".circle" ).each(function( index ) {
          circleValue = $(this).attr('value');
          console.log(circleValue);
          $(this).progressCircle({
            nPercent        : circleValue,
            showPercentText : true,
            thickness       :  4,
            circleSize      :  80,
          });
        });

      },
      data: 'year='+year+'&provinceId='+provinceId
    });

  }

});
