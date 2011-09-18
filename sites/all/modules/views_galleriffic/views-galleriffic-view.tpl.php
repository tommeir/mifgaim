<?php
// $Id: 
/**
 * @file
 *  Views Galleriffic theme wrapper.
 *
 * @ingroup views_templates
 */

$file_path = drupal_get_path('module', 'views_galleriffic') . '/js/jquery.galleriffic.js';
if (!file_exists($file_path)) {
  drupal_set_message('The Views Galleriffic module requires the <a href="http://galleriffic.googlecode.com/svn/tags/1.0/example/js/jquery.galleriffic.js">Galleriffic JS</a> file. Download and place in the \'modules/views_galleriffic/js/\' folder.');
}
drupal_add_js(drupal_get_path('module', 'views_galleriffic') . '/js/jquery.galleriffic.js');
drupal_add_js(drupal_get_path('module', 'views_galleriffic') . '/js/views_galleriffic.js');
if ($view->style_options['css'] == 'true') {
  drupal_add_css(drupal_get_path('module', 'views_galleriffic') . '/css/views_galleriffic_default.css'); 
}
?>
<div id="galleriffic">
  <div id="gallery" class="content">
    <div id="controls" class="controls"></div>
    <div id="loading" class="loader"></div>
    <div id="slideshow" class="slideshow"></div>
    <div id="caption" class="embox"></div>
  </div>
  <div id="thumbs" class="navigation">
    <ul class="thumbs noscript">
      <?php foreach ($rows as $row): ?>
        <?php print $row?>
      <?php endforeach; ?>
    </ul>
  </div>
</div>

