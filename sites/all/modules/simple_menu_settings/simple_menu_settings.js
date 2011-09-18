Drupal.behaviors.simpleMenuSettingsBehavior = function (context) {
  
  $('#edit-menu-delete-wrapper').hide();
  
  $('.menu-item-form').prepend('<div id="edit-menu-add-wrapper" class="form-item"><label for="edit-menu-add" class="option"><input type="checkbox" class="form-checkbox" value="1" id="edit-menu-add" name="menu[add]"> Add this page to a menu.</label></div>');
  
  if($('#edit-menu-link-title').val()) {
    $('#edit-menu-add').attr('checked', true);
  }
  else{
    $('#edit-menu-link-title-wrapper').hide();
    $('#edit-menu-parent-wrapper').hide();
  }
  
  $('#edit-menu-add').click(function(){
    if ($(this).is(':checked')) {
      var title = $('#edit-title').val();
      $('#edit-menu-link-title').val(title);
      $('#edit-menu-link-title-wrapper').show();
      $('#edit-menu-parent-wrapper').show();
      $('#edit-menu-delete').attr('checked', false);
     }
     else {     
      $('#edit-menu-link-title-wrapper').hide();
      $('#edit-menu-parent-wrapper').hide();
      $('#edit-menu-link-title').val('');
      $('#edit-menu-delete').attr('checked', true);
     }
   });
  
  $('#edit-menu-link-title').keyup(function() {
    $(this).addClass('edited');
  })
  
  $('#edit-title').keyup(function() {
    var title = $(this).val();
    if(!$('#edit-menu-link-title').hasClass('edited') && $('#edit-menu-add').is(':checked')){
      $('#edit-menu-link-title').val(title);
    }
  });
  
  $('#edit-title').change(function() {
    $('#edit-title').keyup();
  });
  
};