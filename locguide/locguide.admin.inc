<?php

function locguide_admin_settings_form($form, &$form_state) {
	$form['locate-materials'] = array(
		'#type' => 'fieldset',
		'#title' => t('Locate Materials'),
		'#description' => t('Settings for the <em>Locate Materials</em> page'),
		'#collapsible' => TRUE,
		'#collapsed' => FALSE,
	);
	$form['locate-materials']['perkins-stacks-title'] = array(
		'#type' => 'textfield',
		'#title' => t('Title for the Perkins Stacks Table'),
		'#default_value' => variable_get('locguide.perkins_stacks_title', 'Perkins and Bostock Stacks'),
	);
	$form['locate-materials']['other-stacks-title'] = array(
		'#type' => 'textfield',
		'#title' => t('Title for the Other Stacks Table'),
		'#default_value' => variable_get('locguide.other_stacks_title', 'Other Locations in Perkins and Bostock Libraries'),
	);
	$form['actions'] = array(
		'#type' => 'actions',
	);
	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);
	return $form;
}

function locguide_admin_settings_form_submit($form, &$form_state) {
	variable_set('locguide.perkins_stacks_title', $form_state['values']['perkins-stacks-title']);
	variable_set('locguide.other_stacks_title', $form_state['values']['other-stacks-title']);
}

function locguide_admin_areamaps() {
	db_set_active('django');

	$areamaps = array();

	$query = db_select('locationguide_areamap', 'am')->extend('PagerDefault');
	$query->fields('am');
	$query->limit(20);

	$result = $query->execute();
	foreach ($result as $map) {
		$areamaps[] = array( 'data' => array(
			t($map->building),
			t($map->area),
			t($map->image),
			l(t('Edit'), 'admin/config/locguide/areamap/' . $map->id . '/edit')
		));
	}
	db_set_active();

	$page['table'] = array(
		'#theme' => 'table',
		'#header' => array(t('Building'), t('Area'), t('Image Path'), t('Action(s)')),
		'#rows' => $areamaps,
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
