<?php

function dul_system_staff_page() {
	if (module_exists('devel')) {
		dpm('in staff page');
	}

	return t('This is what happens when you reach a staff page!!');
}


