<?php

$config = require '../config.php';

require_once '../src/php/user/user.php';

$user = new User($config);

$url = "http" . (!empty($_SERVER['HTTPS'])?"s":"") . "://".$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'];
$url = preg_replace('/create\.php.*/', '', $url);

if(!$user->authenticated) {
	header('Location: ' . $url . 'unauthorized');
	exit;
}

require '../src/php/collection/Collection.php';

$collection = new Collection($config);

if(isset($_REQUEST['files']) && isset($_REQUEST['artwork_id'])) {

	$artwork_id = $_REQUEST['artwork_id'];

	$results = $collection->query('SELECT title FROM artwork WHERE id = ' . $artwork_id . ' LIMIT 1;');

	foreach($results as $result) {
		$file_name_prefix = $artwork_id . '_' . str_replace(' ', '_', $result['title']) . '_';
	}

	$files = array();

	foreach($_FILES as $file) {
		$file_name = uniqid($file_name_prefix) . '.' . pathinfo($file['name'], PATHINFO_EXTENSION);

		if(!move_uploaded_file($file['tmp_name'], 'img/' . $file_name)) {
			$error = true;
			$files[] = $file['name'];
		}
		else {
			$query = 'INSERT INTO artwork_image (artwork_id, file_name)'
					. 'VALUES (:artwork_id, :file_name);';

			$statement = $collection->prepare($query);
			$result = $statement->execute(array('artwork_id' => $artwork_id, 'file_name' => $file_name));
		}
	}

	if(isset($error)) {
		echo json_encode(array('error' => 'Failed to upload: ' . implode(', ', $files)));
	}
	else {
		echo '{}';
	}
}
else if(isset($_REQUEST['record_type'])) {

	$criteria = array();
	$data = array();

	parse_str($_REQUEST['criteria'], $criteria);
	
	switch($_REQUEST['record_type']) {
		case 'artwork':

			$title = trim($criteria['title']);
			$title = (empty($title)) ? 'Untitled' : $title;

			$data['title']              = $title;
			$data['artist_id']          = (empty($criteria['artist_full_names_key_value'])) ? '1' : $criteria['artist_full_names_key_value'];
			$data['type']               = (empty($criteria['types'])) ? '' : $criteria['types'];
			$data['medium']             = (empty($criteria['mediums'])) ? '' : $criteria['mediums'];
			$data['parent_location_id'] = (empty($criteria['parent_location_names_key_value'])) ? '' : $criteria['parent_location_names_key_value'];
			$data['building_id']        = (empty($criteria['building_number_names_key_value'])) ? '' : $criteria['building_number_names_key_value'];
			$data['room_id']            = (empty($criteria['room_number_names_key_value'])) ? '' : $criteria['room_number_names_key_value'];
			$data['description']        = trim($criteria['description']);
			$data['collection']         = (empty($criteria['collections'])) ? '' : $criteria['collections'];

			$query = 'INSERT INTO artwork (title, artist_id, type, medium, parent_location_id, building_id, room_id, description, collection)'
					. 'VALUES (:title, :artist_id, :type, :medium, :parent_location_id, :building_id, :room_id, :description, :collection);';
			
			$statement = $collection->prepare($query);
			$result = $statement->execute($data);

			// Respond with artwork id for image upload.
			echo $collection->lastInsertId();

			break;
			
		case 'artist':
			
			$data['first_name'] = (empty($criteria['first_name'])) ? '' : $criteria['first_name'];
			$data['last_name'] = (empty($criteria['last_name'])) ? '' : $criteria['last_name'];
			$data['phone_number'] = (empty($criteria['phone_number'])) ? '' : $criteria['phone_number'];
			$data['email'] = (empty($criteria['email'])) ? '' : $criteria['email'];
			$data['address'] = (empty($criteria['address'])) ? '' : $criteria['address'];
			$data['city'] = (empty($criteria['city'])) ? '' : $criteria['city'];
			$data['province'] = (empty($criteria['province'])) ? '' : $criteria['province'];
			$data['postal_code'] = (empty($criteria['postal_code'])) ? '' : $criteria['postal_code'];
			
			$query = 'INSERT INTO artist (first_name, last_name, phone_number, email, address, city, province, postal_code)'
					. 'VALUES (:first_name, :last_name, :phone_number, :email, :address, :city, :province, :postal_code);';

			$statement = $collection->prepare($query);
			$result = $statement->execute($data);

			break;
			
		case 'artwork_type':

			$data['type'] = (empty($criteria['type'])) ? '' : $criteria['type'];

			$query = 'INSERT INTO artwork_type (type) VALUES (:type);';
			
			$statement = $collection->prepare($query);
			$result = $statement->execute($data);
			
			break;
		
		case 'artwork_medium':
			
			$data['medium'] = (empty($criteria['medium'])) ? '' : $criteria['medium'];
			
			$query = 'INSERT INTO artwork_medium (medium) VALUES (:medium);';
					
			$statement = $collection->prepare($query);
			$result = $statement->execute($data);
					
			break;

		case 'artwork_collection':
			
			$data['collection'] = (empty($criteria['collection'])) ? '' : $criteria['collection'];
			
			$query = 'INSERT INTO artwork_collection (collection) VALUES (:collection);';
					
			$statement = $collection->prepare($query);
			$result = $statement->execute($data);
					
			break;
		
		case 'parent_location':

			$data['name'] = (empty($criteria['name'])) ? '' : $criteria['name'];
			$data['phone_number'] = (empty($criteria['phone_number'])) ? '' : $criteria['phone_number'];
			$data['address'] = (empty($criteria['address'])) ? '' : $criteria['address'];
			$data['city'] = (empty($criteria['city'])) ? '' : $criteria['city'];
			$data['province'] = (empty($criteria['province'])) ? '' : $criteria['province'];
			$data['postal_code'] = (empty($criteria['postal_code'])) ? '' : $criteria['postal_code'];
					
			$query = 'INSERT INTO parent_location (name, phone_number, address, city, province, postal_code)'
					. 'VALUES (:name, :phone_number, :address, :city, :province, :postal_code);';
			
			$statement = $collection->prepare($query);
			$result = $statement->execute($data);

			break;
			
		case 'building':

			$data['parent_location_id'] = (empty($criteria['parent_location_names_key_value'])) ? '' : $criteria['parent_location_names_key_value'];
			$data['number'] = (empty($criteria['number'])) ? '' : $criteria['number'];
			$data['name'] = (empty($criteria['name'])) ? '' : $criteria['name'];

			$query = 'INSERT INTO building (parent_location_id, number, name)'
					. 'VALUES (:parent_location_id, :number, :name);';

			$statement = $collection->prepare($query);
			$result = $statement->execute($data);

			break;
			
		case 'room':

			$data['building_id'] = (empty($criteria['building_number_names_key_value'])) ? '' : $criteria['building_number_names_key_value'];
			$data['number'] = (empty($criteria['number'])) ? '' : $criteria['number'];
			$data['name'] = (empty($criteria['name'])) ? '' : $criteria['name'];
			
			$query = 'INSERT INTO room (building_id, number, name)'
					. 'VALUES (:building_id, :number, :name);';
			
			$statement = $collection->prepare($query);
			$result = $statement->execute($data);
			
			print_r($criteria);
			print_r($data);
			break;
	}
}