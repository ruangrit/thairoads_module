
$ = jQuery;
$(document).ready(function () {

    var pathModule = Drupal.settings.basePath + "sites/all/modules/thairoads/helmet";

    var initYearStart;
    var initYearEnd;
    var yearStart;
    var yearEnd;

    $.ajax({
        type: 'POST',
        url: Drupal.settings.basePath + 'helmet/yearlist',
        dataType: 'json',
        success: function(data) {
            initYearEnd = data.slice(-1)[0];
            initYearStart = data.slice(-5)[0];
            submitFilter();

            var year_option = "";
            $.each(data, function(key_y, value_y) {
              year_option += "<option value="+value_y+" class="+value_y+">"+value_y+"</option>";
            }); 

            $('#start_year').html(year_option);
            $('#end_year').html(year_option);

            $('#start_year  .'+initYearStart).attr({'selected': 'selected'});
            $('#end_year  .'+initYearEnd).attr({'selected': 'selected'});
        },
        data: 'code=C5_01-C2_01',
    });






    function submitFilter() {

        if ($('#start_year').val() == null) {
           yearStart = initYearStart;
           yearEnd = initYearEnd;
        } 
        else {

           yearStart = $('#start_year').val(); 
           yearEnd = $('#end_year').val(); 

        }

        var catId = '';
        var categories;
        var series = [];

        var codeDetail = [
           {'d1': 'dddd'},
           {'d2': 'dddd2'},
        ];

        var codeValueList = $('.code-value-filter-hidden').serialize();
        var codeNameList = $('.code-name-filter-hidden').serialize();
        $.ajax({
          type: 'POST',
          url: Drupal.settings.basePath + 'helmet/chartdata',
          dataType: 'json',
          success: function(data1) {

            var runIndex = 0;  
            $.each(data1['data'], function(index1, value1) {
              // do your stuff here
                var tempData = [];
                var tempYear = [];
                $.each(value1, function(index2, value2) {
                    tempData.push(value2); 
                    tempYear.push(index2); 
                });

                series[runIndex] = {
                    name: index1,
                    data: tempData, 
                }

                categories = tempYear;

              runIndex++;
            });

            renderChart(categories, series, 'อัตราการสวมหมวกนิรภัยตั้งแต่ปี ' + yearStart + ' ถึง ' + yearEnd);
          },
          data: 'yearStart='+yearStart+'&catId='+catId+'&yearEnd='+yearEnd+'&'+codeValueList+'&'+codeNameList
        });

    }

    function renderChart(categories, series, title) {

        Highcharts.chart('container', {
            chart: {
                type: 'line'
            },
            title: {
                text: title 
            },
            xAxis: {
                categories: categories 
            },
            yAxis: {
                title: {
                    text: ''
                },
                labels: {
                    format: "{value} %"
                }
            },
            plotOptions: {
                line: {
                    dataLabels: {
                        enabled: true,
                        format: '{y} %'
                    },
                    enableMouseTracking: false
                }
            },
            series: series

        });

    }

    $('#filter_btn').click(function () {
        submitFilter();
    });

    $('#position_filter_btn').click(function () {

        var code_list = $('#position_filter').val().split(',');
        $( ".code-value-filter-hidden" ).each(function( index ) {
          $(this).val(code_list[index]);
        });
        submitFilter();
    });

});
