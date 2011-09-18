<?php
// $Id: nodewords_basic.nodewords.hooks.inc,v 1.1.2.1 2010/02/14 11:22:23 kiam Exp $

/**
 * @file
 * Nodewords support file.
 */

/**
* Implements hook_nodewords_tags_info().
 */
function nodewords_basic_nodewords_tags_info() {
  $tags = array(
    'abstract' => array(
      'callback' => 'nodewords_basic_abstract',
      'label' => t('Abstract'),
      'permission' => 'edit meta tag ABSTRACT',
      'templates' => array(
        'head' => array(
          'abstract' => NODEWORDS_META,
        ),
      ),
    ),
    'canonical' => array(
      'callback' => 'nodewords_basic_canonical',
      'context' => array(
        'allowed' => array(
          NODEWORDS_TYPE_FRONTPAGE,
          NODEWORDS_TYPE_NODE,
          NODEWORDS_TYPE_PAGE,
          NODEWORDS_TYPE_PAGER,
          NODEWORDS_TYPE_TERM,
          NODEWORDS_TYPE_USER,
          NODEWORDS_TYPE_VOCABULARY,
        ),
        'denied' => array(
          NODEWORDS_TYPE_DEFAULT,
          NODEWORDS_TYPE_ERRORPAGE,
          NODEWORDS_TYPE_TRACKER,
        ),
      ),
      'label' => t('Canonical URL'),
      'permission' => 'edit canonical URL meta tag',
      'templates' => array(
        'head' => array(
          'canonical' => NODEWORDS_LINK_REL,
        ),
      ),
      'weight' => array('canonical' => -10),
    ),
    'copyright' => array(
      'callback' => 'nodewords_basic_copyright',
      'label' => t('Copyright'),
      'permission' => 'edit meta tag COPYRIGHT',
      'templates' => array(
        'head' => array(
          'copyright' => NODEWORDS_META
        ),
        'search index' => array(
          'copyright' => '<h3>%content</h3>'
        ),
      ),
      'weight' => array('copyright' => -7),
    ),
    'description' => array(
      'callback' => 'nodewords_basic_description',
      'label' => t('Description'),
      'permission' => 'edit meta tag DESCRIPTION',
      'templates' => array(
        'head' => array(
          'description' => NODEWORDS_META
        ),
        'search index' => array(
          'description' => '<h3>%content</h3>'
        ),
      ),
      'weight' => array('description' => -6),
    ),
    'keywords' => array(
      'callback' => 'nodewords_basic_keywords',
      'label' => t('Keywords'),
      'permission' => 'edit meta tag KEYWORDS',
      'templates' => array(
        'head' => array(
          'keywords' => NODEWORDS_META
        ),
        'search index' => array(
          'keywords' => '<h2>%content</h2>'
        ),
      ),
      'weight' => array('keywords' => -8),
    ),
    'revisit-after' => array(
      'callback' => 'nodewords_basic_revisit_after',
      'label' => t('Revisit after'),
      'permission' => 'edit meta tag REVISIT-AFTER',
      'templates' => array(
        'head' => array(
          'revisit-after' => NODEWORDS_META,
        ),
      ),
    ),
    'robots' => array(
      'callback' => 'nodewords_basic_robots',
      'context' => array(
        'allowed' => array(
          NODEWORDS_TYPE_DEFAULT,
          NODEWORDS_TYPE_ERRORPAGE,
          NODEWORDS_TYPE_FRONTPAGE,
          NODEWORDS_TYPE_NODE,
          NODEWORDS_TYPE_PAGE,
          NODEWORDS_TYPE_PAGER,
          NODEWORDS_TYPE_TERM,
          NODEWORDS_TYPE_TRACKER,
          NODEWORDS_TYPE_USER,
          NODEWORDS_TYPE_VOCABULARY,
        ),
      ),
      'label' => t('Robots'),
      'permission' => 'edit meta tag ROBOTS',
      'templates' => array(
        'head' => array(
          'robots' => NODEWORDS_META,
        ),
      ),
    ),
  );

  return $tags;
}
