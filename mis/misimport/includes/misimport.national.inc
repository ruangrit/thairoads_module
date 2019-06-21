<?php

function misimport_national_import_form() {
  $result = db_query("SELECT f.* FROM {misimport} m, {files} f WHERE f.fid = m.fid AND m.type = 'national' ORDER BY mid DESC");
  $rows = array();
  while ($row = db_fetch_object($result)) {
    $rows[] = array(
      date('d/m/Y - H:i:s', $row->timestamp),
      l($row->filename, $row->filepath),
      format_size($row->filesize),
    );
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
  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Upload file'),
    '#description' => t('File name must be <strong><em>national.csv</em></strong> only.'),
  );
  $form['type'] = array(
    '#type' => 'hidden',
    '#value' => $type,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  $form['#submit'] = array('misimport_national_import_form_submit');
  $form['#attributes'] = array('enctype' => 'multipart/form-data');
  return $form;
}

/**
 * Import national statistic.
 */
function misimport_national_import_form_submit($form, $form_state) {

  // Check directory.
  $directory = misimport_check_directory('misimport/national');
  if (!$directory) {
    return;
  }

  // Get file data.
  $file = misimport_get_file('file');

  // Check filename.
  if ($file['name'] != 'national.csv') {
    drupal_set_message(t('File name must be <strong><em>national.csv</em></strong> only.'), 'error');
    return;
  }

  // Get mimetype.
  $type = file_get_mimetype($file['name']);

  // Check type is csv.
  if ($type == 'text/csv') {

    // Open file.
    if (($handle = fopen($file['tmp_name'], 'r')) !== FALSE) {

      // Initialize state.
      $count = -1;
      // Hard code for National level.
      $level = misimport_get_tree_by_area(1000);
      // Prepare storage.
      $items = array();

      // Begin read line.
      while (($row = fgetcsv($handle, 1000, ',')) !== FALSE) {

        // Remove Area ID and Area data.
        // Use only year and value.
        $row = array_slice($row, 2);

        // Get head to analyze the data.
        if (!$is_read_head) {

          // Use only SPI.
          array_shift($row);

          // Get all terms hierarchy of type.
          $types = array();
          foreach ($row as $delta => $cell) {
            $code = 'N-'. $row[$delta];
            $types[] = array(
              'code' => $code,
              'terms' => array_merge(misimport_get_tree_by_code($code), misimport_get_tree_by_area(1000)),
              'nid' => misimport_get_node_by_code($code, 'nid'),
            );
          }

          // For the first time. That's all.
          $is_read_head = TRUE;
          continue;
        }

        $col_count = 1;
        $level = array();

        // Year.
        $year = $row[0];

        // Loop each cell in row.
        foreach ($row as $delta => $cell) {
          if ($delta > 0) {
            // Register new SPI.
            if (!isset($items[$types[$delta - 1]['code']])) {
              $items[$types[$delta - 1]['code']] = array();
            }
            // Check number is empty.
            if (empty($cell) || trim($cell) == '-') continue; 
            // Assign new data.
            $items[$types[$delta - 1]['code']]['field_year_value'][] = array(
              'key' => $year,
              'value' => (float) str_replace(',', '', $cell),
            );
          }
        } // End loop.

      } // End read file.

      // Begin count.
      $new = 0;
      $update = 0;
      // Loop items.
      foreach ($types as $delta => $type) {
        // Create new node.
        if (empty($type['nid'])) {
          $node = new stdClass();
          $node->type = 'statistic';
          $node->comment = COMMENT_NODE_DISABLED;
          $node->format = 1;
          $node->uid = 1;
          $node->status = 1;
          $node->title = $type['code'];
          $node->taxonomy = $type['terms'];
          $new++;
        }
        // Load exists node.
        else {
          $node = node_load($type['nid']);
          $node->title = $type['code'];
          $node->taxonomy = $type['terms'];
          unset($node->path);
          $update++;
        }
        // Set value.
        if ($items[$type['code']]['field_year_value']) {
          $node->field_year_value = $items[$type['code']]['field_year_value'];
        }
        // Save node.
        node_save($node);
      }

      // Report result.
      dm($new .' rows created.');
      dm($update .' rows updated.');
      dm('total rows is '. ($new + $update));
    }

    $file_obj = file_save_upload('file', array(), $directory);
    if ($file_obj) {
      file_set_status($file_obj, FILE_STATUS_PERMANENT);
      db_query("INSERT INTO {misimport} (type, delta, fid, timestamp) VALUES ('%s', '%s', %d, %d)", 'national', 'national', $file_obj->fid, time());
    }
    else {
      drupal_set_message(t('Cannot save file'), 'error');
      return;
    }
  }
  else {
    drupal_set_message('Wrong file type. You have to enter csv file.', 'error');
  }
}
