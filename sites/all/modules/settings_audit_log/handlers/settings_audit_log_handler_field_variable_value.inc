<?php

class settings_audit_log_handler_field_variable_value extends views_handler_field {

  function pre_render($values) { 
    foreach ($values as $value) {
      if (!isset($value->unserialized)) {
        $value->settings_audit_log_variable_audit_log_old_value = print_r(unserialize($value->settings_audit_log_variable_audit_log_old_value), TRUE);
        $value->settings_audit_log_variable_audit_log_new_value = print_r(unserialize($value->settings_audit_log_variable_audit_log_new_value), TRUE);
        $value->unserialized = TRUE;
      }
    }
  }

  function theme($row) {
    return '<pre>' . parent::render($row) . '</pre>';
  }
}

