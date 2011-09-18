// $Id: mix_and_match_script.js,v 1.1 2010/12/05 19:50:42 aross Exp $

Drupal.behaviors.mix_and_matchSuperfish = function (context) {
  $("#primary-menu ul.sf-menu").superfish({
    hoverClass:  'sfHover',
    delay:  250,
    animation:   {opacity:'show',height:'show'},
    speed: 'fast',
    autoArrows: false,
    dropShadows: false,
    disableHI:   true
  }).supposition();
};


