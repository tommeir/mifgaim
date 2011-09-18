<?php

/*
  Do not include drupal's default style sheet in this theme !
*/
function phptemplate_stylesheet_import($stylesheet, $media = 'all') {
  if (strpos($stylesheet, 'misc/drupal.css') == 0) {
    return theme_stylesheet_import($stylesheet, $media);
  }
}





/**
* Override the theme_links function
*
* We use this to insert <span></span> tags around anchor text in the
* primary and secondary links. We need these to support Internet Explorer
* when building sliding door tabs with hover effects.
*/
function fields_links($links, $attributes =  array('class' => 'links')) {
  $output = '';
  if (count($links) > 0) {
    $output = '<ul'. drupal_attributes($attributes) .'>';

    $num_links = count($links);
    $i = 1;

    foreach ($links as $key => $link) {
      $class = $key;

      // Add first, last and active classes to the list of links to help out themers.
      if ($i == 1) {
        $class .= ' first';
      }
      if ($i == $num_links) {
        $class .= ' last';
      }
      if (isset($link['href']) && ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && drupal_is_front_page()))) {
        $class .= ' active';
      }

      $output .= '<li'. drupal_attributes(array('class' => $class)) .'>';

      // wrap <span>'s around the anchor text
      if (isset($link['href'])) {
        $link['title'] = '<span>' . check_plain($link['title']) . '</span>';
        $link['html'] = TRUE;
        // Pass in $link as $options, they share the same keys.
        $output .= l($link['title'], $link['href'], $link);
      }
      else if (!empty($link['title'])) {
        // Some links are actually not links, but we wrap these in <span> for adding title and class attributes
        if (empty($link['html'])) {
          $link['title'] = check_plain($link['title']);
        }
        $span_attributes = '';
        if (isset($link['attributes'])) {
          $span_attributes = drupal_attributes($link['attributes']);
        }
        $output .= '<span'. $span_attributes .'>'. $link['title'] .'</span>';
      }

      $i++;
      $output .= "</li>\n";
    }

    $output .= '</ul>';
  }
  return $output;
}

?>