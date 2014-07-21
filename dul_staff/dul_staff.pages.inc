<?php

/**
 * Displays a high-level staff directory
 */
function dul_staff_directory() {	
	$tree = dul_staff_directory_tree();
	return array(
		'#theme' => 'staff_directory',
		'#top_dept' => $tree['top_dept'],
		'#reporting_depts' => $tree['reporting_depts'], 
	);
}

/**
 * Page callback for Staff overview
 * Does nothing at the moment, although one could explore 
 * implementing a search facility
 */
function dul_staff_staff_overview() {
	$page = array();
	return $page();
}

/**
 * Page callback for department overview
 */
function dul_staff_dept_overview() {
	$page = array();
	return $page;
}

/**
 * Page callback that displays a Staff Person or returns the 
 * DisplayName of the Staff Person as a 'title' in response to 
 * a 'title callback'
 * 
 * @param stdClass $person A person object loaded from database
 * @param String $selector NULL or 'title'
 * @returns mixed (render array or string output)
 * 
 * @see dul_staff_person_load()
 */
function dul_staff_person(stdClass $person, $selector=NULL) {
	global $user;
	if ($selector == 'title') {
		return $person->display_name;
	}
	
	# at this point, there are at least 2 options
	# 1) build a render array and return it
	# 2) pass the $staff object to a theme function
	# 3) a hybrid of 1 and 2
	
	$tpl_settings = array('address_info_offset' => '', 'has_profile' => !empty($person->profile));
	if (!is_null($person->photo_url)) {
		$tpl_settings['address_info_offset'] = 'offset1';
	}
	
	$allow_user_edit = FALSE;

	if (module_exists('shib_auth')) {
		$eppn = explode('@', $_SERVER['eppn']);
		$allow_user_edit = ($eppn[0] == $person->net_id);
	}

	$page = array();

	$page['person'] = array(
		'#theme' => 'staff_person',
		'#person' => $person,
		'#settings' => $tpl_settings,
		'#allow_user_edit' => $allow_user_edit,
		'#uid' => $user->uid,
	);

	return $page;
}

function dul_staff_netid($netid) {
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
	
	$page['person'] = array(
		'#theme' => 'staff_person',
		'#person' => $staff,
	);
	
	return $page;
}

function dul_staff_page2($dept) {
	return print_r($dept, TRUE);
}

function dul_staff_dept(stdClass $dept, $selector=NULL) {
	if ($selector == 'title') {
		return $dept->name;
	}

	drupal_set_title($dept->name);
	
	$dept_head_html = '<h3>' . t($dept->head_display_name) . '</h3>';
	if ($dept->head_preferred_title != '') {
		$dept_head_html .= '<em>' . t($dept->head_preferred_title) . '</em>';
	}
	$dept_head_html .= '</h3>';
	
	$build['dept_head'] = array(
		'#type' => 'markup',
		'#markup' => $dept_head_html,
	);
	dpm($dept);
	$build['dept_address'] = array(
		'#theme' => 'staff_dept_head',
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
	foreach (array_merge($dept->members, $dept->allmembers) as $member) {
		$row = array();
		$row[] = l(t($member->display_name), 'about/directory/staff/' . $member->id);
		#$row[] = $member->displayname;
		$row[] = $member->title;
		$row[] = $member->phone;
		$row[] = ($member->email_privacy == NULL) ? $member->email : '';
		$rows[] = array(
			'data' => $row,
		);
	}
	$build['members'] = array(
		'#theme' => 'table',
		'#header' => $header,
		'#rows' => $rows,
		'#empty' => t('This department does not appear to have any staff members.'),
		'#attributes' => array(
			'class' => array('table', 'table-condensed'),
		)
	);
	return $build;
}
