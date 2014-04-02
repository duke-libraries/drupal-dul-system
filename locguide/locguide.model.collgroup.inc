<?php
include_once('locguide.model.helpers.inc');

class CollectionGroupManager {
	public static function collectionGroups($filter=NULL, $usePager=FALSE, $defaultLimit=25) {
		db_set_active('locationguide');
		
		$query = db_select('locationguide_collectiongroup', 'cg')->fields('cg');
		if ($usePager) {
			$query->extend('PagerDefault')->limit($defaultLimit);
		}
		$query->leftJoin('locationguide_sublibrary', 'sl', 'sl.id = cg.sublibrary_id');
		$query->addField('sl', 'code');
		$query->addField('sl', 'code', 'sublibrary_code');
		$query->addField('sl', 'label', 'sublibrary_label');
		$query->orderBy('sl.label');

		$result = $query->execute();
		$collgroups = array();
		foreach ($result as $r) {
			$collgroups[] = $r;
		}
		db_set_active();
		return $collgroups;
	}
}

class CollectionGroup {
	
}
