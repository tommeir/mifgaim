<?php
// $Id: block_edit.views.inc,v 1.1.2.2 2009/12/17 09:09:42 psynaptic Exp $

function block_edit_views_admin_links_alter(&$links, $view) {
  if (strpos($view->current_display, 'block') === 0) {
    $links[] = array(
      'title' => t('Configure'),
      'alt' => t('Configure this block'),
      'href' => "admin/build/block/configure/views/$view->name-$view->current_display",
      'query' => drupal_get_destination(),
    );
  }
}
