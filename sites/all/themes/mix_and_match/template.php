<?php
// $Id: template.php,v 1.1 2010/10/26 23:22:22 aross Exp $

/**
 * Maintenance page preprocessing
 */
function mix_and_match_preprocess_maintenance_page(&$vars) {
  mix_and_match_preprocess_page($vars);
}

/**
 * Page preprocessing
 */
function mix_and_match_preprocess_page(&$vars) {
  // Add body classes for custom design options
  $body_classes = explode(' ', $vars['body_classes']);
  $body_classes[] = theme_get_setting('mix_and_match_body_bg');
  $body_classes[] = theme_get_setting('mix_and_match_accent_color');
  $body_classes[] = theme_get_setting('mix_and_match_footer_color');
  $body_classes[] = theme_get_setting('mix_and_match_header_color');
  $body_classes[] = theme_get_setting('mix_and_match_link_color');
  $body_classes[] = theme_get_setting('mix_and_match_corners');
  $body_classes = array_filter($body_classes);   
  $vars['body_classes'] = implode(' ', $body_classes);
  $vars['page_color'] = theme_get_setting('mix_and_match_page_bg');
}

/**
 * Search form preprocessing
 */
function mix_and_match_preprocess_search_theme_form(&$vars) {
  $vars['accent_color'] = theme_get_setting('mix_and_match_accent_color');
}