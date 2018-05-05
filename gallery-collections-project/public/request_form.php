<?php

$config = require '../config.php';

require_once '../src/php/user/user.php';

$user = new User($config);

$url = "http" . (!empty($_SERVER['HTTPS'])?"s":"") . "://".$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'];
$url = preg_replace('/request_form\.php.*/', '', $url);

if(!$user->authenticated) {
	header('Location: ' . $url . 'unauthorized');
	exit;
}

if(isset($_REQUEST['access_type']) && isset($_REQUEST['record_type'])) {

	require '../src/php/collection/form/FormBuilder.php';

	$access_type = $_REQUEST['access_type'];
	$record_type = $_REQUEST['record_type'];

	// Handle form construction (interface and data population) for
	// search, create, update, and read access types.
	new FormBuilder($config, $access_type, $record_type);
}
