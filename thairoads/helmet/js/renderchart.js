
$ = jQuery;
$(document).ready(function () {

    var pathModule = Drupal.settings.basePath + "sites/all/modules/thairoads/helmet";

    var initYearStart;
    var initYearEnd;
    var yearStart;
    var yearEnd;
    var chartType = $('#chart_type').val();


    $.ajax({
        type: 'POST',
        url: Drupal.settings.basePath + 'helmet/yearlist',
        dataType: 'json',
        success: function(data) {
            initYearEnd = data.slice(-1)[0];
            initYearStart = data.slice(-5)[0];
            if (typeof chartType == 'undefined') {
                submitFilter();
            }
            else {
                submitFilterColumn();
            }

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



    function submitFilterColumn() {

        if ($('#start_year').val() == null) {
           yearStart = initYearStart;
           yearEnd = initYearEnd;
        } 
        else {
           yearStart = $('#start_year').val(); 
           yearEnd = $('#end_year').val(); 
        }

        var catId = '';
        var categories = [];
        var series = [];
        var tempYear = [];

        var codeDetail = [
           {'d1': 'dddd'},
           {'d2': 'dddd2'},
        ];

        var code = $('#position_filter').val();
        $.ajax({
          type: 'POST',
          url: Drupal.settings.basePath + 'helmet/chartdata',
          dataType: 'json',
          success: function(data1) {
            var runIndex = 0;  
            $.each(data1, function(index1, value1) {
              // do your stuff here
                $.each(value1, function(index2, value2) {   
                    if(typeof tempYear[index2+'x'] == 'undefined') {
                        tempYear[index2+'x'] = [];
                    }
                    tempYear[index2+'x'].push(value2);
                    
                });

                categories.push(index1);

              runIndex++;
            });

            runIndex = 0
            for(var key in tempYear) {
                var tempData = [];
                for( key2 in tempYear[key]) {
                    tempData.push(tempYear[key][key2]);
                }
                series[runIndex] = {
                    name: key.replace('x', ''),
                    data: tempData,
                };
                runIndex++;
            }
            renderColumnChart(categories, series, 'อัตราการสวมหมวกนิรภัยตั้งแต่ปี ' + yearStart + ' ถึง ' + yearEnd);
          },
          data: 'yearStart='+yearStart+'&code='+code+'&yearEnd='+yearEnd+'&withArea=1'
        });

    }

    function renderColumnChart(categories, series, title) {
        Highcharts.chart('container', {
            chart: {
                type: 'column'
            },
            title: {
                text: 'Monthly Average Rainfall'
            },
            subtitle: {
                text: 'Source: WorldClimate.com'
            },
            xAxis: {
                categories: categories, 
                crosshair: true
            },
            yAxis: {
                min: 0,
                title: {
                    text: 'Rainfall (mm)'
                }
            },
            tooltip: {
                headerFormat: '<span style="font-size:10px">{point.key}</span><table>',
                pointFormat: '<tr><td style="color:{series.color};padding:0">{series.name}: </td>' +
                    '<td style="padding:0"><b>{point.y:.1f} mm</b></td></tr>',
                footerFormat: '</table>',
                shared: true,
                useHTML: true
            },
            plotOptions: {
                column: {
                    pointPadding: 0.2,
                    borderWidth: 0
                }
            },
            series: series,
        });

    }


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
