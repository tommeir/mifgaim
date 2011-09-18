// $Id: block_edit.js,v 1.1.2.14 2010/02/17 18:24:24 psynaptic Exp $

Drupal.behaviors.block_edit = function (context) {
  if (Drupal.settings.block_edit.hover_links) {
    $('.node-edit-link, .block-edit-link').hide();
    $('div.block, div.node').mouseover(function() {
      $(this).find('.node-edit-link, .block-edit-link').css('display', 'block');
    });

    $('div.block, div.node').mouseout(function() {
      $(this).find('.node-edit-link, .block-edit-link').css('display', 'none');
    });
  };
};
