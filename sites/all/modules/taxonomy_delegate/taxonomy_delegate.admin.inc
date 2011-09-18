<?php
// $Id: taxonomy_delegate.admin.inc,v 1.1.2.6 2008/06/20 16:21:24 nancyw Exp $

/**
 * @file
 * Allows delegating taxonomy maintenance.
 */

/**
 *  Form for admin settings.
 */
function taxonomy_delegate_settings_form() {
  $form = array();
  
  $hide_opts = array(
    'parent' => t('Parents'),
    'synonyms' => t('Synonyms'),
    'weight' => t('Weight'),
    'relations' => t('Related terms'),
    );

    $form['taxonomy_delegate_hides'] = array(
    '#type' => 'checkboxes',
    '#options' => $hide_opts,
    '#title' => t('Hide these fields from the administrator-delegate'),
    '#default_value' => variable_get('taxonomy_delegate_hides', array()),
    '#description' => t('The selected fields will be hidden on the term add/edit page. Users with "administer taxonomy" permission will always see them.'),
    '#prefix' => '<div class="taxonomy_delegate_checkboxes">',
    '#suffix' => '</div><div class="clear-block"></div><br/>',
    );

  $form['taxonomy_delegate_menu'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu text'),
    '#default_value' => variable_get('taxonomy_delegate_menu', t('Administer My Categories')),
    '#description' => t("This text will be shown in the user's menu. No translation will be performed."),
    );
    
  $form['#submit'][] = 'taxonomy_delegate_settings_form_submit';    
  
  return system_settings_form($form);
}

function taxonomy_delegate_settings_form_submit($form, &$form_state) {
  $old_menu = variable_get('taxonomy_delegate_menu', t('Administer My Categories'));
  $new_menu = $form_state['values']['taxonomy_delegate_menu'];
  if ($old_menu != $new_menu) {
    // Save the new value.
    variable_set('taxonomy_delegate_menu', $form_state['values']['taxonomy_delegate_menu']);
    // Invoke menu rebuild.
    menu_rebuild();
//    variable_set('menu_rebuild_needed', TRUE);
  }
}
