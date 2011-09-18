<?php
// $Id: nodewords_extra.nodewords.hooks.inc,v 1.1.2.1 2010/02/14 11:22:23 kiam Exp $

/**
 * @file
 * Nodewords support file.
 */

/**
 * Implements hook_nodewords_tags_info().
*/
function nodewords_extra_nodewords_tags_info() {
  $tags = array(
    'dc.contributor' => array(
      'callback' => 'nodewords_extra_dc_contributor',
      'context' => array(
        'allowed' => array(
          NODEWORDS_TYPE_DEFAULT,
          NODEWORDS_TYPE_NODE,
          NODEWORDS_TYPE_PAGE,
        ),
      ),
      'label' => t('Dublin Core contributor'),
      'permission' => 'edit meta tag Dublin Core CONTRIBUTOR',
      'templates' => array(
        'head' => array(
          'dc.contributor' => NODEWORDS_META,
        ),
      ),
    ),
    'dc.creator' => array(
      'callback' => 'nodewords_extra_dc_creator',
      'context' => array(
        'allowed' => array(
          NODEWORDS_TYPE_DEFAULT,
          NODEWORDS_TYPE_NODE,
          NODEWORDS_TYPE_PAGE,
        ),
      ),
      'label' => t('Dublin Core creator'),
      'templates' => array(
        'head' => array(
          'dc.creator' => NODEWORDS_META,
        ),
      ),
    ),
    'dc.date' => array(
      'callback' => 'nodewords_extra_dc_date',
      'context' => array(
        'allowed' => array( 
          NODEWORDS_TYPE_NODE,
          NODEWORDS_TYPE_PAGE,
        ),
      ),
      'label' => t('Dublin Core date'),
      'permission' => 'edit meta tag Dublin Core DATE',
      'templates' => array(
        'head' => array(
          'dc.date' => NODEWORDS_META,
        ),
      ),
    ),
    'dc.description' => array(
      'callback' => 'nodewords_extra_dc_description',
      'label' => t('Dublin Core description'),
      'permission' => 'edit meta tag Dublin Core DESCRIPTION',
      'templates' => array(
        'head' => array(
          'dc.description' => NODEWORDS_META,
        ),
      ),
    ),
    'dc.publisher' => array(
      'callback' => 'nodewords_extra_dc_publisher',
      'context' => array(
        'allowed' => array(
          NODEWORDS_TYPE_DEFAULT,
          NODEWORDS_TYPE_PAGE,
        ),
      ),
      'label' => t('Dublin Core publisher'),
      'permission' => 'edit meta tag Dublin Core PUBLISHER',
      'templates' => array(
        'head' => array(
          'dc.publisher' => NODEWORDS_META,
        ),
      ),
    ),
    'dc.title' => array(
      'context' => array(
        'denied' => array(
          NODEWORDS_TYPE_DEFAULT,
        ),
      ),
      'callback' => 'nodewords_extra_dc_title',
      'label' => t('Dublin Core title'),
      'permission' => 'edit meta tag Dublin Core TITLE',
      'templates' => array(
        'head' => array(
          'dc.title' => NODEWORDS_META,
        ),
      ),
    ),
    'location' => array(
      'callback' => 'nodewords_extra_location',
      'label' => t('Location'),
      'permission' => 'edit location meta tag',
      'templates' => array(
        'geo.position' => NODEWORDS_META,
        'icbm' => NODEWORDS_META,
      ),
    ),
    'pics-label' => array(
      'attributes' => array(
        'meta' => array(
          'title' => 'ICRA labels',
          'type' => 'application/rdf+xml',
        ),
      ),
      'callback' => 'nodewords_extra_pics',
      'context' => array(
        'allowed' => array(
          NODEWORDS_TYPE_DEFAULT,
          NODEWORDS_TYPE_FRONTPAGE,
          NODEWORDS_TYPE_NODE,
          NODEWORDS_TYPE_PAGE,
        ),
      ),
      'label' => t('PICS labels'),
      'templates' => array(
        'pics-label' => NODEWORDS_HTTP_EQUIV,
        'meta' => NODEWORDS_LINK_REL,
      ),
      'weight' => array('meta' => -10),
      'permission' => 'edit meta tag PICS-LABEL',
    ),
    'shorturl' => array(
      'context' => array(
        'denied' => array(
          NODEWORDS_TYPE_DEFAULT,
          NODEWORDS_TYPE_ERRORPAGE,
          NODEWORDS_TYPE_TRACKER,
        ),
      ),
      'callback' => 'nodewords_extra_shorturl',
      'label' => t('Short URL'),
      'permission' => 'edit short URL meta tag',
      'templates' => array(
        'head' => array(
          'shorturl' => NODEWORDS_LINK_REL,
        ),
      ),
      'weight' => array('shorturl' => -9),
    ),
  );

  return $tags;
}

function nodewords_extra_nodewords_tags_output_alter(&$output, $parameters) {
  $bool = (
    variable_get('nodewords_add_dc_schema', FALSE) &&
    isset($parameters['output']) &&
    $parameters['type'] != NODEWORDS_TYPE_NONE &&
    $parameters['output'] == 'head'
  );

  if ($bool) {
    $output = (
      '<link rel="schema.dc" href="http://purl.org/dc/elements/1.1/" />' . "\n" .
      $output
    );
  }
}
