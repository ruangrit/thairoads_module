<?php

/**
 * Settings form.
 */
function misstat_admin_settings() {
  $form['misstat_stat_fs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Statistic'),
    '#collapsible' => TRUE,
  );
  $form['misstat_stat_fs']['misstat_national_description'] = array(
    '#type' => 'textarea',
    '#title' => t('National description'),
    '#default_value' => variable_get('misstat_national_description', ''),
  );
  $form['misstat_stat_fs']['misstat_provincial_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Provincial description'),
    '#default_value' => variable_get('misstat_provincial_description', ''),
  );
  $form['misstat_stat_fs']['misstat_summary_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Summary description'),
    '#default_value' => variable_get('misstat_summary_description', ''),
  );
  $form['misstat_stat_fs']['misstat_graph_fs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Graph settings'),
    '#collapsible' => TRUE,
  );
  $form['misstat_stat_fs']['misstat_graph_fs']['misstat_graph_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Graph width'),
    '#default_value' => variable_get('misstat_graph_width', 500),
  );
  $form['misstat_stat_fs']['misstat_graph_fs']['misstat_graph_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Graph height'),
    '#default_value' => variable_get('misstat_graph_height', 300),
  );
  $form['misstat_stat_fs']['misstat_graph_fs']['misstat_graph_steps'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of steps'),
    '#default_value' => variable_get('misstat_graph_steps', 10),
  );
  $form['misstat_stat_fs']['misstat_graph_fs']['misstat_graph_title_style'] = array(
    '#type' => 'textfield',
    '#title' => t('Graph title style'),
    '#default_value' => variable_get('misstat_graph_title_style', 'font-size: 20px'),
  );
  $form['misstat_stat_fs']['misstat_graph_fs']['misstat_graph_bg_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Graph background color'),
    '#default_value' => variable_get('misstat_graph_bg_color', ''),
  );
  $form['misstat_stat_fs']['misstat_graph_fs']['misstat_graph_national_color'] = array(
    '#type' => 'textfield',
    '#title' => t('National graph color'),
    '#default_value' => variable_get('misstat_graph_national_color', '#000000'),
  );
  $form['misstat_stat_fs']['misstat_graph_fs']['misstat_graph_regional_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Regional graph color'),
    '#default_value' => variable_get('misstat_graph_regional_color', '#000000'),
  );
  $form['misstat_stat_fs']['misstat_graph_fs']['misstat_graph_provincial_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Provincial graph color'),
    '#default_value' => variable_get('misstat_graph_provincial_color', '#000000'),
  );
  $form['misstat_stat_fs']['misstat_disable_tids'] = array(
    '#type' => 'textfield',
    '#title' => t('Disable terms to display'),
    '#default_value' => variable_get('misstat_disable_tids', ''),
    '#description' => t('Enter tids seperate by comma.'),
  );
  return system_settings_form($form);
}
