<?php
/**
 * @file
 * Default theme implementation to display a single Library Staff Member
 * (a potential replacement for the Django implementation)
 *
 * Available Variable(s):
 * $person - stdClass object with the following member attributes
 * - sid
 * - person_id
 * - netid
 * - firstname
 * - lastname
 * - displayname
 * - fullnamereversed
 * - jobtitle
 * - address
 * - phone
 * - phone2
 * - fax
 * - email
 * - email_privacy
 * - photo
 * - url
 * - profile (when available, this represents a staff member's custom profile text)
 *
 * Usage Example: $person->firstname
 */
?><div class="staff-member">
	<h3><?php print $person->displayname; ?></h3>
	<dl>
		<dt>Title</dt>
		<dd><?php print $person->jobtitle; ?></dd>
	</dl>
	<?php if(!is_null($person->profile)): ?>
		<?php if(!is_null($person->photo)): ?>
		<?php else: ?>
		<?php endif; ?>
	<? else: ?>
	<? endif; ?>
</div>
