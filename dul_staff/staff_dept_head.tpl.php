<?php

?>

<address>
	<?php if (isset($dept->postal_address)): ?>
	<div class="personInfo personAddress personBox" 
			 itemprop="address" 
			 itemtype="http://schema.org/PostalAddress" 
			 property="schema:address"
			 typeof="schema:PostalAddress">

		<div property="schema:streetAddress"><?php print $dept->postal_address['office']; ?></div>
		<?php if (isset($dept->postal_address['box'])): ?>
			<div property="schema:postOfficeBoxNumber"><?php print $dept->postal_address['box']; ?></div>
		<?php endif; ?>
		<span property="schema:addressLocality"><?php print $dept->postal_address['city']; ?></span>,
		<span property="schema:addressRegion"><?php print $dept->postal_address['state']; ?></span>&nbsp;
		<span property="schema:postalCode">
		<?php if(isset($dept->postal_address['zip_plus_4'])): ?><?php print $dept->postal_address['zip_plus_4']; ?>
		<?php else: ?><?php print $dept->postal_address['zipcode']; ?>
		<?php endif; ?>
		</span>
	<?php endif; // POSTAL ADDRESS ?>
	</div>
</address>
<div>
	<div property="schema:telephone" class="personInfo personPhone">
		<strong>Phone:&nbsp;</strong><?php print $dept->phone; ?>
	</div>
	<div property="schema:faxNumber" class="personInfo personFax">
		<strong>Fax:&nbsp;</strong><?php print $dept->fax; ?>
	</div>
	<?php if (empty($dept->email_privacy) && !empty($dept->dept_head_email)): ?>
	<div class="personInfo personEmail"><a property="schema:email" href="mailto:<?php print $dept->dept_head_email; ?>"><?php print $dept->dept_head_email; ?></a></div>
	<?php endif; ?>
</div>