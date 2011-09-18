<?php
// $Id: 
/**
 * @file
 *  Provide the views gallariffic plugin object with default options and form.
 */

/**
  * Implementation of views_plugin_style().
  */
class views_galleriffic_style_plugin extends views_plugin_style {

  function option_definition() {
    $options = parent::option_definition();
    unset($options['grouping']);
    $options['css'] = array('default'=>'true');
    $options['download'] = array('default'=>'true');
    $options['delay']     = array('default'=> '3000');
    $options['numbthumbs']   = array('default'=> '10');
    $options['pager_top']   = array('default'=> '');
    $options['pager_bottom']  = array('default'=> 'true');
    $options['renderss']     = array('default'=> 'true');
    $options['rendernav'] = array('default'=> 'true');
    $options['playtext']    = array('default'=> 'Play');
    $options['pausetext']    = array('default'=> 'Pause');
    $options['prevlink']    = array('default'=> 'Previous');
    $options['nextlink']    = array('default'=> 'Next');	
    $options['nextpage']    = array('default'=> 'Next &rsaquo;');	
    $options['prevpage']    = array('default'=> '&lsaquo; Prev');	
    $options['history']    = array('default'=> 'true');	
    $options['start']    = array('default'=> '');	
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['grouping']);
    $form['css'] = array(
      '#type' => 'select',
      '#title' => t('Use default css'),
      '#description' => t('You can use the default css or not. The default can\'t anticipate the image sizes you use so will likely need to be updated for individual needs.'),
      '#default_value' => $this->options['css'],
      '#options' => array('true'=>t('Yes'), '' => t('No') ),
    );
    $form['download'] = array(
      '#type' => 'select',
      '#title' => t('Include \'Download Original\' Link'),
      '#description' => t('Include a link to the original image file.'),
      '#default_value' => $this->options['download'],
      '#options' => array('true'=>t('Yes'), '' => t('No') ),
    );
    $form['delay'] = array(
      '#type' => 'textfield',
      '#title' => t('Delay'),
      '#default_value' => $this->options['delay'],
      '#description' => t('The delay between each slide. In miliseconds.'),
    );
    $form['numbthumbs'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of Thumbnails'),
      '#default_value' => $this->options['numbthumbs'],
      '#description' => t('This is the number of thumbnails that will show up.'),
    );
    $form['pager_top'] = array(
      '#type' => 'select',
      '#title' => t('Enable Top Pager'),
      '#description' => t('Enable the pager at the top of the gallery. This will only work if the number of slides is higher than the number of thumbs.'),
      '#default_value' => $this->options['pager_top'],
      '#options' => array('true'=>t('Yes'), '' => t('No') ),
    );
    $form['pager_bottom'] = array(
      '#type' => 'select',
      '#title' => t('Enable Bottom Pager'),
      '#description' => t('Enable the pager at the bottom of the gallery.'),
      '#default_value' => $this->options['pager_bottom'],
      '#options' => array('true'=>t('Yes'), '' => t('No') ),
    );	
    $form['renderss'] = array(
      '#type' => 'select',
      '#title' => t('Show Play and Pause Controls'),
      '#description' => t('Specifies whether the slideshow\'s Play and Pause links should be rendered.'),
      '#default_value' => $this->options['renderss'],
      '#options' => array('true'=>t('Yes'), '' => t('No') ),
    );
    $form['rendernav'] = array(
      '#type' => 'select',
      '#title' => t('Show Next and Pause Controls'),
      '#description' => t('Specifies whether the slideshow\'s Next and Previous links should be rendered.'),
      '#default_value' => $this->options['rendernav'],
      '#options' => array('true'=>t('Yes'), '' => t('No') ),
    );
    $form['playtext'] = array(
      '#type' => 'textfield',
      '#title' => t('Play Link Text'),
      '#default_value' => $this->options['playtext'],
      '#description' => t('Text to display for the PLAY link.'),
    );
    $form['pausetext'] = array(
      '#type' => 'textfield',
      '#title' => t('Pause Link Text'),
      '#default_value' => $this->options['pausetext'],
      '#description' => t('Text to display for the PAUSE link.'),
    );
    $form['prevlink'] = array(
      '#type' => 'textfield',
      '#title' => t('Previous Link Text'),
      '#default_value' => $this->options['prevlink'],
      '#description' => t('Text to display for the PREVIOUS link.'),
    );
    $form['nextlink'] = array(
      '#type' => 'textfield',
      '#title' => t('Next Link Text'),
      '#default_value' => $this->options['nextlink'],
      '#description' => t('Text to display for the NEXT link.'),
    );
    $form['nextpage'] = array(
      '#type' => 'textfield',
      '#title' => t('Next Page Link Text'),
      '#default_value' => $this->options['nextpage'],
      '#description' => t('Text to display for the next PAGE link.'),
    );
    $form['prevpage'] = array(
      '#type' => 'textfield',
      '#title' => t('Previous Page Link Text'),
      '#default_value' => $this->options['prevpage'],
      '#description' => t('Text to display for the next PREVIOUS link.'),
    );
    $form['history'] = array(
      '#type' => 'select',
      '#title' => t('Enable History'),
      '#description' => t('Specifies whether the url\'s hash and the browser\'s history cache should update when the current slideshow image changes.'),
      '#default_value' => $this->options['history'],
      '#options' => array('true'=>t('Yes'), '' => t('No') ),
    );
    $form['start'] = array(
      '#type' => 'select',
      '#title' => t('Auto Start'),
      '#description' => t('Specifies whether the slideshow should be playing or paused when the page first loads.'),
      '#default_value' => $this->options['start'],
      '#options' => array('true'=>t('Yes'), '' => t('No') ),
    );
  }

  /** 
   * Make sure the display and all associated handlers are valid.
   *
   * @return
   *   Empty array if the display is valid; an array of error strings if it is not.
   */
  function validate() {
    $row_handler = $this->row_plugin->definition['handler'];
    switch($row_handler) {
      case 'views_galleriffic_plugin_row_gallerifficrows':
      $errors = array();
      $row_fields= $this->row_plugin->options;
      //check to make sure fields aren't empty
      foreach($row_fields as $title => $result) {
        if(empty($result)) {
          $errors[] = t('Views Galleriffic requires you to assign a field for "@field". Currently the "@field" is empty. Click \'Row Style: Galleriffic Fields\' to set.', array('@field' => $title));
        }
      }
      //check to make sure that two of the same image fields are not selected
      $results = array();
      foreach($row_fields as $title => $result) {
        if(in_array($result,$results)) {
          $errors[] = t('Views Galleriffic does not allow you to use the same field instance twice. You can use the same field twice, and are encouraged to for images. You just need to make two instances of the same field.  To do add the same field twice under \'Fields\'. Give each a different label. Then select each instance of that field once under \'Row style: Galleriffic Fields\'. See README.txt for details.');
        }
        $results[] = $result;
      }
      //check to make sure 'group multiple values' is not selected
      $fields = $this->view->display[$this->view->current_display]->handler->handlers['field'];
      foreach ($fields as $name => $data) {
        $group = $data->options['multiple']['group'];
        if(!is_bool($group) && ($group == 1)) {
           $errors[] = t('Views Galleriffic does not support \'Group Multiple Values\'. Please unselect for the "@field" field.', array('@field' => $name));
        }
      }
      return $errors;
    }
  }
}
