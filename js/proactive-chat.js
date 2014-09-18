/**
 * @file
 */
(function ($) {
	Drupal.behaviors.dul_system = {
		attach: function (context) {
			// wire up the Proactive Chat H3lp
			//

		  // TODO - put this in "Institution Skin" specific JS file
		  $( '#proactiveChatUserTrigger' ).click(function(evt) {
		  	$( '.dialog').modal('hide');
	    	ga('send', 'event', { eventCategory: 'Catalog', eventLabel: 'ProactiveChat', eventAction: 'ChatNow'});
			  window.open(
					  'https://libraryh3lp.com/chat/duke-proactive@chat.libraryh3lp.com?skin=21014', 
					  'chat', 
					  'resizable=1,width=285,height=325');
			  return evt.preventDefault();
		  });
		  // end TODO
		  
		  var dialogs = $('.dialog');
	
		  function showDialog() {
	    	var dialog = dialogs.filter(function() {
	    		return $(this).parent().is(':visible');
	    	});
	    	//var dialog = dialogs;
	
	    	// TODO check for the cookie that indicated the patron
	    	// closed the chat dialog in a previous visit.
	    	if (dialog.length > 0 && ph_patronDidNotOpenChat()) {
	    		dialog
	    			.on('shown', function() {
	    				$(document).off('focusin.modal');
	    				ph_setCookie(phChatCookieName, "opened", 1);
	    			})
	    			.on('hidden', function() {
	    				ga('send', 'event', { eventCategory: 'Catalog', eventLabel: 'ProactiveChat', eventAction: 'close'});
	    		}).modal({backdrop: false}).modal('show');
	    	} else {
	    		setTimeout(showDialog, 10000);
	    	}
	    }
    	setTimeout(showDialog, 10000);
		}
	}
})(jQuery);
