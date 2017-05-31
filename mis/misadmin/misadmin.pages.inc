<?php

/**
 * SPI Overview.
 */
function misadmin_spi_overview() {
  $content = array(
    array(
      'title' => 'ข้อมูล SPI ระดับประเทศ',
      'href' => 'admin/spi/national',
      'description' => 'ข้อมูลหมวดหมู่ของ SPI ระดับประเทศ',
      'localized_options' => array(),
    ),
    array(
      'title' => 'ข้อมูล SPI ระดับจังหวัด',
      'href' => 'admin/spi/provincial',
      'description' => 'ข้อมูลหมวดหมู่ของ SPI ระดับจังหวัด',
      'localized_options' => array(),
    ),
  );
  return theme('node_add_list', $content);
}

/**
 * SPI List.
 */
function misadmin_spi_list($tid) {
  drupal_add_js(drupal_get_path('module', 'misadmin') .'/buildtree.js');

  $tree = taxonomy_get_tree(1, $tid);
  $rows = array();

  foreach ($tree as $term) {
    $indentation = '<div class="indentation"></div>';
    $code = db_result(db_query("SELECT field_code FROM {term_fields_term} WHERE tid = %d", $term->tid));

    switch (substr($code, 0, 1)) {
      case 'N': $type = 'national'; break;
      case 'R': $type = 'regional'; break;
      case 'P': $type = 'provincial'; break;
    }
    
    // Edit title.
    $edit_title = $term->name .' ('. $code .')';
    // Edit link.
    $edit_link = 'admin/content/taxonomy/edit/term/'. $term->tid;
    // Edit options.
    $edit_options = array('query' => 'destination='. $_GET['q']);
    // Title.
    $title = l($edit_title, $edit_link, $edit_options);
    // Edit.
    $edit = l('แก้ไขรายละเอียด', $edit_link, $edit_options);
    // Statistic.
    $list = $term->depth == 2 ? l('แก้ไขข้อมูลสถิติ', 'admin/stat/'. $type, array('query' => 'code='. $code)) : '';

    $rows[] = array(
      str_repeat($indentation, $term->depth) . $title,
      $edit,
      $list,
    );
  }
  
  $header = array(t('Name'), array('data' => t('Operation'), 'colspan' => count($rows[0]) - 1));
  
  return theme('table', $header, $rows, array('id' => 'spi-list'));
}

/*
 * SPI National list.
 */
function misadmin_spi_national() {
  $tid = 16;
  return misadmin_spi_list($tid);
}

/**
 * SPI Regional list.
 */
function misadmin_spi_regional() {
  $tid = 17;
  return misadmin_spi_list($tid);
}

/**
 * SPI Provincial list.
 */
function misadmin_spi_provincial() {
  $tid = 18;
  return misadmin_spi_list($tid);
}

/**
 * Statistic SPI list.
 */
function misadmin_stat_overview() {
  $content = array(
    array(
      'title' => 'ข้อมูลสถิติระดับประเทศ',
      'href' => 'admin/stat/national',
      'description' => 'แก้ไขตรวจสอบข้อมูลสถิติระดับประเทศ',
      'localized_options' => array(),
    ),
    array(
      'title' => 'ข้อมูลสถิติระดับภูมิภาค',
      'href' => 'admin/stat/regional',
      'description' => 'แก้ไขตรวจสอบข้อมูลสถิติระดับภูมิภาค',
      'localized_options' => array(),
    ),
    array(
      'title' => 'ข้อมูลสถิติระดับจังหวัด',
      'href' => 'admin/stat/provincial',
      'description' => 'แก้ไขตรวจสอบข้อมูลสถิติระดับจังหวัด',
      'localized_options' => array(),
    ),
  );
  return theme('node_add_list', $content);
}

/**
 * Statistic National list.
 */
function misadmin_stat_national() {
  // Search.
  $output = _misadmin_stat_search_form('national', 16);
  // Result.
  if (isset($_GET['code'])) $output .= _misadmin_stat_result('national', $_GET['code']);
  return $output;
}

/**
 * Statistic Provincial list.
 */
function misadmin_stat_regional() {
  // Search.
  $output = _misadmin_stat_search_form('regional', 17);
  // Result.
  if (isset($_GET['code']) && isset($_GET['rid'])) $output .= _misadmin_stat_result('regional', $_GET['code'], $_GET['rid']);
  return $output;
}

/**
 * Statistic Provincial list.
 */
function misadmin_stat_provincial() {
  // Search.
  $output = _misadmin_stat_search_form('provincial', 18);
  // Result.
  if (isset($_GET['code']) && isset($_GET['pid'])) $output .= _misadmin_stat_result('provincial', $_GET['code'], $_GET['pid']);
  return $output;
}

/**
 * Statistic search form.
 */
function _misadmin_stat_search_form($type, $tid) {
  $output = array();
  $output[] = '<form method="get" id="'. $type .'-search-form">';
  // Code.
  $output[] = '<div class="form-item">';
  $output[] = '<label for="edit-code">ค้นหาตาม SPI</label>';
  if ($prefix) {
    $output[] = '<span class="prefix">'. $prefix .'</span>';
  }
  $terms = taxonomy_get_tree(1, $tid);
  if ($terms) {
    $output[] = '<select name="code">';
    $indentation = '&nbsp;&nbsp;';
    $current_depth = 0;
    foreach ($terms as $term) {
      // Get code.
      $code = db_result(db_query("SELECT field_code FROM {term_fields_term} WHERE tid = %d", $term->tid));
      // Label.
      $label = str_repeat($indentation, $term->depth * 2) . $term->name .' ('. $code .')';
      // Item groups.
      if ($term->depth < $current_depth) {
        $output[] = str_repeat("</optgroup>\n", $current_depth - $term->depth);
      }
      // Items.
      if ($term->depth == 2) {
        $selected = ($_GET['code'] == $code) ? ' selected="selected"' : '';
        $output[] = '<option value="'. $code .'"'.$selected.'>'. $label .'</option>';
      }
      else {
        $output[] = '<optgroup label="'. $label .'">';
      }
      $current_depth = $term->depth;
    }
    $output[] = '</select>';
  }
  $output[] = '</div>';
  if ($type == 'provincial') {
    $output[] = '<div class="form-item">';
    $output[] = '<label for="edit-pid">เลือกจังหวัด</label>';
    $output[] = '<select name="pid" id="edit-pid">';
    $output[] = '<option value="0">ทั้งหมด</option>';
    $terms = taxonomy_get_tree(2, 48);
    foreach ($terms as $term) {
      $selected = ($_GET['pid'] == $term->tid) ? ' selected="selected"' : '';
      $output[] = '<option value="'. $term->tid .'"'. $selected .'>'. $term->name .'</option>';
    }
    $output[] = '</select>';
    $output[] = '</div>';
  }
  elseif ($type == 'regional') {
    $output[] = '<div class="form-item">';
    $output[] = '<label for="edit-rid">เลือกภาค</label>';
    $output[] = '<select name="rid" id="edit-rid">';
    $output[] = '<option value="0">ทั้งหมด</option>';
    $terms = taxonomy_get_tree(2, 42);
    foreach ($terms as $term) {
      $selected = ($_GET['rid'] == $term->tid) ? ' selected="selected"' : '';
      $output[] = '<option value="'. $term->tid .'"'. $selected .'>'. $term->name .'</option>';
    }
    $output[] = '</select>';
    $output[] = '</div>';
  }
  $output[] = '<input type="submit" value="ค้นหา" />';
  $output[] = '</form>';
  return implode("\n", $output);
}

/**
 * Statistic search result.
 */
function _misadmin_stat_result($type, $code, $pid = 0) {
  $func = '_misadmin_stat_'. $type .'_result';
  if ($type == 'national') {
    return $func($code);
  }
  else {
    return $func($code, $pid);
  }
}

/**
 * Statistic national search result.
 */
function _misadmin_stat_national_result($code) {
  
  // Table header.
  $header = array(t('SPI') ,t('Name'), t('Year'), t('Operation'));
  // Prepare rows.
  $rows = array();
  // SQL statement.
  $sql = "SELECT n.nid FROM {node} n WHERE n.type = 'statistic' AND n.title = '%s'";

  // Query.
  $result = db_query($sql, $code);
  // Fetch data.
  while ($row = db_fetch_object($result)) {
    // Load SPI node.
    $node = node_load($row->nid);
    // SPI name.
    $name = db_result(db_query("SELECT t.name FROM {term_fields_term} tf, {term_data} t WHERE t.tid = tf.tid AND field_code = '%s'", $code));
    // Year.
    $year = $node->field_year[0]['value'];
    // Edit link.
    $edit = l(t('edit'), 'node/'. $node->nid .'/edit', array('query' => drupal_get_destination()));
    // Add row.
    $rows[] = array($code, $name, $year, $edit);
  }
  
  // Empty row.
  if (count($rows) == 0) {
    $rows[] = array(
      array(
        'data' => 'ไม่พบข้อมูลจากการค้นหา "'. $code .'"',
        'colspan' => count($header),
        'align' => 'center',
      ),
    );
  }
  
  return theme('table', $header, $rows);
}

/**
 * Statistic provincial search result.
 */
function _misadmin_stat_provincial_result($code, $pid) {
  
  // Table header.
  $header = array(t('SPI') ,t('Name'), t('Year'), t('Operation'));
  // Prepare rows.
  $rows = array();

  // Get term id.
  if ($pid == 0) {
    $tid = 48;
    // SQL statement.
    $sql = "SELECT n.nid FROM {node} n WHERE n.type = 'statistic' AND n.title = '%s'";
    // Query.
    $result = db_query($sql, $code);
  }
  else {
    //$tid = db_result(db_query("SELECT tid FROM {term_fields_term} WHERE field_area_id = '%s'", $pid));
    $tid = $pid;
    $province = taxonomy_get_term($pid);
    array_splice($header, 2, 0, t('Province'));
    // SQL statement.
    $sql = "SELECT n.nid FROM {node} n, {term_node} tn WHERE n.nid = tn.nid AND n.title = '%s' AND tn.tid = %d";
    // Query.
    $result = db_query($sql, $code, $pid);
  }
  // Get term data.
  
  // Fetch data.
  while ($row = db_fetch_object($result)) {
    // Load SPI node.
    $node = node_load($row->nid);
    $terms = taxonomy_node_get_terms_by_vocabulary($node, 2);
    unset($terms[48]);
    $province = end($terms);
    // SPI name.
    $name = db_result(db_query("SELECT t.name FROM {term_fields_term} tf, {term_data} t WHERE t.tid = tf.tid AND field_code = '%s'", $code));
    // Year.
    $year = $node->field_year[0]['value'];
    // Edit link.
    $edit = l(t('edit'), 'node/'. $node->nid .'/edit', array('query' => drupal_get_destination()));
    // Add row.
    if ($province) {
      $rows[] = array($code, $name, $province->name, $year, $edit);
    }
    else {
      $rows[] = array($code, $name, $year, $edit);
    }
  }
  
  // Empty row.
  if (count($rows) == 0) {
    $rows[] = array(
      array(
        'data' => 'ไม่พบข้อมูลจากการค้นหา "'. $code .'"',
        'colspan' => count($header),
        'align' => 'center',
      ),
    );
  }
  
  return theme('table', $header, $rows);
}

/**
 * Statistic reginal search result.
 */
function _misadmin_stat_regional_result($code, $rid) {
  
  // Table header.
  $header = array(t('SPI') ,t('Name'), t('Year'), t('Operation'));
  // Prepare rows.
  $rows = array();

  // Get term id.
  if ($rid == 0) {
    $tid = 42;
    // SQL statement.
    $sql = "SELECT n.nid FROM {node} n WHERE n.type = 'statistic' AND n.title = '%s'";
    // Query.
    $result = db_query($sql, $code);
  }
  else {
    //$tid = db_result(db_query("SELECT tid FROM {term_fields_term} WHERE field_area_id = '%s'", $pid));
    $tid = $rid;
    $region = taxonomy_get_term($rid);
    array_splice($header, 2, 0, t('Region'));
    // SQL statement.
    $sql = "SELECT n.nid FROM {node} n, {term_node} tn WHERE n.nid = tn.nid AND n.title = '%s' AND tn.tid = %d";
    // Query.
    $result = db_query($sql, $code, $rid);
  }
  // Get term data.
  
  // Fetch data.
  while ($row = db_fetch_object($result)) {
    // Load SPI node.
    $node = node_load($row->nid);
    $terms = taxonomy_node_get_terms_by_vocabulary($node, 2);
    unset($terms[42]);
    $region = end($terms);
    // SPI name.
    $name = db_result(db_query("SELECT t.name FROM {term_fields_term} tf, {term_data} t WHERE t.tid = tf.tid AND field_code = '%s'", $code));
    // Year.
    $year = $node->field_year[0]['value'];
    // Edit link.
    $edit = l(t('edit'), 'node/'. $node->nid .'/edit', array('query' => drupal_get_destination()));
    // Add row.
    if ($region) {
      $rows[] = array($code, $name, $region->name, $year, $edit);
    }
    else {
      $rows[] = array($code, $name, $year, $edit);
    }
  }
  
  // Empty row.
  if (count($rows) == 0) {
    $rows[] = array(
      array(
        'data' => 'ไม่พบข้อมูลจากการค้นหา "'. $code .'"',
        'colspan' => count($header),
        'align' => 'center',
      ),
    );
  }
  
  return theme('table', $header, $rows);
}
