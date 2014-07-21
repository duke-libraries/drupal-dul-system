<?php

/**
 * @file
 * Admin page callbacks and forms for dul_staff (directory)
 */

/**
 * Page callback for dul_staff_admin_staff_dept
 */
function dul_staff_admin_staff_dept($form, &$form_state) {
	db_set_active('django');
	
	$query = db_select('directory_orgunit', 'o')->extend('PagerDefault');
	$query->fields('o')
		->orderBy('name')
		->limit(25);
	$result = $query->execute();

		
	$depts = array();
	foreach ($result as $dept) {
		$depts[] = array(
			$dept->id,
			$dept->name,
			l(t('Edit'), 'admin/config/staff/dept/edit/' . $dept->id),
		);
	}
	
	db_set_active();

	$header = array(
		t('ID'),
		t('Name'),
		t('Action'),
	);
	
	$form['depts_table'] = array(
		'#theme' => 'table',
		'#header' => $header,
		'#rows' => $depts,
	);
	$form['depts_pager'] = array(
		'#theme' => 'pager',
	);
	return $form;
}

/**
 * Admin form for department editing
 */
function dul_staff_admin_dept_form($form, &$form_state, $dept, $model) {
	#$dept_options = dul_staff_load_dept_options();
	
	$form['dept_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Department Name'),
		'#description' => t('Enter the full name of this Department'),
		'#default_value' => $dept->name,
		'#required' => TRUE,
	);
	$form['head_id'] = array(
		'#type' => 'select',
		'#title' => t('Department Head'),
		'#description' => t('Select the Head for this Department'),
		'#default_value' => $dept->head_id,
		'#options' => $model['options']['person'],
	);
	$form['description'] = array(
		'#type' => 'textarea',
		'#title' => t('Department Description'),
		'#default_value' => $dept->description,
	);
	$form['parent_dept_id'] = array(
		'#title' => t('Parent Department'),
		'#type' => 'select',
		'#description' => t('Set this to the parent "Department" when applicable.'),
		'#options' => $model['options']['dept'],
		'#default_value' => $dept->parent_id,
	);
	$form['is_admin_unit'] = array(
		'#type' => 'checkbox',
		'#default_value' => $dept->is_admin_unit,
		'#title' => t('Administrative Department?'),
	);
	return $form;
}
