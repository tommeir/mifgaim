<?php
// $Id: systeminfo.admin.inc,v 1.1.2.15.2.2 2010/04/08 21:04:40 flanker Exp $
// $Name: DRUPAL-6--2-0 $

/**
 * @file
 *   Displays information about the Drupal installation and system environment.
 */


/**
 * Menu callback of settings page.
 */
function systeminfo_settings() {
  global $db_url;
  $form = array();

  // Drupal
  $form['drupal'] = array(
    '#type' => 'fieldset',
    '#title' => 'Drupal',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['drupal']['systeminfo_drupal_modules_sort'] = array(
    '#type' => 'radios',
    '#title' => t('Modules list'),
    '#description' => t(''),
    '#default_value' => variable_get('systeminfo_drupal_modules_sort', 'name'),
    '#options' => array(
      'name' => t("Ascending sorted by module's name."),
      'filename' => t("Ascending sorted by module's filename."),
      'callup' => t("Ascending sorted by module's call-up."),
    ),
  );
  $form['drupal']['systeminfo_drupal_themes_sort'] = array(
    '#type' => 'radios',
    '#title' => t('Themes list'),
    '#description' => t(''),
    '#default_value' => variable_get('systeminfo_drupal_themes_sort', 'name'),
    '#options' => array(
      'name' => t("Ascending sorted by theme's name."),
      'filename' => t("Ascending sorted by theme's filename."),
    ),
  );

  // PHP
  $form['php'] = array(
    '#type' => 'fieldset',
    '#title' => 'PHP',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['php']['systeminfo_php_phpinfo_parameter'] = array(
    '#type' => 'radios',
    '#title' => t('PHPinfo'),
    '#description' => t('Outputs a large amount of information about the current state of PHP.'),
    '#default_value' => variable_get('systeminfo_php_phpinfo_parameter', INFO_ALL),
    '#options' => array(
      INFO_GENERAL => t('The configuration line, php.ini location, build date, Web Server, System and more.'),
      INFO_CONFIGURATION => t('Current Local and Master values for PHP directives.'),
      INFO_MODULES => t('Loaded modules and their respective settings.'),
      INFO_ENVIRONMENT => t("Environment Variable information that's also available in \$_ENV."),
      INFO_VARIABLES => t('Shows all predefined variables from EGPCS (Environment, GET, POST, Cookie, Server).'),
      INFO_ALL => t('Shows all of the above.'),
    ),
  );

  // Database server
  $databases_scheme = array();
  $databases = !is_array($db_url) ? array('default' => $db_url) : $db_url;
  foreach ($databases as $database) {
    $db = parse_url($database);
    $databases_scheme[] = $db['scheme'];
  }

  $form['database'] = array(
    '#type' => 'fieldset',
    '#title' => t('Database server'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['database']['database_tables'] = array(
    '#type' => 'fieldset',
    '#title' => t('Database tables'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['database']['database_tables']['systeminfo_database_tables_display'] = array(
    '#type' => 'radios',
    '#title' => t('Display'),
    '#description' => t(''),
    '#default_value' => variable_get('systeminfo_database_tables_display', 'none'),
    '#options' => array(
      'all' => t('All database tables.'),
      'except' => t('Every database table except the listed tables.'),
      'listed' => t('Only the listed database tables.'),
      'none' => t('None database tables.'),
    ),
  );
  $form['database']['database_tables']['systeminfo_database_tables_tables'] = array(
    '#type' => 'textarea',
    '#title' => t('Tables'),
    '#default_value' => variable_get('systeminfo_database_tables_tables', ''),
    '#description' => t("Enter one database table per line. The '*' character is a wildcard."),
  );
  $form['database']['database_tables']['systeminfo_database_tables_rows'] = array(
    '#type' => 'select',
    '#title' => t('Rows'),
    '#default_value' => variable_get('systeminfo_database_tables_rows', 50),
    '#description' => t(''),
    '#options' => drupal_map_assoc(array(30, 50, 100)),
  );
  $form['database']['mysql'] = array(
    '#type' => 'fieldset',
    '#title' => 'MySQL',
    '#collapsible' => TRUE,
    '#collapsed' => !in_array('mysql', $databases_scheme),
  );
  $form['database']['mysql']['systeminfo_database_mysql_show_status_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display SQL statement SHOW STATUS.'),
    '#description' => t('Provides server status information.'),
    '#default_value' => variable_get('systeminfo_database_mysql_show_status_display', 0),
  );
  $form['database']['mysql']['systeminfo_database_mysql_show_table_status_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display SQL statement SHOW TABLE STATUS.'),
    '#description' => t('Provides information about each non-TEMPORARY table.'),
    '#default_value' => variable_get('systeminfo_database_mysql_show_table_status_display', 1),
  );
  $form['database']['mysql']['systeminfo_database_mysql_show_variables_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display SQL statement SHOW VARIABLES.'),
    '#description' => t('Shows the values of system variables.'),
    '#default_value' => variable_get('systeminfo_database_mysql_show_variables_display', 0),
  );
  $form['database']['mysqli'] = array(
    '#type' => 'fieldset',
    '#title' => 'MySQLi',
    '#collapsible' => TRUE,
    '#collapsed' => !in_array('mysqli', $databases_scheme),
  );
  $form['database']['mysqli']['systeminfo_database_mysqli_show_status_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display SQL statement SHOW STATUS.'),
    '#description' => t('Provides server status information.'),
    '#default_value' => variable_get('systeminfo_database_mysqli_show_status_display', 0),
  );
  $form['database']['mysqli']['systeminfo_database_mysqli_show_table_status_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display SQL statement SHOW TABLE STATUS.'),
    '#description' => t('Provides information about each non-TEMPORARY table.'),
    '#default_value' => variable_get('systeminfo_database_mysqli_show_table_status_display', 1),
  );
  $form['database']['mysqli']['systeminfo_database_mysqli_show_variables_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display SQL statement SHOW VARIABLES.'),
    '#description' => t('Shows the values of system variables.'),
    '#default_value' => variable_get('systeminfo_database_mysqli_show_variables_display', 0),
  );
  $form['database']['pgsql'] = array(
    '#type' => 'fieldset',
    '#title' => 'PostgreSQL',
    '#collapsible' => TRUE,
    '#collapsed' => !in_array('pgsql', $databases_scheme),
  );
  $form['database']['pgsql']['systeminfo_database_pgsql_show_all_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display SQL statement SHOW ALL.'),
    '#description' => t('Provides server status information.'),
    '#default_value' => variable_get('systeminfo_database_pgsql_show_all_display', 0),
  );

  return system_settings_form($form);
}
