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

	$items['dul-node-image-check'] = array(
		'description' => 'List the nodes and revisions whose body fields contain hard-coded image and/or link references',
		'aliases' => array('dimgchk'),
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

	// process each node in the XML file
	// NOTE -- will need to use SimpleXML object (see: http://php.net/manual/en/book.simplexml.php)

	$depts = array();
	$persons = array();
	
	try {
		$xml = new SimpleXMLElement($content);
	} catch(Exception $e) {
		return drush_set_error('STAFF_REFRESH', dt($e->getMessage()));
	}

	$depts = $xml->xpath('/directory/depts/dept');
	if (empty($depts)) {
		return drush_set_error('STAFF_REFRESH', t('Unable to read staff data -- depts array is empty'));
	}
	
	$result = db_truncate('dul_dept')->execute();
	$result = db_truncate('dul_dept_member')->execute();
	
	foreach ($depts as $dept) {
		drush_print("processing " . (string) $dept->name);
		
		$status = db_insert('dul_dept')
			->fields(array(
				'dept_id' => $dept['id'],
				'title' => (string) $dept->name,
				'address' => (string) $dept->address,
				'description' => (string) $dept->description,
				'head_person_id' => (int) $dept->head,
				'head_title' => (string) $dept->headtitle,
				'phone' => (string) $dept->phone,
				'parent_did' => (int) $dept->parent,
				'url' => (string) $dept->url,
				'email' => (string) $dept->email,
				'fax' => (string) $dept->fax,
				'campusbox' => (string) $dept->campusbox,
			))
			->execute();
			
			$members = array();
			$members = $dept->xpath('members/member');
			if (!empty($members)) {
				foreach($members as $member) {
					db_insert('dul_dept_member')
						->fields(array(
							'did' => $dept['id'],
							'person_id' => (int) $member,
					))
					->execute();
				}
			}
		drush_print("insert returned: " . $status);
	}
	
	// locate the <person> elements
	$persons = $xml->xpath('/directory/staff/person');
	if (empty($persons)) {
		return drush_set_error('STAFF_REFRESH', t('Unable to read staff data -- persons array is empty.'));
	}	

	// delete (or truncate) the contents of the 'staff' table
	$result = db_truncate('staff')->execute();
	
	foreach ($persons as $person) {
		$sid = db_insert('staff')
		->fields(array(
			'netid' => (string) $person->netid,
			'person_id' => (int) $person['id'],
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

function drush_dul_system_dul_node_image_check() {
	$query = db_select('field_data_body', 'fdb')
		->fields('fdb', array('entity_id','revision_id', 'entity_type'))
		->condition('body_value', '%' . db_like('//libcms.oit.duke.edu') . '%', 'LIKE');
	$query->join('node_revision', 'nr', 'fdb.entity_id = nr.nid AND fdb.revision_id = nr.vid');
	$query->addField('nr', 'title');
	$result = $query->execute();
	
	$now = time();
	$handle = fopen('/srv/web/libcms/node_check/node_check_' . $now . '.tsv', 'w');
	$how_many = 0;
	foreach ($result as $row) {
		$arr = array($row->title);
		$arr[] = sprintf("https://libcms.oit.duke.edu/node/%s/%s", $row->entity_id, $row->revision_id);
		fputcsv($handle, $arr, "\t");
		drush_print_r($row);
		$how_many++;
	}
	drush_print(sprintf("How Many: %s", $how_many));
	fclose($handle);
}
