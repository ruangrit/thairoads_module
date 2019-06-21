<?php

/**
 * Settings form.
 */
function misadmin_settings_form() {
  $options = user_roles(TRUE);
  $form['misadmin_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select roles'),
    '#description' => t('Who is role that selected above, they can see our menu.'),
    '#options' => $options,
    '#default_value' => variable_get('misadmin_roles', array())
  );
  return system_settings_form($form);
}
