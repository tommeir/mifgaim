<?php
// $Id: image.inc,v 1.1.4.6 2010/01/25 10:56:57 kiam Exp $

/**
 * @file
 * Integration file for image.module.
 */

/**
 * @addtogroup nodewords
 * @{
 */

/**
 * Implements hook_nodewords_type_id().
 */
function image_nodewords_type_id(&$result, $arg) {
  if ($arg[0] == 'image') {
    // Image gallery paths: image/ , image/???/$tid.
    if (!isset($arg[1])) {
      $result['type'] = NODEWORDS_TYPE_VOCABULARY;
      $result['id'] = variable_get('image_gallery_nav_vocabulary', 0);
    }
    elseif (isset($arg[2]) && is_numeric($arg[2])) {
      $result['type'] = NODEWORDS_TYPE_TERM;
      $result['id'] = $arg[1];
    }
  }
}

/**
 * @} End of "addtogroup nodewords".
 */
