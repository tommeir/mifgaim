// $Id: revision_all.js,v 1.2 2010/05/12 20:39:52 alexj Exp $
(function ($) {
  Drupal.behaviors.revision_all = function (context) {
    if($('#edit-revision-all-revision-all', context).is(':checked')) {
      $('#revision-all-revision-types', context).hide();
    }

    $('#edit-revision-all-revision-all', context).change(function() {
      $('#revision-all-revision-types', context).toggle();
    });
  }
})(jQuery);
