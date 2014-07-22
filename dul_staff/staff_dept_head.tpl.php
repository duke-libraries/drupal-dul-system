<?php

?>
<!-- START DEPARTMENT HEAD NAME (and TITLE, optional) -->
<h2>
	<a href="/about/directory/staff/<?php print $dept->head_person_id; ?>"><?php print $dept->head_display_name; ?></a>
	<?php if(!empty($dept->head_preferred_title)): ?>
		<br /><small><?php print $dept->head_preferred_title; ?></small>
	<?php endif; ?>
</h2>
<!-- /DEPARTMENT HEAD -->

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
<?php if (!empty($dept->phone)): ?>
<div property="schema:telephone" class="personInfo personPhone">
	<strong>Phone:&nbsp;</strong><?php print $dept->phone; ?>
</div>
<?php endif; ?>
<?php if (!empty($dept->fax)): ?>
<div property="schema:faxNumber" class="personInfo personFax">
	<strong>Fax:&nbsp;</strong><?php print $dept->fax; ?>
</div>
<?php endif; ?>
<?php if (empty($dept->email_privacy) && !empty($dept->dept_email)): ?>
<div class="personInfo personEmail"><a property="schema:email" href="mailto:<?php print $dept->dept_head_email; ?>"><?php print $dept->dept_head_email; ?></a></div>
<?php endif; ?>
