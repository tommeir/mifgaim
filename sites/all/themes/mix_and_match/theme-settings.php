<?php
// $Id: theme-settings.php,v 1.1 2010/10/26 23:22:22 aross Exp $ 

// Include the definition of phptemplate_settings().
include_once './' . drupal_get_path('theme', 'fusion_core') . '/theme-settings.php';


/**
 * Implementation of THEMEHOOK_settings() function.
 *
 * @param $saved_settings
 *   An array of saved settings for this theme.
 * @return
 *   A form array.
 */
function mix_and_match_settings($saved_settings) {

  /*
   * Create the form using Forms API: http://api.drupal.org/api/6
   */
  $form = array();

  $form['mix_and_match_colors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mix and Match custom styles'),
    '#description' => t('Select the base colors for your site.  For the full range of site customization options,
      install the <a href="http://drupal.org/project/skinr">Skinr module</a> and set colors for blocks, nodes, and comments on the block and
      content type configuration pages.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['mix_and_match_colors']['mix_and_match_body_bg'] = array(
    '#type'          => 'select',
    '#title'         => t('Body background color'),
    '#options'       => array(
      'white' => t('White (default)'),
      'gray' => t('Gray'),
      'black' => t('Black'),
      'blue' => t('Blue'),
      'red' => t('Red'),
      'green' => t('Green'),
     ),
    '#default_value' => $saved_settings['mix_and_match_body_bg'],
  );

  $form['mix_and_match_colors']['mix_and_match_page_bg'] = array(
    '#type'          => 'select',
    '#title'         => t('Content area background'),
    '#options'       => array(
      'no-page-bg' => t('Use body background for content area'),
      'white-page-bg' => t('Add a white background to content area'),
     ),
    '#default_value' => $saved_settings['mix_and_match_page_bg'],
  );

  $form['mix_and_match_colors']['mix_and_match_accent_color'] = array(
    '#type'          => 'select',
    '#title'         => t('Navigation bar and submit button background color'),
    '#options'       => array(
      'gray-accents' => t('Gray (default)'),
      'black-accents' => t('Black'),
      'blue-accents' => t('Blue'),
      'red-accents' => t('Red'),
      'orange-accents' => t('Orange'),
      'green-accents' => t('Green'),
     ),
    '#default_value' => $saved_settings['mix_and_match_accent_color'],
  );

  $form['mix_and_match_colors']['mix_and_match_footer_color'] = array(
    '#type'          => 'select',
    '#title'         => t('Footer region background color'),
    '#options'       => array(
      'default-footer' => t('None'),
      'lt-gray-footer' => t('Light Gray'),
      'gray-footer' => t('Medium Gray'),
      'dk-gray-footer' => t('Dark Gray'),
      'black-footer' => t('Black'),
      'red-footer' => t('Red'),
      'lt-blue-footer' => t('Light Blue'),
      'blue-footer' => t('Blue'),
      'lt-green-footer' => t('Light Green'),
      'green-footer' => t('Green'),
      'lt-orange-footer' => t('Light Orange'),
      'orange-footer' => t('Orange'),
     ),
    '#default_value' => $saved_settings['mix_and_match_footer_color'],
  );

  $form['mix_and_match_colors']['mix_and_match_header_color'] = array(
    '#type'          => 'select',
    '#title'         => t('Page title and block header text color'),
    '#options'       => array(
      'default-headers' => t('None - use theme default'),
      'white-headers' => t('White'),
      'gray-headers' => t('Gray'),
      'black-headers' => t('Black'),
      'blue-headers' => t('Blue'),
      'red-headers' => t('Red'),
      'orange-headers' => t('Orange'),
      'green-headers' => t('Green'),
     ),
    '#default_value' => $saved_settings['mix_and_match_header_color'],
  );

  $form['mix_and_match_colors']['mix_and_match_link_color'] = array(
    '#type'          => 'select',
    '#title'         => t('Link text color'),
    '#options'       => array(
      'default-links' => t('None - use theme default'),
      'gray-links' => t('Gray'),
      'blue-links' => t('Blue'),
      'red-links' => t('Red'),
      'orange-links' => t('Orange'),
      'green-links' => t('Green'),
     ),
    '#default_value' => $saved_settings['mix_and_match_link_color'],
  );

  $form['mix_and_match_colors']['mix_and_match_corners'] = array(
    '#type'          => 'select',
    '#title'         => t('Rounded corners'),
    '#options'       => array(
      'default-corners' => t('None'),
      'round-corners-3' => t('3px radius corners'),
      'round-corners-7' => t('7px radius corners'),
      'round-corners-11' => t('11px radius corners'),
    ),
    '#description' => t('This will add rounded corners to blocks and some other
      elements.  Corners are CSS3-based in compliant browsers. To also display
      round corners in IE with CSS3 PIE <a href="http://css3pie.com/">(more info)
      </a>, follow instructions in README.txt'),
    '#default_value' => $saved_settings['mix_and_match_corners'],
  );

  // Add the base theme's settings.
  $form += phptemplate_settings($saved_settings);

  // Remove some of the base theme's settings.
  // unset($form['themedev']['zen_layout']); // We don't need to select the base stylesheet.

  // Return the form
  return $form;
}