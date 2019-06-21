<?php
function jstree_render(){
  drupal_add_js(drupal_get_path('module', 'jstree') . '/_lib/jquery.js');
  drupal_add_js(drupal_get_path('module', 'jstree') . '/_lib/jquery.cookie.js');
  drupal_add_js(drupal_get_path('module', 'jstree') . '/_lib/jquery.hotkeys.js');
  drupal_add_js(drupal_get_path('module', 'jstree') . '/_lib/jquery.scrollTo.js');
  drupal_add_js(drupal_get_path('module', 'jstree') . '/jquery.jstree.js');
  drupal_add_js(drupal_get_path('module', 'jstree') . '/main.js');
  return '<div id="container">
  <div id="search_index_status" class="help">'. t("For search index, please use more than 3 characters keyword.") .'</div>
  <div>
    <form id="form_jstree_search">
      <input id="search_input" type="text" />
      <input id="search_btn" type="submit" value="Loading.." disabled="disabled" /> 
    </form>
  </div>
  <div id="jstree_tree_menu"></div>
  <div id="jstree_show">
    <span class="loading"><span>Loading..</span></span>
  </div>
  <div id="jstree_content"> 
    ' . t('Select item on the left side to view descriptions.') . '
  </div>
</div>
';
}

function jstree_node($node) {
  global $user;
  $output = '';
  $path = drupal_urlencode('species_index');
  
  // Enable caching with jstree
  /*
  $cid = 'jstree: node:nid: ' . $node->nid;
  $cache = cache_get($cid, 'cache');
  
  // Check this node cache is available
  if ($cache && isset($cache->data)) {
    $output .= $cache->data;
  }
  else {
  */
  if (true) {
    // Get node mlid from book
    $node_book = db_fetch_object(db_query("SELECT m.* FROM {book} b, {menu_links} m WHERE b.mlid=m.mlid AND nid=" . $node->nid));
    
    if (!empty($node_book)) {
      if (user_access('create book content')) {
        $max_depth = variable_get('book_max_depth', MENU_MAX_DEPTH);
        if ($node_book->depth < $max_depth) {
          $output .= '
    <div id="jstree_menu">
        ' . l(t('Add new child on <em>%title</em>', array('%title' => $node->title)), 'node/add/book', 
          array('html' => TRUE, 'query' => array(
            'parent'=>$node_book->mlid, 
            'destination' => $path . '#' . $node->nid . '--' . urlencode(trim($node->title)), // Refer to this module path
          ))) . '
    </div>';
        }
        else {
          $output .= '
    <div id="jstree_menu jstree_warning">' . t("Hierarchy level has been exceeded. (%level levels)", array('%level' => $max_depth)) . '</div>';
        }
      }
      
      if (function_exists('imagefield_elements')) {
        // Get imagefield field, only first one.
        $row = db_fetch_object(db_query("SELECT field_name FROM {content_node_field_instance} WHERE type_name='book' AND widget_module='imagefield' AND widget_active=1 LIMIT 1"));
        
        $field_name = $row->field_name;
        
        if (!empty($field_name)) {
          // Check multiple values field or not
          // 1 : multiple = 0, unlimit : multiple = 1, otherwise : multiple = N
          $field_settings = db_fetch_object(db_query("SELECT multiple FROM {content_node_field} WHERE field_name='{$field_name}'"));
          if ($field_settings->multiple > 0) {
            $field_table = "content_{$field_name}";
            $delta = " ORDER BY delta";
          }
          else {
            $field_table = "content_type_book";
            $delta = '';
          }
          
          $res = db_query("SELECT 
            imgfield." . $field_name . "_data data, f.filepath 
            FROM {" . $field_table . "} imgfield, {files} f 
            WHERE vid=" . $node->vid . " AND imgfield." . $field_name . "_fid=f.fid 
            AND f.status=1 AND " . $field_name . "_list=1 
            $delta");
          
          $img_output = '';
          /*
          if (db_affected_rows() > 0) {
            $row = db_fetch_object($res); // Get first item for header.
            $data = unserialize($row->data);
            $attributes['class'] = 'image-item';
            
          }
          */
          
          $img_index = 1;
          while ($row = db_fetch_object($res)) {
            $data = unserialize($row->data);
            $attributes['class'] = 'image-item';
            
            if ($img_index == 1) {
              $header .= ' ' . l(theme('imagecache', variable_get('jstree_large_imagecache_preset', ''), $row->filepath, $data['description'], $data['description'], $item_attributes),
              $row->filepath, 
              array('html' => TRUE, 'attributes'=>array('rel' => 'lightbox[samples][' . check_plain($data['description']) . ']'))) . "<span>{$data['description']}</span>";
            }
            
            $img_output .= ' ' . l(theme('imagecache', variable_get('jstree_imagecache_preset', ''), $row->filepath, $data['description'], $data['description'], $attributes) . "<span>{$data['description']}</span>",
              $row->filepath, 
              array('html' => TRUE, 'attributes'=>array('rel' => 'lightbox[samples][' . check_plain($data['description']) . ']')));
              
              
              if ($img_index % 3 == 0) {
                 $img_output .= "\n<div class=\"clear\">&nbsp;</div>\n";
              }
              
              $img_index++;
            //$img_output .= '<img src="' . $row->filepath . '" />';
          }
        }
        
        $output .= '  <div class="node-body">' . "\n";
        if ((user_access('edit own book content') && $node->uid == $user->uid) || user_access('edit any book content')) {
          $output .= '    <div class="edit-book">' . l(t('Edit content <em>%title</em>', array('%title' => $node->title)), 'node/' . $node->nid . '/edit', array('html' => TRUE, 'query' => array('destination' => $path . '#' . $node->nid . '--' . urlencode(trim($node->title))))) . "</div>\n";
        }
        if (function_exists('genesis_siamensis_node_submitted')) {
          $submitted = genesis_siamensis_node_submitted($node);
        }
        else {
          $submitted = t('Submitted by !username on @datetime',
            array(
              '!username' => theme('username', $node),
        //      '@datetime' => format_date($node->created),
              '@datetime' => strftime('%e %B %Ey %H:%M', $node->created),
            ));
        }
        $output .= "<h2 class=\"node-title\">{$node->title}</h2>";
        $output .= "<div class=\"node-submitted\">{$submitted}</div>";
        $output .= "<div class=\"node-header\">{$header}</div>";
        $output .= $node->body . '
    </div>';

        if ($img_output) {
          $output .= '  <div id="jstree_thumb">' . $img_output . '<div class="clear"></div></div>';
        }
        
        // Send transverse navigation for jstree.
        for ($i = 1; $i < $node_book->depth; $i++) {
          $var_name = 'p' . $i;
          $mlids[] = $node_book->$var_name;
        }
        
        if (count($mlids) > 0) {
          $mlids = implode(', ', $mlids);
          $res = db_query("SELECT nid FROM {book} b, {menu_links} l WHERE b.mlid=l.mlid AND b.mlid IN ($mlids) ORDER BY l.depth");
          while ($row = db_fetch_object($res)) {
            $nav[] = $row->nid;
          }
        }

        $nav[] = $node->nid;
        
        $output .= '<script type="text/javascript">jstree_nav = jQ.parseJSON(\'' . jstree_json($nav) . '\');</script>';
        
        // Save to cache
        cache_set($cid, $output, 'cache', time() + 60);
      }
    } 
  } // Cache
  
  // Cache results for delay some response.
  // Note: Aug 4, 2010 - Only Google Chrome couldn't cache this response properly.
  // Note: Sep 24, 2010 - try cache_set() but still load to db.
  /*
  $expires = 60;
  header("Pragma: public");
  header("Cache-Control: maxage=".$expires);
  header('Expires: ' . gmdate('D, d M Y H:i:s', time()+$expires) . ' GMT');
  */
  die($output);
}

function getTreeData($tree) {
  $data = array();
  foreach ($tree as $node_item) {
    $item = array();
    $item['data'] = $node_item['link']['title'];
    
    $nid = array_pop(explode('/', $node_item['link']['link_path'])); //eg. book-toc-[nid]
    
    $item['attr']['id'] = 'book-node-' . $nid;
    $item['attr']['link'] = url('node/' . $nid);
    if (!empty($node_item['below']) && is_array($node_item['below'])) {
      $item['children'] = getTreeData($node_item['below']);
    }
    $data[] = $item;
  }
  return $data;
}

function json(){
if($_GET['type']=='tree'){

  $books = book_get_books();
  $ans = array();
  
  foreach ($books as $book) {
    // Load menu tree in each book.
    //$tree = book_menu_subtree_data($book);
    $tree = menu_tree_all_data($book['menu_name']);
    //watchdog('tree', '<pre>' . print_r($tree,1) . '</pre>');
    //exit('<pre>' . print_r($tree2,1) . '</pre>');
    //print_dbg($tree);
    $ans[] = getTreeData($tree);
  }
}else{
  $ans=array();
}

die(jstree_json($ans));  // Escape <, >, &
}

function jstree_json($data) {
  return str_replace(array('<', '>', '&'),  array('\u003c', '\u003e', '\u0026'), json_encode($data));
}
