<?php

function block_edit_admin_settings() {
  $form = array();

  $form['block_edit_hover_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use hover effect'),
    '#default_value' => variable_get('block_edit_hover_links', 1),
  );

  $form['block_edit_block_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable block edit links'),
    '#default_value' => variable_get('block_edit_block_links', 1),
  );

  $form['block_edit_node_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable node edit links'),
    '#default_value' => variable_get('block_edit_node_links', 1),
  );

  $form['block_edit_panels_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable panels edit links'),
    '#default_value' => variable_get('block_edit_panels_links', 0),
  );

  $form['links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Link type activation settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['links']['node_link_options'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Types of node links to show'),
    '#options' => block_edit_node_link_options(),
    '#default_value' => variable_get('node_link_options', block_edit_node_link_options()),
  );

  $form['types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content type activation settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['types']['block_edit_content_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types'),
    '#description' => t('Select the content types that node edit links should appear on.'),
    '#options' => node_get_types('names'),
    '#default_value' => variable_get('block_edit_content_types', array_keys(node_get_types('names'))),
  );

  $display_options = array(
    'both' => 'Both',
    'teaser' => 'Teaser only',
    'full' => 'Full page only',
    'none' => 'None',
  );
  $form['types']['block_edit_display_options'] = array(
    '#type' => 'radios',
    '#title' => t('Display options'),
    '#description' => t('Choose which display modes the node edit links should be displayed on.'),
    '#options' => $display_options,
    '#default_value' => variable_get('block_edit_display_options', 'both'),
  );
  $form['types']['block_edit_tabs'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enable tabs'),
    '#description' => t('Enable the tabs for each content type.'),
    '#options' => node_get_types('names'),
    '#default_value' => variable_get('block_edit_tabs', array_keys(node_get_types('names'))),
  );

  $form['active'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page specific activation settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['active']['block_edit_block_active_type'] = array(
    '#type' => 'radios',
    '#title' => t('Enable block edit links on specific pages'),
    '#options' => array('disable' => t('Enable on every page except the listed pages.'), 'enable' => t('Enable on only the listed pages.'), 'php' => t('Enable if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).')),
    '#default_value' => variable_get('block_edit_block_active_type', 'disable'),
  );
  $form['active']['block_edit_block_active_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#default_value' => variable_get('block_edit_block_active_pages', ''),
    '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page. If the PHP-mode is chosen, enter PHP code between php tags. Note that executing incorrect PHP-code can break your Drupal site.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );

  $form['active']['block_edit_node_active_type'] = array(
    '#type' => 'radios',
    '#title' => t('Enable node edit links on specific pages'),
    '#options' => array('disable' => t('Enable on every page except the listed pages.'), 'enable' => t('Enable on only the listed pages.'), 'php' => t('Enable if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).')),
    '#default_value' => variable_get('block_edit_node_active_type', 'disable'),
  );
  $form['active']['block_edit_node_active_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#default_value' => variable_get('block_edit_node_active_pages', ''),
    '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page. If the PHP-mode is chosen, enter PHP code between php tags. Note that executing incorrect PHP-code can break your Drupal site.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );

  return system_settings_form($form);
}
