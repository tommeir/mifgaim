<?php
// $Id: addthis.views.inc,v 1.1.2.1.2.1 2010/07/30 12:58:14 wesku Exp $

/**
 * @file
 * Provide views data and handlers for addthis.module
 */

/**
 * Implementation of hook_views_data().
 */
function addthis_views_data() {
  // We're registering the 'custom_teasers' table.
  $data['node']['addthis_button'] = array(
    'title' => t('AddThis button'),
    'help' => t('An AddThis button, associated with this node.'),
    'field' => array(
      'handler' => 'addthis_handler_field_button',
    ),
  );
  $data['node']['addthis_toolbox'] = array(
    'title' => t('AddThis toolbox'),
    'help' => t('An AddThis toolbox, associated with this node.'),
    'field' => array(
      'handler' => 'addthis_handler_field_toolbox',
    ),
  );
  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function addthis_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'addthis'),
    ),
    'handlers' => array(
      'addthis_handler_field_button' => array(
        'parent' => 'views_handler_field',
        'file' => 'addthis_handler_field.inc',
      ),
      'addthis_handler_field_toolbox' => array(
        'parent' => 'views_handler_field',
        'file' => 'addthis_handler_field.inc',
      ),
    ),
  );
}