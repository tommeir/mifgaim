<?php
// $Id: template.php,v 1.12 2009/05/10 21:56:07 gibbozer Exp $

/**
 * Force refresh of theme registry.
 * DEVELOPMENT USE ONLY - COMMENT OUT FOR PRODUCTION
 */
// drupal_rebuild_theme_registry();

/**
 * Initialize theme settings
 */
if (is_null(theme_get_setting('container_class'))) {
  global $theme_key;
  // Save default theme settings
  $defaults = array(
    'container_class'     => 'medium',
    'iepngfix'       => 1,
    'custom'         => 0,
    'breadcrumb'     => 0,
    'totop'          => 0,
  );

  variable_set(
    str_replace('/', '_', 'theme_'. $theme_key .'_settings'),
    array_merge(theme_get_settings($theme_key), $defaults)
  );
  // Force refresh of Drupal internals
  theme_get_setting('', TRUE);
}

function phptemplate_preprocess(&$vars, $hook) {
  global $theme;

  // Set Page Class
  $vars['container_class'] = theme_get_setting('container_class');

  // Hide breadcrumb on all pages
  if (theme_get_setting('breadcrumb') == 0) {
    $vars['breadcrumb'] = '';  
  }

  // Theme primary and secondary links.
  $vars['primary_menu'] = theme('links', $vars['primary_links']);
  $vars['secondary_menu'] = theme('links', $vars['secondary_links']);

  // Set Accessibility nav bar
  if ($vars['primary_menu'] != '') {
  $vars['nav_access'] = '
    <ul id="nav-access" class="hidden">
      <li><a href="#primary-menu" accesskey="N" title="'.t('Skip to Primary Menu').'">'. t('Skip to Primary Menu') .'</a></li>
      <li><a href="#main-content" accesskey="M" title="'.t('Skip to Main Content').'">'.t('Skip to Main Content').'</a></li>
    </ul>
  ';
  }
  else {
  $vars['nav_access'] = '
    <ul id="nav-access" class="hidden">
      <li><a href="#main-content" accesskey="M" title="'.t('Skip to Main Content').'">'.t('Skip to Main Content').'</a></li>
    </ul>
  ';
  }

  // SEO optimization, add in the node's teaser, or if on the homepage, the mission statement
  // as a description of the page that appears in search engines
  // (Code sample from Blueprint theme.)
  $vars['meta'] = '';

  if ($vars['is_front'] && $vars['mission'] != '') {
    $vars['meta'] .= '<meta name="description" content="'. trim_text($vars['mission']) .'" />'."\n";
  }
  else if (isset($vars['node']->teaser) && $vars['node']->teaser != '') {
    $vars['meta'] .= '<meta name="description" content="'. trim_text($vars['node']->teaser) .'" />'."\n";
  }
  else if (isset($vars['node']->body) && $vars['node']->body != '') {
    $vars['meta'] .= '<meta name="description" content="'. trim_text($vars['node']->body) .'" />'."\n";
  }
  // SEO optimization, if the node has tags, use these as keywords for the page
  if (isset($vars['node']->taxonomy)) {
    $keywords = array();
    foreach ($vars['node']->taxonomy as $term) {
      $keywords[] = $term->name;
    }
    $vars['meta'] .= '<meta name="keywords" content="'. implode(',', $keywords) .'" />'."\n";
  }

  // SEO optimization, avoid duplicate titles in search indexes for pager pages
  if (isset($_GET['page']) || isset($_GET['sort'])) {
    $vars['meta'] .= '<meta name="robots" content="noindex,follow" />'. "\n";
  }

  /* Embed the Google search in various places, uncomment to make use of this
  // setup search for custom placement
  $search = module_invoke('google_cse', 'block', 'view', '0');
  $vars['search'] = $search['content'];
  */

  // Make sure framework styles are placed above all others.
  if ($vars['css']) {
    $vars['css_alt'] = css_rearrange($vars['css']);
    $vars['styles'] = drupal_get_css($vars['css_alt']);
  }

  // Change sitename text style to be look like site logo if there's no logo.
  $vars['sitename_id'] = 'site-name';
  if (!$vars['logo']) {
    $vars['sitename_id'] = 'site-name-logo';
  }

  // Set Back to Top link toggle
  $vars['to_top'] = theme_get_setting('totop');
  if (theme_get_setting('totop') == 0) {
    $vars['to_top'] = '';
  }
  else {
    $vars['to_top'] = '<p id="to-top"><a href="#top-wrapper">'. t('Back To Top') .'</a></p>';
  }

  $vars['closure'] .= '
  <p id="theme-credit"><a href="http://drupal.org/project/beach">Beach Theme</a> '. t('Provided By ') . '<a href="http://drupal.in.th/">'.t('Drupal Thailand').'</a>. '.t('Designed by ').'<a href="http://webzer.net/">Gibbo</a>.</p>
  ';

  // count footer columns and add last-column class for footer block regions
  // this class will useful for layouting
  $vars['footer_column_count'] = 0;
  $vars['column2_is_last'] = '';
  $vars['column3_is_last'] = '';
  $vars['column4_is_last'] = '';

  if ($vars['footer_1']) {
    $vars['footer_column_count']++;
  }

  if ($vars['footer_2']) {
    $vars['footer_column_count']++;
    if ($vars['footer_1'] && empty($vars['footer_3']) && empty($vars['footer_4'])) {
      $vars['column2_is_last'] = ' last-column';
    }
  }

  if ($vars['footer_3']) {
    $vars['footer_column_count']++;
    if ($vars['footer_1'] || $vars['footer_2']) {
      if (empty($vars['footer_4'])) {
        $vars['column3_is_last'] = ' last-column';
      }
    }
  }

  if ($vars['footer_4']) {
    $vars['footer_column_count']++;
    if ($vars['footer_1'] || $vars['footer_2'] || $vars['footer_3']) {
      $vars['column4_is_last'] = ' last-column';
    }
  }

}

/**
 * This rearranges how the style sheets are included so the framework styles
 * are included first.
 * Code sample from "Ninesixty" Drupal7.x Theme
 */
function css_rearrange($css) {
  global $theme_info, $base_theme_info;

  // Remove almost drupal core module CSS files except those specified in $exclude_list
  // to use only one CSS file (drupal-core.css)

  // This will reduce HTTP request for external CSS files

  $exclude_list = array(
    'color.css',
    'color-rtl.css',
  );

  foreach($css['all']['module'] as $key => $value) {
    $file = end(explode('/', $value));
    if (substr($key, 0, 8) == 'modules/' && !in_array($file, $exclude_list)) {
      unset($css['all']['module'][$key]);
    }
  }

  // Dig into the framework .info data.
  $framework = !empty($base_theme_info) ? $base_theme_info[0]->info : $theme_info->info;

  // Pull framework styles from the themes .info file and place them above all stylesheets.
  if (isset($framework['stylesheets'])) {

    foreach ($framework['stylesheets'] as $media => $styles_from_framework) {
      // Setup framework group.
      if (isset($css[$media])) {
        $css[$media] = array_merge(array('framework' => array()), $css[$media]);
      }
      else {
        $css[$media]['framework'] = array();
      }
      foreach ($styles_from_framework as $style_from_framework) {
        // Force framework styles to come first.
        if (strpos($style_from_framework, 'framework') !== FALSE) {
          $framework_shift = $style_from_framework;
          $remove_styles = array($style_from_framework);
          $css[$media]['framework'][$framework_shift] = TRUE;
          foreach ($remove_styles as $remove_style) {
            unset($css[$media]['theme'][$remove_style]);
          }
        }
      }
    }
  }
  return $css;
}

/**
 * Intercept comment template variables
 */
function phptemplate_preprocess_comment(&$vars, $hook) {
  static $comment_count = 1; // keep track the # of comments rendered
  
  // if the author of the node comments as well, highlight that comment
  $node = node_load($vars['comment']->nid);
  if ($vars['comment']->uid == $node->uid) {
    $vars['author_comment'] = TRUE;
  }
  // only show links for users that have permission to post or administer comments
  if (!user_access('post comments')) {
    $vars['links'] = '';
  }

  // if subjects in comments are turned off, don't show the title then
  if (variable_get('comment_subject_field', 1) == 0) {
    $vars['title'] = '';
  }

  // if user has no picture, remove div.picture
  if (empty($vars['comment']->picture)) {
    $vars['picture'] = '';
  }

  $vars['comment_count'] = $comment_count++;  
}

/**
 *  Create some custom classes for comments
 */
function comment_classes($comment) {
  global $user;

  $node = node_load($comment->nid);
  $output .= ($comment->new) ? ' comment-new' : ''; 
  $output .=  ' '. $status .' '; 
  if ($node->name == $comment->name) {	
    $output .= 'node-author';
  }
  if ($user->name == $comment->name) {	
    $output .=  ' mine';
  }
  return $output;
}

/**
 * Override comment wrapper to show you must login to comment.
 */
function beach_comment_wrapper($content, $node) {
  global $user;
  $output = '';

  if ($node = menu_get_object()) {
    $count = $node->comment_count .' '. format_plural($node->comment_count, 'comment', 'comments');

    if ($node->comment_count < 1) {
      $output .= $content;
    }
    else {
      $output .= '<h3 id="comment-number">'.$count.'</h3>';
      $output .= '<div id="comments">'.$content.'</div>';
    }
  }
  return $output;
}

/**
 *  Change Feed Icon to text only or use CSS background instead of IMG tag
 */

function phptemplate_feed_icon($url, $title) {
    return '<a href="'. check_url($url) .'" class="feed-icon" title="'.t('Syndicate content').'">'.t('RSS').'</a>';
}

/**
 * Set default form file input size to prevent overlapping
 */
function phptemplate_file($element) {
  $element['#size'] = 30;
  return theme_file($element);
}

/**
 *  Set Custom Stylesheet
 */
if (theme_get_setting('custom')) {
  drupal_add_css(drupal_get_path('theme', 'beach') .'/css/custom.css', 'theme');
}

/**
 *  Add IE PNG Transparent fix
 */
if (theme_get_setting('iepngfix')) {
  drupal_add_js(drupal_get_path('theme', 'beach') .'/js/jquery.pngFix.js', 'theme');
}

/**
 * Trim a post to a certain number of characters, removing all HTML. (Code sample from Blueprint theme.)
 */
function trim_text($text, $length = 150) {
  // remove any HTML or line breaks so these don't appear in the text
  $text = trim(str_replace(array("\n", "\r"), ' ', strip_tags($text)));
  $text = trim(substr($text, 0, $length));
  $lastchar = substr($text, -1, 1);
  // check to see if the last character in the title is a non-alphanumeric character, except for ? or !
  // if it is strip it off so you don't get strange looking titles
  if (preg_match('/[^0-9A-Za-z\!\?]/', $lastchar)) {
    $text = substr($text, 0, -1);
  }
  // ? and ! are ok to end a title with since they make sense
  if ($lastchar != '!' && $lastchar != '?') {
    $text .= '...';
  }
  return $text;
}
