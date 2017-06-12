
$ = jQuery;
$(document).ready(function () {

    var pathModule = Drupal.settings.basePath + "sites/all/modules/thairoads/helmet";

    var initYearStart = 2554;
    var initYearEnd = 2558;
    var yearStart;
    var yearEnd;

    submitFilter();

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


            var year_list = "";
            $.each(data1['year_list'], function(key_y, value_y) {
              year_list += "<option value="+value_y+" class="+value_y+">"+value_y+"</option>";
            }); 

            $('#start_year').html(year_list);
            $('#end_year').html(year_list);

            $('#start_year  .'+yearStart).attr({'selected': 'selected'});
            $('#end_year  .'+yearEnd).attr({'selected': 'selected'});


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

});
