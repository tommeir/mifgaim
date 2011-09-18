<?php
// $Id: theme-settings.php,v 1.4 2009/05/12 04:39:09 kong Exp $

function phptemplate_settings($saved_settings) {

  $settings = theme_get_settings('beach');

/**
 * The default values for the theme variables. Make sure $defaults exactly
 * matches the $defaults in the template.php file.
 */
  $defaults = array(
    'container_class'     => 'medium',
    'iepngfix'       => 1,
    'custom'         => 0,
    'breadcrumb'     => 0,
    'totop'          => 0,
  );

  // Merge the saved variables and their default values
  $settings = array_merge($defaults, $saved_settings);

  // Create theme settings form widgets using Forms API
  // Theme Settings Fieldset
  $form['container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Beach Theme Settings'),
    '#description' => t('Use these settings to change what and how information is displayed in this theme.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['container']['container_class'] = array(
    '#type' => 'radios',
    '#title' => t('Container Width'),
    '#description'   => t('Select the container width you need. <strong>Be careful</strong>, the Narrow and Medium Width may not suite 2 sidebars page.'),
    '#default_value' => $settings['container_class'],
    '#options' => array(
      'narrow' => t('Narrow (Fixed width: 780px)'),
      'medium' => t('Medium (Fixed width: 840px)'),
      'wide' => t('Wide (Fixed width: 960px)'),
      'super-wide' => t('Super Wide (Fixed width: 1020px)'),
      'extreme-wide' => t('Extreme Wide (Fixed width: 1140px)'),
      'fluid' => t('Fluid (min-width: 780px)'),
    ),
  );

  $form['container']['features'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other Features'),
	'#description'   => t('Check / Uncheck each themes features you want to activate or deactivate for your site.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['container']['features']['iepngfix'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use <strong>IE Transparent PNG Fix</strong>'),
    '#default_value' => $settings['iepngfix'],
  );
  
  $css_path = drupal_get_path('theme', 'beach') . '/css';
  $custom_css = file_exists($css_path . '/custom.css');

  $form['container']['features']['custom'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add <strong>Customized Stylesheet (custom.css)</strong>'),
    '#default_value' => $settings['custom'],
    '#description' => $custom_css ? '' : 'To enable this option, please see ' . $css_path . '/custom-sample.css',
    '#disabled' => $custom_css ? FALSE : TRUE,
  );

  $form['container']['features']['breadcrumb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show <strong>Breadcrumbs</strong>'),
    '#default_value' => $settings['breadcrumb'],
  );

  $form['container']['features']['totop'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show <strong>Back to Top link</strong> (the link will appear at footer)'),
    '#default_value' => $settings['totop'],
  );

  return $form;
}