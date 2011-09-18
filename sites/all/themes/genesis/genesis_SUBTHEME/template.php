<?php
// $Id: template.php,v 1.2.2.6 2009/05/22 20:25:24 jmburnz Exp $

/**
 * @file template.php
 */

/**
 * USAGE
 * 1. Rename each function to match your subthemes name, 
 *    e.g. if you name your theme genesis_foo then the function 
 *    name will be "genesis_foo_preprocess".
 * 2. Uncomment the required fucntion to use. You can delete the
 *    "sample_variable".
 */

/**
 * Override or insert variables into all templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered.
 */
/*
function genesis_SUBTHEME_preprocess(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
*/

/**
 * Override or insert variables into the page templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered.
 */
/*
function genesis_SUBTHEME_preprocess_page(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
*/

/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered.
 */
/*
function genesis_SUBTHEME_preprocess_node(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
*/

/**
 * Override or insert variables into the comment templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered.
 */
/*
function genesis_SUBTHEME_preprocess_comment(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
*/

/**
 * Override or insert variables into the block templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered.
 */
/*
function genesis_SUBTHEME_preprocess_block(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
*/
/**
 * Process variables for comment-wrapper.tpl.php.
 *
 * @see comment-wrapper.tpl.php
 * @see theme_comment_wrapper()
 */
function genesis_SUBTHEME_preprocess_comment_wrapper(&$variables) {
  $variables['content'] = str_replace( '<h2 id="comments-title">'.t('Comments').'</h2>', '' , $variables['content'] );
  //$variables['content'] //= mb_substr($variables['content'],strlen('<h2 id="comments-title">'.t('Comments').'</h2>'));
}
function genesis_SUBTHEME_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    $result='<div class="breadcrumb">' . '<div class="right"></div><div class="center">';
	$result .= '<span class="hide">' . t('You are here:') . '</span>';
	$result .= implode('<span class="arrow">&nbsp;</span>', $breadcrumb);
	$result .='</div><div class="left"></div></div>';
	return $result;
  }
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
function genesis_SUBTHEME_pager($tags = array(), $limit = 10, $element = 0, $parameters = array(), $quantity = 9) {
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

  //$li_first = theme('pager_first', (isset($tags[0]) ? $tags[0] : t('< first')), $limit, $element, $parameters);
  $li_previous = theme('pager_previous', (isset($tags[1]) ? $tags[1] : t('previous')), $limit, $element, 1, $parameters);
  $li_next = theme('pager_next', (isset($tags[3]) ? $tags[3] : t('next')), $limit, $element, 1, $parameters);
  //$li_last = theme('pager_last', (isset($tags[4]) ? $tags[4] : t('last >')), $limit, $element, $parameters);

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

function genesis_SUBTHEME_preprocess_page(&$vars, $hook) {
  //dpm($vars);
  $vars['site_logo '] = '<a href="/" title="'.t('Home page').'" alt="'.t('Home page').'" rel="home">'.$vars['logo'].'</a>';
  if (isset($vars['node'])) {
   // If the node type is "blog" the template suggestion will be "page-blog.tpl.php".
   $vars['template_files'][] = 'page-'. str_replace('_', '-', $vars['node']->type);
  }
    $vars['comments'] = $vars['comment_form'] = '';
    if (module_exists('comment') && isset($vars['node'])) {
    $vars['comments'] = comment_render($vars['node']);
    $vars['comment_form'] = drupal_get_form('comment_form', array('nid' => $vars['node']->nid));
  }
}
function genesis_SUBTHEME_preprocess_node(&$vars) {
  $vars['node']->comment = 0;
}

function genesis_SUBTHEME_menu_item($link, $has_children, $menu = '', $in_active_trail = FALSE, $extra_class = NULL) {
  $class = ($menu ? 'expanded' : ($has_children ? 'collapsed' : 'leaf'));
  if (!empty($extra_class)) {
    $class .= ' ' . $extra_class;
  }
  if ($in_active_trail) {
    $class .= ' active-trail selectedLava';
  }
  return '<li class="' . $class . '">' . $link . $menu . "</li>\n";
}

function genesis_SUBTHEME_menu_item_link($link) {
  if (empty($link['localized_options'])) {
    $link['localized_options'] = array();
  }
  if ($link['in_active_trail'] == TRUE && $link['menu_name'] == 'primary-links') {
    $link['localized_options']['attributes']['class'] = 'selectedLava';
  }
  if($link['menu_name'] == 'menu-footer' && $link['has_children']) {
    return '<h3>' . $link['title'] . '</h3>';
  }
  return l($link['title'], $link['href'], $link['localized_options']);
}

/**
 * Format a link to a specific query result page.
 *
 * @param $page_new
 *   The first result to display on the linked page.
 * @param $element
 *   An optional integer to distinguish between multiple pagers on one page.
 * @param $parameters
 *   An associative array of query string parameters to append to the pager link.
 * @param $attributes
 *   An associative array of HTML attributes to apply to a pager anchor tag.
 * @return
 *   An HTML string that generates the link.
 *
 * @ingroup themeable
 */
function genesis_SUBTHEME_pager_link($text, $page_new, $element, $parameters = array(), $attributes = array()) {
  $page = isset($_GET['page']) ? $_GET['page'] : '';
  if ($new_page = implode(',', pager_load_array($page_new[$element], $element, explode(',', $page)))) {
    $parameters['page'] = $new_page;
  }

  $query = array();
  if (count($parameters)) {
    $query[] = drupal_query_string_encode($parameters, array());
  }
  $querystring = pager_get_querystring();
  if ($querystring != '') {
    $query[] = $querystring;
  }

  // Set each pager link title
  if (!isset($attributes['title'])) {
    static $titles = NULL;
    if (!isset($titles)) {
      $titles = array(
        t('first') => t('Go to first page'),
        t('previous') => t('Go to previous page'),
        t('next') => t('Go to next page'),
        t('last') => t('Go to last page'),
      );
    }
    if (isset($titles[$text])) {
      $attributes['title'] = $titles[$text];
    }
    else if (is_numeric($text)) {
      $attributes['title'] = t('Go to page @number', array('@number' => $text));
    }
  }
  return l($text, $_GET['q'], array('attributes' => $attributes, 'query' => count($query) ? implode('&', $query) : NULL));
}
/**
 * Return a themed site map box.
 *
 * @param $title
 *   The subject of the box.
 * @param $content
 *   The content of the box.
 * @param $class
 *   Optional extra class for the box.
 * @return
 *   A string containing the box output.
 */
function genesis_SUBTHEME_site_map_box($title, $content, $class = '') {
  $output = '';
  if ($title || $content) {
    $class = $class ? 'site-map-box '. $class : 'site-map-box';
    $output .= '<div class="'. $class .'">';
    if ($title) {
      $output .= '<h2 class="title">'. t($title) .'</h2>';
    }
    if ($content) {
      $output .= '<div class="content">'. $content .'</div>';
    }
    $output .= '</div>';
  }

  return $output;
}

function genesis_SUBTHEME_form_element($element, $value) {
  // This is also used in the installer, pre-database setup.
  $t = get_t();

  $output = '<div class="form-item"';
  if (!empty($element['#id'])) {
    $output .= ' id="' . $element['#id'] . '-wrapper"';
  }
  $output .= ">\n";
  $required = !empty($element['#required']) ? '<span class="form-required" title="' . $t('This field is required.') . '">*</span>' : '';

  if (!empty($element['#title'])) {
    $title = $element['#title'];
    if (!empty($element['#id'])) {
      $output .= ' <label for="' . $element['#id'] . '">' . $t('!title: !required', array('!title' => filter_xss_admin($title), '!required' => $required)) . "</label>\n";
    }
    else {
      $output .= ' <label>' . $t('!title: !required', array('!title' => filter_xss_admin($title), '!required' => $required)) . "</label>\n";
    }
  }
  if (!empty($element['#description'])) {
    $output .= ' <div class="description">' . $element['#description'] . "</div>\n";
  }
  $output .= " $value\n";

  $output .= "</div>\n";

  return $output;
}
