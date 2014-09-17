<?php


/**
 * Page callback for admin/config/locguide/collgroup
 */
function locguide_admin_collgroups() {
	//$collgroups = CollectionGroupManager::collectionGroups(NULL, TRUE);
	
	db_set_active('django');
	$query = db_select('locationguide_collectiongroup', 'cg')->extend('PagerDefault')->limit(15);
	$query->fields('cg');
	$query->leftJoin('locationguide_sublibrary', 'sl', 'sl.id = cg.sublibrary_id');
	$query->addField('sl', 'code');
	$query->addField('sl', 'code', 'sublibrary_code');
	$query->addField('sl', 'label', 'sublibrary_label');
	$query->orderBy('sl.label');
	$query->orderBy('cg.description');
	$result = $query->execute();
	
	$rows = array();
	foreach($result as $cg) {
		$rows[] = array(
			t(sprintf('%s %s', $cg->sublibrary_label, $cg->description)),
			t($cg->description),
			l(t('Edit'), 'admin/config/locguide/collgroup/' . $cg->id . '/edit'),
		);
	}
	db_set_active();
	
	$page['table'] = array(
		'#theme' => 'table',
		'#header' => array(t('Collection Group'), t('Description'), t('Action(s)')),
		'#rows' => $rows,
		'#empty' => t('No Collection Group records were discovered.'),
	);
	$page['pager'] = array(
		'#theme' => 'pager',
	);
	return $page;
}

/**
 * Page callback for admin/config/locguide/collgroup/%/edit
 */
function locguide_admin_collgroup_form($form, &$form_state, $collgroup, $model) {
	if ($collgroup == 'new') {
		$collgroup = new stdClass;
		$collgroup->id = NULL;
		$collgroup->description = '';
		$collgroup->sublibrary_id = '';
		$form_state['collgroup']['new'] = TRUE;
	}
	$form['sublibrary_id'] = array(
		'#type' => 'select',
		'#title' => t('Sublibrary'),
		'#options' => $model['options']['sublibrary'],
		'#default_value' => $collgroup->sublibrary_id,
		'#required' => TRUE,
		'#empty_value' => t(''),
		'#empty_option' => t('-- Select Sublibrary --'),
	);
	$form['description'] = array(
		'#type' => 'textfield',
		'#title' => t('Description'),
		'#description' => t('Optional qualification to distinguish multiple collection groups for a sublibrary.'),
		'#default_value' => $collgroup->description,
	);
	$form['collgroup_id'] = array(
		'#type' => 'hidden',
		'#value' => $collgroup->id,
	);
	$form['actions'] = array(
		'#type' => 'actions'
	);
	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);
	#$form['actions']['delete'] = array(
	#	'#type' => 'submit',
	#	'#value' => t('Delete'),
	#);
	$form['actions']['cancel'] = array(
		'#type' => 'markup',
		'#markup' => l(t('Cancel'), 'admin/config/locguide/collgroup'),
	);
	return $form;
}

/**
 * Submit handler for locguide_admin_collgroup_form
 */
function locguide_admin_collgroup_form_submit($form, &$form_state) {
	if ($form_state['values']['op'] == 'Submit') {
		// save record
		db_set_active('django');
		$query = db_merge('locationguide_collectiongroup')
			->key(array('id' => $form_state['values']['collgroup_id']))
			->fields(array(
				'id' => $form_state['values']['collgroup_id'],
				'description' => $form_state['values']['description'],
				'sublibrary_id' => $form_state['values']['sublibrary_id']
			));
			
		$result = $query->execute();

		dpm(_locguide_load_content_types());
		
		db_set_active();
		drupal_set_message('Changes have been saved.');
		$form_state['redirect'] = 'admin/config/locguide/collgroup';
	}
}
