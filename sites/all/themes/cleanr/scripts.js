$(document).ready(function(){
	var devlog = $('.dev-query,.dev-memory-usage').length;
	var help = $('#help').length;
	
	$('#extra-icons a').each(function(){
		var text = $(this).html();
		$(this).html("");
		$(this).after('<span>' + text + '</span>');
	})
	
	if (help > 0)
		$('.show-help').show();
	
	//alert(devlog);
	if (devlog > 0)
		$('.show-devel-query').show();
	var develshown = false;
	var helpshown = false;
	
	$('.show-devel-query a').click(function(){
		if (develshown == false) {
			$(this).addClass('active').parent().addClass('active');
			$('.dev-memory-usage,.devel-querylog,.dev-query').show();
			$('#wrapper').hide();
			develshown = true;
		} else {
			$(this).removeClass('active').parent().removeClass('active');
			$('.dev-memory-usage,.devel-querylog,.dev-query').hide();
			$('#wrapper').show();
			develshown = false;	
		}
		return false;
	});
	
	// Help button
	$('.show-help a').click(function(){
		if (helpshown == false) {
			$(this).addClass('active').parent().addClass('active');
			$('#help').show();
			helpshown = true;
		} else {
			$(this).removeClass('active').parent().removeClass('active');
			$('#help').hide();
			helpshown = false;	
		}
		return false;
	});	

});