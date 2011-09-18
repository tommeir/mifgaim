<?php
// $Id: addthis.admin.inc,v 1.1.2.10.2.7 2011/01/31 03:39:07 lesmana Exp $

/**
 * @file
 * Configuration settings for AddThis.
 */

/**
 * Return a form object for default AddThis settings.
 */
function addthis_config_form(&$form_state) {
  // Load the external AddThis CSS file for displaying service icons.
  // TODO: Optionally cache the CSS file and icons sprite locally
  drupal_set_html_head('<link rel="stylesheet" href="http://cache.addthiscdn.com/icons/v1/sprites/services.css" type="text/css" title="default"/>');
  drupal_add_css(drupal_get_path('module', 'addthis') .'/addthis.admin.css');

  // Drupal visibility settings for AddThis.
  $form['addthis_visibility'] = array(
    '#type' => 'fieldset',
    '#title' => t('AddThis visibility settings'),
    '#collapsible' => TRUE,
    '#description' => t('AddThis can also be displayed as a block, which you can configure on the <a href="!url">block administration page.</a>', array('!url' => url('admin/build/block'))),
  );
  $form['addthis_visibility']['addthis_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enable AddThis for these node types'),
    '#description' => t('Select the node types on which to display an AddThis button. You must also set at least one of the visibility options below.'),
    '#options' => node_get_types('names'),
    '#default_value' => variable_get('addthis_node_types', array()),
  );
  $form['addthis_visibility']['addthis_display_in_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display in full nodes'),
    '#default_value' => variable_get('addthis_display_in_links', '0'),
    '#description' => t('Display the widget in the links section of a node when viewing the full node.'),
  );
  $form['addthis_visibility']['addthis_display_in_teasers'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display in node teasers'),
    '#default_value' => variable_get('addthis_display_in_teasers', '0'),
    '#description' => t('Display the widget in the links section of a node when viewing it as a teaser.'),
  );

  // Select widget type.
  $form['addthis_widget_select'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Widget type'),
  );
  $form['addthis_widget_select']['addthis_widget_type'] = array(
    '#type' => 'radios',
    '#title' => t('AddThis widget type'),
    '#description' => t('See !url for a demonstration of the widget types.', array('!url' => l('http://www.addthis.com/web-button-select', 'http://www.addthis.com/web-button-select'))),
    '#options' => array(
      'addthis_button' => t('Button'),
      'addthis_toolbox' => t('Toolbox'),
    ),
    '#default_value' => variable_get('addthis_widget_type', 'addthis_button'),
  );
  
  // Settings for the addthis_config javascript object.
  // See http://addthis.com/help/menu-api#configuration-ui for details.
  $addthis_config = variable_get('addthis_config', array('ui_use_css' => TRUE));
  $form['addthis_config'] = array(
    '#type'  => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('AddThis global configuration settings'),
    '#tree'  => TRUE,
  );

  $form['addthis_config']['services_help'] = array(
    '#type' => 'fieldset',
    '#title' => t('List of basic service codes'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['addthis_config']['services_help']['services_list'] = array(
    '#type' => 'markup',
    '#value' => _addthis_services_list(),
  );

  $form['addthis_config']['services_toolbox'] = array(
    '#type' => 'textarea',
    '#title' => t('Toolbox services'),
    '#description' => t('A comma-separated list of service codes that determines which services will be shown in the toolbox.<br />In addition to the basic service codes, the toolbox also takes these special or custom codes:') . '<div><strong>' . implode(', ', array_keys(addthis_get_custom_services())) . '</strong></div>',
    '#rows' => 2,
    '#default_value' => $addthis_config['services_toolbox'],
    '#wysiwyg' => FALSE,
  );
  $form['addthis_config']['services_compact'] = array(
    '#type' => 'textarea',
    '#title' => t('Compact menu services'),
    '#description' => t('The compact menu is the shorter list of services that appear when you hover over a button widget or an expandable toolbox item. Enter service codes as a comma-separated list. Leave this blank to use a default list of service codes.'),
    '#rows' => 2,
    '#default_value' => $addthis_config['services_compact'],
    '#wysiwyg' => FALSE,
  );
  $form['addthis_config']['services_expanded'] = array(
    '#type' => 'textarea',
    '#title' => t('Expanded services'),
    '#description' => t('A comma-separated list of service codes that determines which services will be shown in the expanded menu, if enabled. Leave blank to make all services available.'),
    '#rows' => 2,
    '#default_value' => $addthis_config['services_expanded'],
    '#wysiwyg' => FALSE,
  );
  $form['addthis_config']['header_display'] = array(
    '#type' => 'markup',
    '#value' => '<h3>' . t('Display Settings') . '</h3>',
  );
  $form['addthis_config']['addthis_toolbox_classes'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom classes for Toolbox container'),
    '#description' => t('Some AddThis Toolbox features require a class to be added to the Toolbox, which you can set here. For example, to use the large icon set, enter <strong>addthis_32x32_style</strong> here.'),
    '#default_value' => variable_get('addthis_toolbox_classes', ''),
    '#tree' => FALSE,
  );
  $form['addthis_config']['ui_cobrand'] = array(
    '#type' => 'textfield',
    '#title' => t('Branding'),
    '#description' => t('Custom text displayed in the top-right of the AddThis menu. This should probably be less than 15 characters to fit.'),
    '#default_value' => $addthis_config['ui_cobrand'],
  );
  $form['addthis_config']['ui_header_color'] = array(
    '#type' => (module_exists('colorpicker') ? 'colorpicker_' : '') . 'textfield',
    '#title' => t('Header color'),
    '#description' => t('The color to use for the text in the header of the expanded menu, in hex.'),
    '#default_value' => ($addthis_config['ui_header_color'] ? $addthis_config['ui_header_color'] : '#000000'),
  );
  $form['addthis_config']['ui_header_background'] = array(
    '#type' => (module_exists('colorpicker') ? 'colorpicker_' : '') . 'textfield',
    '#title' => t('Header background'),
    '#description' => t('The color to use as a background in the header of the expanded menu, in hex.'),
    '#default_value' => ($addthis_config['ui_header_background'] ? $addthis_config['ui_header_background'] : '#FFFFFF'),
  );
  $form['addthis_config']['header_behavior'] = array(
    '#type' => 'markup',
    '#value' => '<h3>' . t('Behavior Settings') . '</h3>',
  );
  $form['addthis_config']['ui_click'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable opening AddThis on mouse hover'),
    '#description' => t('If checked, the AddThis compact menu will only open if the AddThis button is clicked.  Otherwise, the compact menu will open when the mouse hovers over it.'),
    '#return_value' => TRUE,
    '#default_value' => $addthis_config['ui_click'],
  );
  $form['addthis_config']['ui_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Hover delay'),
    '#size' => 5,
    '#element_validate' => array('_addthis_validate_ui_delay'),
    '#description' => t('Delay, in milliseconds, before the compact menu appears when hovering over a button. Capped at 500 ms.'),
    '#default_value' => $addthis_config['ui_delay'],
  );
  $form['addthis_config']['ui_use_addressbook'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow AddThis to import contacts for email sharing'),
    '#description' => t('If checked, the user will be able import their contacts from popular webmail services when using email sharing.'),
    '#return_value' => TRUE,
    '#default_value' => $addthis_config['ui_use_addressbook'],
  );
  $form['addthis_config']['header_tracking'] = array(
    '#type' => 'markup',
    '#value' => '<h3>' . t('Tracking Settings') . '</h3>',
  );
  $form['addthis_config']['data_track_clickback'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow AddThis to track incoming traffic from shares'),
    '#description' => t('If checked, AddThis will collect data on how many people come back to your content via links shared with AddThis.'),
    '#return_value' => TRUE,
    '#default_value' => $addthis_config['data_track_clickback'],
  );
  $form['addthis_config']['username'] = array(
    '#type' => 'textfield',
    '#title' => t('AddThis username'),
    '#description' => t('If you are using AddThis Analytics, enter the username of the AddThis account where you are collecting AddThis click data.'),
    '#default_value' => $addthis_config['username'],
  );
  $form['addthis_config']['data_ga_tracker'] = array(
    '#type' => 'checkbox',
    '#title' => t('Integrate share data with Google Analytics'),
    '#description' => t('If checked, AddThis will send click data to your Google Analytics profile. You must have a Google Analytics tracking script installed on your site.'),
    '#return_value' => 'pageTracker',
    '#default_value' => $addthis_config['data_ga_tracker'],
  );
  $form['addthis_config']['header_advanced'] = array(
    '#type' => 'markup',
    '#value' => '<h3>' . t('Advanced Settings') . '</h3>',
  );
  $form['addthis_config']['ui_use_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use default style provided by AddThis'),
    '#description' => t('Disable this if you prefer to apply entirely custom CSS to the widget from within your theme.'),
    '#return_value' => TRUE,
    '#default_value' => $addthis_config['ui_use_css'],
  );
  $form['addthis_config']['addthis_cache_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cache JavaScript file locally'),
    '#description' => t('If checked, the JavaScript file will be retrieved from AddThis and cached locally. The file will be regenerated daily to receive updates to the code. Not compatible with private file systems.'),
    '#return_value' => TRUE,
    '#default_value' => variable_get('addthis_cache_js', 0),
    '#tree' => FALSE,
  );

  /**
   * Setting 'ui_508_compliant' doesn't seem to do anything. An API bug?
   *
  $form['addthis_config']['ui_508_compliant'] = array(
    '#type' => 'checkbox',
    '#title' => t('Section 508 compliance mode'),
    '#description' => t('If checked, clicking the AddThis button will open a new window to a page supporting sharing without JavaScript.'),
    '#return_value' => TRUE,
    '#default_value' => $addthis_config['ui_508_compliant'],
  );
  // */
  return system_settings_form($form);
}

/**
 * Returns an array of AddThis service codes and names.
 */
function _addthis_services_list() {
  // json_decode() didn't exist before PHP 5.2.
  if (!function_exists('json_decode')) {
    return _addthis_services_retrieval_error();
  }

  // Retrieve the services table from the cache. If the cache is not set, get
  // the list of services from AddThis and cache the results.
  // See http://www.addthis.com/help/services-api
  if (!$services_cache = cache_get('addthis:services')) {
    // Generate the service codes output and cache it.
    $location = "http://cache.addthiscdn.com/services/v1/sharing.en.json";
    $result = drupal_http_request($location);
    if ($result->code == 200) {
      $data = json_decode($result->data, TRUE);
      $rows = array();
      foreach ($data['data'] AS $service) {
        $rows[] = array(
          0 => check_plain($service['code']),
          1 => '<span class="addthis_service_icon icon_' . check_plain($service['code']) . '"></span> ' . check_plain($service['name'])
        );
      }
      $services = l(t('Refresh this list.'), 'admin/settings/addthis/flush-services', array('query' => drupal_get_destination()));
      $header = array(t('Service code'), t('Service'));
      $services .= theme('table', $header, $rows);
      cache_set('addthis:services', $services);
    }
    else {
      // Couldn't retrieve the services list.
      $services = _addthis_services_retrieval_error();
    }
  }
  else {
    // Use the cached services output.
    $services = $services_cache->data;
  }
  
  return $services;
}

function _addthis_services_retrieval_error() {
    $services_page = 'http://www.addthis.com/services/all';
    $output = '<p>' . t('The services list could not be retrieved. You may view the full list of service codes on the AddThis website.') . '</p>';
    $output .= '<div>' . l($services_page, $services_page) . '</div>';
    return $output;
}

/**
 * Flush the services cache.
 */
function addthis_flush_services() {
  cache_clear_all('addthis:services', 'cache');
  if (_addthis_services_list()) {
    drupal_set_message(t('The list of AddThis services has been refreshed.'));
  }
  drupal_goto();
}

/**
 * Form builder function for the Templates administration form.
 */
function addthis_templates_form(&$form_state) {
  $form = array();
  $form['description'] = array(
    '#type' => 'markup',
    '#value' => t('<p><strong>AddThis templates</strong> allow you to define the default text to be posted when a user shares your content to a third-party service.</p>') .
                t('For more information, visit !url.</p><p>Only service code "twitter" is currently supported by the AddThis API.</p>', array('!url' => l('http://www.addthis.com/help/client-api#configuration-sharing-templates', 'http://www.addthis.com/help/client-api#configuration-sharing-templates'))),
  );
  $form['addthis_templates_twitter'] = array(
    '#type' => 'textfield',
    '#title' => 'Twitter (twitter)',
    '#default_value' => variable_get('addthis_templates_twitter', '{{title}} {{url}} via @AddThis'),
  );
  return system_settings_form($form);
}

/**
 * Form builder function for the Service Customization form.
 */
function addthis_customize_form(&$form_state) {
  $form = array();
  $form['description'] = array(
    '#type' => 'markup',
    '#value' => t('<p>Use this form to define customized Toolbox items. You may override existing services codes or define new service codes.</p>') .
                t('<p>For more information, visit !url.</p>', array('!url' => l('http://www.addthis.com/help/client-api#rendering-decoration', 'http://www.addthis.com/help/client-api#rendering-decoration'))),
  );
  $definitions = variable_get('addthis_service_customizations', array());
  $form['addthis_service_customizations'] = array(
    '#tree' => TRUE,
  );
  foreach ($definitions AS $def) {
    $form['addthis_service_customizations'][] = _addthis_customize_form_row($def);
  }
  
  // Add a form row for entering a new definition.
  $form['addthis_service_customizations'][] = _addthis_customize_form_row();

  return system_settings_form($form);
}

/**
 * Helper function for addthis_customize_form.
 *
 * Create the form elements for a service customization.
 *
 * @param array $def
 *   An array defining a service customization.
 *
 * @return array
 *   The branch of the form for this definition.
 */
function _addthis_customize_form_row($def = array()) {
  $branch = array(
    '#type' => 'fieldset',
    '#title' => (empty($def['code'])) ? t('Add a service customization') : check_plain($def['code']),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $branch['code'] = array(
    '#type' => 'textfield',
    '#title' => t('Service code'),
    '#description' => t('The service code to override or a new code you are defining.'),
    '#default_value' => $def['code'],
  );
  $branch['class'] = array(
    '#type' => 'textfield',
    '#title' => t('HTML classes'),
    '#description' => t('Enter HTML classes to be used on this toolbox item. Example: <strong>addthis_button_tweet</strong> <br />You <strong>must</strong> provide this even if you are overriding an already existing code.'),
    '#default_value' => $def['class'],
  );
  $branch['attributes'] = array(
    '#type' => 'textfield',
    '#title' => t('HTML attributes'),
    '#description' => t('Custom attributes to insert into the toolbox item element. Example: <strong>tw:via="mytwittername"</strong>'),
    '#default_value' => $def['attributes'],
  );
  $branch['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('Text or markup to be rendered inside the item element. Most of the time, this should be left blank.'),
    '#default_value' => $def['title'],
  );

  return $branch;
}

/**
 * Validation function for addthis_customize_form().
 *
 * Do not save definitions if they do not specify a code.
 */
function addthis_customize_form_validate($form, &$form_state) {
  foreach ($form_state['values']['addthis_service_customizations'] AS $delta => $def) {
    if (empty($def['code'])) {
      unset($form_state['values']['addthis_service_customizations'][$delta]);
    }
  }
}



/**
 * Checks whether ui_delay stays is a number between 0 and 500.
 */
function _addthis_validate_ui_delay($element, &$form_state) {
  $value = $element['#value'];
  if ($value && (!is_numeric($value) || $value < 0 || $value > 500)) {
    form_error($element, t('%name must be a number between 0 and 500.', array('%name' => $element['#title'])));
  }
}
