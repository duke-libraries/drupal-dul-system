<?php
include_once('locguide.model.helpers.inc');
class LocMapManager {

	public static function perkins_locationmaps() {
		/* source SQL
		SELECT * FROM `locationguide_collectiongroup` g
		JOIN `locationguide_sublibrary` s on s.id = g.sublibrary_id
		LEFT OUTER JOIN `locationguide_locationmap` m on m.collection_group_id = g.id
		WHERE s.code = 'PERKN'
		*/
		db_set_active('django');
		$query = db_select('locationguide_collectiongroup', 'g');
		$query->join('locationguide_sublibrary', 's', 's.id = g.sublibrary_id');
		$query->leftJoin('locationguide_locationmap', 'm', 'm.collection_group_id = g.id');
		$query->leftJoin('locationguide_externallink', 'l', 'l.id = m.external_link_id');
		$query->leftJoin('locationguide_genericmessage', 'msg', 'msg.id = l.generic_msg_id');
		$query->leftJoin('locationguide_areamap', 'a', 'a.id = m.map_id');
		$query->addField('g', 'id', 'collgroup_id');
		$query->addField('m', 'id', 'locmap_id');
		$query->addField('a', 'id', 'areamap_id');
		$query->addField('msg', 'label', 'generic_msg_label');
		$query->addField('msg', 'content', 'generic_msg_content');
		$query->fields('g')
			->fields('l')
			->fields('m')
			->fields('s')
			->fields('a')
			->condition('s.code', 'PERKN')
			->orderBy('description');
		$result = $query->execute();
		
		$objs = $result->fetchAll();
		db_set_active();
		
		$stacks_locmaps = array();
		$other_locmaps = array();
		$stack_filter = function($o) {
			return preg_match('/^Stacks/', $o->description) == 1;
		};
		$stack_locmaps = array_filter($objs, $stack_filter);
		$others_filter = function($o) {
			return !empty($o->description);
		};
		$other_locmaps = array_filter($objs, $others_filter);
		$return = array(
			'stacks_locmaps' => $stack_locmaps,
			'other_locmaps' => $other_locmaps
		);
		$link_text_map = function($o) {
			if (empty($o->link_text)) {
				$o->link_text = sprintf("%s - %s", $o->building, $o->area);
			}
		};
		$link_url_map = function($o) {
			if (empty($o->url)) {
				$o->url = sprintf('services/locationguide/areamap/%s', $o->areamap_id);
			}
		};
		array_map($link_text_map, $return['stacks_locmaps']);
		array_map($link_text_map, $return['other_locmaps']);
		array_map($link_url_map, $return['stacks_locmaps']);
		#dpm($return);
		return $return;
	}
	
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
