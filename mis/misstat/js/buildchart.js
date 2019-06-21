$(document).ready(function() {
  if (Drupal.settings.misstat.flotEnabled) {
    var flot = Drupal.settings.misstat.flot;
    if ($('#'+flot.element.id).size() == 0) return;
    var offset = { x: -17, y: -30 };

    var showTooltip = function(x, y, contents) {
      $('<div class="tooltip">'+ contents + '</div>').css( {
        position: 'absolute',
        display: 'none',
        top: y + offset.y,
        left: x + offset.x
      }).appendTo('body').fadeIn(200);
    }
    var formatNumber = function(num) {
      num = num.toString();
      tmp = num.split('.');
      f = tmp[0];
      e = tmp.length > 1 ? '.' + tmp[1] : '';
      var r = /(\d+)(\d{3})/;
      while (r.test(f)) {
        f = f.replace(r, '$1' + ',' + '$2');
      }
      return f + e;
    }

    flot.options.yaxis.tickFormatter = function(val, axis) {
      return formatNumber(val.toFixed(axis.tickDecimals));
    }

    // Draw chart.
    var chart = $.plot($('#'+flot.element.id), flot.data, flot.options);
    // Bind plothover.
    $('#'+ flot.element.id).bind('plothover', function(event, pos, item) {
      if (item) {
        $('.tooltip').remove();
        var val = item.datapoint[1];
        showTooltip(item.pageX, item.pageY, formatNumber(val));
      }
      else {
        $('.tooltip').remove();
      }
    });
  }
});
