<?php
// $Id: recommender.drush.inc,v 1.1 2009/06/06 16:13:05 danithaca Exp $

/**
 * @file
 *   RecommenderAPI drush command.
 *   To use this, please install Drush at http://drupal.org/project/drush
 */

/**
 * Implementation of hook_drush_command().
 */
function recommender_drush_command() {
  $items = array();
  $items['recommender'] = array(
    'callback' => 'recommender_drush_run',
    'description' => "Run RecommenderAPI based modules, compute similarity and/or prediction.",
    'arguments' => array(
      'modules' => 'Optional. A space delimited list of modules.',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function recommender_drush_help($section) {
  switch ($section) {
    case 'drush:recommender':
      return dt("Run RecommenderAPI based modules, compute similarity and/or prediction.");
  }
}

function recommender_drush_run() {
  $command = drush_get_command();
  $modules = $command['arguments'];
  if (count($modules)==0) {
    $modules = module_implements("run_recommender");
  }
  
  // TODO: test whether users input valid modules that have hook_run_recommender()
  
  if (count($modules)==0) {
    drush_print("No recommender modules enabled. Exit.");
  }
  
  drush_print("\n\nRecommender modules to run: ". implode(', ', $modules));
  drush_print("Process might take a long time. ***Please be patient. Thanks.***\n\n");
  foreach ($modules as $module) {
    drush_print("Running $module at ". date(DateTime::W3C) ." ..........");
    call_user_func($module ."_run_recommender");
    drush_print("Finish running $module at ". date(DateTime::W3C));
  }
  
  drush_print("\n\nAll recommender tasks complete.");
}
