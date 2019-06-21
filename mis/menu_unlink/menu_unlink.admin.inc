<?php

/**
 * Overview.
 */
function menu_unlink_overview() {
  $headers = array(
    'mlid',
    t('Link title'),
    t('Type'),
    array(
      'data' => t('Operations'),
      'colspan' => 2,
    ),
  );
  $rows = array();
  $result = db_query("SELECT name, value FROM {variable} WHERE name LIKE 'menu_unlink_type_mlid_%%' ORDER BY name ASC");
  while ($row = db_fetch_object($result)) {
    $mlid = drupal_substr($row->name, drupal_strlen('menu_unlink_type_mlid_'));
    $link = menu_link_load($mlid);
    $fragment = variable_get('menu_unlink_fragment_mlid_'. $mlid, MENU_UNLINK_DEFAULT_FRAGMENT);
    $type = unserialize($row->value);
    $type_name = menu_unlink_type_readable($type);
    if ($type == MENU_UNLINK_TYPE_FRAGMENT) {
      $type_name .= ': #'. $fragment;
    }
    $rows[] = array(
      $link['mlid'],
      $link['title'],
      $type_name,
      l(t('Edit'), 'admin/build/menu/item/'. $mlid .'/edit', array('query' => array('destination' => $_GET['q']))),
      l(t('Delete'), 'admin/settings/menu_unlink/'. $mlid .'/delete', array('query' => array('destination' => $_GET['q']))),
    );
  }
  return theme('table', $headers, $rows);
}

/**
 * Add form.
 */
function menu_unlink_bulk_form() {
  $form['menu_unlink_mlids'] = array(
    '#type' => 'textarea',
    '#title' => t('Menu link id(s)'),
    '#description' => t('Enter menu link id(mlid) separate by comma or new line.'),
    '#rows' => 5,
  );
  $form['menu_unlink_type'] = array(
    '#type' => 'radios',
    '#title' => t('Unlink type'),
    '#description' => t('Choose what you you want a link to be.'),
    '#default_value' => MENU_UNLINK_TYPE_NONE,
    '#options' => array(
      -1 => t('None'),
      0 => ('Remove'),
      1 => t('Fragment'),
    ),
  );
  $form['menu_unlink_fragment'] = array(
    '#type' => 'textfield',
    '#title' => t('Fragment'),
    '#description' => t('If you choose type to fragment, you can enter fragment name here.'),
    '#default_value' => MENU_UNLINK_DEFAULT_FRAGMENT,
    '#size' => 50,
  );
  $form['menu_unlink_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['#redirect'] = 'admin/settings/menu_unlink';
  return $form;
}

/**
 * Add submit form.
 */
function menu_unlink_bulk_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $mlids = menu_unlink_extract($values['menu_unlink_mlids']);
  foreach ($mlids as $mlid) {
    if (is_numeric($mlid)) {
      if ($values['menu_unlink_type'] == MENU_UNLINK_TYPE_NONE) {
        menu_unlink_delete($mlid);
      }
      else {
        menu_unlink_save($mlid, $values['menu_unlink_type'], $values['menu_unlink_fragment']);
      }
    }
  }
}