<?php
// $Id: sitedoc.admin.inc,v 1.1.2.1.2.4 2008/08/22 16:57:14 nancyw Exp $

// @file sitedoc.admin.inc

/********************************/
/**   Main settings function   **/
/********************************/
function sitedoc_settings_page() {
  return drupal_get_form('sitedoc_settings_form');
}  

function sitedoc_settings_form(&$form_state) {
  drupal_add_css(drupal_get_path('module', 'sitedoc') .'/sitedoc.css');
  $sitedoc_settings = variable_get('sitedoc_settings', array());

// Sections Fieldset 
  $form['sections'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Show Sections'), 
    '#weight' => -5,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#prefix' => '<div class="sitedoc_sections_block">',
    '#suffix' => '',
  );

  $form['sections']['drupal_section'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include Basic Drupal information?'),
    '#prefix' => '<div class="sitedoc_sections">',
//    '#suffix' => '</div>',
    '#default_value' => $sitedoc_settings['drupal_section'],
  );

  $kill_time = drupal_map_assoc(array(0, 1800, 3600, 7200, 10800, 14400, 18000, 21600, 32400, 43200, 86400, 172800), 'format_interval');
  $kill_time['0'] = t('Never');
  $form['sections']['kill_cron'] = array(
    '#type' => 'select',
    '#options' => $kill_time,
    '#title' => t('Delete Cron variables after'),
    '#prefix' => '<div class="sitedoc_options">',
    '#suffix' => '</div></div>',
    '#default_value' => $sitedoc_settings['kill_cron'],
  ); 

  $form['sections']['table_section'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include Table Summary?'),
    '#prefix' => '<div class="sitedoc_sections">',
//    '#suffix' => '</div>',
    '#default_value' => $sitedoc_settings['table_section'],
  );

  $form['sections']['show_indexes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show indexes?'),
    '#prefix' => '<div class="sitedoc_options">',
    '#suffix' => '</div>',
    '#default_value' => $sitedoc_settings['show_indexes'],
  );

  $form['sections']['optimize_tables'] = array(
    '#type' => 'checkbox',
    '#title' => t('Release overhead?'),
    '#prefix' => '<div class="sitedoc_options">',
    '#suffix' => '</div></div>',
    '#description' => t('(SQL intensive - uses OPTIMIZE)'),
    '#default_value' => $sitedoc_settings['optimize_tables'],
  );

  $form['sections']['node_section'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include Node Summary?'),
    '#prefix' => '<div class="sitedoc_sections">',
//    '#suffix' => '</div>',
    '#description' => t('(SQL intensive)'),
    '#default_value' => $sitedoc_settings['node_section'],
    );

  $form['sections']['node_show_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Show nodes exceeding (KB)'),
    '#prefix' => '<div class="sitedoc_options">',
    '#suffix' => '</div>',
    '#default_value' => $sitedoc_settings['node_show_size'],
    '#size' => 8,
    '#maxlength' => 7,
    );

  $form['sections']['node_max_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Warn if nodes exceed (KB)'),
    '#prefix' => '<div class="sitedoc_options">',
    '#suffix' => '</div>',
    '#default_value' => $sitedoc_settings['node_max_size'],
    '#size' => 8,
    '#maxlength' => 7,
    );

  $form['sections']['include_node_access'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include node access summary?'),
    '#prefix' => '<div class="sitedoc_options">',
    '#suffix' => '</div>',
    '#default_value' => $sitedoc_settings['include_node_access'],
    );

  $form['sections']['include_comment_count'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include comment count on nodes?'),
    '#prefix' => '<div class="sitedoc_options">',
    '#suffix' => '</div></div>',
    '#default_value' => $sitedoc_settings['include_comment_count'],
    );

  $form['sections']['variables_section'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include System Variables?'),
    '#prefix' => '<div class="sitedoc_sections">',
    '#suffix' => '</div>',
    '#description' => t('(CPU intensive)'),
    '#default_value' => $sitedoc_settings['variables_section'],
    );

  $form['sections']['module_section'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include Module summary?'),
    '#prefix' => '<div class="sitedoc_sections">',
//    '#suffix' => '</div>',
    '#default_value' => $sitedoc_settings['module_section'],
    );

  $form['sections']['module_suppress'] = array(
    '#type' => 'checkbox',
    '#title' => t('Exclude disabled modules?'),
    '#prefix' => '<div class="sitedoc_options">',
    '#suffix' => '</div>',
    '#default_value' => $sitedoc_settings['module_suppress'],
    );

  $module_sort_order = array(
    0 => 'package, project, module',
    1 => 'path, module',
    );

  $form['sections']['module_sort_order'] = array(
    '#type' => 'radios',
    '#title' => t('List sort order'),
    '#options' => $module_sort_order,
    '#prefix' => '<div class="sitedoc_options">',
    '#suffix' => '</div></div>',
    '#default_value' => $sitedoc_settings['module_sort_order'],
    );

  $form['sections']['content_section'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include Content Type summary?'),
    '#prefix' => '<div class="sitedoc_sections">',
    '#suffix' => '</div>',
    '#default_value' => $sitedoc_settings['content_section'],
    );

  if (module_exists('taxonomy')) {
    $form['sections']['vocabulary_section'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include Vocabulary summary?'),
      '#prefix' => '<div class="sitedoc_sections">',
      '#description' => t('(may be CPU and SQL intensive)'),
      '#default_value' => $sitedoc_settings['vocabulary_section'],
      );

  $form['sections']['orphan_term_node'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check for orphan Term_nodes?'),
    '#prefix' => '<div class="sitedoc_options">',
    '#suffix' => '</div>',
    '#description' => t('(may be SQL intensive)'),
    '#default_value' => $sitedoc_settings['orphan_term_node'],
    );

  $form['sections']['delete_orphan_term_nodes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete orphan term nodes?'),
    '#prefix' => '<div class="sitedoc_options">',
    '#suffix' => '</div></div>',
    '#default_value' => $sitedoc_settings['delete_orphan_term_nodes'],
    );
  }  /* end if taxo exists */

  $form['sections']['theme_section'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include Themes?'),
    '#prefix' => '<div class="sitedoc_sections">',
    '#suffix' => '</div>',
    '#default_value' => $sitedoc_settings['theme_section'],
    );

  $form['sections']['block_section'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include Blocks and Boxes?'),
    '#prefix' => '<div class="sitedoc_sections">',
    '#default_value' => $sitedoc_settings['block_section'],
    );

  $form['sections']['block_warn'] = array(
    '#type' => 'checkbox',
    '#title' => t('Warn on missing theme for blocks?'),
    '#prefix' => '<div class="sitedoc_options">',
    '#suffix' => '</div>',
    '#description' => t('This allows the module to display a warning message if a block exists for a missing theme.'),
    '#default_value' => $sitedoc_settings['block_warn'],
    );

  $form['sections']['block_delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete the orphan blocks?'),
    '#prefix' => '<div class="sitedoc_options">',
    '#suffix' => '</div></div>',
    '#description' => t('Delete the blocks with a missing theme.'),
    '#default_value' => $sitedoc_settings['block_delete'],
    );

  $form['sections']['roles_section'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include Roles and Permissions?'),
    '#prefix' => '<div class="sitedoc_sections">',
    '#default_value' => $sitedoc_settings['roles_section'],
    );

  $form['sections']['role_users'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show list of users for each role?'),
    '#prefix' => '<div class="sitedoc_options">',
    '#suffix' => '</div>',
    '#description' => t('Displays the list of users assigned the role.'),
    '#default_value' => $sitedoc_settings['role_users'],
    );

  $form['sections']['role_perms_list'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show role permissions as a list?'),
    '#prefix' => '<div class="sitedoc_options">',
    '#suffix' => '</div></div>',
    '#description' => t('If not chosen, the roles will be shown as a stream.'),
    '#default_value' => $sitedoc_settings['role_perms_list'],
    );

  if (module_exists('contact')) {
    $form['sections']['contacts_section'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include Contacts?'),
      '#prefix' => '<div class="sitedoc_sections">',
      '#suffix' => '</div>',
      '#default_value' => $sitedoc_settings['contacts_section'],
      );
  }

  if (module_exists('profile')) {
    $form['sections']['profile_fields_section'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include Profile Fields?'),
      '#prefix' => '<div class="sitedoc_sections">',
      '#suffix' => '</div>',
      '#default_value' => $sitedoc_settings['profile_fields_section'],
      );
  }

  if (module_exists('path')) {
    $form['sections']['url_alias_section'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include URL Aliases?'),
      '#prefix' => '<div class="sitedoc_sections">',
      '#suffix' => '</div>',
      '#description' => t('(SQL intensive)'),
      '#default_value' => $sitedoc_settings['url_alias_section'],
      );
  }

  $form['sections']['input_format_section'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include Input Formats and Filters?'),
    '#prefix' => '<div class="sitedoc_sections">',
    '#suffix' => '</div>',
    '#default_value' => $sitedoc_settings['input_format_section'],
    );

  // Update Button
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 5,
    '#prefix' => '</div><div class="clear-block"></div>'
    );

  return $form;
}

/**
 * Save the settings values.
 */
function sitedoc_settings_form_submit($form, &$form_state) {
// update the settings array and save it
  $settings = variable_get('sitedoc_settings', array());

  $settings['drupal_section'] = $form_state['values']['drupal_section'];
  $settings['kill_cron'] = $form_state['values']['kill_cron'];
  $settings['table_section'] = $form_state['values']['table_section'];
  $settings['show_indexes'] = $form_state['values']['show_indexes'];
  $settings['optimize_tables'] = $form_state['values']['optimize_tables'];
  $settings['node_section'] = $form_state['values']['node_section'];
  $settings['include_comment_count'] = $form_state['values']['include_comment_count'];
  $settings['include_node_access'] = $form_state['values']['include_node_access'];
  $settings['node_show_size'] = $form_state['values']['node_show_size'];
  $settings['node_max_size'] = $form_state['values']['node_max_size'];
  $settings['module_section'] = $form_state['values']['module_section'];
  $settings['module_suppress'] = $form_state['values']['module_suppress'];
  $settings['module_sort_order'] = $form_state['values']['module_sort_order'];
  $settings['content_section'] = $form_state['values']['content_section'];
  $settings['vocabulary_section'] = $form_state['values']['vocabulary_section'];
  $settings['orphan_term_node'] = $form_state['values']['orphan_term_node'];
  $settings['delete_orphan_term_nodes'] = $form_state['values']['delete_orphan_term_nodes'];
  $settings['theme_section'] = $form_state['values']['theme_section'];
  $settings['variables_section'] = $form_state['values']['variables_section'];
  $settings['block_section'] = $form_state['values']['block_section'];
  $settings['block_warn'] = $form_state['values']['block_warn'];
  $settings['block_delete'] = $form_state['values']['block_delete'];
  $settings['roles_section'] = $form_state['values']['roles_section'];
  $settings['role_users'] = $form_state['values']['role_users'];
  $settings['role_perms_list'] = $form_state['values']['role_perms_list'];
  $settings['contacts_section'] = $form_state['values']['contacts_section'];
  $settings['profile_fields_section'] = $form_state['values']['profile_fields_section'];
  $settings['url_alias_section'] = $form_state['values']['url_alias_section'];
  $settings['input_format_section'] = $form_state['values']['input_format_section'];

  variable_set('sitedoc_settings', $settings);

  drupal_set_message(t('Configuration has been updated. ') . l(t('Run now'), 'admin/build/sitedoc'), 'status'); 
}

function sitedoc_archive_form(&$form_state) {
  drupal_add_css(drupal_get_path('module', 'sitedoc') .'/sitedoc.css');
  $sitedoc_settings = variable_get('sitedoc_settings', array());

  $save_time = drupal_map_assoc(array(0, 3600, 7200, 14400, 21600, 43200, 86400, 172800, 259200, 604800), 'format_interval');
  $save_time['0'] = t('Never');
  $save_time['999999'] = t('Always');

  $form['archive_frequency'] = array(
    '#type' => 'select',
    '#options' => $save_time,
    '#title' => t('Archive frequency'),
    '#prefix' => '<div class="sitedoc_archive">',
    '#suffix' => '</div>',
    '#default_value' => $sitedoc_settings['archive_frequency'],
    '#description' => t('The Site Documentation module will run at the next scheduled Cron run after this period has elapsed. A zero value suppresses creation of an archive. "Always" means a file will be created every time Cron runs.'),
  );

  $form['archive_directory'] = array(
    '#type' => 'textfield',
    '#title' => t('Archive directory'),
    '#prefix' => '<div class="sitedoc_archive">',
    '#suffix' => '</div>',
    '#default_value' => $sitedoc_settings['archive_directory'],
    '#description' => t('This is directory within \'!filepath\' where the Site Documentation module will place the archive file. The current date will be appended to the file name.', array('!filepath' => file_directory_path())),
  );

// Add the Buttons
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 5,
    '#prefix' => '<div class="clear-block"></div>'
  );

  // If $form_values exists, then we've been here before (submitted), so now add a "run cron" button.
  if ($sitedoc_settings['archive_frequency'] > 0) {
    $form['cron'] = array(
      '#type' => 'submit',
      '#value' => t('Run Cron Now'),
      '#weight' => 6,
    );
  }

  return $form;
}

/**
 * Validate the submission.
 */
function sitedoc_archive_form_validate($form, &$form_state) {
  $archive_path = file_directory_path() .'/'. $form_state['values']['archive_directory'];
  if (!file_check_directory($archive_path, TRUE, 'archive_directory')) {
    form_set_error('archive_directory', t('The archive path could not be found or could not be created.'));
  }  // end path not found 
}

/**
 * Save the settings values.
 */
function sitedoc_archive_form_submit($form, &$form_state) {
  switch ($form_state['values']['op']) {
    case t('Run Cron Now'):
      drupal_goto('admin/reports/status/run-cron');
      break;

    case t('Save configuration'):
      // update the settings array and save it
      $sitedoc_settings = variable_get('sitedoc_settings', array());
      $sitedoc_settings['archive_frequency'] = $form_state['values']['archive_frequency'];
      $sitedoc_settings['archive_directory'] = $form_state['values']['archive_directory'];

      variable_set('sitedoc_settings', $sitedoc_settings);

      drupal_set_message(t('Configuration has been updated. ') . l(t('Run now'), 'admin/build/sitedoc'), 'status');
  }
}
