Drupal.node_edit_protection = {};
Drupal.node_edit_protection.confirmNavigate = function() {
  return confirm(Drupal.t("You are about to leave a form that may contain changes that will not be saved.") + "\n\n" + Drupal.t("Click <OK> to continue anyway."));
};

Drupal.behaviors.nodeEditProtection = function(context) {
    
  // If they leave an input field, assume they changed it.
  $(".node-form :input").each(function() {
    $(this).blur(function() {
      $(window).attr('node_edit_protection_edited', true);
    });
  });

  // Let all form submit buttons through
  $(".node-form input[@type='submit']").each(function() {
    $(this).addClass('node-edit-protection-processed');
    $(this).click(function() {
      $(window).attr('node_edit_protection_clicked', true);
    });
  });

  // Catch all links and buttons
  $("a, button, input[@type='submit']:not(.node-edit-protection-processed)").each(function() {
    $(this).click(function() {
      var edited = $(window).attr('node_edit_protection_edited');
      if(edited) {
        $(window).attr('node_edit_protection_clicked', true);
        return Drupal.node_edit_protection.confirmNavigate();
      }
    });
  });

  $(window).unload(function() {
    var clicked = $(this).attr('node_edit_protection_clicked');
    var edited = $(this).attr('node_edit_protection_edited');
    if(edited && !clicked) {
      return Drupal.node_edit_protection.confirmNavigate();
    }
  });
};