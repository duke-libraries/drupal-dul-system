<?php

function theme_staff_dept_address($variables) {
	$dept = $variables['dept'];
	$out = '<address>';
	if ($dept->address != '') {
		$out .= '<abbr title="Campus Box">CB:</abbr>' . $dept->address . '<br />';
	}
	if ($dept->phone != '') {
		$out .= '<abbr title="Phone">P:</abbr>' . $dept->phone . '<br />';
	}
	if ($dept->fax != '') {
		$out .= '<abbr title="Fax">F:</abbr>' . $dept->fax . '<br />';
	}
	if ($dept->dept_email != '') {
		$out .= '<abbr title="Email">E:</abbr>' . $dept->dept_email;
	}
	$out .= '</address>';
	return $out;
}

function theme_staff_directory($variables) {
	$header = array(
		t('DEPARTMENT'),
		t('TELEPHONE'),
		t('LOCATION'),
		t('DIRECTOR/HEAD'),
	);
	
	extract($variables);
	$rows = array();
	foreach ($reporting_depts as $dept) {
		$row = array(
			'data' => array(
				array('data' => l(t(strtoupper($dept->title)), 'about/dept/' . $dept->dept_id), 'colspan' => 3, 'header' => TRUE,),
				array('data' => l(t($dept->head_displaytitle), 'about/directory/staff/' . $dept->head_person_id), 'header' => TRUE,),
			),
		);
		$rows[] = $row;
		foreach ($dept->child_depts as $o) {
			$rows[] = array(
				'data' => array(
					l(t($o->title), 'about/dept/' . $o->dept_id),
					t($o->phone),
					t($o->address),
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

