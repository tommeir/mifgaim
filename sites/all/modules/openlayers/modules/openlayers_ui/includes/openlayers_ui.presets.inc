<?php

/**
 * @file
 * This file holds the functions handling presets in the
 * Openlayers UI.
 *
 * @ingroup openlayers
 */

function openlayers_ui_presets_clone($preset) {
  $form_state = array();
  drupal_set_title(
    t('Clone preset %preset',
      array('%preset' => $preset->name)
    )
  );
  return drupal_get_form('openlayers_ui_presets_form', $preset);
}

/**
 * Menu Callback for Add Preset
 */
function openlayers_ui_presets_form(&$form_state, $preset = NULL, $edit = FALSE) {
  openlayers_include();
  ctools_include('dependent');
  drupal_add_js(drupal_get_path('module', 'openlayers_ui') . '/js/openlayers_ui.presets.js', 'module');
  drupal_add_css(drupal_get_path('module', 'openlayers_ui') . '/openlayers_ui.css');

  // If cloning or editing, attempt to get preset
  if (empty($preset)) {
    $default_preset = openlayers_preset_load(variable_get('openlayers_default_preset', 'default'));
    if ($default_preset) {
      $defaults = $default_preset->data;
    }
  }
  else {
    $defaults = $preset->data;
  }

  $form = array(
    '#tree' => TRUE,
    '#cache' => TRUE,
    'preset_edit' => array('#type' => 'value', '#value' => $edit),
    'default_map' => array('#type' => 'value', '#value' => $defaults),
  );

  // General information
  $form['info'] = array(
    '#title' => t('General information'),
    '#tree' => FALSE,
  );
  $form['info']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('This is the machine readable identifier.
      This should be all lowercase characters, numbers, or underscores (_).'),
    '#maxlength' => 255,
    '#default_value' => !empty($preset->name) ? $preset->name : '',
    '#disabled' => $edit,
  );
  if ($edit) {
    $form['info']['name']['#value'] = $preset->name;
  }
  $form['info']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('This is the descriptive title of the preset and will show up most often in the interface.'),
    '#maxlength' => 255,
    '#default_value' => !empty($preset->title) ? $preset->title : '',
  );
  $form['info']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Preset Description'),
    '#description' => t('This is full description of the preset and is mostly used on the preset overview list page.'),
    '#default_value' => !empty($preset->description) ? $preset->description : '',
  );
  $form['info']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('The map\'s width. "auto" will make the map fill
      the space it is given; otherwise, enter a value in pixels, like 400px.'),
    '#default_value' => !empty($defaults['width']) ? $defaults['width'] : '',
    '#maxlength' => 128,
  );
  $form['info']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('The map\'s height. Enter a value in pixels,
      like 400px.'),
    '#default_value' => !empty($defaults['height']) ? $defaults['height'] : '',
    '#maxlength' => 128,
  );
  $form['info']['image_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Path'),
    '#description' => t('The path to a directory for OpenLayers to look for UI
      graphics. If blank, default graphics are used.  Use either a Drupal path
      (and the appropriate base path will be prepended), an absolute path
      (such as !ab_example), or a full URL.',
      array('!ab_example' => '/path/to/images/')
    ),
    '#default_value' => !empty($defaults['image_path']) ?
      $defaults['image_path'] : '',
  );

  $form['info']['css_path'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS Path'),
    '#description' => t('Path for where OpenLayers should look for its theme
      CSS file. The default hosted OpenLayers URL is
      http://openlayers.org/api/theme/default/style.css.  Use either a Drupal path
      (and the appropriate base path will be prepended), an absolute path
      (such as !ab_example), or a full URL.',
      array('!ab_example' => '/path/to/css/')
    ),
    '#default_value' => !empty($defaults['css_path']) ?
      $defaults['css_path'] : '',
  );

  $form['info']['proxy_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Proxy Host'),
    '#description' => t('A proxy (typically on the same domain as this site)
      which enables requests to cross-domain AJAX resources (including remote
      KML).  Use either a Drupal path (and the appropriate base path will
      be prepended), an absolute path (such as !ab_example), or a full URL.',
      array('!ab_example' => '/cgi_proxy?request=')
    ),
    '#default_value' => !empty($defaults['proxy_host']) ?
      $defaults['proxy_host'] : '',
  );

  $form['info']['hide_empty_map'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide empty map'),
    '#description' => t("Show views empty text or hide the map if there are 
    no map overlays with features. Otherwise an empty map is displayed."),
    '#default_value' => isset($defaults['hide_empty_map']) ? $defaults['hide_empty_map'] : FALSE,  
  );
  
  // Center
  $form['center'] = array(
    '#title' => t('Center & Bounds'),
    '#description' => t('Where the map will center itself initially. Shift-drag on the map to set the Restricted Extent.'),
    '#tree' => TRUE,
  );
  $form['center']['helpmap'] = array(
    '#value' => '<div class="form-item openlayers-center-helpmap"
      style="display:block">'. openlayers_ui_presets_form_center_map($defaults) .'</div>'
  );
  $form['center']['initial'] = array(
    '#type' => 'fieldset',
    '#title' => t('Initial Map View')
  );
  $form['center']['initial']['centerpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Centerpoint'),
    '#description' => t('Coordinates that are the intial focus of the map'),
    '#default_value' => $defaults['center']['initial']['centerpoint'],
    '#attributes' => array('class' => 'openlayers-form-centerpoint'),
    '#size' => 25,
  );
  $form['center']['initial']['zoom'] = array(
    '#type' => 'textfield',
    '#title' => t('Zoom Level'),
    '#description' => t('Initial Zoom Level when the map intially displays'),
    '#default_value' => $defaults['center']['initial']['zoom'],
    '#attributes' => array('class' => 'openlayers-form-zoom'),
    '#size' => 25,
  );
  $form['center']['restrict'] = array(
    '#type' => 'fieldset',
    '#title' => t('Restrict Extent')
  );
  $form['center']['restrict']['restrictextent'] = array(
    '#type' => 'checkbox',
    '#title' => t('Restrict Extent'),
    '#description' => t('Setting the restricted extent of a map prevents users
      from panning the map outside a specified area. This can be set
      interactively by holding the shift key and dragging a box over the map
      above. Setting the extent does not restrict how far users can zoom out,
      so setting restricted zoom levels (via individual layer settings) is
      recommended.'),
    '#id' => 'restrictextent',
    '#default_value' => isset($defaults['center']['restrict']['restrictextent']) ? $defaults['center']['restrict']['restrictextent'] : '',
  );
  $form['center']['restrict']['restrictedExtent'] = array(
    '#type' => 'textfield',
    '#title' => t('Restricted Extent'),
    '#description' => t('Prevents users from panning outside of a specific bounding box'),
    '#default_value' => isset($defaults['center']['restrict']['restrictedExtent']) ? $defaults['center']['restrict']['restrictedExtent'] : '',
    '#attributes' => array('class' => 'openlayers-form-restrictedExtent'),
    '#size' => 25,
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('restrictextent' => array(1)),
  );

  // Behaviors
  $form['behaviors'] = array(
    '#title' => t('Behaviors'),
    '#description' => t('Configure interactive map behaviors.'),
    '#theme' => 'openlayers_ui_presets_form_behaviors',
    '#tree' => TRUE,
  );
  $form['behaviors'] = $form['behaviors'] + openlayers_ui_get_behavior_options('map', $defaults);

  // Layers & styles
  $form['layerstyles'] = array(
    '#title' => t('Layers & Styles'),
    '#description' => t('Layer settings.  The Layer options will change based on the projection chosen.'),
    '#theme' => 'openlayers_ui_presets_form_layers',
    '#tree' => FALSE,
  );

  // Start AHAH Wrapper
  $form['layerstyles']['ahah-start'] = array('#value' => '<div id="openlayers-layers-select">');

  // Projections

  // Grab default from submitted form values on AHAH rebuild.
  if (isset($form_state['values']['projections'])) {
    if ($form_state['values']['projections']['easy_projection'] == 'other') {
      $defaults['projection'] = $form_state['values']['projections']['projection'];
    }
    else {
      $defaults['projection'] = $form_state['values']['projections']['easy_projection'];
    }
  }

  // Projection options
  $projections = array();
  foreach (openlayers_ui_get_projection_options() as $projection) {
    $projection_layers = array_merge(openlayers_ui_get_layer_options('baselayer', $projection), openlayers_ui_get_layer_options('overlay', $projection));
    $projections[$projection] = theme('openlayers_ui_form_projection_description', $projection, array_keys($projection_layers));
  }

  // $form['layerstyles']['projections'] = $base_fieldset;
  $form['layerstyles']['projections']['#collapsible'] = FALSE;
  $form['layerstyles']['projections']['#tree'] = TRUE;

  $form['layerstyles']['projections']['#title'] = t('Projection');
  $form['layerstyles']['projections']['#description'] = t('Select the EPSG code of the !link_proj for your map.
    The list next to each projection is the layers that support this projection.', array(
    '!link_proj' => l(t('geographical projection'), 'http://en.wikipedia.org/wiki/Map_projection'),
  ));
  $form['layerstyles']['projections']['easy_projection'] = array(
    '#type' => 'radios',
    '#title' => t('Map Projection'),
    '#description' => t('The !link_proj of this map: all layers will either be requested or reprojected to this format.', array(
      '!link_proj' => l(t('geographical projection'), 'http://en.wikipedia.org/wiki/Map_projection'),
    )),
    '#default_value' => $defaults['projection'],
    '#options' => $projections,
    '#attributes' => array('class' => 'openlayers-form-easy-projection'),
    '#ahah' => array(
      'path' => 'openlayers/ahah/preset',
      'wrapper' => 'openlayers-layers-select',
      'event' => 'change',
      'method' => 'replace',
    ),
  );

  // Map options properties
  $form['layerstyles']['projections']['displayProjection'] = array(
    '#type' => 'textfield',
    '#title' => t('Display Projection'),
    '#description' => t('All interaction with the map - drawing, panning, 
      centering, and more - occurs in the display projection. The vast majority 
      of maps use 4326 (latitude/longitude) for this value.'),
    '#default_value' => !empty($defaults['displayProjection']) ? 
      $defaults['displayProjection'] : '4326',
    '#maxlength' => 6
  );

  // Construct data for theme_openlayers_ui_presets_form_layers
  $form['layerstyles']['layers']['#tree'] = TRUE;
  $form['layerstyles']['layers']['baselabels'] = array();
  $base_options = openlayers_ui_get_layer_options('baselayer', $defaults['projection']);
  if (!empty($base_options)) {
    foreach ($base_options as $id => $description) {
      $form['layerstyles']['layers']['baselabels'][$id] = array('#type' => 'markup', '#value' => $description);
    }
  }
  $form['layerstyles']['layers']['baselayers'] = array(
    '#type' => 'checkboxes',
    '#options' => $base_options,
    '#default_value' => !empty($defaults['layers']) ? array_intersect_key($defaults['layers'], $base_options) : array(),
  );
  $form['layerstyles']['layers']['default_layer'] = array(
    '#type' => 'radios',
    '#options' => $base_options,
    '#default_value' => !empty($defaults['default_layer']) && isset($base_options[$defaults['default_layer']]) ? $defaults['default_layer'] : NULL,
  );

  // Overlay layers
  $overlay_options = openlayers_ui_get_layer_options('overlay', $defaults['projection']);
  $form['layerstyles']['layers']['overlaylabels'] = array();
  if (!empty($overlay_options)) {

    // Have enabled layers maintain their order
    $available = array_keys($overlay_options);
    $enabled = array_intersect(array_keys($defaults['layers']), $available);
    //$enabled = array_reverse($enabled);
    $disabled = array_diff($available, $enabled);
    //$overlay_options_keys = array_merge($enabled, $disabled);
    $overlay_options_keys = array_merge($disabled, $enabled);

    $nextweight = -10;
    $form['layerstyles']['layer_styles']['#tree'] = TRUE;
    $form['layerstyles']['layer_weight']['#tree'] = TRUE;
    foreach ($overlay_options_keys as $id) {
      $description = $overlay_options[$id];
      $form['layerstyles']['layers']['overlaylabels'][$id] = array('#type' => 'markup', '#value' => $description);
      $form['layerstyles']['layer_styles'][$id]['#tree'] = TRUE;
      $roles = array('default', 'select', 'temporary');
      foreach ($roles as $role) {
        $form['layerstyles']['layer_styles'][$id][$role] = array(
          '#type' => 'select',
          '#title' => $role,
          '#options' => array('-- '. t('Default style') .' --') + openlayers_ui_get_style_options(),
          // UPGRADE NOTE:
          // Presets up to 6.x-2.x-alpha10 always had a single style
          // per layer specified. Newer ones have them splitted by role
          '#default_value' => is_array($defaults['layer_styles'][$id]) ?
            ( !empty($defaults['layer_styles'][$id][$role]) ?
              $defaults['layer_styles'][$id][$role] : '' )
            : 
            ( !empty($defaults['layer_styles'][$id]) ?
              $defaults['layer_styles'][$id] : '' )
        );
      }
      $form['layerstyles']['layer_weight'][$id] = array(
        '#type' => 'weight',
        '#default_value' => $nextweight++,
        '#attributes' => array(
          'class' => 'layer-weight'
        )
      );
    }
  }
  $form['layerstyles']['layers']['overlays'] = array(
    '#type' => 'checkboxes',
    '#options' => $overlay_options,
    '#default_value' => !empty($defaults['layers']) ?
      array_intersect_key($defaults['layers'], $overlay_options) : array(),
  );
  $form['layerstyles']['layer_activated'] = array(
    '#type' => 'checkboxes',
    '#options' => $overlay_options,
    '#default_value' => !empty($defaults['layer_activated']) ?
      array_intersect_key($defaults['layer_activated'], $overlay_options) : array(),
  );
  $form['layerstyles']['layer_switcher'] = array(
    '#type' => 'checkboxes',
    '#options' => $overlay_options,
    '#default_value' => !empty($defaults['layer_switcher']) ?
      array_intersect_key($defaults['layer_switcher'], $overlay_options) : array(),
  );

  // Styles
  $form['layerstyles']['styles'] = array('#tree' => TRUE);
  $form['layerstyles']['styles']['default'] = array(
    '#type' => 'select',
    '#title' => t('Default style'),
    '#description' => t('Default style for features in a vector.'),
    '#options' => openlayers_ui_get_style_options(),
    '#default_value' => !empty($defaults['styles']['default']) ?
      $defaults['styles']['default'] : NULL,
  );
  $form['layerstyles']['styles']['select'] = array(
    '#type' => 'select',
    '#title' => t('Select style'),
    '#description' => t('Style for features in a vector that are selected.'),
    '#options' => openlayers_ui_get_style_options(),
    '#default_value' => !empty($defaults['styles']['select']) ?
      $defaults['styles']['select'] : NULL,
  );
  $form['layerstyles']['styles']['temporary'] = array(
    '#type' => 'select',
    '#title' => t('Temporary Style'),
    '#description' => t('Default style for any temporary features in a vector.
      This will also be used for rollovers for things like Tooltips.'),
    '#options' => openlayers_ui_get_style_options(),
    '#default_value' => !empty($defaults['styles']['temporary']) ?
      $defaults['styles']['temporary'] : NULL,
  );

  // End AHAH Wrapper
  $form['layerstyles']['ahah-end'] = array('#value' => '</div>');

  $form['buttons'] = array('#tree' => FALSE);
  $form['buttons']['openlayers_save'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );
  $form['buttons']['openlayers_cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel')
  );

  return $form;
}

/**
 * OpenLayers Preset Form Validate
 *
 * Validates a preset form submission.
 *
 * @param $map_form
 *   Array of values to validate
 * @return
 *   Does not return anything. Uses form_set_error() to communicate errors.
 */
function openlayers_ui_presets_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $found_error = FALSE;

  // Check if ahah submitting
  if ($form_state['clicked_button']['#id'] == 'edit-openlayers-projection-ahah') {
    return TRUE;
  }

  // Check for cancel
  if ($form_state['clicked_button']['#id'] == 'edit-openlayers-cancel') {
    return TRUE;
  }

  // Check for values.  We manually do required fields because, it would otherwise
  // mess with the AHAH stuff.  Maybe a way around it.
  if (empty($values['name'])) {
    form_set_error('name', t('Preset name is required.'));
    $found_error = TRUE;
  }
  if (empty($values['title'])) {
    form_set_error('title', t('Preset title is required.'));
    $found_error = TRUE;
  }
  if (empty($values['description'])) {
    form_set_error('description', t('Preset description is required.'));
    $found_error = TRUE;
  }
  if (empty($values['width'])) {
    form_set_error('width', t('Width is required.'));
    $found_error = TRUE;
  }
  if (empty($values['height'])) {
    form_set_error('height', t('Height is required.'));
    $found_error = TRUE;
  }

  // Check preset name first
  if (!preg_match('!^[a-z0-9_]+$!', $values['name'])) {
    form_set_error('name', t('Preset Name must contain only lowercase letters, numbers, and underscores.'));
    $found_error = TRUE;
  }

  // Check if adding and name already exists
  $existing = openlayers_preset_load($values['name'], TRUE);
  if (!empty($existing) && (!$form_state['values']['preset_edit']) && ($existing->export_type & EXPORT_IN_DATABASE)) {
    form_set_error('name', t('The Preset Name already exists.'));
    $found_error = TRUE;
  }

  // Attempt to render map to find any errors
  $map = openlayers_ui_presets_form_process($form_state['values']);
  $map = openlayers_build_map($map);
  // Check if any errors found
  if (is_array($map['errors']) && count($map['errors']) > 0) {
    foreach ($map['errors'] as $error) {
      form_set_error('openlayers',
        t('OpenLayers Map Rendering Error: !error', array('!error' => $error)));
      $found_error = TRUE;
    }
  }

  // If found error, rebuild form
  if ($found_error) {
    // Add OpenLayers CSS & Javascript, because the form function will not be run on rebuild
    openlayers_include();
    drupal_add_js(drupal_get_path('module', 'openlayers_ui') . '/js/openlayers_ui.presets.js', 'module');
    drupal_add_css(drupal_get_path('module', 'openlayers_ui') . '/openlayers_ui.css');
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Form submit for preset add form, for the projection add ahah
 */
function openlayers_ui_presets_add_projection_submit($form, &$form_state) {
  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $form_state['rebuild'] = TRUE;
}

/**
 * Form submit for preset add form
 */
function openlayers_ui_presets_form_submit($form, &$form_state) {
  // Check for cancel
  if ($form_state['clicked_button']['#id'] == 'edit-openlayers-cancel') {
    $form_state['redirect'] = 'admin/build/openlayers/presets/list';
  }

  // Only save if save button is pressed
  if ($form_state['clicked_button']['#id'] == 'edit-openlayers-save') {
    $map = openlayers_ui_presets_form_process($form_state['values']);

    // Save preset
    $preset = new stdClass();
    $preset->name = $form_state['values']['name'];
    $preset->title = $form_state['values']['title'];
    $preset->description = $form_state['values']['description'];
    $preset->data = $map;
    $success = openlayers_preset_save($preset);

    // Redirect to edit page
    if ($success) {
      drupal_set_message(t('Map saved.'));
      $form_state['redirect'] = "admin/build/openlayers/presets/{$preset->name}/edit";
    }
    else {
      form_set_error('openlayers', t('Error trying to save map'));
    }
  }
}

/**
 * OpenLayers AHAH
 *
 * Function to handle the AHAH request of the openlayers form
 */
function openlayers_ui_preset_ahah() {
  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);

  // Get variables
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form_state['post'] = $form['#post'] = $_POST;
  $form['#programmed'] = $form['#redirect'] = FALSE;

  // Reprocess form with new form state
  drupal_process_form($form_id, $form, $form_state);

  // Rebuild form and remove any submit handlers
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);

  // Unset wrapper and create output
  $output = theme('status_messages') . drupal_render($form['layerstyles']);

  // Final rendering callback.
  drupal_json(array('status' => TRUE, 'data' => $output));
}

/**
 * Process form submission into a map array.
 *
 * @param $values
 *   Array of values to process.
 * @return
 *   Map data array.
 */
function openlayers_ui_presets_form_process($values = array()) {
  // Valid keys that we will cherry-pick from the form values.
  $keys = array(
    'width',
    'height',
    'image_path',
    'css_path',
    'proxy_host',
    'hide_empty_map',
    'center',
    'behaviors',
    'layers',
    'layer_styles',
    'layer_activated',
    'layer_switcher',
    'projections',
    'styles'
  );

  // TODO: eliminate this process, too much disconnect between
  // forms and data
  $processed = array();
  foreach ($keys as $key) {
    switch ($key) {
      case 'behaviors':
        $processed['behaviors'] = array();
        foreach ($values['behaviors'] as $behavior => $settings) {
          if ($settings['enabled']) {
            $processed['behaviors'][$behavior] = isset($settings['options_set']['options']) ?
              $settings['options_set']['options'] : array();
          }
        }
        break;

      case 'projections':
        $processed['projection'] = $values['projections']['easy_projection'];
        $processed['displayProjection'] = $values['projections']['displayProjection'];
        break;

      case 'layers':
        // Put the default layer in the right place.
        // TODO: finish port here
        $processed['default_layer'] = $values['layers']['default_layer'];

        $baselayers = array_filter($values['layers']['baselayers']);

        // Sort overlay layers by weight, in case browser didn't
        // send them already sorted
        $overlays = array();
        if ( ! empty($values['layers']['overlays']) ) {
          asort($values['layer_weight']);
          foreach (array_keys($values['layer_weight']) as $key) {
            if ( $values['layers']['overlays'][$key] ) {
               $overlays[$key] = $key;
            }
          }
        }

        // Merge our different layer sections together
        $processed['layers'] = array_merge($baselayers, $overlays);

        break;

      case 'layer_styles':
        $processed['layer_styles'] = array();
        if ( ! empty($values['layer_styles']) ) {
          foreach ($values['layer_styles'] as $layer => $stylespec) {
            $layerarray = array_filter($stylespec);
            if ( ! empty($layerarray) ) {
              $processed['layer_styles'][$layer] = $layerarray;
            }
          }
        }
        break;

      default:
        $processed[$key] = is_array($values[$key]) ? array_filter($values[$key]) : $values[$key];
        break;

    }
  }

  // Ensure these values are arrays
  $ensure_array = array('behaviors', 'layers', 'layer_styles', 'styles');
  foreach ($ensure_array as $key) {
    if (empty($processed[$key])) {
      $processed[$key] = array();
    }
  }

  return $processed;
}

/**
 * Get behavior options.
 */
function openlayers_ui_get_behavior_options($type, $defaults) {
  $form = array();

  foreach (openlayers_behaviors() as $key => $plugin) {
    // Get behavior class
    $class = ctools_plugin_get_class($plugin, 'behavior');
    if (!empty($class)) {
      $options = isset($defaults['behaviors'][$key]) ?
        $defaults['behaviors'][$key] : array();

      $behavior = new $class($options, $defaults);

      if (!isset($plugin['ui_visibility']) || $plugin['ui_visibility']) {
        // Create basic form structure for behavior
        $form[$key] = array(
          'enabled' => array(
            '#type' => 'checkbox',
            '#title' => $plugin['title'],
            '#description' => $plugin['description'],
            '#default_value' => isset($defaults['behaviors'][$key]),
            '#id' => $key . '-enabled',
          ),
          'dependencies' => openlayers_dependency_widget(
            $behavior->js_dependency()),
          'options' => array(),
        );

        // Create options items
        $options = $behavior->options_form($options);
        if (!empty($options)) {
          // HACK.  In order to use ctools form dependencies, we have to use a hidden
          // field as it supports processing and IDs.
          $form[$key]['options_set'][$key . '-prefix'] = array(
            '#type' => 'hidden',
            '#id' => $key . '-options',
            '#prefix' => '<div><fieldset id="' . $key . '-options' .
              '" class="collapsible">',
            '#process' => array('ctools_dependent_process'),
            '#dependency' => array($key . '-enabled' => array(1)),
            '#weight' => -1000,
          );
          $form[$key]['options_set']['options'] = $options;
          $form[$key]['options_set'][$key . '-suffix'] = array(
            '#value' => '</fieldset></div>',
            '#weight' => 1000,
          );
        }
      }
    }
  }
  return $form;
}

/**
 * Create Centering Map
 *
 * Create map for interactive default centering
 *
 * @param $defaults
 *   Array of defults to use for the map of centering and zooming
 * @return
 *   Themed map array
 */
function openlayers_ui_presets_form_center_map($defaults = array()) {
  // Pass variables etc. to javascript
  $pass_values = array(
    'openlayersForm' => array(
      'projectionLayers' => openlayers_ui_get_projection_options(),
    ),
  );
  drupal_add_js($pass_values, 'setting');

  // centerpoint & zoom of this preset are overridden
  // by the mapformvalues behavior on page load
  $centermap_def = array(
    'id' => 'openlayers-center-helpmap',
    'projection' => '900913',
    'displayProjection' => '900913',
    'default_layer' => 'osm_mapnik',
    'width' => '500px',
    'height' => '400px',
    'center' => array(
      'initial' => array(
        'centerpoint' => "0, 0",
        'zoom' => 2,
      ),
    ),
    'layers' => array(
      'osm_mapnik',
    ),
    'behaviors' => array(
      'openlayers_behavior_panzoombar' => array(),
      'openlayers_behavior_dragpan' => array(),
      'openlayers_behavior_mapformvalues' => array(),
      'openlayers_behavior_boxselect' => array(),
    ),
  );
  return openlayers_render_map($centermap_def);
}
