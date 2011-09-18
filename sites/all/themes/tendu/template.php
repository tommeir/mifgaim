<?php
// $Id: template.php,v 1.2.2.6.2.12 2009/02/22 23:07:50 tombigel Exp $
/**
 * Tendu Drupal - A CSS Theme For Developers
 * Author: Tom Bigelajzen (http://drupal.org/user/173787) - http://tombigel.com
 * Initial Drupal 6 porting: 
 *   Lior Kesos (http://drupal.org/user/41517)
 *   Zohar Stolar (http://drupal.org/user/48488) 
 *   http://www.linnovate.net
 */

/* 
 * Force refresh of theme registry.
 * DEVELOPMENT USE ONLY - COMMENT OUT FOR PRODUCTION
 */
//drupal_rebuild_theme_registry();

/*
 * Initialize theme settings
 */
if (is_null(theme_get_setting('toggle_accesibility_links'))) { 
  global $theme_key;
  /*
   * The default values for the theme variables. Make sure $defaults exactly
   * matches the $defaults in the theme-settings.php file.
   */
  $defaults = array(            
    'toggle_language_switcher' => 1,
    'toggle_accesibility_links' => 1,
  );

  // Get default theme settings.
  $settings = theme_get_settings($theme_key);
  // Don't save the toggle_node_info_ variables.
  if (module_exists('node')) {
    foreach (node_get_types() as $type => $name) {
      unset($settings['toggle_node_info_' . $type]);
    }
  }
  // Save default theme settings.
  variable_set(
    str_replace('/', '_', 'theme_'. $theme_key .'_settings'),
    array_merge($defaults, $settings)
  );
  // Force refresh of Drupal internals.
  theme_get_setting('', TRUE);
}

function set_language_switcher(){  
  //If there is more then one language defined, add language switcher to page.tpl (defined in theme settings)  
  $lang_switch =  module_invoke('locale', 'block', 'view');
  return '<h2>'.$lang_switch['subject'].'</h2>'.$lang_switch['content'];
}
/**
 * Implement HOOK_theme
 * - Add conditional stylesheets:
 *   For more information: http://msdn.microsoft.com/en-us/library/ms537512.aspx
 */
function tendu_theme(&$existing, $type, $theme, $path){
  
  // Compute the conditional stylesheets.
  if (!module_exists('conditional_styles')) {
    include_once $base_path . drupal_get_path('theme', 'tendu') . '/template.conditional-styles.inc';
    // _conditional_styles_theme() only needs to be run once.
    if ($theme == 'tendu') {
      _conditional_styles_theme($existing, $type, $theme, $path);
    }
  }  
  $templates = drupal_find_theme_functions($existing, array('phptemplate', $theme));
  $templates += drupal_find_theme_templates($existing, '.tpl.php', $path);
  return $templates;
}

/**
 * Override or insert PHPTemplate variables into the page templates.
 * 
 * Note about body classes:
 *  Most of the variables here are Drupals default.  
 *  I changed "page_type" and "node_type" to not add the page/node id to the class,
 *  because I never needed the Drupal classes but I did find a use for a more general page or node type
 *  class, and also added some of my own.
 *  You can change anything here but the dependencies of Tendu's layout must stay intact:
 *  if ($vars['left'] && $vars['right']) {
 *    $body_classes[] = 'two-sidebars';
 *  } elseif (!$vars['left'] && !$vars['right']){
 *    $body_classes[] = 'no-sidebars';
 *  } else{
 *    $body_classes[] = 'one-sidebar';
 *  }
 *  if ($vars['left']) {
 *    $body_classes[] = 'with-sidebar-first';
 *  }
 *  if ($vars['right']) {
 *    $body_classes[] = 'with-sidebar-second';
 *  }  
 *    
 * @param $vars
 *   A sequential array of variables to pass to the theme template.
 */

function tendu_preprocess_page(&$vars) {  
  
  //Set Theme Settings dependent variables
  $vars['language_switcher'] = (theme_get_setting('toggle_language_switcher'))? set_language_switcher() : '';
  $vars['accesibility_links'] = (theme_get_setting('toggle_accesibility_links'))? true : false;
      
  // Add conditional stylesheets.
  if (!module_exists('conditional_styles')) {
    $vars['styles'] .= $vars['conditional_styles'] = variable_get('conditional_styles_' . $GLOBALS['theme'], '');
  }
  
  // Build array of helpful body classes
  $body_classes = array();
  // Page user is logged in
  $body_classes[] = ($vars['logged_in']) ? 'logged-in' : 'not-logged-in';
  // Page is front page
  $body_classes[] = ($vars['is_front']) ? 'front' : 'not-front'; 
  
  //Clean these strings from special characters (TODO: do we need this check?)
  $_page_type = str_replace(array('][', '_', ' '), '-', arg(0));
  $_node_type = str_replace(array('][', '_', ' '), '-', $vars['node']->type);
  // Page type (for admin, node, etc.)
  $body_classes[] = preg_replace('![^abcdefghijklmnopqrstuvwxyz0-9-_]+!s', '', 'page-' . $_page_type);
  //If node page, print node type
  if (isset($vars['node']) && $vars['node']->type) {
    $body_classes[] = 'node-type-'. $_node_type;
  }
  
  //Add classes depended on sidebars
  if ($vars['left'] && $vars['right']) {
    $body_classes[] = 'two-sidebars';
  } elseif (!$vars['left'] && !$vars['right']){
    $body_classes[] = 'no-sidebars';
  } else{
    $body_classes[] = 'one-sidebar';
  }

  if ($vars['left']) {
    $body_classes[] = 'with-sidebar-first';
  }
  if ($vars['right']) {
    $body_classes[] = 'with-sidebar-second';
  }
  $body_classes = array_filter($body_classes); // Remove empty elements
  $vars['body_classes'] = implode(' ', $body_classes);// Create class list separated by spaces
}
/**
 * Override block variables to add a variable with "first" and "last" classes to blocks per region
 * Again, most of the variables here are Drupals default.
 * I added $block_region_placement to pass the first/last string to the tpl.
 *  
 * @param $variables
 * A list of block variables
 */
function tendu_preprocess_block(&$variables) {
  
  $block_region_placement = array();  
  static $block_counter = array();
  
  // All blocks get an independent counter for each region.
  if (!isset($block_counter[$variables['block']->region])) {
    $block_counter[$variables['block']->region] = 1;
  }
  
  //Get a list of all blocks in this block's region
  $list = block_list($variables['block']->region);
  //Set class "first" to the first block
  if ($block_counter[$variables['block']->region] == 1) {
     $block_region_placement[] = 'block-first';
  }
  //Set class "last" to the last block
  if ($block_counter[$variables['block']->region] == count($list)) {
     $block_region_placement[] = 'block-last';
  }
  $block_region_placement = array_filter($block_region_placement); // Remove empty elements
  $variables['block_region_placement'] = implode(' ', $block_region_placement);// Create class list separated by spaces
  
  // Continue with Drupal default variables
  $variables['block_zebra'] = ($block_counter[$variables['block']->region] % 2) ? 'odd' : 'even';
  $variables['block_id'] = $block_counter[$variables['block']->region]++;

  $variables['template_files'][] = 'block-'. $variables['block']->region;
  $variables['template_files'][] = 'block-'. $variables['block']->module;
  $variables['template_files'][] = 'block-'. $variables['block']->module .'-'. $variables['block']->delta;
}


/**
 * Format a query pager.
 *
 * Menu callbacks that display paged query results should call theme('pager') to
 * retrieve a pager control so that users can view other results.
 * Format a list of nearby pages with additional query results.
 *
 * @param $tags
 *   An array of labels for the controls in the pager.
 * @param $limit
 *   The number of query results to display per page.
 * @param $element
 *   An optional integer to distinguish between multiple pagers on one page.
 * @param $parameters
 *   An associative array of query string parameters to append to the pager links.
 * @param $quantity
 *   The number of pages in the list.
 * @return
 *   An HTML string that generates the query pager.
 *
 * @ingroup themeable
 */
function tendu_pager($tags = array(), $limit = 10, $element = 0, $parameters = array(), $quantity = 9) {
  global $pager_page_array, $pager_total;

  // Calculate various markers within this pager piece:
  // Middle is used to "center" pages around the current page.
  $pager_middle = ceil($quantity / 2);
  // current is the page we are currently paged to
  $pager_current = $pager_page_array[$element] + 1;
  // first is the first page listed by this pager piece (re quantity)
  $pager_first = $pager_current - $pager_middle + 1;
  // last is the last page listed by this pager piece (re quantity)
  $pager_last = $pager_current + $quantity - $pager_middle;
  // max is the maximum page number
  $pager_max = $pager_total[$element];
  // End of marker calculations.

  // Prepare for generation loop.
  $i = $pager_first;
  if ($pager_last > $pager_max) {
    // Adjust "center" if at end of query.
    $i = $i + ($pager_max - $pager_last);
    $pager_last = $pager_max;
  }
  if ($i <= 0) {
    // Adjust "center" if at start of query.
    $pager_last = $pager_last + (1 - $i);
    $i = 1;
  }
  // End of generation loop preparation.

  $li_first = theme('pager_first', (isset($tags[0]) ? $tags[0] : t('« first')), $limit, $element, $parameters);
  $li_previous = theme('pager_previous', (isset($tags[1]) ? $tags[1] : t('‹ previous')), $limit, $element, 1, $parameters);
  $li_next = theme('pager_next', "next ›", $limit, $element, 1, $parameters);
  $li_last = theme('pager_last', (isset($tags[4]) ? $tags[4] : t('last »')), $limit, $element, $parameters);

  if ($pager_total[$element] > 1) {
    if ($li_first) {
      $items[] = array(
        'class' => 'pager-first',
        'data' => $li_first,
      );
    }
    if ($li_previous) {
      $items[] = array(
        'class' => 'pager-previous',
        'data' => $li_previous,
      );
    }

    // When there is more than one page, create the pager list.
    if ($i != $pager_max) {
      if ($i > 1) {
        $items[] = array(
          'class' => 'pager-ellipsis',
          'data' => '…',
        );
      }
      // Now generate the actual pager piece.
      for (; $i <= $pager_last && $i <= $pager_max; $i++) {
        if ($i < $pager_current) {
          $items[] = array(
            'class' => 'pager-item',
            'data' => theme('pager_previous', $i, $limit, $element, ($pager_current - $i), $parameters),
          );
        }
        if ($i == $pager_current) {
          $items[] = array(
            'class' => 'pager-current',
            'data' => $i,
          );
        }
        if ($i > $pager_current) {
          $items[] = array(
            'class' => 'pager-item',
            'data' => theme('pager_next', $i, $limit, $element, ($i - $pager_current), $parameters),
          );
        }
      }
      if ($i < $pager_max) {
        $items[] = array(
          'class' => 'pager-ellipsis',
          'data' => '…',
        );
      }
    }
    // End generation.
    if ($li_next) {
      $items[] = array(
        'class' => 'pager-next',
        'data' => $li_next,
      );
    }
    if ($li_last) {
      $items[] = array(
        'class' => 'pager-last',
        'data' => $li_last,
      );
    }
    return theme('item_list', $items, NULL, 'ul', array('class' => 'pager'));
  }
}