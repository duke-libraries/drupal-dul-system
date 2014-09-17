<?php

/**
 * Page callback for admin/config/locguide/sublibrary
 */
function locguide_admin_genericmessages() {
	
	db_set_active('django');
	$query = db_select('locationguide_genericmessage', 'm')->extend('PagerDefault')->limit(15);
	$query->fields('m');
	$query->orderBy('m.label');
	$result = $query->execute();
	
	$rows = array();
	foreach($result as $message) {
		$rows[] = array(
			t($message->label),
			l(t('edit'), 'admin/config/locguide/genericmessage/' . $message->id . '/edit'),
		);
	}
	db_set_active();
	
	$page['table'] = array(
		'#theme' => 'table',
		'#header' => array(t('Label'), t('Action(s)')),
		'#rows' => $rows,
		'#empty' => t('No Generic Message records were discovered.'),
	);
	$page['pager'] = array(
		'#theme' => 'pager',
	);
	return $page;
}

/**
 * Page callback for admin/config/locguide/collgroup/%/edit
 */
function locguide_admin_genericmessage_form($form, &$form_state, $genericmessage, $model) {
	if (!isset($form_state['storage']['confirm'])) {
		if ($genericmessage == 'new') {
			$genericmessage = new stdClass;
			$genericmessage->id = '';
			$genericmessage->content = '';
			$genericmessage->label = '';
			$form_state['genericmessage']['new'] = TRUE;
		}
		$form_state['referer'] = $_SERVER['HTTP_REFERER'];
		$form['label'] = array(
			'#type' => 'textfield',
			'#title' => t('Label'),
			'#description' => t('Brief identifier for use in selection lists.'),
			'#default_value' => $genericmessage->label,
			'#required' => TRUE,
		);
		$form['content'] = array(
			'#type' => 'textarea',
			'#title' => t('Content'),
			'#default_value' => $genericmessage->content,
			'#required' => TRUE,
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
			t('Are you sure you want to delete this Generic Message...?'), 
			$path,
			t('This action can not be undone.')
		);
	}
}

function locguide_admin_genericmessage_form_submit($form, &$form_state) {
	drupal_set_message('Faux save attempt');
}
