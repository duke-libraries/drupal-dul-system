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
