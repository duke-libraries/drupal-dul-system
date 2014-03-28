<?php

function locguide_admin_locmaps() {
	db_set_active('locationguide');

	$locmaps = array();

	$query = db_select('locationguide_locationmap', 'm')->extend('PagerDefault');
	$query->fields('m');
	$query->join('locationguide_collectiongroup', 'cg', 'cg.id = m.collection_group_id');
	$query->join('locationguide_sublibrary', 'sl', 'sl.id = cg.sublibrary_id');
	$query->leftJoin('locationguide_externallink', 'el', 'm.external_link_id = el.id');
	$query->leftJoin('locationguide_areamap', 'am', 'am.id = m.map_id');
	$query->addField('cg', 'sublibrary_id');
	$query->addField('cg', 'description', 'collgroup_description');
	$query->fields('am', array('building', 'area'));
	$query->addField('sl', 'label', 'sublibrary_label');
	$query->addField('el', 'title', 'extlink_title');
	$query->fields('m');
	$query->limit(20);

	try {
		$result = $query->execute();
	} catch(Exception $x) {
		dpm($x->getMessage());
	}
	foreach ($result as $locmap) {
		$locmaps[] = array(
			t(sprintf("%s %s", $locmap->sublibrary_label, $locmap->collgroup_description)),
			is_null($locmap->map_id) ? t('-') : t(sprintf("%s %s", $locmap->sublibrary_label, $locmap->area)),
			t($locmap->extlink_title),
			l(t('edit'), 'admin/config/locguide/locmap/' . $locmap->id . '/edit')
		);
		#dpm($locmap);
	}
	db_set_active();

	$page['table'] = array(
		'#theme' => 'table',
		'#header' => array(t('Location Map'), t('Map'), t('External Link'), t('Action(s)')),
		'#rows' => $locmaps,
		'#empty' => t('No Areamap records were discovered.'),
	);
	$page['pager'] = array(
		'#theme' => 'pager',
	);
	return $page;
}

/**
 * Edit form for Location Guide / Area Map
 */
function locguide_admin_areamap_form($form, &$form_state, $areamap = NULL) {
	$return_path = current_path();

	if (!isset($form_state['storage']['confirm'])) {
		$form['building'] = array(
			'#type' => 'select',
			'#options' => LocGuideModel::Buildings(),
			'#empty_option' => t('Select a Building...'),
			'#default_value' => !is_null($areamap) ? $areamap->building : '',
			'#title' => t('Building'),
			'#required' => TRUE,
		);
		$form['areamap'] = array(
			'#type' => 'textfield',
			'#default_value' => !is_null($areamap) ? $areamap->area : '',
			'#title' => t('Area'),
			'#size' => 60,
			'#required' => TRUE,
		);
		$form['image'] = array(
			'#type' => 'textfield',
			'#default_value' => !is_null($areamap) ? $areamap->image : '',
			'#title' => t('Image'),
			'#size' => 60,
			'#required' => TRUE,
		);
		$form['areamap_id'] = array(
			'#type' => 'hidden',
			'#value' => !is_null($areamap) ? $areamap->id : '',
		);
		$form['actions'] = array(
			'#type' => 'actions',
		);
		$form['actions']['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Save'),
		);
		if (!is_null($areamap)) {
			$form_state['locguide']['areamap_id'] = $areamap->id;
			$form['actions']['delete'] = array(
				'#type' => 'submit',
				'#value' => t('Delete Area Map'),
			);
		}
		return $form;
	} else {
		$form['intro'] = array(
			'#type' => 'markup',
			'#markup' => t('<p>Are you sure you want to delete this area map record...?</p>')
		);
		return confirm_form(
			$form,
			$question = t('Are you sure...?'),
			$return_path
		);
	}
}

/**
 * Validation callback for areamap form
 */
function locguide_admin_areamap_form_validate($form, &$form_state) {
}

/**
 * Submit callback for areamap form
 */
function locguide_admin_areamap_form_submit($form, &$form_state) {
	if (!isset($form_state['storage']['confirm'])) {
		if ($form_state['values']['op'] == 'Delete Area Map') {
			$_SESSION['areamap_id'] = $form_state['values']['areamap_id'];
			$form_state['storage']['confirm'] = TRUE;
			$form_state['rebuild'] = TRUE;
		} else {
			# save the area map data
			drupal_set_message(t('Save the area map data...'));
		}
	} else {
		$areamap_id = $_SESSION['areamap_id'];
		drupal_set_message('Area Map ID = ' . $areamap_id);
	}
}
