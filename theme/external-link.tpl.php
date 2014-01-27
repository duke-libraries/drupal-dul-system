<?php
/**
 * @file
 *
 * Default template to display an External Link Location Guide
 */
?>
<div class="externallink_content">
	<?php print t($extLink->extlink_content); ?>
</div>
<div class="externallink_url">
	<a href="<?php print $extLink->extlink_url; ?>" target="_blank"><?php print $extLink->extlink_link_text; ?></a>
</div>
