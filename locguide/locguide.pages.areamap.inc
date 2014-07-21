<?php

/**
 * Return output representing aa area map
 */
function locguide_areamap($areamap=NULL) {
	if (is_null($areamap)) {
		return '';
	}
	return theme('areamap', array('areamap' => $areamap));
}

function locguide_theme_areamap($variables) {
	$areamap = $variables['areamap'];
	if (isset($_REQUEST['format']) && $_REQUEST['format'] == 'json') {
		$response = array();
		$response['src'] = sprintf("http://%s/sites/default/media/django/web/media/%s", $_SERVER['SERVER_NAME'], $areamap->image);
		drupal_json_output($response);
		drupal_exit();
	}
	else {
		$src = '/sites/default/media/django/web/media/' . $areamap->image;
		return '<img src="' . $src . '" alt="' . $areamap->building . ' - ' . $areamap->area . '" />';
	}
}
