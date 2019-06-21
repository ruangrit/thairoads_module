<?php

function misimport_regional_import_form() {
  $result = db_query("SELECT f.* FROM {misimport} m, {files} f WHERE f.fid = m.fid AND m.type = 'regional' ORDER BY mid DESC");
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
    '#description' => t('File name must be <strong><em>regional.csv</em></strong> only.'),
  );
  $form['type'] = array(
    '#type' => 'hidden',
    '#value' => $type,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  $form['#submit'] = array('misimport_regional_import_form_submit');
  $form['#attributes'] = array('enctype' => 'multipart/form-data');
  return $form;
}

/**
 * Import regional statistic.
 */
function misimport_regional_import_form_submit($form, $form_state) {

  // Check directory.
  $directory = misimport_check_directory('misimport/regional');
  if (!$directory) {
    return;
  }

  // Get file data.
  $file = misimport_get_file('file');

  // Check filename.
  if ($file['name'] != 'regional.csv') {
    drupal_set_message(t('File name must be <strong><em>regional.csv</em></strong> only.'), 'error');
    return;
  }

  // Get mimetype.
  $type = file_get_mimetype($file['name']);

  // Check file type is csv.
  if ($type == 'text/csv') {

    // Open file.
    if (($handle = fopen($file['tmp_name'], 'r')) !== FALSE) {

      // Initialize state.
      $count = -1;
      // Prepare storage.
      $items = array();

      // Begin read line.
      while (($row = fgetcsv($handle, 2048, ',')) !== FALSE) {

        // Analyze head at the first time.
        if (!$is_read_head) {

          // Skip first 3 cells (0 = Area ID, 1 = Area, 2 = Year).
          $row = array_slice($row, 3);

          // Get all terms hierarchy of type.
          $types = array();
          foreach ($row as $delta => $cell) {
            $cell = strtoupper(trim($cell));
            if (strpos($cell, 'SPI') === FALSE) break;

            $code = 'R-'. $cell;
            $types[] = array(
              'code' => $code,
              'terms' => misimport_get_tree_by_code($code),
            );
          }

          $is_read_head = TRUE;
          continue;
        }

        // Area.
        $area = $row[0];
        $level = misimport_get_tree_by_area($area);

        // Year.
        $year = $row[2];

        // Skip first 3 cells.
        $row = array_slice($row, 3);

        foreach ($types as $delta => $type) {
          $cell = $row[$delta];

          // Register new SPI (array).
          if (!isset($items[$area])) {
            $items[$area] = array();
          }

          // Skip if number is empty.
          if (empty($cell) || trim($cell) == '-') continue; 

          // Assign new data.
          $items[$area][$type['code']]['field_year_value'][] = array(
            'key' => $year,
            'value' => (float) str_replace(',', '', $cell),
          );

          // Assign terms.
          $items[$area][$type['code']]['terms'] = array_merge($type['terms'], $level);

          // Check node exists.
          $items[$area][$type['code']]['nid'] = misimport_get_node_by_code_area($type['code'], $area, 'nid');
        }
      }

      // Loop items.
      foreach ($items as $area_id => $item) {
        // Begin count.
        $new[$area_id] = 0;
        $update[$area_id] = 0;
        foreach ($item as $code => $data) {
          // Create new node.
          if (empty($data['nid'])) {
            $node = new stdClass();
            $node->type = 'statistic';
            $node->comment = COMMENT_NODE_DISABLED;
            $node->format = 1;
            $node->uid = 1;
            $node->status = 1;
            $node->title = $code;
            $node->taxonomy = $data['terms'];
            $new[$area_id]++;
          }
          // Load exists node.
          else {
            $node = node_load($data['nid']);
            $node->title = $code;
            $node->taxonomy = $data['terms'];
            unset($node->path);
            $update[$area_id]++;
          }
          // Set value.
          $node->field_year_value = $data['field_year_value'];
          // Save node.
          node_save($node);
        }
      }

      // Report result.
      $sum = 0;
      foreach ($new as $id => $num) {
        dm($num .' rows created on area #'. $id .'.');
        $sum += $num;
      }
      dm('<hr />');
      foreach ($update as $id => $num) {
        dm($num .' rows updated on area #'. $id .'.');
        $sum += $num;
      }
      dm('total rows is '. $sum);
    }

    $file_obj = file_save_upload('file', array(), $directory);
    if ($file_obj) {
      file_set_status($file_obj, FILE_STATUS_PERMANENT);
      db_query("INSERT INTO {misimport} (type, delta, fid, timestamp) VALUES ('%s', '%s', %d, %d)", 'regional', 'regional', $file_obj->fid, time());
    }
    else {
      drupal_set_message(t('Cannot save file'), 'error');
      return;
    }
  }
  else {
    dm('Wrong file type. You have to enter csv file.', 'error');
  }
}
