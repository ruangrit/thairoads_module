var jQ = jQuery.noConflict();

/**
 * Get querystring parameters function.
*/
getParameterByName = function( name, path ) {
  name = name.replace(/[\[]/,"\\\[").replace(/[\]]/,"\\\]");
  var regexS = "[\\?&]"+name+"=([^&#]*)";
  var regex = new RegExp( regexS );
  var results = regex.exec( path );
  if( results == null )
    return "";
  else
    return decodeURIComponent(results[1].replace(/\+/g, " "));
};

setTreeLinks = function() {
  jQ('.roti-search-item a').click( function(e) {
      e.preventDefault();
      var title = jQ(this).html();
      var target = (jQ(this).attr('href')).split('#');
      var last_index = target.length -1;
      jQ('#form_jstree_search').append('<span class="loading"><span>Loading..</span></span>');
      
      // Open tree
      jQ.get(document.location.pathname + '/node/' + target[last_index], function(data) {
        document.location.hash = target[last_index];
        jQ('#jstree_content').html(data);
        
        jQ('#jstree_thumb').html($('#image-items-hidden').html());
        jQ('#image-items-hidden').detach();
        if (jQ('#jstree_menu').hasClass('jstree_warning')) {
           jQ('#jstree_tree_menu').addClass('jstree_warning');
        }
        else {
          jQ('#jstree_tree_menu').removeClass('jstree_warning');
        } 
        
        jQ('#jstree_tree_menu').html(jQ('#jstree_menu').html());
        jQ('#jstree_menu').remove();
        jQ('.loading').remove();
        
        if (jQ('#jstree_thumb').html().length > 0) {
          Lightbox.initList();
        }
        
        var tree = $.jstree._reference("#jstree_show");
        for (id in jstree_nav) {
          tree.open_node(jQ('#book-node-' + jstree_nav[id]));
        }
        jQ('#book-node-' + jstree_nav[id] + ' > a').addClass("jstree-clicked");
      });
      //jQ('#jstree_show').jstree('search', title);
      /*
      jQ('#jstree_show li a').each( function(index) {
        if (jQ(this).contents()[1].data == title) {
          jQ(this).click();
          
          //var limitter = 30;
          var current_list = jQ(this).parent();
          while (current_list.parent('ul').length > 0) {
            if (current_list.hasClass('jstree-closed')) {
              current_list.removeClass('jstree-closed').addClass('jstree-open');
            }
            current_list = current_list.parent().parent();
            //if (limitter > 30) break;
            //limitter++;
          }
          return false;
       }
     });
     */
  });
}

searchIndex = function() {
  var keywords = jQ('#search_input').val();
  if (keywords.length > 2) {
     jQ('#form_jstree_search').append('<span class="loading"><span>Loading..</span></span>');
     if (typeof(page) === 'undefined')
       page = 0;
     
     jQ.post('/rotisearch/result', { 'raw': '1', 'query': keywords, 'quick': 'type:book', 'page':page}, function(data) {
        jQ('#jstree_content').html(data);
        jQ('#jstree_thumb').html('');
        // Hide images
        jQ('#jstree_content .layer-first, #jstree_content .terms').hide();
        // Reset tree.
        jQ('#jstree_show li').addClass('jstree-closed');
        
        // Create load contents events.
        jQ('.pager a').click( function(e) {
          e.preventDefault();
          page = getParameterByName('page', jQ(this).attr('href'));
          searchIndex();
        });
        
      setTreeLinks();
      jQ.scrollTo( '#search_index_status', 800 );
      jQ('.loading').remove();
    });
  }
  else {
  }
}

jQ(document).ready(function (e) {
  $.get('json', {'type':'tree'}, function(response) {
    data = jQ.parseJSON(response);
    //data = response;
    jQ('#jstree_show').bind("loaded.jstree", function (event, data) {
          // Enable search engine.
          jQ('#search_btn').removeAttr('disabled').val('Search');
	        var current_item = '';
          jQ('#jstree_show li a').each( function(index) {
            var link = jQ(this).parent().attr('link');
            if (link !== undefined) {
              jQ(this).attr('href', link);
              jQ(this).click( function(e) {
                e.preventDefault();
                var target = (jQ(this).attr('href')).split('/');
                var last_index = target.length -1;
                
                if (current_item != target[last_index]) {
                  target = current_item = target[last_index];
                  //var this_title = jQ(this).text().trim();
                  var this_title=jQ.trim(jQ(this).text());
                  jQ(this).append('<span class="loading"><span>Loading..</span></span>');
                  
                  jQ.get(document.location.pathname + '/node/' + target, function(data) {
                    document.location.hash = this_title;
                    jQ('#jstree_content').html(data);
                    jQ('#jstree_thumb').html($('#image-items-hidden').html());
                    jQ('#image-items-hidden').detach();
                    if (jQ('#jstree_menu').hasClass('jstree_warning')) {
                       jQ('#jstree_tree_menu').addClass('jstree_warning');
                    }
                    else {
                      jQ('#jstree_tree_menu').removeClass('jstree_warning');
                    } 
                    
                    jQ('#jstree_tree_menu').html(jQ('#jstree_menu').html());
                    jQ('#jstree_menu').remove();
                    jQ('.loading').remove();
                    if (jQ('#jstree_thumb').html().length > 0) {
                      Lightbox.initList();
                    }
                  });
                }
              });
            }
            jQ('.loading').remove();
          });

          jQ('#form_jstree_search').submit(function(e) {
            e.preventDefault();
            searchIndex();
          });
           
          var target = document.location.hash;
          if (target.length > 1) {
            target = target.substring(1, target.length);
            // Open tree
            jQ.get(document.location.pathname + '/node/' + target, function(data) {
              document.location.hash = target;
              jQ('#jstree_content').html(data);
              
              jQ('#jstree_thumb').html($('#image-items-hidden').html());
              jQ('#image-items-hidden').detach();
              if (jQ('#jstree_menu').hasClass('jstree_warning')) {
                 jQ('#jstree_tree_menu').addClass('jstree_warning');
              }
              else {
                jQ('#jstree_tree_menu').removeClass('jstree_warning');
              } 
              
              jQ('#jstree_tree_menu').html(jQ('#jstree_menu').html());
              jQ('#jstree_menu').remove();
              jQ('.loading').remove();
              
              if (jQ('#jstree_thumb').html().length > 0) {
                Lightbox.initList();
              }
              
              var tree = $.jstree._reference("#jstree_show");
              for (id in jstree_nav) {
                tree.open_node(jQ('#book-node-' + jstree_nav[id]));
              }
              jQ('#book-node-' + jstree_nav[id] + ' > a').addClass("jstree-clicked");
            });
            /*
            
            if (title != '') {
              jQ('#jstree_show li a').each( function(index) {
                if (jQ.trim(jQ(this).contents()[1].data) == title) {
                  jQ(this).click();
                  var limitter = 20;
                  var current_list = jQ(this).parents();
                  while (current_list.parents().is('ul')) {
                    if (current_list.hasClass('jstree-closed')) {
                      current_list.removeClass('jstree-closed').addClass('jstree-open');
                    }
                    current_list = jQ(this).parents().parents();
                    if (limitter > 20) break;
                    limitter++;
                  }
                  
                  jQ(this).addClass('activeLocation');
                  return false;
               }
             });
            }
            */
          }
	    }).jstree({
      'ext' : { 'preload' : document.location.hash.length > 1 ? true : false },
      'json_data' : {
        'data' : data
        ,'progressive_render': true
      },
      'search' : { 'case_insensitive' : true },
      'plugins' : [ 'themes', 'json_data','search' ,'ui']
    }).bind("open_node.jstree", function(e,data) {
      var current_item = '';
      testVar = data.args[0];
      if (typeof(data.args[0].children)  == 'function' && typeof(data.args[0].children('ul').children) == 'function') {
        data.args[0].children('ul').children('li').children('a').each( function(index) {
          var link = jQ(this).parent().attr('link');
          if (link !== undefined) {
            jQ(this).attr('href', link);
            jQ(this).click( function(e) {
              e.preventDefault();
              var target = (jQ(this).attr('href')).split('/');
              var last_index = target.length -1;
              
              if (current_item != target[last_index]) {
                target = current_item = target[last_index];
                var this_title=jQ.trim(jQ(this).text());
                jQ(this).append('<span class="loading"><span>Loading..</span></span>');
                testVar = jQ(this);
                
                jQ.get(document.location.pathname + '/node/' + target, function(data) {
                  document.location.hash = this_title;
                  jQ('#jstree_content').html(data);
                  jQ('#jstree_thumb').html($('#image-items-hidden').html());
                  jQ('#image-items-hidden').detach();
                  if (jQ('#jstree_menu').hasClass('jstree_warning')) {
                     jQ('#jstree_tree_menu').addClass('jstree_warning');
                  }
                  else {
                    jQ('#jstree_tree_menu').removeClass('jstree_warning');
                  }
                  jQ('#jstree_tree_menu').html(jQ('#jstree_menu').html());
                  jQ('#jstree_menu').remove();
                  jQ('.loading').remove();
                  if (jQ('#jstree_thumb').html().length > 0) {
                    Lightbox.initList();
                  }
                  
                  jQ('.loading').remove();
                });
              }
            });
          }
        });
      }
    });
 });
 
 // Enable search button when things done.
 jQ('#form_jstree_search #btn').removeAttr('disabled');
});

$ = jQuery;
