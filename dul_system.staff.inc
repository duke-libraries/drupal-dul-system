<?php

/**
 * Displays a high-level staff directory
 */
function dul_system_staff_directory() {
	// get the top level directory (parent = NULL)

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

function dul_system_staff_page($dept) {
	drupal_set_title($dept->title);
	return t('Coming soon!');
	
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
				array('data' => t(strtoupper($dept->title)), 'colspan' => 3, 'header' => TRUE,),
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
