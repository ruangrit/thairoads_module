<?php

/**
 * Provincial overview.
 */
function misstat_provincial_overview($province = NULL, $spi1 = NULL, $spi2 = NULL) {
  // Add Javascript and CSS files.
  drupal_add_js(drupal_get_path('module', 'misstat') .'/js/misstat_collapse.js');
  drupal_add_css(drupal_get_path('module', 'misstat') .'/css/misstat.css');
  
  // Initial breadcrumb.
  $breadcrumb = array(
    l('หน้าแรก', '<front>'),
    l('สถิติและข้อมูล', 'statistic'),
  );
  
  if ($province && $spi1) {
    // Add breadcrumb.
    $breadcrumb[] = l('ระดับจังหวัด', 'statistic/provincial');
    $breadcrumb[] = l($province->name, 'statistic/provincial/'. $province->area_id);
    
    $items = array();
    foreach ($spi1->children as $parent) {
      // Exclude term 26 when on bangkok (area_id=4).
      if ($province->area_id == 4 && $parent->tid == 26) continue;
      
      if ($spis = taxonomy_get_children($parent->tid)) {
        $parent_code = db_result(db_query("SELECT field_code FROM {term_fields_term} WHERE tid = %d", $parent->tid));
        $expanded = FALSE;
        if ($spi2) {
          if ($spi2->tid == $parent->tid) $expanded = TRUE;
        }

        if (user_access('administer taxonomy')) {
          $parent->description .= '<br />'. l(t('[edit]'), 'admin/content/taxonomy/edit/term/'. $parent->tid, array('query' => drupal_get_destination()));
        }

        $items[$parent->tid] = array(
          'title' => $parent->name,
          'description' => $parent->description,
          'expanded' => $expanded,
        );

        if ($url = taxonomy_image_get_url($parent->tid)) {
          $items[$parent->tid]['image'] = $url;
        }
        
        $paths = explode('/', $_GET['q']);
        $paths = array_slice($paths, 0, 4);
        
        foreach ($spis as $spi) {
          $code = db_result(db_query("SELECT field_code FROM {term_fields_term} WHERE tid = %d", $spi->tid));
          $path = implode('/', $paths) .'/'. $parent_code .'/'. $code;
          $normal_path = drupal_get_normal_path($path);
          if (user_access('administer taxonomy')) {
            $link = l(t('[edit]'), 'admin/content/taxonomy/edit/term/'. $spi->tid, array('query' => drupal_get_destination()));
          }
          $items[$parent->tid]['items'][] = array(
            'class' => 'provincial-'. $spi->tid,
            'title' => l($spi->name, $path),
            'description' => $spi->description,
            'link' => $link,
          );
        }
      }
    }
    
    $output = theme('misstat_statistic_list', $items);
  }
  else {
    // Base path.
    $paths = array(arg(0), arg(1));
    // Add Javascript.
    $settings['misstat']['prefix_path'] = base_path() . implode('/', $paths);
    drupal_add_js($settings, 'setting');
    drupal_add_js(drupal_get_path('module', 'misstat') .'/js/misstat.js');
    // Description.
    $output = variable_get('misstat_provincial_description', '');
    // Edit link.
    if (user_access('administer misstat')) {
      $output .= '<div class="edit-content">'. l(t('Edit content'), 'admin/settings/misstat', array('query' => drupal_get_destination())) .'</div>';
    }
    // Provincial tid.
    $tid = 18;
    $terms = taxonomy_get_children($tid);

    // Get fullpath of taxonomy image.
    if (module_exists('taxonomy_image')) {
      $mypath = variable_get('taxonomy_image_path', 'category_pictures') .'/';
      $fullpath = file_directory_path() .'/'. $mypath;
    }

    // Get provinces.
    $tid = 48;
    $provinces = taxonomy_get_children($tid);

    // Make province options for select element.
    $options = array(0 => t('- Please select -'));
    foreach ($provinces as $p) {
      if ($area_id = db_result(db_query("SELECT field_area_id FROM {term_fields_term} WHERE tid = %d", $p->tid))) {
        $options[$area_id] = $p->name;
      }
    }

    // Make select element.
    $elements['province'] = array(
      '#type' => 'select',
      '#title' => t('Please select province'),
      '#options' => $options,
      '#value' => $province ? $province->area_id : 0,
      '#parents' => array(),
    );

    $output .= '<div class="province-select">';
    // Use form for W3C standard.
    $output .= '<form method="post">';
    $output .= drupal_render($elements);
    $output .= '</form>';
    $output .= '</div>';

    if ($province) {
      $class = '';
      $paths[] = $province->area_id;
    }
    else {
      $class = ' hidden';
    }

    $output .= '<ul class="page-provincial statistic'. $class .'">';
    foreach ($terms as $term) {
      $output .= '<li>';
      if ($fullpath) {
        if ($filename = db_result(db_query("SELECT path FROM {term_image} WHERE tid = %d", $term->tid))) {
          $output .= theme('image', $fullpath .'/'. $filename);
        }
      }
      $output .= '<h3>'. $term->name .'</h3>';
      $output .= $term->description;
      // Get code.
      $code = db_result(db_query("SELECT field_code FROM {term_fields_term} WHERE tid = %d", $term->tid));
      // Generate output.
      $output .= l(t('View all'), implode('/', $paths) .'/'. $code, array('attributes' => array('class' => 'viewall', 'id' => $code)));
      if (user_access('administer taxonomy')) {
        $output .= l(t('[edit]'), 'admin/content/taxonomy/edit/term/'. $term->tid, array('query' => drupal_get_destination()));
      }
    }
    $output .= '</ul>';
  }
  
  if ($breadcrumb) {
    drupal_set_breadcrumb($breadcrumb);
  }
  
  return $output;
}

/**
 * 
 */
function misstat_provincial($province = NULL, $spi1 = NULL, $spi2 = NULL, $spi3 = NULL, $is_printing = FALSE) {

  // Javascript and CSS
  drupal_add_js(drupal_get_path('module', 'misstat'). '/js/misstat.js');

  // Initial breadcrumb.
  $breadcrumb = array(
    l('หน้าแรก', '<front>'),
    l('สถิติและข้อมูล', 'statistic'),
    l('ระดับจังหวัด', 'statistic/provincial'),
    l($province->name, 'statistic/provincial/'. $province->area_id),
    l($spi1->name, 'statistic/provincial/'. $province->area_id .'/'. $spi1->code),
    l($spi2->name, 'statistic/provincial/'. $province->area_id .'/'. $spi1->code .'/'. $spi2->code),
  );
  drupal_set_breadcrumb($breadcrumb);
  
  // Province term id.
  $ptid = $province->tid;
  
  // Region term id.
  $rid = ($province->area_id == 4) ? 1300 : misstat_get_region($province->area_id);
  $rtid = db_result(db_query("SELECT tid FROM {term_fields_term} WHERE field_area_id = '%s'", $rid));
  $region = taxonomy_get_term($rtid);
  
  // Base code.
  $base_code = substr($spi3->code, 2);
  
  // Prepare vars.
  $label = array();
  $years = array();
  $error = array();
  
  // Check start is less than end.
  if ($_GET['start'] && $_GET['end']) {
    $start = (int) $_GET['start'];
    $end = (int) $_GET['end'];

    if ($start > $end) {
      $error[] = t('Start year must greater than end year');
    }
  }

  // Province data.
  $pnid = db_result(db_query("SELECT n.nid FROM {node} n, {term_node} tn WHERE tn.nid = n.nid AND tn.tid = %d AND title = '%s'", $ptid, 'P-'. $base_code));
  $pnode = node_load($pnid);
  $pvalues = $pnode->field_year_value;
  foreach ($pvalues as $_data) {
    $years[] = $_data['key'];
  }
  
  rsort($pvalues);
  if (isset($start) && isset($end)) {
    foreach ($pvalues as $i => $row) {
      if ($row['key'] == $start) $kstart = $i;
      if ($row['key'] == $end) $kend = $i;
    }
    if (isset($kstart) && isset($kend)) $pvalues = array_slice($pvalues, $kend, ($kstart - $kend) + 1);
  }
  else {
    $pvalues = array_slice($pvalues, 0, 5);
  }
  
  foreach ($pvalues as $_data) {
    $value = $_data['value'] + 0;
    $data[$_data['key']]['provincial'] = $value;
    $label[] = $_data['key'];
  }
  
  // Region data.
  $rnid = db_result(db_query("SELECT n.nid FROM {node} n, {term_node} tn WHERE tn.nid = n.nid AND tn.tid = %d AND title = '%s'", $rtid, 'R-'. $base_code));
  $rnode = node_load($rnid);
  foreach ($rnode->field_year_value as $_data) {
    if ($data[$_data['key']]) {
      $value = $_data['value'] + 0;
      $data[$_data['key']]['regional'] = $value;
    }
  }
  // National data.
  $nnid = db_result(db_query("SELECT nid FROM {node} WHERE title = '%s'", 'N-'. $base_code));
  $nnode = node_load($nnid);
  foreach ($nnode->field_year_value as $_data) {
    if ($data[$_data['key']]) {
      $value = $_data['value'] + 0;
      $data[$_data['key']]['national'] = $value;
    }
  }

  // Re-order data.
  if ($label[0] > $label[1]) {
    $data = array_reverse($data, TRUE);
    $label = array_reverse($label);
  }

  // Make table headers.
  $headers = array(
    t('Year'),
    'จังหวัด'.$province->name,
    $region->name,
    'ประเทศ',
  );
  
  // If Bangkok (area_id=4).
  if ($province->area_id == 4) {
    // Replace header.
    $headers[1] = $province->name;
    // Remove region.
    //unset($headers[2]);
  }

  if (count($error) == 0) {
    // Make table rows and graph data.
    $is_float = FALSE;
    $rows = $pdata = $rdata = $ndata = array();
    foreach ($data as $year => $row) {
      $row['national'] += 0;
      $row['regional'] += 0;
      $row['provincial'] += 0;

      // Check is float.
      if (is_float($row['national']) || is_float($row['regional']) || is_float($row['provincial'])) $is_float = TRUE;

      // Table rows.
      $new = array(
        $year,
        $row['provincial'],
        $row['regional'],
        $row['national']
      );
      
      // Remove region data.
      /*if ($province->area_id == 4) {
        unset($new[2]);
      }*/

      // Graph data.
      $pdata[] = array($year, $row['provincial']);
      $rdata[] = array($year, $row['regional']);
      $ndata[] = array($year, $row['national']);
      
      // Add new row.
      $rows[] = $new;
    }

    // Make sure if some value is float, the rest of values must be the same.
    if ($is_float) {
      foreach ($rows as $i => $row) {
        foreach ($row as $j => $value) {
          if ($j == 0) continue;
          $tmp = explode('.', $value);
          //==$rows[$i][$j] = ($value == 0 || $value == 'N/A') ? t('N/A') : number_format($value, 3);//strlen($tmp[1]));
          $rows[$i][$j] = ($value == 0 || $value == 'N/A') ? t('N/A') : thairoads_num_format($value);
        }
      }
    }
    else {
      foreach ($rows as $i => $row) {
        foreach ($row as $j => $value) {
          if ($j == 0) continue;
          $rows[$i][$j] = ($value == 'N/A') ? t('N/A') : number_format($value);
        }
      }
    }

    // Make table.
    $table = theme('table', $headers, $rows);
    
    // Graph.
    if (module_exists('flot') && $ndata && $rdata && $pdata) {
      flot_add_js();
      $options = array(
        'xaxis' => array(
          'tickDecimals' => 0,
          'tickSize' => 1,
        ),
        'yaxis' => array(
          'min' => 0
        ),
        'grid' => array(
          'hoverable' => TRUE,
          'labelMargin' => 20,
        ),
        'legend' => array(
          'show' => TRUE,
          'position' => 'se',
          'margin' => 10
        ),
      );
      $data = array(
        array(
          'label' => $headers[1],
          'data' => $pdata,
          'color' => variable_get('misstat_graph_provincial_color', 'red'),
          'lines' => array('show' => TRUE),
          'points' => array('show' => TRUE),
        ),
        array(
          'label' => $headers[2],
          'data' => $rdata,
          'color' => variable_get('misstat_graph_regional_color', 'red'),
          'lines' => array('show' => TRUE),
          'points' => array('show' => TRUE),
        ),
        array(
          'label' => 'ทั่วประเทศ',
          'data' => $ndata,
          'color' => variable_get('misstat_graph_national_color', 'red'),
          'lines' => array('show' => TRUE),
          'points' => array('show' => TRUE),
        ),
      );
      $settings['misstat']['flotEnabled'] = TRUE;
      $settings['misstat']['flot'] = array(
        'options' => $options,
        'data' => $data,
        'element' => array(
          'id' => 'chart',
          'style' => array(
            'width' => variable_get('misstat_graph_width', 500),
            'height' => variable_get('misstat_graph_height', 300),
            'background-color' => variable_get('misstat_graph_bg_color', 'none'),
          ),
        ),
      );
    }
    else {
      $settings['misstat']['flotEnabled'] = FALSE;
    }
  }

  // Generate output.
  if (!$is_printing) {
    // Get provinces.
    $tid = 48;
    $provinces = taxonomy_get_children($tid);

    // Make province options for select element.
    $options = array(0 => t('- select one -'));
    foreach ($provinces as $p) {
      if ($area_id = db_result(db_query("SELECT field_area_id FROM {term_fields_term} WHERE tid = %d", $p->tid))) {
        $options[$area_id] = $p->name;
      }
    }

    // Make select element.
    $elements['province'] = array(
      '#type' => 'select',
      '#title' => t('Please select another province'),
      '#options' => $options,
      //'#value' => $province->area_id,
      '#parents' => array(),
    );

    $output .= '<div class="province-switch">';
    // Use form for W3C standard.
    $output .= '<form method="post">';
    $output .= drupal_render($elements);
    $output .= '</form>';
    $output .= '</div>';
  }

  $current_path = drupal_get_path_alias($_GET['q']);
  $split_path = explode('/', $current_path);
  $prefix_path = implode('/', array_slice($split_path, 0, 2)) .'/';
  $suffix_path = '/'. implode('/', array_slice($split_path, 3));
  $settings['misstat']['path'] = array(
    'prefix' => $prefix_path,
    'suffix' => $suffix_path,
  );

  $output .= '<div class="text-box">';
  if ($is_printing) {
    if (mb_strrpos($spi1->name, 'จังหวัด') !== FALSE) {
      $title = $spi1->name.$province->name;
    }
    else {
      $title = $spi1->name .'จังหวัด'. $province->name;
    }
    $output .= '<h2>'. $title .'</h2>';
  }
  // Topic.
  $output .= '<div class="head-stat"><h4>'. $spi3->name .'</h4></div>';
  // Description.
  $output .= '<p>'. $spi3->description .'</p>';
  // Source.
  $source = db_result(db_query("SELECT field_source FROM {term_fields_term} WHERE tid = %d", $spi3->tid));
  $output .= '<h5>'. t('Source') .'</h5><p>'. $source .'</p>';

  if (!($start && $end)) {
    $start = reset($label);
    $end = end($label);
  }

  // Year select.
  $output .= '<h5>'. t('Period year') .'</h5>';
  $output .= '<div class="period-year" style="padding: 0 0 1.5em">';
  if (!$is_printing) {
    foreach ($years as $year) {
    
      if ($year == $start) {
        $start_selected = ' selected="selected"';
      }
      else {
        $start_selected = '';
      }
      if ($year == $end) {
        $end_selected = ' selected="selected"';
      }
      else {
        $end_selected = '';
      }
      $start_options .= '<option'. $start_selected .'>'. $year .'</option>';
      $end_options .= '<option'. $end_selected .'>'. $year .'</option>';
    }
    $output .= '<form method="get">';
    $output .= '<select id="range-start" name="start">'. $start_options .'</select>';
    $output .= '<span> '. t('to') .' </span>';
    $output .= '<select id="range-end" name="end">'. $end_options .'</select>';
    $output .= '<input type="submit" id="range-submit" value="'. t('Go') .'" />';
    if ($_GET['start'] && $_GET['end']) {
      $output .= l('แสดงข้อมูล 5 ปี ล่าสุด', $_GET['q']);
    }
    $output .= '</form>';
  }
  else {
    $output .= t('!start to !end', array('!start' => $start, '!end' => $end));
  }
  $output .= '</div>';
  // Graph.
  // Excludes.
  // TODO
  // Add field to exclude or settings.
  if (!in_array($spi3->code, array('P-SPI-A1-01', 'P-SPI-A1-02', 'P-SPI-A1-04', 'P-SPI-A1-05', 'P-SPI-A1-06'))) {
    if ($settings['misstat']['flotEnabled']) {
      $output .= '<div id="graph">';
      $output .= '<h5>'. t('!name year !start to !end', array('!name' => $spi3->name, '!start' => $start, '!end' => $end)) .'</h5>';
      if ($unit = db_result(db_query("SELECT unit FROM {term_fields_term} WHERE tid = %d", $spi3->tid))) {
      //if ($unit = 'รายต่อแสนประชากร') {
        $output .= '<div class="unit-description">หน่วย: '. $unit .'</div>';
      }
      $output .= '<div id="chart" style="width: '. variable_get('misstat_graph_width', '500px') .'; height: '. variable_get('misstat_graph_height', '300px') .';"></div>';
      $output .= '</div>';
    }
  }
  $output .= '<p>';
  // Table.
  $output .= $table;
  $output .= '</p>';
  $output .= '</div>';

  if (!empty($error)) {
    foreach ($error as $err) {
      drupal_set_message($err, 'error');
    }
  }

  $settings['misstat']['alert_message'] = t('Start year must greater than end year');
  drupal_add_js(drupal_get_path('module', 'misstat') .'/js/range.js');
  drupal_add_js(drupal_get_path('module', 'misstat') .'/js/buildchart.js');
  drupal_add_js($settings, 'setting');

  return $output;
}
