<?php
include_once('locguide.model.helpers.inc');
class BibTitle {
	
	public $_xml;
	
	public static function newBibTitleFromSysNo($sysno) {
		// http://www.lullabot.com/blog/article/beginners-guide-caching-data-drupal-7
		$bibTitle = &drupal_static(__FUNCTION__);
		if (!isset($bibTitle)) {
			// Cry foul if 'libcatalog' is not enabled
			if (!module_exists('libcatalog')) {
				$trace = debug_backtrace();
				trigger_error(
					'libcatalog module not enabled: ' .
					' in ' . $trace[0]['file'] . 
					' on line ' . $trace[0]['line'],
					E_USER_NOTICE);
			}
			
			# strip the 'DUKE' portion from the sysno, when applicable
			$sysno = str_replace('DUKE', '', $sysno);
			$cache_key = sha1($sysno);
			if ($cache = cache_get($cache_key)) {
				$bibTitle = $cache->data;
			} 
			else {
				global $conf;
		
				// did we cache this title record?
				$xserver = new XServer($conf['xserver']['host']);
				$bibTitle = $xserver->getTitleAsXServerTitle($sysno);
				if (is_a($bibTitle, 'XServerItem')) {
					cache_set($cache_key, $bibTitle, 'cache', CACHE_TEMPORARY);	
				}
			}
		}
		return $bibTitle;
		
	}
	
	function __construct () {
		
	}

}
