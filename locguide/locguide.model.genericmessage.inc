<?php
include_once('locguide.model.helpers.inc');

class GenericMessage {
	
	public static function genericMessages() {
		db_set_active('locationguide');
		$query = db_select('locationguide_genericmessage', 'm')->fields('m');
		$genericmessages = array();
		try {
			$result = $query->execute();
			foreach ($result as $r) {
				$genericmessages[] = $r;
			}
		} catch(Exception $exc) {
			db_set_active();
		}
		db_set_active();
		return $genericmessages;
	}
}
