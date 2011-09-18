<?php
// $Id: nodewords_basic.nodewords.tags.inc,v 1.1.2.2 2010/02/16 15:32:17 kiam Exp $

/**
 * @file
 * Meta tags definition file.
 */

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_basic_abstract_form(&$form, $content, $options) {
  $form['abstract'] = array(
    '#tree' => TRUE,
  );

  $form['abstract']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Abstract'),
    '#description' => t('Enter a short abstract. Typically it is one sentence.'),
    '#default_value' => empty($content['value']) ? (!empty($options['default']['abstract']['value']) ? $options['default']['abstract']['value'] : '') : $content['value'],
    '#size' => 60,
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Set the meta tag content.
 */
function nodewords_basic_abstract_prepare(&$tags, $content, $options) {
  $tags['abstract'] = empty($content['value']) ? (!empty($options['default']['abstract']['value']) ? $options['default']['abstract']['value'] : '') : $content['value'];
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_basic_canonical_form(&$form, $content, $options) {
  $form['canonical'] = array(
    '#tree' => TRUE,
  );

  $form['canonical']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Canonical URL'),
    '#description' => t('Canonical URLs are used from the search engines, and allow them to not report duplicate titles for HTML pages that are accessible from different URLs. Use a relative URL without the initial slash; canonical URLs that point to a different domain are normally not accepted.'),
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#element_validate' => array('nodewords_validate_element'),
    '#validate_args' => array(
      'module' => 'nodewords_basic',
      'callback' => 'nodewords_basic_canonical_form_validate',
    ),
    '#size' => 60,
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

function nodewords_basic_canonical_form_validate($element, &$form_state) {
  if (!empty($element['#value'])) {
    $canonical_url = trim($element['#value'], '/');

    if (!empty($canonical_url) && !valid_url(drupal_urlencode($canonical_url))) {
      form_error($element, t('Canonical URL must be a relative URL.'));
    }
  }
}

/**
 * Set the meta tag content.
 */
function nodewords_basic_canonical_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    $content['value'] = trim($content['value'], '/');
  }

  if (empty($content['value'])) {
    $path = '';

    switch ($options['type']) {
      case NODEWORDS_TYPE_FRONTPAGE:
        $content['value'] = '<front>';
        break;

      case NODEWORDS_TYPE_NODE:
        $path = 'node/' . $options['id'];
        break;

      case NODEWORDS_TYPE_PAGE:
        $path = $_GET['q'];
        break;

      case NODEWORDS_TYPE_TERM:
        $path = 'taxonomy/term/' . $options['id'];
        break;

      case NODEWORDS_TYPE_USER:
        $path = 'user/' . $options['id'];
        break;
    }

    if ($path) {
      $content['value'] = $path;
    }
  }
  elseif (!empty($content['value']) && strpos($content['value'], '/') === 0) {
    $content['value'] = drupal_substr($content['value'], 1);
  }

  $tags['canonical'] = !empty($content['value']) ? check_url(nodewords_url($content['value'], $options)) : '';
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_basic_copyright_form(&$form, $content, $options) {
  $form['copyright'] = array(
    '#tree' => TRUE,
  );

  $form['copyright']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Copyright'),
    '#description' => t('Enter a short copyright statement.'),
    '#default_value' => empty($content['value']) ? (!empty($options['default']['copyright']['value']) ? $options['default']['copyright']['value'] : '') : $content['value'],
    '#size' => 60,
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Set the meta tag content.
 */
function nodewords_basic_copyright_prepare(&$tags, $content, $options) {
  $tags['copyright'] = empty($content['value']) ? (!empty($options['default']['copyright']['value']) ? $options['default']['copyright']['value'] : '') : $content['value'];
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_basic_description_form(&$form, $content, $options) {
  $bool = (
    $options['type'] == NODEWORDS_TYPE_NODE &&
    isset($options['content type']) &&
    variable_get('nodewords_metatags_generation_method_' . $options['content type'], NODEWORDS_GENERATION_NEVER) == NODEWORDS_GENERATION_ALWAYS
  );

  if (!$bool) {
    $form['description'] = array(
      '#tree' => TRUE,
    );

    $form['description']['value'] = array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#description' => t('Enter a description. Limit your description to about 20 words, with a maximum of %count characters. It should not contain any HTML tags or other formatting.', array('%count' => variable_get('nodewords_max_size', 350))),
      '#default_value' => empty($content['value']) ? (!empty($options['default']['description']['value']) ? $options['default']['description']['value'] : '') : $content['value'],
      '#cols' => 60,
      '#rows' => 6,
      '#wysiwyg' => FALSE,
    );
  }
}

/**
 * Set the meta tag content.
 */
function nodewords_basic_description_prepare(&$tags, $content, $options) {
  $bool = (
    $options['type'] == NODEWORDS_TYPE_NODE &&
    ($node = node_load($options['id']))
  );

  if ($bool) {
    $value = empty($content['value']) ? (empty($options['default']['description']['value']) ? '' : $options['default']['description']['value']) : $content['value'];
    $content['value'] = nodewords_metatag_from_node_content($node, $value);
  }

  $tags['description'] = empty($content['value']) ? (!empty($options['default']['description']['value']) ? $options['default']['description']['value'] : '') : $content['value'];
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_basic_keywords_form(&$form, $content, $options) {
  $form['keywords'] = array(
    '#tree' => TRUE,
  );

  $form['keywords']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Keywords'),
    '#description' => t('Enter a comma separated list of keywords. Avoid duplication of words as this will lower your search engine ranking.'),
    '#default_value' => empty($content['value']) ? (!empty($options['default']['keywords']['value']) ? $options['default']['keywords']['value'] : '') : $content['value'],
    '#size' => 60,
    '#element_validate' => array('nodewords_validate_element'),
    '#validate_args' => array(
      'module' => 'nodewords_basic',
      'callback' => 'nodewords_basic_keywords_form_validate',
    ),
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Validate the values passed as keywords.
 */
function nodewords_basic_keywords_form_validate($element, &$form_state) {
  if (trim($element['#value']) == ',') {
    form_error($element, t('The meta tag KEYWORDS value must be a comma separated list of words.'));
  }
}

/**
 * Set the meta tag content.
 */
function nodewords_basic_keywords_prepare(&$tags, $content, $options) {
  $value = variable_get('nodewords_first_keywords', '');

  if (empty($content['value'])) {
    $value .= ',' . (!empty($options['default']['keywords']['value']) ? $options['default']['keywords']['value'] : '');
  }
  else {
    $value .= ',' . $content['value'];
  }

  if ($options['type'] == NODEWORDS_TYPE_NODE && module_exists('taxonomy')) {
    $node = node_load($options['id']);
    foreach (taxonomy_node_get_terms($node) as $term) {
      if (in_array($term->vid, variable_get('nodewords_keyword_vids', array()))) {
        $value .= ',' . $term->name;
      }
    }
  }

  $tags['keywords'] = nodewords_unique_values($value . ',' . variable_get('nodewords_last_keywords', ''));
}

function nodewords_basic_keywords_settings_form(&$form, $form_id, $options) {
  if ($form_id == 'nodewords_settings_form') {
    if (module_exists('taxonomy')) {
      $vids = array();

      foreach (taxonomy_get_vocabularies() as $vocabulary) {
        $vids[$vocabulary->vid] = check_plain($vocabulary->name);
      }

      if ($count = count($vids)) {
        $form['metatags_creation']['nodewords_keyword_vids'] = array(
          '#type' => $count > 10 ? 'select' : 'checkboxes',
          '#title' => t('Auto-keywords vocabularies'),
          '#description' => t('Select the vocabularies which contain terms you want to add to the keywords meta tag for nodes. The terms of these vocabularies are added before the global keywords but after the page-specific keywords.'),
          '#default_value' => variable_get('nodewords_keyword_vids', array()),
          '#options' => $vids,
          '#multiple' => TRUE,
        );

        if ($count > 10) {
          $form['metatags_creation']['nodewords_keyword_vids']['#multiple'] = TRUE;
        }
        elseif ($count > 2) {
          // Add support for Check All if the checkboxes are more than two.
          $form['metatags_creation']['nodewords_keyword_vids']['#checkall'] = TRUE;
        }
      }
    }

    if (empty($vids)) {
      $form['metatags_creation']['nodewords_keyword_vids'] = array(
        '#type' => 'value',
        '#default_value' => variable_get('nodewords_keyword_vids', array()),
      );

      $form['metatags_creation']['taxonomy_message'] = array(
        '#value' => module_exists('taxonomy') ? t('There are no vocabularies currently defined.') : t('The module %module is not enabled; if you want to use taxonomy terms to automatically set the content of the meta tag KEYWORDS, you first need to <a href="@modules-page">enable %module</a>', array('%module' => 'taxonomy.module', '@modules-page' => url('admin/build/modules'))),
        '#prefix' => '<div>',
        '#suffix' => '</div>',
      );
    }

    $form['metatags_creation']['nodewords_first_keywords'] = array(
      '#type' => 'textfield',
      '#title' => t('First keywords'),
      '#description' => t('Enter a comma separated list of keywords that will be always added to each page, and before any other keywords.'),
      '#default_value' => variable_get('nodewords_first_keywords', ''),
      '#size' => 60,
      '#element_validate' => array('nodewords_validate_element'),
      '#validate_args' => array(
        'module' => 'nodewords_basic',
        'callback' => 'nodewords_basic_keywords_form_validate',
      ),
      '#maxlength' => variable_get('nodewords_max_size', 350),
    );

    $form['metatags_creation']['nodewords_last_keywords'] = array(
      '#type' => 'textfield',
      '#title' => t('Last keywords'),
      '#description' => t('Enter a comma separated list of keywords that will be always added to each page, and after any other keywords.'),
      '#default_value' => variable_get('nodewords_last_keywords', ''),
      '#size' => 60,
      '#element_validate' => array('nodewords_validate_element'),
      '#validate_args' => array(
        'module' => 'nodewords_basic',
        'callback' => 'nodewords_basic_keywords_form_validate',
      ),
      '#maxlength' => variable_get('nodewords_max_size', 350),
    );
  }
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_basic_revisit_after_form(&$form, $content, $options) {
  $form['revisit-after'] = array(
    '#tree' => TRUE,
  );

  $form['revisit-after']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Revisit after'),
    '#description' => t('The meta tag REVISIT-AFTER defines how often a search engine or spider should come to your website for re-indexing. This tag is used for websites that change their content and on a regular basis. This tag can also be beneficial in boosting your rankings if search engines display results based on the most recent submissions.'),
    '#default_value' => empty($content['value']) ? (!empty($options['default']['revisit-after']['value']) ? (integer) $options['default']['revisit-after']['value'] : 1) : (integer) $content['value'],
    '#size' => 3,
    '#element_validate' => array('nodewords_validate_element'),
    '#validate_args' => array(
      'module' => 'nodewords_basic',
      'callback' => 'nodewords_basic_revisit_after_form_validate',
    ),
    '#maxlength' => 3,
    '#field_suffix' => t('day(s)'),
  );
}

/**
 * Validate the meta tag value.
 */
function nodewords_basic_revisit_after_form_validate($element, &$form_state) {
  if (!preg_match('/^[0-9]*$/', $element['#value'])) {
    form_error($element, t('The meta tag REVISIT-AFTER value must be a positive number.'));
  }
}

/**
 * Set the meta tag content.
 */
function nodewords_basic_revisit_after_prepare(&$tags, $content, $options) {
  $value = empty($content['value']) ? (!empty($options['default']['revisit-after']['value']) ? (integer) $options['default']['revisit-after']['value'] : 1) : (integer) $content['value'];
  $tags['revisit-after'] = $value > 1 ? "$value days" : '1 day';
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_basic_robots_form(&$form, $content, $options) {
  $field_options = array(
    'noarchive' => t('NOARCHIVE'),
    'nofollow' => t('NOFOLLOW'),
    'noindex' => t('NOINDEX'),
    'noodp' => t('NOODP'),
    'nosnippet' => t('NOSNIPPET'),
    'noydir' => t('NOYDIR'),
  );

  if (isset($content['value']['noopd'])) {
    $content['value']['noodp'] = is_string($content['value']['noopd']) ? 'noodp' : 0;
    unset($content['value']['noopd']);
  }

  $form['robots'] = array(
    '#tree' => TRUE,
  );

  $form['robots']['value'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Robots'),
    '#description' => t('The meta tag ROBOTS offers a simple mechanism to indicate to web robots and crawlers whether the page should be indexed (NOINDEX) and whether links on the page should be followed (NOFOLLOW).'),
    '#default_value' => empty($content['value']) ? array() : $content['value'],
    '#options' => $field_options,
    '#checkall' => TRUE,
  );

  if ($options['type'] != NODEWORDS_TYPE_DEFAULT) {
    $form['robots']['use_default'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use the default value'),
      '#default_value' => !empty($content['use_default']),
    );

    if (!empty($options['default']['robots']['value'])) {
      $default_values = array_keys(array_filter($options['default']['robots']['value']));
    }

    if (!empty($default_values)) {
      $description = t(
        'The default value is currently %default_values.',
        array('%default_values' => implode(', ', $default_values))
      );
    }
    else {
      $description = t('The default value is currently not set.');
    }

    $form['robots']['use_default']['#description'] = $description;
  }
}

/**
 * Set the meta tag content.
 */
function nodewords_basic_robots_prepare(&$tags, $content, $options) {
  if ($options['type'] == NODEWORDS_TYPE_PAGER) {
    $value = variable_get('nodewords_list_robots', array());
  }
  elseif (empty($content['use_default'])) {
    $value = !empty($content['value']) ? $content['value'] : array();
  }
  else {
    $value = !empty($options['default']['robots']['value']) ? $options['default']['robots']['value'] : array();
  }

  if (!empty($value)) {
    $value = array_keys(array_filter($value));
    if (!empty($value)) {
      $tags['robots'] = implode(', ', $value);
    }
  }
}

function nodewords_basic_robots_settings_form(&$form, $form_id, $options) {
  switch ($form_id) {
    case 'nodewords_settings_form':
      $field_options = array(
        'noarchive' => t('NOARCHIVE'),
        'nofollow' => t('NOFOLLOW'),
        'noindex' => t('NOINDEX'),
        'noodp' => t('NOODP'),
        'nosnippet' => t('NOSNIPPET'),
        'noydir' => t('NOYDIR'),
      );

      $form['metatags_creation']['nodewords_list_robots'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Meta tag ROBOTS for lists'),
        '#description' => t('The value to use for the meta tag ROBOTS for the pages that use a pager; this setting is not used for the first page of the list, or when the option to repeat the meta tags for all the pages in the same list is selected.'),
        '#default_value' => variable_get('nodewords_list_robots', array()),
        '#options' => $field_options,
        '#checkall' => TRUE,
      );
  }
}
