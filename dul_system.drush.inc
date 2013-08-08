<?php

/**
 * @file
 * Drush command file including the following command(s):
 *
 * dul-refresh-staff (drs) - fetch data from staff directory feed and store in 'staff' table
 *
 */

/**
 * Implementation of hook_drush_command()
 */
function dul_system_drush_command() {
	// store the commands in this array...
	$items = array();

	// the 'dul-refresh-staff' command
	$items['dul-refresh-staff'] = array(
		'description' => 'Fetch data from staff directory feed and store in "staff" table',
		
		// below are developer-defined command aliases (or shortcuts)
		'aliases' => array('drs', 'staff-update'),
		
		// this seems to be needed by drush
		'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
	);

	return $items;
}

function drush_dul_system_dul_refresh_staff_validate() {
	$feedURL = variable_get('dul_system.staff_directory_feed_url', FALSE);
	if (!$feedURL) {
		return drush_set_error('STAFF_REFRESH', dt('ERROR: Please specify a URL for the staff feed.'));
	}
	return TRUE;
}

/**
 * Command file for refresh staff
 */
function drush_dul_system_dul_refresh_staff() {
	// read data from xml feed
	$url = variable_get('dul_system.staff_directory_feed_url');
	$content = file_get_contents($url);

	// this serves as validation that we actually could read the file
	// TODO: remove
	//drush_print_r($content);

	// delete (or truncate) the contents of the 'staff' table
	$result = db_truncate('staff')->execute();

	// process each node in the XML file
	// NOTE -- will need to use SimpleXML object (see: http://php.net/manual/en/book.simplexml.php)

	$persons = array();
	try {
		$xml = new SimpleXMLElement($content);
	} catch(Exception $e) {
		return drush_set_error('STAFF_REFRESH', dt($e->getMessage()));
	}

	// locate the <person> elements
	$persons = $xml->xpath('/directory/staff/person');

	// foreach node, create a stdClass object:
	// example:
	// $o = new stdClass();
	// $o->firstname = "the value of the attribute that holds the 'firstname' in the xml data row";
	// --> repeat for all data values
	
	foreach ($persons as $person) {
		$sid = db_insert('staff')
		->fields(array(
			'netid' => (string) $person->netid,
			'firstname' => (string) $person->firstname,
			'lastname' => (string) $person->lastname,
			'jobtitle' => (string) $person->title,
			'displayname' => (string) $person->displayname,
			'fullnamereversed' => (string) $person->fullnamereversed,
			'address' => (string) $person->address,
			'phone' => (string) $person->phone,
			'phone2' => (string) $person->phone2,
			'email' => (string) $person->email,
			'email_privacy' => (string) $person->email_privacy,
			'photo' => (string) $person->photo,
			'url' => (string) $person->url,
		))
		->execute();

		// then...
		# $result = drupal_write_record('staff', $o);
		if (!$sid) {
			drush_print('error happened...');
			drush_print_r(Database::getConnection());
		}
	}
	// END foreach

	// done. :)
}
