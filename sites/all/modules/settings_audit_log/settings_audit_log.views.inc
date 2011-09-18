<?php

/**
 * Implementation of hook_views_data().
 */
function settings_audit_log_views_data() {
  $data = array();

  $data['settings_audit_log_variable_audit_log']['table']['group'] = t('Settings audit log');
  $data['settings_audit_log_variable_audit_log']['table']['base'] = array(
    'field' => 'name',
    'title' => t('Settings audit log'),
    'help' => t("This table contains a history of changes to the {variable} table."),
    'weight' => 0,
  );

  $data['settings_audit_log_variable_audit_log']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  $data['settings_audit_log_variable_audit_log']['uid'] = array(
    'title' => t('User'),
    'help' => t('The user who made the change to variable value.'),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('User'),
    ),
  );
  $data['settings_audit_log_variable_audit_log']['name'] = array(
    'title' => t('Variable name'),
    'help' => t('The name of variable as passed to variable_get() or variable_set().'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['settings_audit_log_variable_audit_log']['old_value'] = array(
    'title' => t('Previous variable value'),
    'help' => t('The previous value of the variable in the {variable} table.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
      'handler' => 'settings_audit_log_handler_field_variable_value',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'serialize' => TRUE,
  );
  $data['settings_audit_log_variable_audit_log']['new_value'] = array(
    'title' => t('New variable value'),
    'help' => t('The new value of the variable in the {variable} table.'),
    'field' => array(
      'handler' => 'settings_audit_log_handler_field_variable_value',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'serialize' => TRUE,
  );
  $data['settings_audit_log_variable_audit_log']['event_timestamp'] = array(
    'title' => t('The timestamp of the change'),
    'help' => t('The timestamp of the change.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  return $data;
}


