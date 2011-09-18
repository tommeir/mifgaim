$(document).ready(function() {
    $('#edit-products-all').click(function() {
        new_value = false;
        
        if($('#edit-products-all').attr('checked')) {
            new_value = true;    
        }
        
        $('.form-checkbox').each(function() {
            if(this.id != 'edit-products-all') {
                $('#' + this.id).attr('checked', new_value);
            }
        });
    });
});