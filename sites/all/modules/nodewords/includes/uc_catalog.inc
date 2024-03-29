<?php
// $Id: uc_catalog.inc,v 1.1.2.4 2010/01/25 10:56:57 kiam Exp $

/**
 * @file
 * Integration file for uc_catalog.module.
 */

/**
 * @addtogroup nodewords
 * @{
 */

/**
 * Implements hook_nodewords_type_id().
 */
function uc_catalog_nodewords_type_id(&$result, $arg) {
  if ($arg[0] == 'catalog') {
    if (isset($arg[1]) && is_numeric($arg[1])) {
      $result['type'] = NODEWORDS_TYPE_TERM;
      $result['id'] = $arg[1];
    }
  }
}

/**
 * @} End of "addtogroup nodewords".
 */