<?php
include_once('locguide.model.helpers.inc');
class ExternalLink {

	public static function externalLinks() {
		db_set_active('locationguide');
		
		$query = db_select('locationguide_externallink', 'l')->fields('l');
		$query->leftJoin('locationguide_genericmessage', 'm', 'l.generic_msg_id = m.id');
		$query->fields('m');
		$query->orderBy('m.title');
		
		$result = $query->execute();
		$externalinks = array();
		foreach ($result as $l) {
			$externalinks[] = $l;
		}
		db_set_active();
		return $externalinks;
	}
	
	public function whatChanged($old) {
		
	}
}
