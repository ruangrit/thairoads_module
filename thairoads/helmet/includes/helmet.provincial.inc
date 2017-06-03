<?php

function helmet_provincial_import_form() {

  $result = db_query("SELECT m.*, f.* FROM {helmet} m, {files} f WHERE f.fid = m.fid AND m.type = 'provincial' ORDER BY mid DESC");
  $rows = array();
  $ctime = 0;
  while ($row = db_fetch_object($result)) {
    $rows[] = array(
      date('d/m/Y - H:i:s', $row->timestamp), //($ctime != $row->timestamp) ? date('d/m/Y - H:i:s', $row->timestamp) : '',
      l($row->delta, $row->filepath),
      format_size($row->filesize),
    );
    $ctime = $row->timestamp;
  }
  $header = array(
    t('Date'),
    t('File name'),
    t('File size'),
  );
  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('No update found.'),
        'colspan' => count($header),
      ),
    );
  }
  $table = theme('table', $header, $rows);

  $form['table'] = array(
    '#type' => 'markup',
    '#value' => $table,
  );

  $code = _helmet_get_provincial_code();
  foreach ($code as $_code) {
    $name = 'file_'. strtolower(str_replace('-', '_', $_code));
    $form[$name] = array(
      '#type' => 'file',
      '#title' => $_code,
      '#description' => t('File name must be <strong><em>!name.csv</em></strong> only.', array('!name' => $_code)),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  $form['#submit'] = array('helmet_provincial_import_form_submit');
  $form['#theme'] = 'helmet_provincial_import_form';
  $form['#attributes'] = array('enctype' => 'multipart/form-data');
  return $form;
}

/**
 * Process import.
 */
function helmet_provincial_import_process($name, $code, $file, $time, $max, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = $max;
  }

  $context['message'] = t('Now processing %code', array('%code' => $code));

  $data = array();
  if (($handle = fopen($file->filepath, 'r')) !== FALSE) {
    $new = 0;
    $update = 0;
    $nodes = array();
    while (($data = fgetcsv($handle, 1000, ',')) !== FALSE) {
      if (empty($data[0])) continue;

      if (!$is_read_head) {
        $heads = $data;
        $count++;
        $is_read_head = TRUE;
        continue;
      }

      // Prepare variables.
      $field_name = 'P-'. $data[0];
      $area = $data[1];

      if ($nid = misimport_get_node_by_code_area($field_name, $area)) {
        $node = node_load($nid);
        $node->field_year_value = array();
        unset($node->path);
        $update++;
      }
      else {
        $node = new stdClass();
        $node->type = 'statistic';
        $node->comment = COMMENT_NODE_DISABLED;
        $node->format = 1;
        $node->uid = 1;
        $node->status = 1;
        $node->title = $field_name;
        $level = misimport_get_tree_by_area($area);
        $terms = misimport_get_tree_by_code($field_name);
        $node->taxonomy = array_merge($level, $terms);
        $new++;
      }

      foreach ($data as $no => $col) {
        if ($no > 2) {
          $value = trim($col);
          if ($value != 'N/A') {
            $value = (float) str_replace(',', '', $value);
          }
          $node->field_year_value[$heads[$no]] = array(
            'key' => $heads[$no],
            'value' => $value,
          );
        }
      }

      sort($node->field_year_value);
      $nodes[] = $node;
    }

    if ($nodes) {
      foreach ($nodes as $node) {
        node_save($node);
      }

      $context['results'][] = array(
        'code' => $code,
        'new' => $new,
        'update' => $update,
      );
    }
    else {
      $context['results'][] = t('Cannot found data to import.');
    }
  }

  $context['sandbox']['progress']++;

  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Finish import.
 */
function helmet_provincial_import_form_finish($success, $results, $operations) {
  if ($success) {
    $message = count($results) .' processed.';
    $message .= '<hr />';
    foreach ($results as $result) {
      if (is_array($result)) {
        $message .= '<strong>'. $result['code'] .'</strong>';
        $message .= '<div>'. $result['new'] .' nodes created.</div>';
        $message .= '<div>'. $result['update'] .' nodes updated.</div>';
        $message .= '<hr />';
      }
      else {
        $message .= $result;
      }
    }
  }
  else {
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array('%error_operation' => $error_operation[0], '@arguments' => print_r($error_operation[1], TRUE)));
  }
  drupal_set_message($message);
}

/**
 * Import provincial statistic.
 */
function helmet_provincial_import_form_submit($form, $form_state) {
  // Check directory.
  $directory = misimport_check_directory('helmet/provincial/');
  if (!$directory) {
    return;
  }

  $operpations = array();
  $import_time = time();

  foreach ($form_state['values'] as $name => $value) {
    if (substr($name, 0, 4) !== 'file') continue;

    // Reform code.
    $code = array_slice(explode('_', $name), 1);
    $code = strtoupper(implode('-', $code));

    // Check directory.
    $directory = misimport_check_directory('helmet/provincial/'. $code .'/');
    if (!$directory) {
      continue;
    }

    $file = misimport_get_file($name);
    if (!$file) continue;

    $expect_filename = $code .'.csv';
    if ($file['orig_name'] != $expect_filename) {
      drupal_set_message(t('File name must be <strong><em>!name</em></strong> only.', array('!name' => $expect_filename)), 'error');
      continue;
    }

    $file_obj = file_save_upload($name, array(), $directory);
    if ($file_obj) {
      file_set_status($file_obj, FILE_STATUS_PERMANENT);
      db_query("INSERT INTO {helmet} (type, delta, fid, timestamp) VALUES ('%s', '%s', %d, %d)", 'provincial', $code .'.csv', $file_obj->fid, $import_time);
    }
    else {
      drupal_set_message(t('Cannot save file !name', array('!name' => $expect_filename)), 'error');
      continue;
    }
    dpm($name);
    dpm($code);
    dpm($file_obj);
    dpm($import_time);
    continue;
    // Build operation.
    $operations[] = array(
      'helmet_provincial_import_process',
      array($name, $code, $file_obj, $import_time),
    );
  }

  if ($operations) {

    $count = count($operations);
    foreach ($operations as $index => $operation) {
      $operations[$index][1][] = $count;
    }

    // Build batch process.
    $batch = array(
      'title' => t('Importing'),
      'operations' => $operations,
      'finished' => 'helmet_provincial_import_form_finish',
      'init_message' => t('Reading data'),
      'progress_message' => t('Importing'),
      'error_message' => t('Error'),
      'file' => drupal_get_path('module', 'helmet') .'/includes/helmet.provincial.inc',
    );
    
    // Set batch process.
    batch_set($batch);
  }
}
