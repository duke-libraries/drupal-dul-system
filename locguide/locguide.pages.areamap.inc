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
	$src = '/sites/default/media/django/web/media/' . $areamap->image;
	return '<img src="' . $src . '" alt="' . $areamap->building . ' - ' . $areamap->area . '" />';
}
