<?php
// $Id: user.inc,v 1.1.4.8 2010/01/25 10:56:57 kiam Exp $

/**
 * @file
 * Integration file for user.module.
 */

/**
 * @addtogroup nodewords
 * @{
 */

/**
 * Implements hook_nodewords_type_id().
 */
function user_nodewords_type_id(&$result, $arg) {
  if ($arg[0] == 'user') {
    // User page paths: user/$uid.
    if (isset($arg[1]) && is_numeric($arg[1])) {
      $result['type'] = NODEWORDS_TYPE_USER;
      $result['id'] = $arg[1];
    }
  }
}

/**
 * @} End of "addtogroup nodewords".
 */
