$(document).ready(function() {
  $('select.summary-select').change(function(e) {
    if (this.value != 0) {
      //window.open(this.value, 'download_window', 'width=400,height=200');
      window.open(this.value, '_blank');
      this.value = 0;
    }
  });

  $('select.summary-year').change(function(e) {
    var url = $(this).val();
    if (url) {
      window.location = url;
    }
    return false;
  });
  if ($(location).attr('search')) {
    $('select.summary-year').val($(location).attr('href'));
  }
});
