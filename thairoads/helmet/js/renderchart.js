
$ = jQuery;
$(document).ready(function () {

    var pathModule = Drupal.settings.basePath + "sites/all/modules/thairoads/helmet";

    var initYearStart = 2554;
    var initYearEnd = 2558;

    var yearStart = initYearStart;
    var yearEnd = initYearEnd;
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
        //myhook_get_map_data(data, mapId); 
        var runIndex = 0;  
        $.each(data1, function(index1, value1) {
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

        renderChart(categories, series);
      },
      data: 'yearStart='+yearStart+'&catId='+catId+'&yearEnd='+yearEnd+'&codeValueList='+codeValueList+'&codeNameList='+codeNameList
    });

    function renderChart(categories, series, title) {

        Highcharts.chart('container', {
            chart: {
                type: 'line'
            },
            title: {
                text: 'Monthly Average Temperature'
            },
            xAxis: {
                categories: categories 
            },
            yAxis: {
                title: {
                    text: ''
                }
            },
            plotOptions: {
                line: {
                    dataLabels: {
                        enabled: true
                    },
                    enableMouseTracking: false
                }
            },
            series: series

        });

    }

});
