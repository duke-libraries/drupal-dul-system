<div>
	<p>
		<?php print l(t('Floor map of the Perkins and Bostock Library buildings'), 'about/libraries/perkins-bostock-floors.pdf'); ?>
		<img src="/sites/default/files/dul/icons/pdficon_small.gif" height="17" width="17" alt="PDF Icon" title="PDF" />
	</p>
	<h2><?php print $stacks_table_title; ?></h2>
	<table class="table">
		<thead>
			<tr>
				<th>Call Number</th>
				<th>Where Is It?</th>
			</tr>
		</thead>
		<tbody>
			<?php foreach($stacks_locmaps as $locmap): ?>
			<tr>
				<td><?php print $locmap->callno_range; ?></td>
				<td>
					<?php print l(t($locmap->link_text), $locmap->url); ?>
				</td>
			</tr>
			<?php endforeach; ?>
		</tbody>
	</table>
	
	<h2><?php print $other_table_title; ?></h2>
	<table class="table">
		<thead>
			<tr>
				<th>Location</th>
				<th>Where Is It?</th>
			</tr>
		</thead>
		<tbody>
			<?php foreach($other_locmaps as $locmap): ?>
			<tr>
				<td><?php print $locmap->description; ?> <?php print $locmap->callno_range; ?></td>
				<td>
					<?php if (!empty($locmap->areamap_id)): ?>
						<?php print l(t($locmap->link_text), $locmap->url); ?>
					<?php else: ?>
						<?php if (!empty($locmap->generic_msg_id)): ?>
							<?php print $locmap->generic_msg_label; ?>&nbsp;
							(<?php print l(t(($locmap->generic_msg_id == 11) ? 'tutorial' : 'map'), $locmap->url); ?>)
						<?php else: ?>
							<?php print l(t($locmap->link_text), $locmap->url); ?>
						<?php endif; ?>
					<?php endif; ?>
				</td>
			</tr>
			<?php endforeach; ?>
		</tbody>
		
	</table>
</div>