/**
 * @file
 */
(function ($) {
	Drupal.behaviors.dul_system = {
		attach: function (context) {
			// wire up the Proactive Chat H3lp
			//

			/*
			$( 'window' ).popover({
				html : true,
				placement : 'bottom',
				content : function() {
					return $('#ph_popover').html();
				},
				trigger : 'manual',
				title : 'Test',
			});
			$( 'window' ).popover('show');
			*/
			
		  // TODO - put this in "Institution Skin" specific JS file
		  $( '#proactiveChatUserTrigger' ).click(function(evt) {
		  	$( '.dialog').modal('hide');
			  //$( '.dialog' ).dialog('close');
			  _gaq.push(['_trackEvent', 'ChatNow', 'ProactiveChat', 'Catalog']);
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
	    			.on('show', function() {
	    				// remove the modal event handler initially 
	    				// added by Bootstrap
	    				$(document).off('focusin.modal')	
	    			})
	    			.on('shown', function() {
	    				ph_setCookie(phChatCookieName, "opened", 1);
	    			})
	    			.on('hidden', function() {
	    			_gaq.push(['_trackEvent', 'Close', 'ProactiveChat', 'Catalog']);
	    		}).modal({backdrop: false}).modal('show');
	    		
	    		/*
	    		dialog.dialog({
	    			dialogClass : "proactiveH3lp",
	    			resizable : false,
	    			position: { my: "right bottom", at: "right bottom", of: window },
	    			buttons: {
	    				"No Thanks" : function() {
	    					// initiate google analytics event tracking
	    					_gaq.push(['_trackEvent', 'NoThanks', 'ProactiveChat', 'Catalog']);
	    					$( this ).dialog( 'close' );
	    				}
	    			},
	    			open: function(evt, ui) {
	    				// TODO set a cookie to indicate the patron closed the chat
	    				// so that we don't open the chat in future site visits
	    				ph_setCookie(phChatCookieName, "opened", 1);
	    			},
	    			close: function(evt, ui) {
	    				_gaq.push(['_trackEvent', 'Close', 'ProactiveChat', 'Catalog']);
	    			},
	    			show: {
	    				effect: 'fade',
	    				duration: 1000
	    			}
	    		});
	    		*/
	    	} else {
	    		setTimeout(showDialog, 15000);
	    	}
	    }
    	setTimeout(showDialog, 15000);
		}
	}
})(jQuery);
