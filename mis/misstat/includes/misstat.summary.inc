<?php

/**
 * Summary page.
 */
function misstat_summary() {
  // Load JavaScript.
  drupal_add_js(drupal_get_path('module', 'misstat') .'/js/summary.js');
  drupal_add_css(drupal_get_path('module', 'misstat') .'/css/misstat.css');
  // Select summary contents.
  $year_result = db_query("SELECT DISTINCT field_year_value FROM `content_type_summary` LEFT JOIN node ON content_type_summary.nid = node.nid WHERE node.status = 1 ORDER BY content_type_summary.field_year_value DESC");
  $year_options = array();
  $index = 0;
  while($year = db_fetch_object($year_result)) {
    $year_value = $year->field_year_value;
    if ($index === 0) {
      $last_year = url($_GET['q'], array('query' => 'year='.$year_value, 'absolute' => 'TRUE'));
      $sql_year = $year_value;
    }
    $year_url = url($_GET['q'], array('query' => 'year='.$year_value, 'absolute' => 'TRUE'));
    $year_options[$year_url] = $year_value;
    $index = $index + 1;
  }

  if ($_GET['year']) {
    $last_year = url($_GET['q'], array('query' => 'year='.$_GET['year'], 'absolute' => 'TRUE'));
    $sql_year = $_GET['year'];
  }
  $year_form['summary-year'] = array(
    '#type' => 'select',
    '#title' => t('Choose year'),
    '#options' => $year_options,
    '#weight' => '0',
    '#attributes' => array(
      'id' => 'summary-year-opt',
      'class' => 'summary-year',
    ),
  );

  $result = db_query("SELECT * FROM node LEFT JOIN content_type_summary ON node.nid = content_type_summary.nid WHERE type = 'summary' AND status = 1 AND content_type_summary.field_year_value = %d ORDER BY title ASC", $sql_year);

  $form = array();
  while ($row = db_fetch_object($result)) {
    $node = node_load($row->nid);
    // Generate options.
    $options = array(0 => t('- Please select -'));
    foreach ($node->files as $file) {
      if ($file->list == 1) $options[file_create_url($file->filepath)] = $file->description;
    }
    // Edit permission.
    if (user_access('administer misstat') && user_access('edit any summary content')) {
      $op['query'] = array(
        'destination' => $_GET['q'],
        'title' => t('Click to edit'),
      );
      $title = l(t('Edit >'), 'node/'. $node->nid .'/edit', $op) .' '. $node->title;
    }
    else {
      $title = $node->title;
    }
    // Generate form.
    $form['summary-'. $node->nid] = array(
      '#type' => 'select',
      '#title' => $title,
      '#options' => $options,
      '#weight' => $node->nid,
      '#name' => 'summary-'. $node->nid,
      '#attributes' => array(
        'id' => 'summary-'. $node->nid,
        'class' => 'summary-select',
      ),
    );
  }
  $output .= '<p>'. variable_get('misstat_summary_description', '') .'</p>';
  $output .= '<div id="statistic-summary">';
  $output .= drupal_render($year_form);
  $output .= drupal_render($form);
  $output .= '</div>';
  //$output .= '<img src="sites/default/files/map.jpg" id="statistic-map" />';
  return $output;
}
