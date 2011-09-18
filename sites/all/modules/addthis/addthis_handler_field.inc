<?php
// $Id: addthis_handler_field.inc,v 1.1.2.2 2010/12/13 07:26:29 lesmana Exp $

/**
 * @file
 * A handler to provide an AddThis button for a particular node.
 *
 * @ingroup views_field_handlers
 */
class addthis_handler_field_button extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = 'nid';
    $this->additional_fields['title'] = 'title';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function option_definition() {
    $options = parent::option_definition();

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Alter options form defaults.
    unset($form['alter']);
    unset($form['empty']);
    unset($form['empty_zero']);
    unset($form['hide_empty']);
  }

  function render($values) {
    $nid = $values->{$this->aliases['nid']};
    $title = $values->{$this->aliases['title']};
    return theme('addthis_button', NULL, array(
      'url' => url('node/' . $nid, array('absolute' => TRUE)),
      'title' => check_plain($title),
    ));
  }
}

/**
 * A handler to provide an AddThis toolbox for a particular node.
 *
 * @ingroup views_field_handlers
 */
class addthis_handler_field_toolbox extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = 'nid';
    $this->additional_fields['title'] = 'title';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function option_definition() {
    $options = parent::option_definition();

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Allow for custom toolbox services.
    $form['services'] = array(
      '#type' => 'textarea',
      '#rows' => 3,
      '#title' => t('Toolbox services'),
      '#description' => t('A comma-separated list of AddThis service codes to use in the Toolbox field. Leave blank to use your configured defaults.'),
      '#default_value' => $this->options['services'],
    );
    
    // Alter options form defaults.
    unset($form['alter']);
    unset($form['empty']);
    unset($form['empty_zero']);
    unset($form['hide_empty']);
  }

  function render($values) {
    $nid = $values->{$this->aliases['nid']};
    $title = $values->{$this->aliases['title']};
    $services = (!empty($this->options['services'])) ? $this->options['services'] : NULL;
    return theme('addthis_toolbox', $services, array(
      'url' => url('node/' . $nid, array('absolute' => TRUE)),
      'title' => check_plain($title),
    ));
  }
}
