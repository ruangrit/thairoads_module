<?php
function jstree_admin_settings() {
  $form['jstree_imagecache_preset'] = array(
    '#type' => 'textfield',
    '#title' => t('Imagecache preset name'),
    '#description' => t('Specify preset name which be created in Imagecache.'),
    '#default_value' => variable_get('jstree_imagecache_preset', ''),
  );
  
  $form['jstree_large_imagecache_preset'] = array(
    '#type' => 'textfield',
    '#title' => t('Imagecache preset name (Large)'),
    '#description' => t('Specify preset name which be created in Imagecache (for header image).'),
    '#default_value' => variable_get('jstree_large_imagecache_preset', ''),
  );
  
  return system_settings_form($form);
}
