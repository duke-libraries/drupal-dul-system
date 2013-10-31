<?php

function external_header_html() {
	$args = func_get_args();
	$path = filter_xss(implode('/', $args));
	
	// do I need this one?
	$cid = isset($_GET['comment']) ? (int)$_GET['comment'] : NULL;

	$query = $_GET;
	unset($query['q']);

	$html = '<h1>Satisfaction Guaranteed</h1>';

	drupal_add_http_header('Content-Type', 'text/html; charset=utf-8');
	drupal_send_headers();
	print $html;
}
