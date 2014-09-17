<?php

/**
 * page callback for "locate materials" 
 */
function locguide_materials() {
	// define and return a table element
	
	// TODO implement entity
	db_set_active('django');
	error_log('1');
	$query = db_select('locationguide_locationmap', 'm');
	$query->leftJoin('locationguide_sublibrary', 's', 'm.id = s.default_locmap_id');
	$query->leftJoin('locationguide_externallink', 'l', 'l.id = m.external_link_id');
	$query->leftJoin('locationguide_areamap', 'a', 's.default_locmap_id = a.id');
	$query->addField('s', 'label', 'sublibrary_label');
	$query->addField('l', 'url', 'external_link_url');
	$query->addField('l', 'link_text', 'external_link_text');
	$query->addField('a', 'area');
	$query->addField('a', 'building');
	$query->addField('a', 'id', 'areamap_id');
	$query->condition('s.collgroup_choice', '1')
		->condition('m.collection_group_id', 'NULL', '<>');
	$query->orderBy('s.label');
	
	$result = FALSE;
	try {
		$result = $query->execute();
	} catch (Exception $e) {
		db_set_active();
		error_log($e->getMessage());
		return 'Coming soon';
	}
	$objs = $result->fetchAll();
	db_set_active();
	
	$rows = array();
	foreach ($objs as $o) {
		$row = array($o->sublibrary_label);
		if (empty($o->external_link_text)) {
			$path = sprintf("services/locationguide/areamap/%s", $o->areamap_id);
			$row[] = l(t('@label - @area', array('@label' => $o->sublibrary_label, '@area' => $o->area)), $path);
		} else {
			$row[] = l($o->external_link_text, $o->external_link_url);
		}
		$rows[] = $row;
	}
	
	$header = array(
		t('Library / Location'),
		t('Where Is It? / How Do I Find It?')
	);
	
	$page['preamble'] = array(
		'#type' => 'markup',
		'#markup' => '<p>Major locations for materials held by Duke University Libraries.</p>',
	);
	
	$page['table'] = array(
		'#theme' => 'table',
		'#header' => $header,
		'#rows' => $rows,
		'#attributes' => array(
			'class' => array('table', 'table-striped'),
		),
	);
	
	return $page;
}
