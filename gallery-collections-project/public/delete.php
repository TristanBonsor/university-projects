<?php

$config = require '../config.php';

require_once '../src/php/user/user.php';

$user = new User($config);

$url = "http" . (!empty($_SERVER['HTTPS'])?"s":"") . "://".$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'];
$url = preg_replace('/delete\.php.*/', '', $url);

if(!$user->authenticated) {
	header('Location: ' . $url . 'unauthorized');
	exit;
}

if(isset($_REQUEST['record_type']) && isset($_REQUEST['record_id'])) {

	require '../src/php/collection/Collection.php';
	
	$collection = new Collection($config);

	$query = 'DELETE FROM ' . $_REQUEST['record_type'] . ' WHERE id = ?;';

	$statement = $collection->prepare($query);
	$result = $statement->execute(array($_REQUEST['record_id']));

	if($_REQUEST['record_type'] === 'artwork') {
		unset($statement);
		$statement = $collection->prepare('SELECT file_name FROM artwork_image WHERE artwork_id = ?;');
		$result = $statement->execute(array($_REQUEST['record_id']));

		while($image = $statement->fetch()) {
			unlink('img/' . $image['file_name']);
		}

		unset($statement);
		$statement = $collection->prepare('DELETE FROM artwork_image WHERE artwork_id = ?;');
		$result = $statement->execute(array($_REQUEST['record_id']));
	}

	if($result) {
		echo '<div id="deleteSuccess"><strong>Record Deleted</strong></div>';
	}
	else {
		echo '<div id="deleteFailure"><strong>An Error Occured</strong></div>';
	}
}