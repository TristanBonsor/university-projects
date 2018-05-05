<h2>Search Results</h2>

<?php

$config = require '../config.php';

require_once '../src/php/user/user.php';

$user = new User($config);

$url = "http" . (!empty($_SERVER['HTTPS'])?"s":"") . "://".$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'];
$url = preg_replace('/search\.php.*/', '', $url);

if(!$user->authenticated) {
	header('Location: ' . $url . 'unauthorized');
	exit;
}

function filter_criteria($elem) {
	return !empty($elem);
}

if(isset($_REQUEST['record_type'])) {
	
	require '../src/php/collection/Collection.php';
	
	$collection = new Collection($config);

	$record_type = $_REQUEST['record_type'];
	
	$criteria = array();
	$data = array();
	
	parse_str($_REQUEST['criteria'], $criteria);
	
	unset($criteria['record_type']);
	
	$criteria = array_filter($criteria, 'filter_criteria');

	if($record_type === 'artwork') {

		if(empty($criteria)) {
			$results = $collection->query('SELECT * FROM v_artwork_listings;');
		}
		else {
			$sql_query = 'SELECT * FROM v_artwork_listings WHERE ';
			
			$sql_and = array();
			$sql_or = array(
				'type'       => array(),
				'medium'     => array(),
				'collection' => array()
			);

			$sql_and[] = ((!empty($criteria['title'])) ? "title like '" . $criteria['title'] . "%'" : '');
			$sql_and[] = ((!empty($criteria['artist_first_name'])) ? "first_name like '" . $criteria['artist_first_name'] . "%'" : '');
			$sql_and[] = ((!empty($criteria['artist_last_name'])) ? "last_name like '" . $criteria['artist_last_name'] . "%'" : '');
			$sql_and[] = ((!empty($criteria['parent_location_names_key_value'])) ? "parent_location_id = " . $criteria['parent_location_names_key_value'] : '');
			$sql_and[] = ((!empty($criteria['building_number_names_key_value'])) ? "building_id = " . $criteria['building_number_names_key_value'] : '');
			$sql_and[] = ((!empty($criteria['room_number_names_key_value'])) ? "room_id = " . $criteria['room_number_names_key_value'] : '');

			if(!empty($criteria['types_checked'])) {
				foreach($criteria['types_checked'] as $type) {
					$sql_or['type'][] = "type = '" . $type . "'";
				}
				$sql_and[] = '(' . implode(' OR ', array_filter($sql_or['type'])) . ')';
			}

			if(!empty($criteria['mediums_checked'])) {
				foreach($criteria['mediums_checked'] as $medium) {
					$sql_or['medium'][] = "medium = '" . $medium . "'";
				}
				$sql_and[] = '(' . implode(' OR ', array_filter($sql_or['medium'])) . ')';
			}

			if(!empty($criteria['collections_checked'])) {
				foreach($criteria['collections_checked'] as $collect) {
					$sql_or['collection'][] = "collection = '" . $collect . "'";
				}
				$sql_and[] = '(' . implode(' OR ', array_filter($sql_or['collection'])) . ')';
			}

			$sql_query .= implode(' AND ', array_filter($sql_and)) . ';';

			$results = $collection->query($sql_query);
		}

		$i = 0;
		foreach($results as $result) {
			$data[$i] = $result;
			$data[$i]['artist_full_name'] = $result['first_name'] . ' ' . $result['last_name'];
			$data[$i]['building_number_name'] = $result['building_number'] . ' - ' . $result['building_name'];
			$data[$i]['room_number_name'] = $result['room_number'] . ' - ' . $result['room_name'];
			$data[$i]['images'] = array();

			$images = $collection->query('SELECT file_name FROM artwork_image WHERE artwork_id = ' . $result['record_id'] . ';');

			foreach($images as $image) {
				$data[$i]['images'][] = '<img class="thumbnail" src="img/' . $image['file_name'] . '">';
			}

			++$i;
		}

		$len = count($data);

		echo '<h2>Artwork</h2>';
		echo '<h3><span id="resultCount">' . $len . '</span>' . (($len == 1) ? ' record matches ' : ' records match ') . 'your search.</h3>';
		echo '<div id="searchResultsScrollBox">';

		if($len >= 1) {
			require '../src/php/collection/form/Form.php';

			for($i = 0; $i < $len; ++$i) {
				new Form('artwork-resultForm' . $data[$i]['record_id'], 'result', 'artwork', $data[$i]);
			}
		}

		echo '</div>';
	}
	else if($record_type === 'artist') {
		
		if(empty($criteria)) {
			$results = $collection->query('SELECT * FROM artist;');
		}
		else {
			$sql_query = 'SELECT * FROM artist WHERE ';

			$sql_and = array();

			$sql_and[] = ((!empty($criteria['first_name'])) ? "first_name like '" . $criteria['first_name'] . "%'" : '');
			$sql_and[] = ((!empty($criteria['last_name'])) ? "last_name like '" . $criteria['last_name'] . "%'" : '');
			$sql_and[] = ((!empty($criteria['phone_number'])) ? "phone_number like '" . $criteria['phone_number'] . "%'" : '');
			$sql_and[] = ((!empty($criteria['email'])) ? "email like '" . $criteria['email'] . "%'" : '');
			$sql_and[] = ((!empty($criteria['address'])) ? "address like '%" . $criteria['address'] . "%'" : '');
			$sql_and[] = ((!empty($criteria['city'])) ? "city like '" . $criteria['city'] . "%'" : '');
			$sql_and[] = ((!empty($criteria['province'])) ? "province like '" . $criteria['province'] . "%'" : '');
			$sql_and[] = ((!empty($criteria['postal_code'])) ? "postal_code like '" . $criteria['postal_code'] . "%'" : '');

			$sql_query .= implode(' AND ', array_filter($sql_and)) . ';';

			$results = $collection->query($sql_query);
		}

		$i = 0;
		foreach($results as $result) {
			$data[$i] = $result;
			$data[$i]['record_id'] = $result['id'];
			++$i;
		}
		
		$len = count($data);
		
		echo '<h2>Artist</h2>';
		echo '<h3><span id="resultCount">' . $len . '</span>' . (($len == 1) ? ' record matches ' : ' records match ') . 'your search.</h3>';
		echo '<div id="searchResultsScrollBox">';
		
		if($len >= 1) {
			require '../src/php/collection/form/Form.php';
		
			for($i = 0; $i < $len; ++$i) {
				new Form('artist-resultForm' . $data[$i]['id'], 'result', 'artist', $data[$i]);
			}
		}
		
		echo '</div>';
	}
	else if($record_type === 'artwork_type') {
		if(empty($criteria)) {
			$results = $collection->query('SELECT * FROM artwork_type;');
		}
		else {
			$sql_query = 'SELECT * FROM artwork_type WHERE ';
			
			$sql_and = array();
			
			$sql_and[] = ((!empty($criteria['type'])) ? "type like '" . $criteria['type'] . "%'" : '');

			$sql_query .= implode(' AND ', array_filter($sql_and)) . ';';

			$results = $collection->query($sql_query);
		}
		
		$i = 0;
		foreach($results as $result) {
			$data[$i] = $result;
			$data[$i]['record_id'] = $result['id'];
			++$i;
		}
		
		$len = count($data);
		
		echo '<h2>Artwork Type</h2>';
		echo '<h3><span id="resultCount">' . $len . '</span>' . (($len == 1) ? ' record matches ' : ' records match ') . 'your search.</h3>';
		echo '<div id="searchResultsScrollBox">';
		
		if($len >= 1) {
			require '../src/php/collection/form/Form.php';
		
			for($i = 0; $i < $len; ++$i) {
				new Form('artwork_type-resultForm' . $data[$i]['id'], 'result', 'artwork_type', $data[$i]);
			}
		}
		
		echo '</div>';
	}
	else if($record_type === 'artwork_medium') {
		if(empty($criteria)) {
			$results = $collection->query('SELECT * FROM artwork_medium;');
		}
		else {
			$sql_query = 'SELECT * FROM artwork_medium WHERE ';
				
			$sql_and = array();
				
			$sql_and[] = ((!empty($criteria['medium'])) ? "medium like '" . $criteria['medium'] . "%'" : '');
	
			$sql_query .= implode(' AND ', array_filter($sql_and)) . ';';

			$results = $collection->query($sql_query);
		}
	
		$i = 0;
		foreach($results as $result) {
			$data[$i] = $result;
			$data[$i]['record_id'] = $result['id'];
			++$i;
		}
	
		$len = count($data);
	
		echo '<h2>Artwork Medium</h2>';
		echo '<h3><span id="resultCount">' . $len . '</span>' . (($len == 1) ? ' record matches ' : ' records match ') . 'your search.</h3>';
		echo '<div id="searchResultsScrollBox">';
	
		if($len >= 1) {
			require '../src/php/collection/form/Form.php';
	
			for($i = 0; $i < $len; ++$i) {
				new Form('artwork_medium-resultForm' . $data[$i]['id'], 'result', 'artwork_medium', $data[$i]);
			}
		}
	
		echo '</div>';
	}
	else if($record_type === 'artwork_collection') {
		if(empty($criteria)) {
			$results = $collection->query('SELECT * FROM artwork_collection;');
		}
		else {
			$sql_query = 'SELECT * FROM artwork_collection WHERE ';
				
			$sql_and = array();
				
			$sql_and[] = ((!empty($criteria['collection'])) ? "collection like '" . $criteria['collection'] . "%'" : '');
	
			$sql_query .= implode(' AND ', array_filter($sql_and)) . ';';

			$results = $collection->query($sql_query);
		}
	
		$i = 0;
		foreach($results as $result) {
			$data[$i] = $result;
			$data[$i]['record_id'] = $result['id'];
			++$i;
		}
	
		$len = count($data);
	
		echo '<h2>Artwork Collection</h2>';
		echo '<h3><span id="resultCount">' . $len . '</span>' . (($len == 1) ? ' record matches ' : ' records match ') . 'your search.</h3>';
		echo '<div id="searchResultsScrollBox">';
	
		if($len >= 1) {
			require '../src/php/collection/form/Form.php';
	
			for($i = 0; $i < $len; ++$i) {
				new Form('artwork_collection-resultForm' . $data[$i]['id'], 'result', 'artwork_collection', $data[$i]);
			}
		}
	
		echo '</div>';
	}
	else if($record_type === 'parent_location') {
		if(empty($criteria)) {
			$results = $collection->query('SELECT * FROM parent_location;');
		}
		else {
			$sql_query = 'SELECT * FROM parent_location WHERE ';
	
			$sql_and = array();
	
			$sql_and[] = ((!empty($criteria['name'])) ? "name like '" . $criteria['name'] . "%'" : '');
			$sql_and[] = ((!empty($criteria['address'])) ? "address like '" . $criteria['address'] . "%'" : '');
			$sql_and[] = ((!empty($criteria['city'])) ? "city like '" . $criteria['city'] . "%'" : '');
			$sql_and[] = ((!empty($criteria['province'])) ? "province like '" . $criteria['province'] . "%'" : '');
			$sql_and[] = ((!empty($criteria['postal_code'])) ? "postal_code like '" . $criteria['postal_code'] . "%'" : '');
			$sql_and[] = ((!empty($criteria['phone_number'])) ? "phone_number like '" . $criteria['phone_number'] . "%'" : '');
	
			$sql_query .= implode(' AND ', array_filter($sql_and)) . ';';

			$results = $collection->query($sql_query);
		}
	
		$i = 0;
		foreach($results as $result) {
			$data[$i] = $result;
			$data[$i]['record_id'] = $result['id'];
			++$i;
		}
	
		$len = count($data);
	
		echo '<h2>Location</h2>';
		echo '<h3><span id="resultCount">' . $len . '</span>' . (($len == 1) ? ' record matches ' : ' records match ') . 'your search.</h3>';
		echo '<div id="searchResultsScrollBox">';
	
		if($len >= 1) {
			require '../src/php/collection/form/Form.php';
	
			for($i = 0; $i < $len; ++$i) {
				new Form('parent_location-resultForm' . $data[$i]['id'], 'result', 'parent_location', $data[$i]);
			}
		}
	
		echo '</div>';
	}
	else if($record_type === 'building') {
		if(empty($criteria)) {
			$results = $collection->query('SELECT * FROM v_building_listings;');
		}
		else {
			$sql_query = 'SELECT * FROM v_building_listings WHERE ';
	
			$sql_and = array();
	
			$sql_and[] = ((!empty($criteria['parent_location_names_key_value'])) ? "parent_location_id = '" . $criteria['parent_location_names_key_value'] . "'" : '');
			$sql_and[] = ((!empty($criteria['number'])) ? "number like '" . $criteria['number'] . "%'" : '');
			$sql_and[] = ((!empty($criteria['name'])) ? "name like '" . $criteria['name'] . "%'" : '');
	
			$sql_query .= implode(' AND ', array_filter($sql_and)) . ';';

			$results = $collection->query($sql_query);
		}
	
		$i = 0;
		foreach($results as $result) {
			$data[$i] = $result;
			$data[$i]['record_id'] = $result['record_id'];
			++$i;
		}
	
		$len = count($data);
	
		echo '<h2>Building</h2>';
		echo '<h3><span id="resultCount">' . $len . '</span>' . (($len == 1) ? ' record matches ' : ' records match ') . 'your search.</h3>';
		echo '<div id="searchResultsScrollBox">';
	
		if($len >= 1) {
			require '../src/php/collection/form/Form.php';
	
			for($i = 0; $i < $len; ++$i) {
				new Form('building-resultForm' . $data[$i]['record_id'], 'result', 'building', $data[$i]);
			}
		}
	
		echo '</div>';
	}
	else if($record_type === 'room') {
		if(empty($criteria)) {
			$results = $collection->query('SELECT * FROM v_room_listings;');
		}
		else {
			$sql_query = 'SELECT * FROM v_room_listings WHERE ';
	
			$sql_and = array();
	
			$sql_and[] = ((!empty($criteria['parent_location_names_key_value'])) ? "parent_location_id = '" . $criteria['parent_location_names_key_value'] . "'" : '');
			$sql_and[] = ((!empty($criteria['building_number_names_key_value'])) ? "building_id = '" . $criteria['building_number_names_key_value'] . "'" : '');
			$sql_and[] = ((!empty($criteria['number'])) ? "number like '" . $criteria['number'] . "%'" : '');
			$sql_and[] = ((!empty($criteria['name'])) ? "name like '" . $criteria['name'] . "%'" : '');
	
			$sql_query .= implode(' AND ', array_filter($sql_and)) . ';';

			$results = $collection->query($sql_query);
		}
	
		$i = 0;
		foreach($results as $result) {
			$data[$i] = $result;
			$data[$i]['record_id'] = $result['record_id'];
			++$i;
		}
	
		$len = count($data);
	
		echo '<h2>Room</h2>';
		echo '<h3><span id="resultCount">' . $len . '</span>' . (($len == 1) ? ' record matches ' : ' records match ') . 'your search.</h3>';
		echo '<div id="searchResultsScrollBox">';
	
		if($len >= 1) {
			require '../src/php/collection/form/Form.php';
	
			for($i = 0; $i < $len; ++$i) {
				new Form('room-resultForm' . $data[$i]['record_id'], 'result', 'room', $data[$i]);
			}
		}
	
		echo '</div>';
	}
}