<?php

$config = require '../config.php';

require_once '../src/php/user/user.php';

$user = new User($config);

$url = "http" . (!empty($_SERVER['HTTPS'])?"s":"") . "://".$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'];
$url = preg_replace('/backup\.php.*/', '', $url);

if(!$user->authenticated) {
	header('Location: ' . $url . 'unauthorized');
	exit;
}

const ZIP_PATH = 'download/gcp_backup.zip';

const ZIP_PATH = 'download/gcp_backup.zip';

$config = require '../config.php';

require '../src/php/collection/Collection.php';

$collection = new Collection($config);

date_default_timezone_set($config['time_zone']);

$date = date("Y-m-d-H:i:s");

$tables = array('artwork', 'artist', 'artwork_image', 'artwork_type', 'artwork_medium', 'artwork_collection', 'parent_location', 'building', 'room');

$images = array_diff(scandir('img'), array('..', '.'));

$zip = new ZipArchive;
$zip->open(ZIP_PATH, ZipArchive::OVERWRITE);

foreach($images as $image) {
	$zip->addFile('img/' . $image);
}

foreach($tables as $table) {
	$results = $collection->query('SELECT * FROM ' . $table . ';');

	$rows = array();

	$first_row = true;
	while($row = $results->fetch(PDO::FETCH_ASSOC)) {
		if($first_row) {
			$first_row = false;
			$field_names = array_keys($row);
			$rows[] = '"' . stripslashes(implode('","', $field_names)) . "\"\n";
		}
		$rows[] = '"' . stripslashes(implode('","', $row)) . "\"\n";
	} 

	$zip->addFromString($table . '.csv', implode('', $rows));
}

$results = $collection->query('SELECT * FROM v_master_listings;');

$rows = array();

$first_row = true;
while($row = $results->fetch(PDO::FETCH_ASSOC)) {
	if($first_row) {
		$first_row = false;
		$field_names = array_keys($row);
		$rows[] = '"' . stripslashes(implode('","', $field_names)) . "\"\n";
	}
	$rows[] = '"' . stripslashes(implode('","', $row)) . "\"\n";
}

$zip->addFromString('MASTER.csv', implode('', $rows));

$zip->close();

$url = "http" . (!empty($_SERVER['HTTPS'])?"s":"") . "://".$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'];
$url = explode('/', $url);
array_pop($url);
$url = implode('/', $url);
echo $url . '/' . ZIP_PATH;