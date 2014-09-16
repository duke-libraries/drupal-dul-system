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
		watchdog('libcatalog.xserveritem', print_r($xmlElement, TRUE));
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
		if ($name == XServerItem::$CALLNO) {
			$matches = array();
			if (preg_match('/\$\$h([^\$]+)/', $v, $matches) == 1) {
				watchdog('libcatalog', print_r($matches, TRUE));
				$v = $matches[1];
			}
		}
		return $v;
	}
	
	function getXML() {
		return $this->_xml;
	}
}
