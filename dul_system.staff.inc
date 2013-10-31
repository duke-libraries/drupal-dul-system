<?php

/**
 * Displays a high-level staff directory
 */
function dul_system_staff_directory() {
	// get the top level directory (parent = NULL)

	# query the staff department table
	$q = db_select('dul_dept', 'd');
	$q->leftJoin('staff', 's', 'd.head_person_id = s.person_id');
	$q->fields('d');
	$q->addField('s', 'displayname', 'head_displaytitle');
	$q->addField('s', 'jobtitle', 'head_jobtitle');
	$q->condition('parent_did', '0');
	$res = $q->execute();
		
	$top_dept = $res->fetchObject();
	
	// next, get the departments directly underneath
	$q = db_select('dul_dept', 'd');
	$q->leftJoin('staff', 's', 'd.head_person_id = s.person_id');
	$q->fields('d');
	$q->addField('s', 'displayname', 'head_displaytitle');
	$q->addField('s', 'jobtitle', 'head_jobtitle');
	$q->condition('parent_did', $top_dept->dept_id);
	$res = $q->execute();
	
	$reporting_depts = array();
	foreach ($res as $dept) {
		$q = db_select('dul_dept', 'd');
		$q->leftJoin('staff', 's', 'd.head_person_id = s.person_id');
		$q->fields('d');
		$q->addField('s', 'displayname', 'head_displaytitle');
		$q->addField('s', 'jobtitle', 'head_jobtitle');
		$q->condition('parent_did', $dept->dept_id);
		$q->orderBy('d.title');
		$sub_res = $q->execute();
		
		$dept->child_depts = array();
		foreach ($sub_res as $o) {
			$dept->child_depts[] = $o;
		}

		$reporting_depts[] = $dept;
	}
	
	return array(
		'#theme' => 'staff_directory',
		'#top_dept' => $top_dept,
		'#reporting_depts' => $reporting_depts, 
	);
}

function dul_system_staff_netid($netid) {
	if ($netid == '') {
		// go somewhere else
		// or
		drupal_set_message('Please specify a netid!', 'error');
	}
	
	$res = db_query('SELECT * FROM staff WHERE netid = :netid', array(':netid' => $netid));
	
	# OR
	
	//$q = db_select('staff', 's');
	//$q->fields('s');
	//$q->condition('netid', $netid);
	//$res = $q->execute();
	
	$staff = $res->fetchObject(); // or fetchAssoc()
	
	# at this point, there are at least 2 options
	# 1) build a render array and return it
	# 2) pass the $staff object to a theme function
	# 3) a hybrid of 1 and 2
	
	$page = array();
	$page['staff'] = array(
		'#type' => 'markup',	// or, 'container' (<div> element), 
		'#markup' => '<div>' . $staff->displayname . '</div>',
	);
	
	$page['staff_person'] = array(
		'#theme' => 'staff_person',
		'#staff_person' => $staff,
	);
	
	return $page;
}

/**
 * Theme function for the 'staff_person' theme
 */
function theme_staff_person($variables) {
	extract($variables);
	return '<h3>' . $staff_person->displayname . '</h3><small>' . $staff_person->jobtitle . '</small>';
	
}

function dul_system_staff_page2($dept) {
	return print_r($dept, TRUE);
}

function dul_system_staff_page($dept) {
	drupal_set_title($dept->title);
	
	$dept_head_html = '<h3>' . t($dept->displayname);
	if ($dept->jobtitle != '') {
		$dept_head_html .= ', <small><em>' . t($dept->jobtitle) . '</em></small>';
	}
	$dept_head_html .= '</h3>';
	
	$build['dept_head'] = array(
		'#type' => 'markup',
		'#markup' => $dept_head_html,
	);
	$build['dept_address'] = array(
		'#theme' => 'staff_dept_address',
		'#dept' => $dept,
	);
	
	// Create a table to display the members of this department
	$header = array(
		t('Name'),
		t('Title'),
		t('Phone'),
		t('Email'),
	);
	$rows = array();
	foreach ($dept->members as $member) {
		$row = array();
		$row[] = l(t($member->displayname), 'http://library.duke.edu/apps/directory/staff/' . $member->person_id);
		#$row[] = $member->displayname;
		$row[] = $member->jobtitle;
		$row[] = $member->phone;
		$row[] = ($member->email_privacy == 'False') ? $member->email : '';
		$rows[] = array(
			'data' => $row,
		);
	}
	$build['members'] = array(
		'#theme' => 'table',
		'#header' => $header,
		'#rows' => $rows,
		'#empty' => t('This department reportedly has no staff members.'),
		'#attributes' => array(
			'class' => array('table', 'table-condensed'),
		)
	);
	return $build;
}

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
				array('data' => l(t(strtoupper($dept->title)), 'about/staff/dept/' . $dept->dept_id), 'colspan' => 3, 'header' => TRUE,),
				array('data' => t($dept->head_displaytitle), 'header' => TRUE,),
			),
		);
		$rows[] = $row;
		foreach ($dept->child_depts as $o) {
			$rows[] = array(
				'data' => array(
					l(t($o->title), 'about/staff/dept/' . $o->dept_id),
					t($o->phone),
					t($o->address),
					t($o->head_displaytitle),
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
