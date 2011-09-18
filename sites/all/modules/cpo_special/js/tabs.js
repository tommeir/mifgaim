$(document).ready(function() {
		//var icons = {
		//	header: "ui-icon-circle-arrow-e",
		//	headerSelected: "ui-icon-circle-arrow-s"
		//};
		//$("#views-accordion-ui").accordion({ 
	   	//    header: 'h3',
		//    event: "mouseover",
		//	fillSpace: true,
		//	icons: icons
		//});
		//$("#datatabs").accordion({ 
	   	//    header: 'div.title',
		//	autoHeight: false,
		//	navigation: true
		//});
		/*$(".block-content > .menu").lavaLamp({ 
		    returnHome:true ,
			target:'li > a',
			container:'li'
		});*/
        $("#block-menu-primary-links .block-content > .menu").lavaLamp({ 
		    returnHome:true ,
			target:'li > a',
			container:'li'
		});
		$("#tmore a").click(function(){
		  $(".tmore").toggle();
		  return false;
		});
		$("#formtabs").tabs();

        var target = window.location.hash; // look for a link
        if(target) {
		  $('h3'+target).parent().parent().siblings().show();
		  $.scrollTo($('h3'+target),{ speed:1000 });//scroll to it
		};
});