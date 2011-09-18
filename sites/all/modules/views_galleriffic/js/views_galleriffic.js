Drupal.behaviors.ViewsGalleriffic = function () { 
  var settings = Drupal.settings.views_galleriffic;
  
  // Initially set opacity on thumbs and add
  // additional styling for hover effect on thumbs
  var onMouseOutOpacity = 0.67;
  $('#thumbs ul.thumbs li').css('opacity', onMouseOutOpacity)
   .hover(
     function () {
       $(this).not('.selected').fadeTo('fast', 1.0);
     },
     function () {
      $(this).not('.selected').fadeTo('fast', onMouseOutOpacity);
     }
   );

        // Initialize Advanced Galleriffic Gallery
        var galleryAdv = $('#gallery').galleriffic('#thumbs', {
          delay:                  settings.delay,
          numThumbs:              settings.numbthumbs,
          preloadAhead:           settings.numbthumbs,
          enableTopPager:         settings.enableTopPager,
          enableBottomPager:      settings.enableBottomPager,
          imageContainerSel:      '#slideshow',
          controlsContainerSel:   '#controls',
          captionContainerSel:    '#caption',
          loadingContainerSel:    '#loading',
          renderSSControls:       settings.renderSSControls,
          renderNavControls:      settings.renderNavControls,
          playLinkText:           settings.playLinkText,
          pauseLinkText:          settings.pauseLinkText,
          prevLinkText:           settings.prevLinkText,
          nextLinkText:           settings.nextLinkText,
          nextPageLinkText:       settings.nextPageLinkText,
          prevPageLinkText:       settings.prevPageLinkText,
          enableHistory:          settings.enableHistory,
          autoStart:              settings.autoStart,
          onChange:               function(prevIndex, nextIndex) {  $('#thumbs ul.thumbs').children() .eq(prevIndex).fadeTo('fast', onMouseOutOpacity).end() .eq(nextIndex).fadeTo('fast', 1.0);},
          onTransitionOut:        function(callback) {$('#caption').fadeOut('fast');  $('#slideshow').fadeOut('fast', callback);},
          onTransitionIn:         function() {$('#slideshow, #caption').fadeIn('fast');},
          onPageTransitionOut:    function(callback) {$('#thumbs ul.thumbs').fadeOut('fast', callback);},
          onPageTransitionIn:     function() {$('#thumbs ul.thumbs').fadeIn('fast');}
        });
}










