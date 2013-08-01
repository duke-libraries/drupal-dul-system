<?php

function dul_system_address_settings() {
	$form['footer_address'] = array(
		'#type' => 'text_format',
		'#title' => t('Library Mailing Address'),
		'#description' => t('Enter the mailing address of this library location, as it is to appear in the footer of the website'),
		'#rows' => 4,
		'#cols' => 10,
		'#default_value' => variable_get('dul_system.footer_address', ''),
	);

	$form['#submit'][] = 'dul_system_address_settings_submit';
	return system_settings_form($form);
}

/**
 * Process dul_system address settings submission
 */
function dul_system_address_settings_submit($form, $form_state) {
	variable_set('dul_system.footer_address', $form_state['values']['footer_address']['value']);
}

/**
 * Form to specify the Staff Directory feed URL.
 */
function dul_system_feed_settings() {
	$form['staff_directory_feed_url'] = array(
		'#type' => 'textfield',
		'#title' => t('URL of the Staff Directory XML Feed'),
		'#default_value' => variable_get('dul_system.staff_directory_feed_url', ''),
	);
	// specify the "submit" callback function here...
	$form['#submit'][] = 'dul_system_feed_settings_submit';

	// return the form filtered through the system_settings_form
	return system_settings_form($form);
}

/**
 * Submit callback for the above form function^
 */
function dul_system_feed_settings_submit($form, $form_state) {
	variable_set('dul_system.staff_directory_feed_url', $form_state['values']['staff_directory_feed_url']);
}
