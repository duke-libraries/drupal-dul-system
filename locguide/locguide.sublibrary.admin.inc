<?php

/**
 * Page callback for admin/config/locguide/sublibrary
 */
function locguide_admin_sublibraries() {
	
	db_set_active('locationguide');
	$query = db_select('locationguide_sublibrary', 'sl')->extend('PagerDefault')->limit(15);
	$query->fields('sl');
	$query->orderBy('sl.label');
	$result = $query->execute();
	
	$rows = array();
	foreach($result as $sl) {
		$rows[] = array(
			t($sl->label),
			$sl->collgroup_choice ? t('Yes') : t('No'),
			l(t('Edit'), 'admin/config/locguide/sublibrary/' . $sl->id . '/edit'),
		);
	}
	db_set_active();
	
	$page['table'] = array(
		'#theme' => 'table',
		'#header' => array(t('Sublibrary'), t('Collection Group Choice'), t('Action(s)')),
		'#rows' => $rows,
		'#empty' => t('No Sublibrary records were discovered.'),
	);
	$page['pager'] = array(
		'#theme' => 'pager',
	);
	return $page;
}

/**
 * Page callback for admin/config/locguide/collgroup/%/edit
 */
function locguide_admin_sublibrary_form($form, &$form_state, $sublibrary, $model) {
	if (!isset($form_state['storage']['confirm'])) {
		if ($sublibrary == 'new') {
			$sublibrary = new stdClass;
			$sublibrary->id = '';
			$sublibrary->code = '';
			$sublibrary->label = '';
			$sublibrary->collgroup_choice = 0;
			$sublibrary->default_locmap_id = NULL;
			$form_state['sublibrary']['new'] = TRUE;
		}
		$form_state['referer'] = $_SERVER['HTTP_REFERER'];
		$form['code'] = array(
			'#type' => 'textfield',
			'#title' => t('Code'),
			'#default_value' => $sublibrary->code,
			'#required' => TRUE,
		);
		$form['label'] = array(
			'#type' => 'textfield',
			'#title' => t('Label'),
			'#default_value' => $sublibrary->label,
			'#required' => TRUE,
		);
		$form['collgroup_choice'] = array(
			'#type' => 'checkbox',
			'#value' => $sublibrary->collgroup_choice,
			'#title' => t('Collection Group Choice'),
			'#description' => t('Include in SubLibrary selection list for Collection Groups.'),
		);
		$form['default_locmap_id'] = array(
			'#type' => 'select',
			'#options' => $model['options']['locationmap'],
			'#empty_value' => t(''),
			'#empty_option' => t('-- Select Location Map --'),
			'#default_value' => $sublibrary->default_locmap_id,
			'#title' => t('Default Location Map'),
		);
		$form['actions'] = array(
			'#type' => 'actions'
		);
		$form['actions']['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Save Changes'),
		);
		$form['actions']['cancel'] = array(
			'#type' => 'markup',
			'#markup' => l(t('Cancel'), 'admin/config/locguide/sublibrary'),
		);
		//$form['actions']['delete'] = array(
		//	'#type' => 'submit',
		//	'#value' => t('Delete'),
		//);
		return $form;
	} else {
		$path = current_path();
		return confirm_form(
			$form, 
			t('Are you sure you want to delete this Sublibrary...?'), 
			$path,
			t('This action can not be undone.')
		);
	}
}

function locguide_admin_sublibrary_form_submit($form, &$form_state) {
	drupal_set_message('Faux save attempt');
}
