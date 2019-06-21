$ = jQuery;
//$(function() {
$(document).ready(function () {
//========================================================================

});


if (Drupal.jsEnabled) {
  $(document).ready(function () {

  var sub_cat = $('#sub_cat').val(); 
  var pathModule = Drupal.settings.basePath + "sites/all/modules/thairoads/myhook";
  var svgmap = $('#svgmap').svg();
  svgmap.load(pathModule+'/img/Thai_map.svg', 'get', function (svg) {
    $('path').mouseover(function() {
     $(this).css({opacity: 0.4}); 
    })
    .mouseout(function() {
       $(this).css({opacity: 1}); 
    });
    load_map_data(2548, sub_cat, 'svgmap');
    
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
      load_map_data(this.value, sub_cat, 'svgmap');
    }
  });
  $('#filter2').change(function() {
    if ($('#filter2year').attr('checked')) {
      load_map_data(this.value, sub_cat, 'svgmap2');
    }
  });
  // =================================== Duration filter
  $('#filter2_end').change(function() {
    var check_value = map_check_value($(this), $('#filter2_start'), 'svgmap2'); 
    if ($('#filter2duration').attr('checked') && $('#filter2_end').val() != 'all' && $('#filter2_start').val() != 'all' && check_value) {
      load_map_data(this.value, sub_cat, 'svgmap2', $('#filter2_start').val());
    }
  });
  $('#filter2_start').change(function() {
    $('#filter2_end').trigger('change');
  });
  $('#filter2duration').click(function() {
    $('#filter2_end').trigger('change');
  });
  $('#filter2year').click(function() {
    $('#filter2').trigger('change');
  });
  // ===========================  Duration filter 2
  $('#filter1_end').change(function() {
    var check_value = map_check_value($(this), $('#filter1_start'), 'svgmap'); 
    if ($('#filter1duration').attr('checked') && $('#filter1_end').val() != 'all' && $('#filter1_start').val() != 'all' && check_value) {
      load_map_data(this.value, sub_cat, 'svgmap', $('#filter1_start').val());
    }
  });
  $('#filter1_start').change(function() {
    $('#filter1_end').trigger('change');
  });
  $('#filter1duration').click(function() {
    $('#filter1_end').trigger('change');
  });
  $('#filter1year').click(function() {
    $('#filter1').trigger('change');
  });


  function map_check_value (obj, objStart, mapId) {
    $('.warning-year-select'+mapId).remove();
    if (obj.val() < objStart.val() && !(obj.val() == 'all' || objStart.val() == 'all')) {
      $(obj).after("<div class='warning warning-year-select warning-year-select"+mapId+"'>ปีที่สิ้นสุดต้องไม่น้อยกว่าปีเริ่มต้น</div>");
      return false;
    }
    else {
      return true;
    }
  }

  var cur_timestamp_click = 0;
    // ajax return
  var load_map_year_list_first = true;
  function myhook_get_map_data(data, mapId) {
    if (!data.subCateTerm) {
      $('#warning_term_level').show();
    }
    // render year list to select box
    var map_year_list = "";
    $.each(data.map_year_list, function(key_y, value_y) {
      map_year_list += "<option value="+value_y+" class="+value_y+">"+value_y+"</option>";
    }); 
    var yearLength   = data.map_year_list.length;
    var yearDefault1 = data.map_year_list[1];
    var yearDefault2 = data.map_year_list[0];
    if (load_map_year_list_first) {
      $('.filter-year').html(map_year_list);
      $('#filter2_start').prepend("<option value='all'>เลือกปีเริ่มต้น</option>");
      $('#filter1_start').prepend("<option value='all'>เลือกปีเริ่มต้น</option>");
      $('#filter2_end').prepend("<option value='all'>เลือกปีสิ้นสุด</option>");
      $('#filter1_end').prepend("<option value='all'>เลือกปีสิ้นสุด</option>");
      load_map_year_list_first = false;
      $('#filter1  .'+yearDefault1).attr({'selected': 'selected'});
      $('#filter2  .'+yearDefault2).attr({'selected': 'selected'});
      $('#filter1').trigger('change');
      $('#filter2').trigger('change');
      
    }
    $.each(data, function(key, value){
      if (typeof data[key]['name'] !== "undefined") {
        $('#'+mapId+' #pv'+key+' path').attr({'fill': data[key]['color']});
        $('#'+mapId+' #pv'+key+' path').attr({'data': data[key]['name']+": "+data[key]['value']});
        $('#'+mapId+'-info .'+key).html(data[key]['value']);
        $('#'+mapId+' #pv'+key+' path').attr({'ismap': 'ismap'});
        //$('#'+mapId+' #pv'+key+' path[ismap = "ismap"]').tipsy('show');
        //===console.log(key + ": " + data[key]['name']+": "+ data[key]['value']);
      }
    });


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


  function load_map_data(year, sub_cat, mapId, yearStart) {
    $('.loading'+mapId).remove();
    $('.warning-year-select'+mapId).remove();
    $('#'+mapId).before("<div class='loading loading"+mapId+"'>Data loading...</div>"); 
    if (typeof yearStart === 'undefined') {
      yearStart = year;
    }
    // ajax call
    $.ajax({
      type: 'POST',
      url: Drupal.settings.basePath + 'myhook/mapdata',
      dataType: 'json',
      success: function(data) {
        myhook_get_map_data(data, mapId);   
        $('.loading'+mapId).remove();
      },
      data: 'year='+year+'&sub_cat='+sub_cat+'&yearStart='+yearStart
    });
      
  }

  // ====================================== End qtip

  });
}

