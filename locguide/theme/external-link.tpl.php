<?php
/**
 * @file
 *
 * Default template to display an External Link Location Guide
 */
?>
<div class="externallink_content">
	<p><?php print t($extLink->generic_msg_content); ?></p>
</div>
<div class="externallink_url">
	<a href="<?php print $extLink->extlink_url; ?>" target="_blank"><?php print $extLink->extlink_text; ?></a>
</div>
