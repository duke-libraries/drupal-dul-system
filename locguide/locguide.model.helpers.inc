<?php

function callNumberSortKey($callno) {
	//error_log(sprintf("callNumberSortKey called with %s", $callno));
	$pattern = '/^([A-Z]+)(\d+)/';
	$matches = array();
	$result = preg_match($pattern, $callno, $matches);
	if ($result == 1) {
		return $matches[1] . sprintf('%04d', $matches[2]);
	}
	return $callno;
}
