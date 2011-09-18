<?php
// $Id: nodewords_extra.nodewords.tags.inc,v 1.1.2.1 2010/02/14 11:22:23 kiam Exp $

/**
 * @file
 * Nodewords support file.
 */

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_extra_dc_contributor_form(&$form, $content, $options) {
  $form['dc.contributor'] = array(
    '#tree' => TRUE,
  );

  $form['dc.contributor']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Dublin Core contributor'),
    '#description' => t('Enter the name of an entity responsible for making contributions to the resource. Examples of a contributor include a person, an organization, or a service.'),
    '#default_value' => empty($content['value']) ? (!empty($options['default']['dc.contributor']['value']) ? $options['default']['dc.contributor']['value'] : '') : $content['value'],
    '#size' => 60,
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Set the meta tag content.
 */
function nodewords_extra_dc_contributor_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    $tags['dc.contributor'] = $content['value'];
  }
  elseif (!empty($options['default']['dc.contributor']['value'])) {
    $tags['dc.contributor'] = $options['default']['dc.contributor']['value'];
  }
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_extra_dc_creator_form(&$form, $content, $options) {
  $form['dc.creator'] = array(
    '#tree' => TRUE,
  );

  $form['dc.creator']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Dublin Core creator'),
    '#description' => t('Enter the name of an entity primarily responsible for making the resource. Examples of a creator include a person, an organization, or a service.'),
    '#default_value' => empty($content['value']) ? (!empty($options['default']['dc.creator']['value']) ? $options['default']['dc.creator']['value'] : '') : $content['value'],
    '#size' => 60,
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Set the meta tag content.
 */
function nodewords_extra_dc_creator_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    $tags['dc.creator'] = $content['value'];
  }
  elseif (!empty($options['default']['dc.creator']['value'])) {
    $tags['dc.creator'] = $options['default']['dc.creator']['value'];
  }
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_extra_dc_date_form(&$form, $content, $options) {
  $form['dc.date'] = array(
    '#tree' => TRUE,
  );

  $form['dc.date']['value'] = array(
    '#type' => 'date',
    '#title' => t('Dublin Core date'),
    '#description' => t('A point or period of time associated with an event in the lifecycle of the resource. The date should be relative to UTC.'),
    '#default_value' => empty($content['value']) ? array() : $content['value'],
  );
}

/**
 * Set the meta tag content.
 */
function nodewords_extra_dc_date_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    $time = mktime(0, 0, 0, $content['value']['month'], $content['value']['day'], $content['value']['year']);
    if ($time > 0) {
      $tags['dc.date'] = date('Y-m-d\TH:i:s\Z', $time);
    }
  }
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_extra_dc_description_form(&$form, $content, $options) {
  if (!isset($form['description'])) {
    $bool = (
      isset($options['content type']) &&
      variable_get('nodewords_use_teaser_' . $options['content type'], FALSE)
    );

    if (!$bool) {
      $form['dc.description'] = array(
        '#tree' => TRUE,
      );

      $form['dc.description']['value'] = array(
        '#type' => 'textarea',
        '#title' => t('Description'),
        '#description' => t('Enter a description. Limit your description to about 20 words, with a maximum of %count characters. It should not contain any HTML tags or other formatting.', array('%count' => variable_get('nodewords_max_size', 350))),
        '#default_value' => empty($content['value']) ? (!empty($options['default']['dc.description']['value']) ? $options['default']['dc.description']['value'] : '') : $content['value'],
        '#cols' => 60,
        '#rows' => 6,
        '#wysiwyg' => FALSE,
      );
    }
  }
}

/**
 * Set the meta tag content.
 */
function nodewords_extra_dc_description_prepare(&$tags, $content, $options) {
  if (empty($tags['description'])) {
    if (empty($content['value'])) {
      if (!empty($options['default']['dc.description']['value'])) {
        $value = $options['default']['dc.description']['value'];
      }
      elseif (!empty($options['default']['description']['value'])) {
        $value = $options['default']['description']['value'];
      }
      else {
        $value = '';
      }
    }
    else {
      $value = $content['value'];
    }

    $tags['dc.description'] = $value;
  }
  else {
    $tags['dc.description'] = $tags['description'];
  }
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_extra_dc_publisher_form(&$form, $content, $options) {
  $form['dc.publisher'] = array(
    '#tree' => TRUE,
  );

  $form['dc.publisher']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Dublin Core publisher'),
    '#description' => t('Enter a name of an entity responsible for making the resource available. Examples of a publisher include a person, an organization, or a service.'),
    '#default_value' => empty($options['default']['dc.publisher']['value']) ? '' : $options['default']['dc.publisher']['value'],
    '#size' => 60,
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Set the meta tag content.
 */
function nodewords_extra_dc_publisher_prepare(&$tags, $content, $options) {
  $tags['dc.publisher'] = empty($options['default']['dc.publisher']['value']) ? '' : $options['default']['dc.publisher']['value'];
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_extra_dc_title_form(&$form, $content, $options) {
  $form['dc.title'] = array(
    '#tree' => TRUE,
  );

  $form['dc.title']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Dublin Core title'),
    '#description' => t('Enter an alternative title. Do not use the value already used for the HTML tag TITLE, or you will probably get warning reports about duplicated titles from the search engines.'),
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#size' => 60,
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Set the meta tag content.
 */
function nodewords_extra_dc_title_prepare(&$tags, $content, $options) {
  $tags['dc.title'] = empty($content['value']) ? '' : $content['value'];
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_extra_location_form(&$form, $content, $options) {
  $form['location'] = array(
    '#tree' => TRUE,
  );

  $form['location']['latitude'] = array(
    '#type' => 'textfield',
    '#title' => t('Location coordinates (latitude, longitude)'),
    '#default_value' => empty($content['latitude']) ? (!empty($options['default']['location']['latitude']) ? $options['default']['location']['latitude'] : '') : $content['latitude'],
    '#element_validate' => array('nodewords_validate_element'),
    '#validate_args' => array(
      'module' => 'nodewords_basic',
      'callback' => 'nodewords_extra_location_form_validate',
    ),
    '#size' => 14,
    '#maxlength' => 14,
    '#prefix' => '<div class="container-inline">',
    '#latitude' => TRUE,
  );

  $form['location']['longitude'] = array(
    '#type' => 'textfield',
    '#title' => '',
    '#default_value' => empty($content['longitude']) ? (!empty($options['default']['location']['longitude']) ? $options['default']['location']['longitude'] : '') : $content['longitude'],
    '#size' => 14,
    '#element_validate' => array('nodewords_validate_element'),
    '#validate_args' => array(
      'module' => 'nodewords_basic',
      'callback' => 'nodewords_extra_location_form_validate',
    ),
    '#maxlength' => 14,
    '#suffix' => '</div>',
  );
}

/**
 * Validate the values passed as latitude, and longitude.
 */
function nodewords_extra_location_form_validate($element, &$form_state) {
  if (isset($element['#latitude'])) {
    if (!empty($element['#value']) && (!is_numeric($element['#value']) || $element['#value'] < -90.0 || $element['#value'] > 90.0)) {
      form_error($element, t('Latitude must be a number between -90.0 and 90.0 (extrems included).'));
    }
  }
  else {
    if (!empty($element['#value']) && (!is_numeric($element['#value']) || $element['#value'] < -180.0 || $element['#value'] > 180.0)) {
      form_error($element, t('Longitude must be a number between -180.0 and 180.0 (extrems included).'));
    }
  }
}

/**
 * Set the meta tag content.
 */
function nodewords_extra_location_prepare(&$tags, $content, $options) {
  if (empty($content['latitude']) || empty($content['longitude'])) {
    $content['latitude'] = !empty($options['default']['location']['latitude']) ? $options['default']['location']['latitude'] : '';
    $content['longitude'] = !empty($options['default']['location']['longitude']) ? $options['default']['location']['longitude'] : '';
  }

  if (!empty($content['latitude']) && !empty($content['longitude'])) {
    $tags['location:geo.position'] = $content['latitude'] . ';' . $content['longitude'];
    $tags['location:icbm'] = $content['latitude'] . ',' . $content['longitude'];
  }

  $bool = (
    empty($tags['location:geo.position']) &&
    $options['type'] == NODEWORDS_TYPE_NODE &&
    module_exists('location') &&
    ($node = node_load($options['id']))
  );

  if ($bool && isset($node->locations[0]['latitude']) && isset($node->locations[0]['longitude'])) {
    $tags['location:geo.position'] = $node->locations[0]['latitude'] . ';' . $node->locations[0]['longitude'];
    $tags['location:icbm'] = $node->locations[0]['latitude'] . ',' . $node->locations[0]['longitude'];
  }
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_extra_pics_form(&$form, $content, $options) {
  $form['pics-label'] = array(
    '#tree' => TRUE,
  );

  $form['pics-label']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('PICS labels'),
    '#description' => t('Enter the ICRA PICS labels only, without any extra characters; ICRA PICS labels should be something similar to %icra-pics-example. You can use the online <a href="@icra-label-generator">ICRA label generator</a>.', array('%icra-pics-example' => 'n 0 s 0 v 0 l 0 oa 0 ob 0 oc 0 od 0 oe 0 of 0 og 0 oh 0 c 1', '@icra-label-generator' => 'http://www.icra.org/label/generator/')),
    '#default_value' => empty($content['value']) ? (!empty($options['default']['pics-label']['value']) ? $options['default']['pics-label']['value'] : '') : $content['value'],
    '#size' => 60,
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Set the meta tag content.
 */
function nodewords_extra_pics_prepare(&$tags, $content, $options) {
  if (empty($content['value'])) {
    $content['value'] = !empty($options['default']['pics-label']['value']) ? $options['default']['pics-label']['value'] : '';
  }

  if (!empty($content['value'])) {
    if ($options['type'] == NODEWORDS_TYPE_NODE) {
      $tags['pics-label'] = strtr(
        '(pics-1.1 "http://www.icra.org/pics/vocabularyv03/" l gen true for %url r (%pics))',
        array(
          '%url' => nodewords_url("node/{$options['id']}"),
          '%pics' => $value,
        )
      );
    }
    elseif ($options['type'] == NODEWORDS_TYPE_PAGE) {
      $tags['pics-label'] = strtr(
        '(pics-1.1 "http://www.icra.org/pics/vocabularyv03/" l gen true for %url r (%pics))',
        array(
          '%url' => nodewords_url("node/{$options['id']}"),
          '%pics' => $value,
        )
      );
    }
  }

  if (variable_get('nodewords_icra_validation_content', '')) {
    $tags['pics-label:meta'] = nodewords_url('labels.rdf', array('absolute' => TRUE));
  }
}

function nodewords_extra_pics_settings_form(&$form, $form_id, $options) {
  if ($form_id == 'nodewords_settings_form') {
    $form['metatags_creation']['nodewords_icra_validation_content'] = array(
      '#type' => 'textarea',
      '#title' => t('ICRA validation content'),
      '#description' => t('Enter the validation content you obtained from the <a href="@icra-generator">ICRA label generator</a>. For more information, see <a href="@icra-support">ICRA support</a>.', array('@icra-support' => 'http://www.icra.org/support/', '@icra-generator' => 'http://www.icra.org/label/generator/')),
      '#default_value' => variable_get('nodewords_icra_validation_content', ''),
      '#size' => 60,
      '#wysiwyg' => FALSE,
    );
  }
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_extra_shorturl_form(&$form, $content, $options) {
  $form['shorturl'] = array(
    '#tree' => TRUE,
  );

  $form['shorturl']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Short URL'),
    '#description' => t('Short URLs are used instead of the original URL because they are shorter, and easier to remember. Short URLs are provided by some web services, such as <a href="@bitly">bit.ly</a>, <a href="@shorturl">ShortURL</a>, and <a href="@tinyurl">TinyURL</a>.', array('@bitly' => 'http://bit.ly', '@shorturl' => 'http://shorturl.com', '@tinyurl' => 'http://tinyurl.com')),
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#element_validate' => array('nodewords_validate_element'),
    '#validate_args' => array(
      'module' => 'nodewords_basic',
      'callback' => 'nodewords_extra_shorturl_form_validate',
    ),
    '#size' => 60,
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

function nodewords_extra_shorturl_form_validate($element, &$form_state) {
  if (!empty($element['#value'])) {
    if (!valid_url(drupal_urlencode($element['#value']), TRUE)) {
      form_error($element, t('The short URL is not a valid absolute URL.'));
    }
  }
}

/**
 * Set the meta tag content.
 */
function nodewords_extra_shorturl_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    $tags['shorturl'] = check_url($content['value']);
  }
}
