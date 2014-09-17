<!-- This needs-js div handles errors such as if the browser cannot reach the libraryh3lp server. -->
<div class="needs-js">Chat loading...</div>

<!-- Swap out YOUR-QUEUE-NAME for your real queue. This libraryh3lp div controls
what displays when your queue is AVAILABLE. -->

<div class="libraryh3lp" jid="duke-proactive@chat.libraryh3lp.com" style="display: none;">
<!-- The dialog id div is for your proactive chat invitation. Swap out YOUR-QUEUE-NAME
with your real queue name and 9999 with your desired Widget ID number. -->


	<!-- Markup for Bootstrap-modal dialog -->
	<p id="ph_popover">
  	<p class="proactiveHelpPrompt" style="text-align:center; font-size:1.3em;">Can we help you?</p>
  	<p>
  		<a id="proactiveChatUserTrigger" href="#">
  		<img style="margin-left:auto; margin-right:auto; display:block;" 
  				 border="0" alt="We're online, click to chat" 
  				 src="http://library.duke.edu/static/librarycatalog/images/chat-now3.png"/>
    	</a>
    </p>
    <p>No Thanks</p>		
	</p>
	<div id="ph_modal" class="dialog modal hide fade" tabindex="1" role="dialog" aria-labelledby="phModalLabel" aria-hidden="true">
		<div class="modal-header">
			<button type="button" class="close" data-dismiss="modal" aria-hidden="true">x</button>
			<h3 id="phModalLabel">Duke University Libraries</h3>
		</div>
		<div class="modal-body">
	  	<p class="proactiveHelpPrompt" style="text-align:center; font-size:1.3em;">Can we help you?</p>
	  	<a id="proactiveChatUserTrigger" href="#">
	  		<img style="margin-left:auto; margin-right:auto; display:block;" 
	  				 border="0" alt="We're online, click to chat" 
	  				 src="http://library.duke.edu/static/librarycatalog/images/chat-now3.png"/>
	    </a>		
		</div>
		<div class="modal-footer text-center">
			<button class="btn" data-dismiss="modal" aria-hidden="true">No Thanks</button>
		</div>
	</div>	

  <!--div class="dialog" style="display: none; background-color:#F8F8F8;">
  	<p class="proactiveHelpPrompt" style="text-align:center; font-size:1.3em;">Can we help you?</p>
  	<a id="proactiveChatUserTrigger" href="#">
  		<img style="margin-left:auto; margin-right:auto; display:block;" border="0" alt="We're online, click to chat" 
  			 src="http://library.duke.edu/static/librarycatalog/images/chat-now3.png"/>
    </a>
  </div-->

  <!-- Put your regular (non-proactive) in-page chat code here. This example uses a pop-out
  widget (swap out your real queue name and widget ID number) arising from a small image. Leave
  this out to ONLY use proactive chat. -->

    <!--p style="font-size:0.83m;">Optional regular in-page chat option:</p>
    <a href="#"
     onclick="window.open('https://libraryh3lp.com/chat/duke-proactive@chat.libraryh3lp.com?skin=21014',
     'chat', 'resizable=1,width=285,height=325'); return false;">
      <img border="0" alt="We're online, click to chat" src="http://chatstaff.us/testbed/hosted-presence-images/small-square-green-bubble.png">
    </a-->

</div> <!-- end libraryh3lp div -->
<!-- Place the HTML you want to show when your chat queue is offline here. -->

<div class="libraryh3lp" style="display: none;"></div>
<!-- LibraryH3lp JavaScript. This handles checking the availability of your queue. -->
<script type="text/javascript">
  (function() {
    var x = document.createElement("script"); x.type = "text/javascript"; x.async = true;
x.src = (document.location.protocol === "https:" ? "https://" : "http://") + "libraryh3lp.com/js/libraryh3lp.js?multi,poll";
var y = document.getElementsByTagName("script")[0]; y.parentNode.insertBefore(x, y);
  })();

  function ph_setCookie(cname, cvalue, exdays) {
	  var d = new Date();
	  d.setTime(d.getTime() + (exdays * 24 * 60 * 60 * 1000));
	  var expires = "expires=" + d.toGMTString();
	  document.cookie = cname + "=" + cvalue + "; " + expires;
  }
  function ph_getCookie(cname) {
	  var name = cname + "=";
	  var ca = document.cookie.split(';');
	  for (var i=0; i < ca.length; i++) {
		  var c = ca[i];
		  while (c.charAt(0) == ' ') c = c.substring(1);
		  if (c.indexOf(name) != -1) return c.substring(name.length, c.length);
	  }
	  return "";
  }
  
  // TODO put this in institution-skin based file
  var phChatCookieName = "ph_opened";
  function ph_patronDidNotOpenChat() {
	  var closedChat = ph_getCookie(phChatCookieName);
	  return closedChat.length == 0;
  }
</script>
