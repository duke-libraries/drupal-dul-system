<?php

class XServerItem {
	public static $BARCODE = 'z30-barcode';
	public static $SUBLIBRARY = 'z30-sub-library';
	public static $COLLECTION = 'z30-collection';
	public static $CALLNO_TYPE = 'z30-call-no-type';
	public static $CALLNO = 'z30-call-no';

	private $_xml;

	function __construct($xmlElement) {
		$this->_xml = $xmlElement;
	}

	function getValueForTagName($tagName) {
		if (($values = $this->_xml->xpath('//' . $tagName . '/text()')) !== FALSE) {
			return (string) $values[0];
		}
		return NULL;
	}

	function __get($name) {
		$v = $this->getValueForTagName($name);
		if ($v == NULL) {
			$trace = debug_backtrace();
			trigger_error(
				'Undefined property via __get(): ' . $name .
				' in ' . $trace[0]['file'] . 
				' on line ' . $trace[0]['line'],
				E_USER_NOTICE);
		}
		return $v;
	}
}
