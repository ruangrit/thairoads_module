<?php

/**
 * Overview.
 */
function misimport_overview() {
  $national = l(t('Import national statistic'), 'admin/settings/misimport/national');
  $regional = l(t('Import regional statistic'), 'admin/settings/misimport/regional');
  $provincial = l(t('Import provincial statistic'), 'admin/settings/misimport/provincial');
  $output = <<<OUTP
<ul>
  <li>$national</li>
  <li>$regional</li>
  <li>$provincial</li>
</ul>
OUTP;
  return $output;
}

/**
 * Common import form.
 */
function misimport_common_import_form($form_state, $type) {
  require_once drupal_get_path('module', 'misimport') .'/includes/misimport.'. $type .'.inc';

  $result = db_query("SELECT f.* FROM {misimport} m, {files} f WHERE f.fid = m.fid AND m.type = '%s' ORDER BY mid DESC", $type);
  $rows = array();
  while ($row = db_fetch_object($result)) {
    $rows[] = array(
      $row->timestamp,
      $row->filename,
      $row->filesize,
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
  $form['#submit'] = array('misimport_'. $type .'_import_form_submit');
  $form['#attributes'] = array('enctype' => 'multipart/form-data');
  return $form;
}
