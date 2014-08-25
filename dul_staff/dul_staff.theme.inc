<?php

function theme_staff_dept_address($variables) {
	$dept = $variables['dept'];
	$out = '<address>';
	if ($dept->physical_address != '') {
		//$out .= '<abbr title="Campus Box">CB</abbr>: ' . $dept->physical_address . '<br />';
		$out .= $dept->physical_address . '<br />';
	}
	if ($dept->phone != '') {
		$out .= '<abbr title="Phone">Phone</abbr>: ' . $dept->phone . '<br />';
	}
	if ($dept->fax != '') {
		$out .= '<abbr title="Fax">Fax</abbr>: ' . $dept->fax . '<br />';
	}
	if ($dept->dept_email != '') {
		$out .= '<abbr title="Email">E</abbr>: ' . $dept->dept_email;
	}
	$out .= '</address>';
	return $out;
}

function theme_staff_directory($variables) {
	$header = array(
		t('DEPARTMENTS'),
		t('TELEPHONE'),
		t('LOCATION'),
		t('DIRECTOR/HEAD'),
	);
	
	extract($variables);

	$rows = array();
	foreach ($reporting_depts as $dept) {
		$row = array(
			'data' => array(
				array('data' => l(t(strtoupper($dept->name)), 'about/directory/dept/' . $dept->id), 'colspan' => 3, 'header' => TRUE,),
				array('data' => l(t($dept->head_displaytitle), 'about/directory/staff/' . $dept->head_person_id), 'header' => TRUE,),
			),
		);
		$rows[] = $row;
		foreach ($dept->child_depts as $o) {
			$rows[] = array(
				'data' => array(
					l(t($o->name), 'about/directory/dept/' . $o->id),
					t($o->phone),
					t($o->physical_address),
					l(t($o->head_displaytitle), 'about/directory/staff/' . $o->head_person_id),
				)
			);
		}
	}
	
	$table = theme('table', 
		array(
			'header' => $header,
			'rows' => $rows, 
			'attributes' => array('class' => array('table'))
		)
	);
	return render($table);
}

/**
 * Default Theme implementation for staff search results
 * 
 */
function theme_staff_search_results($variables) {
	return 'Staff results here';
}
