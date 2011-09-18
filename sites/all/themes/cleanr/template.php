<?php 

/**
 * Implementation of theme_fieldset().
 */
function cleanr_fieldset(&$element) {
  if (!empty($element['#collapsible'])) {
    drupal_add_js('misc/collapse.js');

    if (!isset($element['#attributes']['class'])) {
      $element['#attributes']['class'] = '';
    }
    $element['#attributes']['class'] .= ' collapsible';
    if (!empty($element['#collapsed'])) {
      $element['#attributes']['class'] .= ' collapsed';
    }
  }
  $element['#attributes']['id'] = $element['#id'];

  $description = !empty($element['#description']) ? "<div class='description'>{$element['#description']}</div>" : '';
  $children = !empty($element['#children']) ? $element['#children'] : '';
  $value = !empty($element['#value']) ? $element['#value'] : '';
  $content = $description . $children . $value;


  return '<fieldset' . drupal_attributes($element['#attributes']) . '>' . ($element['#title'] ? '<legend><span class="title"><span class="border-right"><span class="border-left">' . $element['#title'] . '</span></span></span></legend>' : '') .
  '<div class="fieldset-content">' . $content . '</div></fieldset>';
}

/**
 * Theme function for manage options on admin/content/node, admin/user/user.
 */
function cleanr_admin_manage_options($form) {
  $output = "<div class='clear-block admin-options'>";
  $output .= "<label>{$form['#title']}</label>";
  foreach (element_children($form) as $id) {
    $output .= drupal_render($form[$id]);
  }
  $output .= "</div>";
  return $output;
}

/**
*  Implementation on theme_button()
*/
function cleanr_button($element) {
  if (isset($element['#attributes']['class'])) {
    $element['#attributes']['class'] = 'form-'. $element['#button_type'] .' '. $element['#attributes']['class'];
  }
  else {
    $element['#attributes']['class'] = 'form-'. $element['#button_type'];
  }

  // Wrap non-hidden input elements with span tags for button graphics
  if (stristr($element['#attributes']['style'], 'display: none;') || 
      stristr($element['#attributes']['class'], 'fivestar-submit')  || 
      is_array($element['#upload_validators'])) {
    return '<input type="submit" '. (empty($element['#name']) ? '' : 'name="'. $element['#name'] .'" ')  .'id="'. $element['#id'].'" value="'. check_plain($element['#value']) .'" '. drupal_attributes($element['#attributes']) ." />\n";
  }
  else {
    return '<span class="button-wrapper"><input type="submit" '. (empty($element['#name']) ? '' : 'name="'. $element['#name'] .'" ')  .'id="'. $element['#id'].'" value="'. check_plain($element['#value']) .'" '. drupal_attributes($element['#attributes']) ." /></span>\n";
  } 
}

function cleanr_breadcrumb($breadcrumb) {
   if (!empty($breadcrumb)) {
     return '<div class="breadcrumb">'. implode(' <span class="bc-separator">&gt;</span> ', $breadcrumb) .'</div>';
   }
}

function cleanr_preprocess_page(&$vars){
}
