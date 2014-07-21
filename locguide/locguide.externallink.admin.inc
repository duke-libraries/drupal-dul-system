<?php

/**
 * Page callback for admin/config/locguide/sublibrary
 */
function locguide_admin_externallinks() {
	
	db_set_active('locationguide');
	$query = db_select('locationguide_externallink', 'l')->extend('PagerDefault')->limit(15);
	$query->fields('l');
	$query->leftJoin('locationguide_genericmessage', 'm', 'm.id = l.generic_msg_id');
	$query->addField('m', 'id', 'generic_msg_id');
	$query->orderBy('l.title');
	$result = $query->execute();
	
	$rows = array();
	foreach($result as $l) {
		$rows[] = array(
			t($l->title),
			t($l->link_text),
			t($l->url),
			l(t('Edit'), 'admin/config/locguide/externallink/' . $l->id . '/edit'),
		);
	}
	db_set_active();
	
	$page['table'] = array(
		'#theme' => 'table',
		'#header' => array(t('Title'), t('Link Text'), t('URL'), t('Action(s)')),
		'#rows' => $rows,
		'#empty' => t('No External Link records were discovered.'),
	);
	$page['pager'] = array(
		'#theme' => 'pager',
	);
	return $page;
}

/**
 * Page callback for admin/config/locguide/externallink/%/edit
 */
function locguide_admin_externallink_form($form, &$form_state, $externallink, $model) {
	if (!isset($form_state['storage']['confirm'])) {
		if ($externallink == 'new') {
			$externallink = new stdClass;
			$externallink->id = NULL;
			$externallink->title = '';
			$externallink->link_text = '';
			$externallink->content = '';
			$externallink->url = 0;
			$externallink->generic_msg_id = NULL;
			$form_state['externallink']['new'] = TRUE;
		}
		
		$form['#tree'] = TRUE;
		$form['url'] = array(
			'#type' => 'textfield',
			'#value' => $externallink->url,
			'#title' => t('URL'),
			'#required' => TRUE,
		);
		$form['title'] = array(
			'#type' => 'textfield',
			'#title' => t('Title'),
			'#default_value' => $externallink->title,
			'#required' => TRUE,
			'#description' => t('Title of the page displayed in the catalog.'),
		);
		$form['link_text'] = array(
			'#type' => 'textfield',
			'#title' => t('Link Text'),
			'#default_value' => $externallink->link_text,
			'#description' => t('Text of the link when output as HTML.'),
			'#required' => TRUE,
		);
		$form['content'] = array(
			'#type' => 'textarea',
			'#title' => t('Content'),
			'#description' => t('Custom message associated with this link (overrides generic message).'),
			'#default_value' => $externallink->content,
		);
		$form['generic_msg_id'] = array(
			'#type' => 'select',
			'#options' => $model['options']['genericmessage'],
			'#empty_value' => t(''),
			'#empty_option' => t('-- Select Generic Message --'),
			'#default_value' => $externallink->generic_msg_id,
			'#title' => t('Generic Message'),
			'#description' => t('Select a standard message to display as the content.'),
		);
		$form['externallink_id'] = array(
			'#type' => 'hidden',
			'#value' => $externallink->id,
		);
		$form['new_genericmessage'] = array(
			'#type' => 'fieldset',
			'#title' => t('Create Generic Message'),
			'#description' => t('Create a new Generic Message and assign it to this External Link'),
			'#collapsible' => TRUE,
			'#collapsed' => TRUE,
		);
		$form['new_genericmessage']['generic_msg_label'] = array(
			'#type' => 'textfield',
			'#title' => t('Label'),
		);
		$form['new_genericmessage']['generic_msg_content'] = array(
			'#type' => 'textfield',
			'#title' => t('Content'),
		);
		$form['new_genericmessage']['actions'] = array(
			'#type' => 'actions',
		);
		$form['new_genericmessage']['actions']['save_genericmsg'] = array(
			'#type' => 'submit',
			'#value' => t('Save New Generic Message'),
		);
		$form['actions'] = array(
			'#type' => 'actions'
		);
		$form['actions']['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
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

/**
 * Submit handler for externallink admin form
 */
function locguide_admin_externallink_form_submit($form, &$form_state) {
	if ($form_state['values']['op'] == 'Submit') {
		// save changes
		dpm($form_state);
		db_set_active('locationguide');
		
		$query = db_merge('locationguide_externallink')
			->key(array('id' => $form_state['values']['externallink_id']))
			->fields(array(
				'id' => $form_state['values']['externallink_id'],
				'url' => $form_state['values']['url'],
				'content' => $form_state['values']['content'],
				'generic_msg_id' => check_plain($form_state['values']['generic_msg_id']),
				'link_text' => check_plain($form_state['values']['link_text']),
				'title' => check_plain($form_state['values']['title']),
			));
			
		$result = $query->execute();
		
		//_locguide_record_log('locationguide', 'externallink')
		db_set_active();
		drupal_set_message('Changes have been saved.');
		$form_state['redirect'] = 'admin/config/locguide/externallink';
	}
}
