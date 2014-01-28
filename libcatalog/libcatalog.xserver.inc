<?php

define('BIB_PATRON_LIB', 'DUK50');
define('BIB_LIB', 'DUK01');
define('COURSE_LIB', 'DUK30');
define('COURSE_BASE', 'u-duke30');

define('MARC21_NAMESPACE', 'http://www.loc.gov/MARC21/slim/');

class XServer {
	private $_session_id = '';
	private $_host = '';

	function __construct($host) {
		$this->_host = $host;
	}

	function url() {
		return sprintf("https://%s/X", $this->_host);
	}

	function request($query) {
		# see sites/<yoursite>/settings.php
		global $conf;

		# prep here
		# next, append the username & password
		if ($this->_session_id) {
			$query['session_id'] = $this->_session;
		} else {
			$query['user_name'] = $conf['xserver']['user'];
			$query['user_password'] = $conf['xserver']['password'];
		}

		$qstring = array();
		foreach ($query as $k => $v) {
			$qstring[] =  sprintf("%s=%s", $k, urlencode($v));
		}
		$qstring = implode('&', $qstring);
		$url = $this->url() . '?' . $qstring;
		
		$old_timeout = ini_set('default_socket_timeout', $conf['xserver']['timeout']);
		$response = '';
		try {
			$response = file_get_contents($this->url() . '?' . $qstring);
		} catch (Exception $x) {
			int_set('default_socket_timeout', $old_timeout);
		}
		$result = new XServerResult($response);
		return $result;
		// $this->_session_id = $result.sessionId();
	}

	function getItemByBarcode($barcode) {
		$item = NULL;
		
		// remove legacy '__'
		$barcode = str_replace('__', '', $barcode);
		
		$query = array(
			'op' => 'ill-item-by-bc',
			'library' => BIB_PATRON_LIB,
			'translate' => 'N',
			'barcode' => $barcode,
		);

		$result = $this->request($query);
		$item = new XServerItem($result->xml()->x30);
		return $item;
	}
	
	/**
	 * 
	 */
	function getTitleInfoFromSession($request=NULL, $sysno=NULL) {
		
	}
	
	/**
	 * Get bibliographic information on the system-number. 
	 * This is a way to get title data as an object (not an HTML-formatted string).
	 * Preferred usage is to be invoked by 'getTitleInfoFromSession' method
	 * to keep x-server traffic to a minimum.
	 */
	function getTitleAsXServerTitle($sysno, $base=BIB_LIB) {
		$result = NULL;
		
		$query = array(
			'op' => 'ill-get-doc',
			'library' => $base,
			'doc_number' => $sysno
		);
		$result = $this->request($query);

		$xml = $result->xml();
		$xml->registerXPathNamespace('marc21', MARC21_NAMESPACE);
		$title_node = $xml->xpath('//marc21:record');
		if (sizeof($title_node)) {
			// dpm($title_node);
			return new XServerTitle($title_node[0]);
		}
	
		return new XServerEmptyResultSet;
	}
}

class XServerEmptyResultSet {}

class XServerTitle {
	private $_element;
	function __construct($elem) {
		$this->_element = $elem;
	}
}

