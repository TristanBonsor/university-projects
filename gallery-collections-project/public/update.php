<?php

$config = require '../config.php';

require_once '../src/php/user/user.php';

$user = new User($config);

$url = "http" . (!empty($_SERVER['HTTPS'])?"s":"") . "://".$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'];
$url = preg_replace('/update\.php.*/', '', $url);

if(!$user->authenticated) {
	header('Location: ' . $url . 'unauthorized');
	exit;
}

require '../src/php/collection/Collection.php';
require '../src/php/collection/form/Form.php';

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

	$data['record_id'] = $criteria['record_id'];

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

			$query = 'UPDATE artwork '
					. 'SET title = :title, '
					. 'artist_id = :artist_id, '
					. 'type = :type, '
					. 'medium = :medium, '
					. 'parent_location_id = :parent_location_id, '
					. 'building_id = :building_id, '
					. 'room_id = :room_id, '
					. 'description = :description, '
					. 'collection = :collection '
					. 'WHERE id = :record_id;';
				
			$statement = $collection->prepare($query);
			$result = $statement->execute($data);

			if(!empty($criteria['images_checked'])) {

				$select_query = 'SELECT file_name FROM artwork_image WHERE id = ?;';
				$select_statement = $collection->prepare($select_query);

				$delete_query = 'DELETE FROM artwork_image WHERE id = ?;';
				$delete_statement = $collection->prepare($delete_query);

				foreach($criteria['images_checked'] as $image_id) {
					$arr_id = array($image_id);
					$select_statement->execute($arr_id);
					$delete_statement->execute($arr_id);
					while($image = $select_statement->fetch()) {
						unlink('img/' . $image['file_name']);
					}
				}
			}

			$query = 'SELECT * FROM v_artwork_listings WHERE record_id = ? LIMIT 1;';

			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));

			$result = $statement->fetch();

			$data['result'] = $result;
			$data['result']['artist_full_name'] = $result['first_name'] . ' ' . $result['last_name'];
			$data['result']['building_number_name'] = $result['building_number'] . ' - ' . $result['building_name'];
			$data['result']['room_number_name'] = $result['room_number'] . ' - ' . $result['room_name'];
			$data['result']['images'] = array();

			$images = $collection->query('SELECT file_name FROM artwork_image WHERE artwork_id = ' . $result['record_id'] . ';');

			foreach($images as $image) {
				$data['result']['images'][] = '<img class="thumbnail" src="img/' . $image['file_name'] . '">';
			}

			new Form('artwork-resultForm' . $data['record_id'], 'result', 'artwork', $data['result']);

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

			$query = 'UPDATE artist '
					. 'SET first_name = :first_name, '
					. 'last_name = :last_name, '
					. 'phone_number = :phone_number, '
					. 'email = :email, '
					. 'address = :address, '
					. 'city = :city, '
					. 'province = :province, '
					. 'postal_code = :postal_code '
					. 'WHERE id = :record_id;';
			
			$statement = $collection->prepare($query);
			$result = $statement->execute($data);

			$query = 'SELECT * FROM artist WHERE id = ? LIMIT 1;';
			
			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
			
			$result = $statement->fetch();
			
			$data['result'] = $result;
			$data['result']['record_id'] = $data['record_id'];

			new Form('artist-resultForm' . $data['record_id'], 'result', 'artist', $data['result']);

			break;
			
		case 'artwork_type':
			
			$data['type'] = (empty($criteria['type'])) ? '' : $criteria['type'];
			
			$query = 'UPDATE artwork_type '
					. 'SET type = :type '
					. 'WHERE id = :record_id;';

			$statement = $collection->prepare($query);
			$result = $statement->execute($data);

			$query = 'SELECT * FROM artwork_type WHERE id = ? LIMIT 1;';

			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));

			$result = $statement->fetch();

			$data['result'] = $result;
			$data['result']['record_id'] = $data['record_id'];

			new Form('artwork_type-resultForm' . $data['record_id'], 'result', 'artwork_type', $data['result']);

			break;

		case 'artwork_medium':
					
			$data['medium'] = (empty($criteria['medium'])) ? '' : $criteria['medium'];
					
			$query = 'UPDATE artwork_medium '
					. 'SET medium = :medium '
					. 'WHERE id = :record_id;';
			
			$statement = $collection->prepare($query);
			$result = $statement->execute($data);
			
			$query = 'SELECT * FROM artwork_medium WHERE id = ? LIMIT 1;';
			
			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
			
			$result = $statement->fetch();
			
			$data['result'] = $result;
			$data['result']['record_id'] = $data['record_id'];
			
			new Form('artwork_medium-resultForm' . $data['record_id'], 'result', 'artwork_medium', $data['result']);

			break;

		case 'artwork_collection':
					
			$data['collection'] = (empty($criteria['collection'])) ? '' : $criteria['collection'];
					
			$query = 'UPDATE artwork_collection '
					. 'SET collection = :collection '
					. 'WHERE id = :record_id;';
			
			$statement = $collection->prepare($query);
			$result = $statement->execute($data);
			
			$query = 'SELECT * FROM artwork_collection WHERE id = ? LIMIT 1;';
			
			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
			
			$result = $statement->fetch();
			
			$data['result'] = $result;
			$data['result']['record_id'] = $data['record_id'];
			
			new Form('artwork_collection-resultForm' . $data['record_id'], 'result', 'artwork_collection', $data['result']);
			
			break;

		case 'parent_location':
			
			$data['name'] = (empty($criteria['name'])) ? '' : $criteria['name'];
			$data['phone_number'] = (empty($criteria['phone_number'])) ? '' : $criteria['phone_number'];
			$data['address'] = (empty($criteria['address'])) ? '' : $criteria['address'];
			$data['city'] = (empty($criteria['city'])) ? '' : $criteria['city'];
			$data['province'] = (empty($criteria['province'])) ? '' : $criteria['province'];
			$data['postal_code'] = (empty($criteria['postal_code'])) ? '' : $criteria['postal_code'];
			
			$query = 'UPDATE parent_location '
					. 'SET name = :name, '
					. 'phone_number = :phone_number, '
					. 'address = :address, '
					. 'city = :city, '
					. 'province = :province, '
					. 'postal_code = :postal_code '
					. 'WHERE id = :record_id;';
					
			$statement = $collection->prepare($query);
			$result = $statement->execute($data);

			$query = 'SELECT * FROM parent_location WHERE id = ? LIMIT 1;';

			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));

			$result = $statement->fetch();
					
			$data['result'] = $result;
			$data['result']['record_id'] = $data['record_id'];
			
			new Form('parent_location-resultForm' . $data['record_id'], 'result', 'parent_location', $data['result']);

			break;
			
		case 'building':

			$data['number'] = (empty($criteria['number'])) ? '' : $criteria['number'];
			$data['name'] = (empty($criteria['name'])) ? '' : $criteria['name'];

			$query = 'UPDATE building '
					. 'SET name = :name, '
					. 'number = :number '
					. 'WHERE id = :record_id;';

			$statement = $collection->prepare($query);
			$result = $statement->execute($data);
			
			$query = 'SELECT * FROM v_building_listings WHERE record_id = ? LIMIT 1;';
			
			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
			
			$result = $statement->fetch();
					
			$data['result'] = $result;
			$data['result']['record_id'] = $data['record_id'];

			new Form('building-resultForm' . $data['record_id'], 'result', 'building', $data['result']);

			break;
			
		case 'room':
			
			$data['number'] = (empty($criteria['number'])) ? '' : $criteria['number'];
			$data['name'] = (empty($criteria['name'])) ? '' : $criteria['name'];
			
			$query = 'UPDATE room '
					. 'SET name = :name, '
					. 'number = :number '
					. 'WHERE id = :record_id;';
			
			$statement = $collection->prepare($query);
			$result = $statement->execute($data);
					
			$query = 'SELECT * FROM v_room_listings WHERE record_id = ? LIMIT 1;';
					
			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
					
			$result = $statement->fetch();
					
			$data['result'] = $result;
			$data['result']['record_id'] = $data['record_id'];
			
			new Form('room-resultForm' . $data['record_id'], 'result', 'room', $data['result']);
			
			break;
	}
}