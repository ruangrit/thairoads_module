$ = jQuery;
//$(function() {
$(document).ready(function () {
//========================================================================

});


if (Drupal.jsEnabled) {
  $(document).ready(function () {

  var pathModule = Drupal.settings.basePath + "sites/all/modules/thairoads/helmet";
  var defaultYear = 2558;
  var currentYear;

  function renderMap(year) {
    $('.header-year').html(year);
    $( ".main-map > .map" ).each(function( index ) {
      var code = $(this).find('.code').val();
      var cateId = $(this).find('.cateId').val();
      var svgmap = $('#svgmap'+index).svg();

      svgmap.load(pathModule+'/img/Thai_map.svg', 'get', function (svg) {
        $('path').mouseover(function() {
         $(this).css({opacity: 0.4}); 
        })
        .mouseout(function() {
           $(this).css({opacity: 1}); 
        });
        load_map_data(year, code, 'svgmap'+index, cateId);
        
      });

    });
  }
  renderMap(defaultYear);

  $('#filter1').change(function() {
    renderMap(this.value);
  });

  $('#filter_cat').change(function() {
    var code_list = this.value.split(',');

    $( ".main-map > .map" ).each(function( index ) {
      var code = $(this).find('.code').val(code_list[index]);
    });
    var year = $('#filter1').val();
    renderMap(year);

  });
  
  /*--------------------------
  var code = $('#code').val(); 
  var cateId = $('#cateId').val(); 

  var svgmap = $('#svgmap').svg();
  svgmap.load(pathModule+'/img/Thai_map.svg', 'get', function (svg) {
    $('path').mouseover(function() {
     $(this).css({opacity: 0.4}); 
    })
    .mouseout(function() {
       $(this).css({opacity: 1}); 
    });
    load_map_data(2553, code, 'svgmap', catId);
    
  });

  // Map 2 ============================================================

  var svgmap2 = $('#svgmap2').svg();
  svgmap2.load(pathModule+'/img/Thai_map.svg', 'get', function (svg) {
    $('path').mouseover(function() {
     $(this).css({opacity: 0.4}); 
    })
    .mouseout(function() {
       $(this).css({opacity: 1}); 
    });
    
    
  });


  $('#filter1').change(function() {
    if ($('#filter1year').attr('checked')) {
      load_map_data(this.value, code, 'svgmap', catId);
    }
  });
  $('#filter2').change(function() {
    if ($('#filter2year').attr('checked')) {
      load_map_data(this.value, code, 'svgmap2', catId);
    }
  });
  */


  var cur_timestamp_click = 0;
    // ajax return
  var load_map_year_list_first = true;
  function helmet_get_map_data(data, mapId, year) {
    if (!data.subCateTerm) {
      $('#warning_term_level').show();
    }
    // render year list to select box
    var map_year_list = "";
    $.each(data.map_year_list, function(key_y, value_y) {
      map_year_list += "<option value="+value_y+" class="+value_y+">"+value_y+"</option>";
    }); 
    var yearLength   = data.map_year_list.length;
    if (load_map_year_list_first) {
      $('.filter-year-choice').html(map_year_list);
      load_map_year_list_first = false;
      $('#filter1  .'+year).attr({'selected': 'selected'});
    }
    var listOfValue = [];
    $.each(data, function(key, value){
      if (typeof data[key]['name'] !== "undefined") {
        $('#'+mapId+' #pv'+key+' path').attr({'fill': data[key]['color']});
        $('#'+mapId+' #pv'+key+' path').attr({'data': data[key]['name']+": "+data[key]['value']});
        $('#'+mapId+'-info .'+key).html(data[key]['value']);
        $('#'+mapId+' #pv'+key+' path').attr({'ismap': 'ismap'});

        listOfValue[data[key]['name']] = data[key]['value'].replace('%', '');
        //$('#'+mapId+' #pv'+key+' path[ismap = "ismap"]').tipsy('show');
        //===console.log(key + ": " + data[key]['name']+": "+ data[key]['value']);
      }
    });

    // find Top-Ten
    var sortable = [];
    for (var vehicle in listOfValue) {
      sortable.push([vehicle, listOfValue[vehicle]]);
    }
    sortable.sort(function(a, b) {
      return b[1] - a[1];
    });

    var runNumSort = 1; 
    var topTenOutput = '';
    $.each(sortable, function(key, value){
      if (runNumSort <= 10) {
        topTenOutput += '<div>'+runNumSort+'. '+value[0]+' '+value[1]+'%</div>';

      }
      else {
        return false;
      }

      runNumSort++;
    });
    $('#'+mapId).siblings('.top-ten').html(topTenOutput);


    var options = {
        title: 'data', 
        delayIn: 300, 
        gravity: 'e', 
        offset: -15, 
        follow: 'y', 
        delayOut: 300, 
        trigger: 'manual'
    }

    function get(ele) {
        var tipsy = $.data(ele, 'tipsy');
        if (!tipsy) {
            tipsy = new Tipsy(ele, $.fn.tipsy.elementOptions(ele, options));
            $.data(ele, 'tipsy', tipsy);
        }
        return tipsy;
    }

    function enter(event) {

        if (cur_timestamp_click == event.timeStamp) {
          return false;
        }
        cur_timestamp_click = event.timeStamp;
        
        var me = this;
        
        if (parseInt($(this).attr('data-clicked'))) {
          return false;
        }

        var tipsy = get(this);
        tipsy.hoverState = 'in';

        if (options.delayIn == 0) {
            tipsy.show();
        } else {
            setTimeout(function() { 
              if (tipsy.hoverState == 'in') { 
                tipsy.show(); 
                $(tipsy.$tip).css('top', event.pageY-($(tipsy.$tip).outerHeight()/2));

                $(tipsy.$tip).mouseleave(function () {

                  if (parseInt($(me).attr('data-clicked'))) {
                    return false;
                  }
                  tipsy.hide();
                }).click(function () {
                  $(me).trigger('click');
                });

              }
            }, options.delayIn);
        }
    };

    function move(event) {

        if (cur_timestamp_click == event.timeStamp) {
          return false;
        }
        cur_timestamp_click = event.timeStamp;


        if (parseInt($(this).attr('data-clicked'))) {
          return false;
        }

        var tipsy = get(this);
        tipsy.hoverState = 'in';
        if (options.follow == 'x') {
            var arrow = $(tipsy.$tip).children('.tipsy-arrow');
            if (/^[^w]w$/.test(options.gravity) && arrow.position() != null) {
                var x = event.pageX - ($(arrow).position().left+($(arrow).outerWidth()/2));
            } else if (/^[^e]e$/.test(options.gravity) && arrow.position() != null) {
                var x = event.pageX - ($(arrow).position().left+($(arrow).outerWidth()/2));
            } else {
                var x = event.pageX - ($(tipsy.$tip).outerWidth()/2);
            }
            $(tipsy.$tip).css('left', x);
        } else if (options.follow == 'y') {
            if (/^w|^e/.test(options.gravity) ) {
                $(tipsy.$tip).css('top', event.pageY-($(tipsy.$tip).outerHeight()/2));
            }
        }

    }

    function leave(event) {

        if (cur_timestamp_click == event.timeStamp) {
          return false;
        }
        cur_timestamp_click = event.timeStamp;


        if (parseInt($(this).attr('data-clicked'))) {
          return false;
        }

        var tipsy = get(this);

        if (tipsy.$tip && tipsy.$tip[0] == $(event.relatedTarget).parent().get(0)) {
          return false;
        }
        tipsy.hoverState = 'out';

        if (options.delayOut == 0) {
            tipsy.hide();
        } else {
            setTimeout(function() { 
              if (tipsy.hoverState == 'out') {
                tipsy.hide(); 
              } 
            }, options.delayOut);
        }

    };


    
    function click(event) {
        
        if (cur_timestamp_click == event.timeStamp) {
          return false;
        }
        cur_timestamp_click = event.timeStamp;

        var tipsy = get(this);

        if (parseInt($(this).attr('data-clicked'))) {
          $(this).attr({'data-clicked': 0}); 
          //$(this).tipsy('hide');
          tipsy.hide();
        }
        else {
          $(this).attr({'data-clicked': 1}); 
         // $(this).tipsy('show');
          tipsy.show();
          //$(tipsy.$tip).click(function () {
          //  $(this).hide();
          //});
        }
        $(tipsy.$tip).css('top', event.pageY-($(tipsy.$tip).outerHeight()/2));

    }

    $('#maps g path[ismap = "ismap"]').tipsy(options).mouseenter(enter).mouseleave(leave).mousemove(move).click(click);


  }


  function load_map_data(year, code, mapId, catId) {
    $('.loading'+mapId).remove();
    $('.warning-year-select'+mapId).remove();
    $('#'+mapId).before("<div class='loading loading"+mapId+"'>Data loading...</div>"); 
    if (typeof yearStart === 'undefined') {
      yearStart = year;
    }
    // ajax call
    $.ajax({
      type: 'POST',
      url: Drupal.settings.basePath + 'helmet/mapdata',
      dataType: 'json',
      success: function(data) {
        helmet_get_map_data(data, mapId, year);   
        $('.loading'+mapId).remove();
      },
      data: 'year='+year+'&code='+code+'&catId='+catId
    });
      
  }

  // ====================================== End qtip

  });
}

