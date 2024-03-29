<?php
// $Id: taxonomy.inc,v 1.1.4.8 2010/01/25 10:56:57 kiam Exp $

/**
 * @file
 * Integration file for taxonomy.module.
 */

/**
 * @addtogroup nodewords
 * @{
 */

/**
 * Implements hook_nodewords_type_id().
 */
function taxonomy_nodewords_type_id(&$result, $arg) {
  if ($arg[0] == 'taxonomy') {
    // Taxonomy paths: taxonomy/term/$tid , taxonomy/term/$tid1+$tid2.
    if (isset($arg[1]) && isset($arg[2]) && $arg[1] == 'term') {
      $ids = preg_split('![+, ]!', $arg[2]);

      if (count($ids)) {
        // Take the first ID that is numeric.
        foreach ($ids as $id) {
          if (is_numeric($id)) {
            $result['type'] = NODEWORDS_TYPE_TERM;
            $result['id'] = $id;
            return;
          }
        }
      }
    }
  }
}

/**
 * @} End of "addtogroup nodewords".
 */
