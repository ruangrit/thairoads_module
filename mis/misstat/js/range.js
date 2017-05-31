$(document).ready(function() {
  $('#range-start, #range-end').change(function(e) {
    if (this.id == 'range-start') {
      var comparator = $('#range-end');
      if (comparator.val() - this.value <= 0) {
        var possibleVal = parseInt(this.value) + 1;
        if ($('option[value='+possibleVal+']', comparator).length > 0) {
          comparator.val(possibleVal);
        }
        else {
          this.value = parseInt(this.value) - 1
          comparator.val(parseInt(this.value) + 1);
        }
      }
    }
    else {
      var comparator = $('#range-start');
      if (this.value - comparator.val() <= 0) {
        var possibleVal = parseInt(this.value) - 1;
        if ($('option[value='+possibleVal+']', comparator).length > 0) {
          comparator.val(possibleVal);
        }
        else {
          this.value = parseInt(this.value) + 1;
          comparator.val(parseInt(this.value) - 1);
        }
      }
    }
  });
  $('#range-submit').click(function(e) {
    var $start = $('#range-start').val();
    var $end = $('#range-end').val();
    if ($start > $end) {
      alert(Drupal.settings.misstat.alert_message);
      e.preventDefault();
    }
  });
});
