<?php

class XServerError extends Exception {}

/**
 * Represents a loose port of the Django LibraryCatalog XServer Result
 * Interface
 */

class XServerResult {

	private $message = NULL;

	private $_response;
	
	private $_session_id;
	
	private $_xml;
	
	private $_error;

	function __construct($response=NULL) {
		$this->messages = array();
		$this->_response = $response;
		
		$responseContent = NULL;
		if ($this->_response == NULL) {
			$responseContent = '<xserver/>';
		} else {
			$responseContent = str_replace('&amp;nbsp;', '&#160;', $response);
		}
	
		try {
			# parsing
			$this->_xml = new SimpleXMLElement($responseContent);
			dpm($responseContent);
		} catch(Exception $e) {
			throw new XServerError($e->getMessage());
		}
		$this->_session_id = (string) $this->_xml->{'session-id'};
	}
	
	function valueByTagName($tagName) {
		if (($values = $this->_xml->xpath('//' . $tagName . '/text()')) !== FALSE) {
			return (string) $values[0];
		}
		return NULL;
	}
	
	function xml() {
		return $this->_xml;
	}
	
	function parsed_xml() {
		return $this->_xml;
	}
	
	function session_id() {
		return $this->_session_id;
	}
	
	function response() {
		return $this->_response;
	}
	
	function error() {
		return $this->_error;
	}
}
