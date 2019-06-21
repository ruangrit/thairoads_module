<?php

/**
 * 
 */
function misstat_overview() {
  drupal_goto('statistic/national');
  return '';
}

/**
 * National overview.
 */
function misstat_national_overview($spi1 = NULL, $spi2 = NULL) {
  // Add Javascript and CSS.
  drupal_add_js(drupal_get_path('module', 'misstat') .'/js/misstat_collapse.js');
  drupal_add_css(drupal_get_path('module', 'misstat') .'/css/misstat.css');
  
  // Initial breadcrumb.
  $breadcrumb = array(
    l('หน้าแรก', '<front>'),
    l('สถิติและข้อมูล', 'statistic'),
  );
  
  // Display default task.
  if ($spi1) {
    // Add breadcrumb.
    $breadcrumb[] = l('ระดับประเทศ', 'statistic/national');
    $items = array();
    foreach ($spi1->children as $parent) {
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
        
        $disabled = variable_get('misstat_disable_tids', '');
        $disabled = explode(',', $disabled);

        $paths = explode('/', $_GET['q']);
        $paths = array_slice($paths, 0, 3);

        foreach ($spis as $spi) {
          if (in_array($spi->tid, $disabled)) continue;
          $code = db_result(db_query("SELECT field_code FROM {term_fields_term} WHERE tid = %d", $spi->tid));
          if (user_access('administer taxonomy')) {
            $link = l(t('[edit]'), 'admin/content/taxonomy/edit/term/'. $spi->tid, array('query' => drupal_get_destination()));
          }
          $items[$parent->tid]['items'][] = array(
            'class' => 'national-'.$spi->tid,
            'title' => l($spi->name, implode('/', $paths) .'/'. $parent_code .'/'. $code),
            'description' => $spi->description,
            'link' => $link,
          );
        }
      }
    }
    
    $output = theme('misstat_statistic_list', $items);
  }
  else {
    $output = variable_get('misstat_national_description', '');
    if (user_access('administer misstat')) {
      $output .= '<br />'. l(t('Edit content'), 'admin/settings/misstat', array('query' => drupal_get_destination()));
    }
    // National tid.
    $tid = 16;
    $terms = taxonomy_get_children($tid);

    // Get fullpath of taxonomy image.
    if (module_exists('taxonomy_image')) {
      $mypath = variable_get('taxonomy_image_path', 'category_pictures') .'/';
      $fullpath = file_directory_path() .'/'. $mypath;
    }

    $output .= '<ul class="page-national statistic">';
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
      $output .= l(t('View all'), $_GET['q'].'/'. $code, array('attributes' => array('class' => 'viewall')));
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
function misstat_national($spi1, $spi2, $spi3, $is_printing = FALSE) {
  // Initial breadcrumb.
  $breadcrumb = array(
    l('หน้าแรก', '<front>'),
    l('สถิติและข้อมูล', 'statistic'),
    l('ระดับประเทศ', 'statistic/national'),
    l($spi1->name, 'statistic/national/'. $spi1->code),
    l($spi2->name, 'statistic/national/'. $spi1->code .'/'. $spi2->code),
  );
  drupal_set_breadcrumb($breadcrumb);
  
  // National term id.
  $tid = 41;

  $output = '<div class="text-box">';
  // Topic.
  $output .= '<div class="head-stat"><h4>'. $spi3->name .'</h4></div>';
  // Description.
  $output .= '<p>'. $spi3->description .'</p>';
  // Source.
  $source = db_result(db_query("SELECT field_source FROM {term_fields_term} WHERE tid = %d", $spi3->tid));
  $output .= '<h5>'. t('Source') .'</h5><p>'. $source .'</p>';

  $error = array();
  // Check start is less than end.
  if ($_GET['start'] && $_GET['end']) {
    $start = (int) $_GET['start'];
    $end = (int) $_GET['end'];

    if ($start > $end) {
      $error[] = t('Start year must greater than end year');
    }
  }

  // Query data.
  $nid = db_result(db_query("SELECT nid FROM {node} WHERE title = '%s'", $spi3->code));
  $node = node_load($nid);
  $years = array();
  //$values = array();
  $all_rows = array();
  $is_float = FALSE;
  foreach ($node->field_year_value as $data) {
    $years[] = $data['key'];
    //$values[$data['key']] = $data['value'];
    $value = $data['value'] + 0;
    if (is_float($value)) $is_float = TRUE;
    $all_rows[] = array(
      $data['key'],
      $value,
    );
  }

  if ($years[0] < $years[1]) {
    rsort($all_rows);
    rsort($years);
  }

  if (count($error) == 0) {
    // Slice needed range.
    $rows = array();
    if ($start && $end) {
      foreach ($all_rows as $i => $row) {
        if ($row[0] == $start) $kstart = $i;
        if ($row[0] == $end) $kend = $i;
      }
      if (isset($kstart) && isset($kend)) $rows = array_slice($all_rows, $kend, ($kstart - $kend) + 1);
    }
    else {
      $rows = array_slice($all_rows, 0, 5);
    }

    $label = array();
    $data = array();
    if (count($rows) > 0) {
      foreach ($rows as $i => $row) {
        // Label.
        $label[] = $row[0];
        // Graph data.
        $data[] = array((int)$row[0], $row[1]);
      }
    }

    if ($rows) {
      // Make sure if some value is float, the rest of values must be the same.
      if ($is_float) {
        foreach ($rows as $i => $row) {
          $rows[$i][1] = number_format($row[1], 2);
        }
      }
      else {
        foreach ($rows as $i => $row) {
          $rows[$i][1] = number_format($row[1]);
        }
      }

      $rows = array_reverse($rows);
      $data = array_reverse($data);
      $label = array_reverse($label);

      $headers = array(
        t('Year'),
        'ทั่วประเทศ', //'จำนวน'. $spi3->name,
      );

      $table = theme('table', $headers, $rows);

      // Graph.
      if (module_exists('flot') && $data) {
        flot_add_js();
        $options = array(
          'xaxis' => array(
            'tickDecimals' => 0,
            'tickSize' => 1,
          ),
          'yaxis' => array('min' => 0),
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
            'label' => 'ทั่วประเทศ',
            'data' => $data,
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
  }

  if (!($start && $end)) {
    $arr_start = reset($rows);
    $arr_end = end($rows);
    $start = $arr_start[0];
    $end = $arr_end[0];
  }

  // Year select.
  $output .= '<h5>'. t('Period year') .'</h5>';
  $output .= '<div class="period-year" style="padding: 0 0 1.5em">';
  if (!$is_printing) {
    sort($years);
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
      $output .= l('แสดงข้อมูล 5 ปีล่าสุด', $_GET['q']);
    }
    $output .= '</form>';
  }
  else {
    $output .= t('!start to !end', array('!start' => $start, '!end' => $end));
  }
  $output .= '</div>';
  if (empty($error) && $rows) {
    // Statistic.
    $output .= '<p>';
    // Graph.
    if ($settings['misstat']['flotEnabled']) {
      $output .= '<div id="graph">';
      $output .= '<h5>'. t('!name year !start to !end', array('!name' => $spi3->name, '!start' => min($label), '!end' => max($label))) .'</h5>';
      if ($unit = db_result(db_query("SELECT unit FROM {term_fields_term} WHERE tid = %d", $spi3->tid))) {
      //if ($unit = 'รายต่อแสนประชากร') {
        $output .= '<div class="unit-description">หน่วย: '. $unit .'</div>';
      }
      $output .= '<div id="chart" style="width: '. variable_get('misstat_graph_width', '500px') .'; height: '. variable_get('misstat_graph_height', '300px') .';"></div>';
      $output .= '</div>';
    }
    $output .= '</p><p>';
    // Table.
    $output .= $table;
    $output .= '</p>';
  }
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
