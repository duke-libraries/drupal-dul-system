<?php
include_once('locguide.model.helpers.inc');
class LocMapManager {

	function locationMaps() {
		db_set_active('django');
		
		$query = db_select('locationguide_locationmap', 'lm')->fields('lm');
		$query->leftJoin('locationguide_collectiongroup', 'cg', 'lm.collection_group_id = cg.id');
		$query->leftJoin('locationguide_sublibrary', 'sl', 'cg.sublibrary_id = sl.id');
		$query->addField('cg', 'description', 'collgroup_description');
		$query->addField('sl', 'label', 'sublibrary_label');
		$query->addField('sl', 'code', 'sublibrary_code');
		$query->orderBy('sl.label');
		
		$result = $query->execute();
		$locationmaps = array();
		foreach ($result as $l) {
			$locationmaps[] = $l;
		}
		db_set_active();
		return $locationmaps;
	}
	
	function getLocationMapByBarcode($barcode) {
		global $conf;
		
		$xserver = new XServer($conf['xserver']['host']);
		$item = $xserver->getItemByBarcode($barcode);
	
		$callno = callNumberSortKey($item->{XServerItem::$CALLNO});
		watchdog('locguide', print_r($item->getXML(), TRUE));
		//dpm($callno);
		
		# query road map
		# a) select from lguide_collectiongroup left join to lguide_collection on collection_group_id
		#    where collection.code = $item's COLLECTION attr value
		# IF NO ROWS:
		# b) select from sublibrary table where sublibrary.code = $item's SUBLIBRARY attr value
		
		//$query = db_select ('locationguide_collection', 'lc')
		//	->join('locationguide_collectiongroup', 'lcg', 'lcg.sublibrar')
		
		db_set_active('django');
		
		$query = db_select('locationguide_collectiongroup', 'lcg')
			->fields('lcg');
		$query->join('locationguide_collection', 'lc', 'lc.collection_group_id = lcg.id');
		$query->join('locationguide_locationmap', 'lm', 'lm.collection_group_id = lcg.id');
		$query->addField('lcg', 'description', 'collection_group_description');
		$query->fields('lm')
			->fields('lc')
			->condition('lc.code', $item->getValueForTagName(XServerItem::$COLLECTION));
			
		$callno_condition = db_or();
		$callno_condition->condition('lm.callno_range', '');
		
		#			$$hPR878.R5$$iB87 2013
		
		$callrange_start_end = db_and();
		$callrange_start_end->condition('lm.callno_range', '', '<>'); 
		$callrange_start_end->condition('lm.callno_start', $callno, '<=');
		$callrange_start_end->condition('lm.callno_end', $callno, '>');
		$callno_condition->condition($callrange_start_end);
		$query->condition($callno_condition);
		
		watchdog('locguide', print_r(array('callno' => $callno), TRUE));

		$result = $query->execute();
		$o = $result->fetchObject();
		
		if ($o == NULL) {

			$query = db_select('locationguide_sublibrary', 's');
			$query->join('locationguide_locationmap', 'lm', 'lm.id = s.default_locmap_id');
			$query->fields('s')
				->fields('lm')
				->condition('s.code', $item->getValueForTagName(XServerItem::$SUBLIBRARY));
			try {
				$result = $query->execute();
				$o = $result->fetchObject();
				watchdog('locguide', print_r($o, TRUE));
			} catch(Exception $x) {
				error_log($x->getMessage());
				watchdog('locguide', $x->getMessage(), WATCHDOG_ERROR);
			}
		}
		db_set_active();

		if ($o == NULL) {
			return NULL;
		}
		$locmap = new LocationMap($o, $this);
		$locmap->fetch_sublibrary($item->getValueForTagName(XServerItem::$SUBLIBRARY));
		$locmap->set_callno($item->getValueForTagName(XServerItem::$CALLNO));
		return $locmap;
	}

	function getAreaMap($mapId) {
		$old = db_set_active('django');

		$result = db_select('locationguide_areamap', 'm')
			->fields('m')
			->condition('m.id', $mapId)
			->execute();

		$o = $result->fetchObject();
		
		db_set_active($old);
		return $o == FALSE ? NULL : $o;
	}
	
	function getExternalLink($externalLinkId) {
		$old = db_set_active('django');

		$query = db_select('locationguide_externallink', 'l');
		$query->leftJoin('locationguide_genericmessage', 'gm', 'gm.id = l.generic_msg_id');
		$query->condition('l.id', $externalLinkId);
		$query->addField('l', 'url', 'extlink_url');
		$query->addField('l', 'title', 'extlink_title');
		$query->addField('l', 'link_text', 'extlink_text');
		$query->addField('l', 'content', 'extlink_content');
		$query->addField('gm', 'content', 'generic_msg_content');
		$query->addField('gm', 'label', 'generic_msg_label');
			
		$result = $query->execute();
			
		$o = $result->fetchObject();
		
		db_set_active($old);
		return $o == FALSE ? NULL : $o;
	}
}

class LocationMap {
	private $_manager;
	private $_o;
	private $_areaMap = NULL;
	private $_areaMapFetched = FALSE;
	private $_externalLink = NULL;
	private $_externalLinkFetched = FALSE;
	private $_sublibrary = NULL;
	
	public $location_description = '';
	
	public function __construct($o, $manager) {
		if (is_a($o, 'stdClass')) {
			$this->_o = $o;
			$this->_manager = $manager;
		}
		$this->_manager = new LocMapManager($o->map_id);
	}
	
	function __get($name) {
		// lazy load?
		if ($name == 'areaMap') {
			//error_log('fetching areaMap...');
			if (!$this->_areaMapFetched) {
				$this->_areaMap = $this->_manager->getAreaMap($this->_o->map_id);
				$this->_areaMapFetched = TRUE;
			}
			if (is_null($this->_areaMap)) {
				error_log('area map is null');
			} else {
				if (!empty($this->_o->collection_group_description)) {
					$this->_areaMap->collection_group_description = $this->_o->collection_group_description;
				}
				$this->_areaMap->type = 'areamap';
				$this->_areaMap->sublibrary = $this->_sublibrary;
				$this->_areaMap->callno = $this->_callno;
				$this->_areaMap->callno_formatted = trim(preg_replace('/\$\$h?/', ' ', $this->_areaMap->callno));
			}
			return $this->_areaMap;
		}
		if ($name == 'externalLink') {
			error_log('fetching external link');
			if (!$this->_externalLinkFetched) {
				$this->_externalLink = $this->_manager->getExternalLink($this->_o->external_link_id);
				$this->_externalLink->type = "externallink";
				$this->_externalLinkFetched = TRUE;
				$this->_externalLink->sublibrary = $this->_sublibrary;
				$this->_externalLink->callno = $this->_callno;
				$this->_externalLink->callno_formatted = trim(preg_replace('/\$\$h?/', ' ', $this->_externalLink->callno));
			}
			if (is_null($this->_externalLink)) {
				error_log('externalLink is null');
			}
			return $this->_externalLink;
		}
		return NULL;
	}
	
	function set_callno($v) {
		$this->_callno = $v;
	}
	
	function callno() {
		return $this->_callno;
	}
	
	function sublibrary() {
		return $this->_sublibrary;
	}
	
	function fetch_sublibrary($sublibrary_code) {
		if (empty($this->_sublibrary)) {
			db_set_active('django');
			$result = db_select('locationguide_sublibrary', 's')
				->fields('s')
				->condition('code', $sublibrary_code)
				->execute();
				
			$o = $result->fetchObject();
			$this->_sublibrary = $o;
			db_set_active();
		}
	}
}
