$(document).ready(function () {
  var nav  = '#frameHoverNav';
  $(nav).live('mouseover', function() {
    console.log(Lightbox);
    var total = Lightbox.total;
    var current = Lightbox.activeImage;
    if (current == 0 && total > 0) {
      $(nav+' #frameNextLink').show(); 
    }
    else if (current == (total-1) && total > 0) {
      $(nav+' #framePrevLink').show(); 
    }
    else {
      $(nav+' a').show(); 
    }
  });
  $(nav).live('mouseout', function() { $(nav+' a').hide(); });
});
