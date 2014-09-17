<?php
include_once('locguide.model.helpers.inc');

class SubLibrary {
	
	public static function sublibraries($collgroup_choice=NULL) {
		db_set_active('django');
		$query = db_select('locationguide_sublibrary', 'sl')->fields('sl');
		if ($collgroup_choice) {
			$query->condition('sl.collgroup_choice','1');
		}
		$query->orderBy('sl.label');
		$sublibraries = array();
		try {
			$result = $query->execute();
			foreach ($result as $r) {
				$sublibraries[] = $r;
			}
		} catch(Exception $exc) {
			db_set_active();
		}
		db_set_active();
		return $sublibraries;
	}
}
