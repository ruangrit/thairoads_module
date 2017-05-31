$(document).ready(function() {
  var prefix_path = Drupal.settings.misstat.prefix_path;
  $('div.province-select select').change(function(e) {
    var self = this;
    $('ul.page-provincial a').each(function(i, obj) {
      $(obj).attr('href', prefix_path +'/'+ self.value +'/'+ obj.id);
    });
    
    // Toggle.
    if (self.value == 0) {
      $('ul.page-provincial').slideUp();
    }
    else {
      $('ul.page-provincial').slideDown();
    }
  });

  $('div.province-switch select').change(function(e) {
    var redirect = Drupal.settings.basePath;
    redirect += Drupal.settings.misstat.path.prefix;
    redirect += this.value;
    redirect += Drupal.settings.misstat.path.suffix;
    window.location = redirect;
  });
});
