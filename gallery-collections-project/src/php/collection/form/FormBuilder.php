<?php
require '../src/php/collection/form/Form.php';
require '../src/php/collection/Collection.php';

class FormBuilderHelper {

	private $collection;

	function __construct($collection) {
		$this->collection = $collection;
	}

	function getTitles() {
		$data = array();

		$results = $this->collection->query('SELECT DISTINCT title FROM artwork;');

		foreach($results as $result) {
			$data[$result['title']] = $result['title'];
		}

		return $data;
	}

	function getArtistFirstNames() {
		$data = array();
	
		$results = $this->collection->query('SELECT first_name FROM artist;');
	
		foreach($results as $result) { 
			$data[$result['first_name']] = $result['first_name'];
		}
	
		return $data;
	}

	function getArtistLastNames() {
		$data = array();
	
		$results = $this->collection->query('SELECT last_name FROM artist;');
	
		foreach($results as $result) {
			$data[$result['last_name']] = $result['last_name'];
		}
	
		return $data;
	}

	function getArtistPhoneNumbers() {
		$data = array();
	
		$results = $this->collection->query('SELECT phone_number FROM artist;');
	
		foreach($results as $result) {
			$data[$result['phone_number']] = $result['phone_number'];
		}
	
		return $data;
	}

	function getArtistEmails() {
		$data = array();
	
		$results = $this->collection->query('SELECT email FROM artist;');
	
		foreach($results as $result) {
			$data[$result['email']] = $result['email'];
		}
	
		return $data;
	}

	function getArtistAddresses() {
		$data = array();
	
		$results = $this->collection->query('SELECT address FROM artist;');
	
		foreach($results as $result) {
			$data[$result['address']] = $result['address'];
		}
	
		return $data;
	}
	
	function getArtistCities() {
		$data = array();
	
		$results = $this->collection->query('SELECT city FROM artist;');
	
		foreach($results as $result) {
			$data[$result['city']] = $result['city'];
		}
	
		return $data;
	}
	
	function getArtistProvinces() {
		$data = array();
	
		$results = $this->collection->query('SELECT province FROM artist;');
	
		foreach($results as $result) {
			$data[$result['province']] = $result['province'];
		}
	
		return $data;
	}

	function getArtistPostalCodes() {
		$data = array();
	
		$results = $this->collection->query('SELECT postal_code FROM artist;');
	
		foreach($results as $result) {
			$data[$result['postal_code']] = $result['postal_code'];
		}
	
		return $data;
	}

	function getArtistFullNames() {
		$data = array();

		$results = $this->collection->query('SELECT first_name, last_name FROM artist;');

		foreach($results as $result) {
			$full_name = $result['first_name'] . ' ' . $result['last_name'];
			$data[$full_name] = $full_name;
		}

		return $data;
	}

	function getArtistFullNamesKeyValue() {
		$data = array();

		$results = $this->collection->query('SELECT id, first_name, last_name FROM artist;');

		foreach($results as $result) {
			$full_name = $result['first_name'] . ' ' . $result['last_name'];
			$data[$full_name] = $result['id'];
		}

		return $data;
	}

	function getArtworkTypes() {
		$data = array();

		$results = $this->collection->query('SELECT type FROM artwork_type;');
		
		foreach($results as $result) {
			$data[$result['type']] = $result['type'];
		}

		return $data;
	}

	function getArtworkMediums() {
		$data = array();
	
		$results = $this->collection->query('SELECT medium FROM artwork_medium;');
	
		foreach($results as $result) {
			$data[$result['medium']] = $result['medium'];
		}
	
		return $data;
	}

	function getArtworkCollections() {
		$data = array();
	
		$results = $this->collection->query('SELECT collection FROM artwork_collection;');
	
		foreach($results as $result) {
			$data[$result['collection']] = $result['collection'];
		}
	
		return $data;
	}

	function getParentLocationNames() {
		$data = array();

		$results = $this->collection->query('SELECT name FROM parent_location;');

		foreach($results as $result) {
			$data[$result['name']] = $result['name'];
		}

		return $data;
	}

	function getParentLocationAddresses() {
		$data = array();
	
		$results = $this->collection->query('SELECT address FROM parent_location;');
	
		foreach($results as $result) {
			$data[$result['address']] = $result['address'];
		}
	
		return $data;
	}
	
	function getParentLocationCities() {
		$data = array();
	
		$results = $this->collection->query('SELECT city FROM parent_location;');
	
		foreach($results as $result) {
			$data[$result['city']] = $result['city'];
		}
	
		return $data;
	}
	
	function getParentLocationProvinces() {
		$data = array();
	
		$results = $this->collection->query('SELECT province FROM parent_location;');
	
		foreach($results as $result) {
			$data[$result['province']] = $result['province'];
		}
	
		return $data;
	}
	
	function getParentLocationPostalCodes() {
		$data = array();
	
		$results = $this->collection->query('SELECT postal_code FROM parent_location;');
	
		foreach($results as $result) {
			$data[$result['postal_code']] = $result['postal_code'];
		}
	
		return $data;
	}
	
	function getParentLocationPhoneNumbers() {
		$data = array();
	
		$results = $this->collection->query('SELECT phone_number FROM parent_location;');
	
		foreach($results as $result) {
			$data[$result['phone_number']] = $result['phone_number'];
		}
	
		return $data;
	}
	
	function getParentLocationNamesKeyValue() {
		$data = array();

		$results = $this->collection->query('SELECT id, name FROM parent_location;');

		foreach($results as $result) {
			$data[$result['name']] = $result['id'];
		}

		return $data;
	}

	function getBuildingNumbers($parent_location_id) {
		$data = array();
	
		$results = $this->collection->query('SELECT number FROM building WHERE parent_location_id = ' . $parent_location_id . ';');
	
		foreach($results as $result) {
			$data[$result['number']] = $result['number'];
		}
	
		return $data;
	}
	
	function getBuildingNames($parent_location_id) {
		$data = array();
	
		$results = $this->collection->query('SELECT name FROM building WHERE parent_location_id = ' . $parent_location_id . ';');
	
		foreach($results as $result) {
			$data[$result['name']] = $result['name'];
		}
	
		return $data;
	}

	function getBuildingNumberNamesKeyValue($parent_location_id) {
		$data = array();

		$results = $this->collection->query('SELECT id, name, number FROM building WHERE parent_location_id = ' . $parent_location_id . ';');

		foreach($results as $result) {
			$data[$result['number'] . ' - ' . $result['name']] = $result['id'];
		}

		return $data;
	}

	function getRoomNumberNamesKeyValue($building_id) {
		$data = array();

		$results = $this->collection->query('SELECT id, name, number FROM room WHERE building_id = ' . $building_id . ';');
			
		foreach($results as $result) {
			$data[$result['number'] . ' - ' . $result['name']] = $result['id'];
		}

		return $data;
	}

	function getImagesKeyValue($artwork_id) {
		$data = array();
		
		$results = $this->collection->query('SELECT id, file_name FROM artwork_image WHERE artwork_id = ' . $artwork_id . ';');
			
		foreach($results as $result) {
			$data['<img src="img/' . $result['file_name'] . '">'] = $result['id'];
		}
		
		return $data;
	}
}

class FormBuilder {

	private $builder_functions = array(
		'artwork'            => array(),
		'artist'             => array(),
		'artwork_type'       => array(),
		'artwork_medium'     => array(),
		'parent_location'    => array(),
		'building'           => array(),
		'room'               => array(),
		'artwork_collection' => array(),
	);

	private $config = false;

	private $access_type = false;

	private $record_type = false;

	function __construct($config, $access_type, $record_type) {
		$this->config = $config;
		$this->access_type = $access_type;
		$this->record_type = $record_type;

		$this->initBuilderFunctions();
		
		$this->processAccessType();
	}

	private function initBuilderFunctions() {
		$this->initArtworkBuilderFunctions();
		$this->initArtistBuilderFunctions();
		$this->initArtworkTypeBuilderFunctions();
		$this->initArtworkMediumBuilderFunctions();
		$this->initParentLocationBuilderFunctions();
		$this->initBuildingBuilderFunctions();
		$this->initRoomBuilderFunctions();
		$this->initArtworkCollectionBuilderFunctions();
	}

	/**
	 * Initialize artwork form builder functions.
	 */
	private function initArtworkBuilderFunctions() {
		/**
		 * Populate and return artwork search form.
		 */
		$this->builder_functions['artwork']['search'] = function($config) {
		
			$collection = new Collection($config);

			$helper = new FormBuilderHelper($collection);

			$data = array(
					'title'                => $_REQUEST['title'],
					'artist_first_name'    => $_REQUEST['artist_first_name'],
					'artist_last_name'     => $_REQUEST['artist_last_name'],
					/*'artist_full_name'     => $_REQUEST['artist_full_name'],*/
					'types_checked'        => $_REQUEST['types_checked'],
					'mediums_checked'      => $_REQUEST['mediums_checked'],
					'parent_location_id'   => $_REQUEST['parent_location_names_key_value'],
					'building_id'          => $_REQUEST['building_number_names_key_value'],
					'room_id'              => $_REQUEST['room_number_names_key_value'],
					'collections_checked'  => $_REQUEST['collections_checked']
			);

			$data['titles']                          = $helper->getTitles();
			$data['artist_first_names']              = $helper->getArtistFirstNames();
			$data['artist_last_names']               = $helper->getArtistLastNames();
			/*$data['artist_full_names']               = $helper->getArtistFullNames();*/
			$data['types']                           = $helper->getArtworkTypes();
			$data['mediums']                         = $helper->getArtworkMediums();
			$data['parent_location_names_key_value'] = $helper->getParentLocationNamesKeyValue();
			$data['building_number_names_key_value'] = $helper->getBuildingNumberNamesKeyValue($data['parent_location_id']);
			$data['room_number_names_key_value']     = $helper->getRoomNumberNamesKeyValue($data['building_id']);
			$data['collections']                     = $helper->getArtworkCollections();

			new Form('artwork-searchForm', 'search', 'artwork', $data);
		};
		
		/**
		 * Populate and return artwork create form.
		 */
		$this->builder_functions['artwork']['create'] = function($config) {

			$collection = new Collection($config);

			$helper = new FormBuilderHelper($collection);

			$data = array(
					'title'                       => $_REQUEST['title'],
					'artist_full_name_key_value'  => $_REQUEST['artist_full_names_key_value'],
					'type'                        => $_REQUEST['types'],
					'medium'                      => $_REQUEST['mediums'],
					'parent_location_id'          => $_REQUEST['parent_location_names_key_value'],
					'building_id'                 => $_REQUEST['building_number_names_key_value'],
					'room_id'                     => $_REQUEST['room_number_names_key_value'],
					'description'                 => $_REQUEST['description'],
					'collection'                  => $_REQUEST['collections']
			);

			$data['artist_full_names_key_value']     = $helper->getArtistFullNamesKeyValue();
			$data['types']                           = $helper->getArtworkTypes();
			$data['mediums']                         = $helper->getArtworkMediums();
			$data['parent_location_names_key_value'] = $helper->getParentLocationNamesKeyValue();
			$data['building_number_names_key_value'] = $helper->getBuildingNumberNamesKeyValue($data['parent_location_id']);
			$data['room_number_names_key_value']     = $helper->getRoomNumberNamesKeyValue($data['building_id']);
			$data['collections']                     = $helper->getArtworkCollections();

			new Form('artwork-createForm', 'create', 'artwork', $data);
		};
		
		/**
		 * Populate and return artwork update form.
		 */
		$this->builder_functions['artwork']['update'] = function($config) {

			$collection = new Collection($config);

			$helper = new FormBuilderHelper($collection);

			$data = array(
				'record_id' => $_REQUEST['record_id']
			);

			$data['artist_full_names_key_value']     = $helper->getArtistFullNamesKeyValue();
			$data['types']                           = $helper->getArtworkTypes();
			$data['mediums']                         = $helper->getArtworkMediums();
			$data['parent_location_names_key_value'] = $helper->getParentLocationNamesKeyValue();
			$data['collections']                     = $helper->getArtworkCollections();

			$query = 'SELECT * FROM v_artwork_listings WHERE record_id = ? LIMIT 1;';

			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
			$result = $statement->fetch();

			$data['title']                = $result['title'];
			$data['type']                 = $result['type'];
			$data['medium']               = $result['medium'];
			$data['description']          = $result['description'];
			$data['artist_id']            = $result['artist_id'];
			$data['artist_full_name']     = $result['first_name'] . ' ' . $result['last_name'];
			$data['parent_location_id']   = $result['parent_location_id'];
			$data['parent_location_name'] = $result['parent_location_name'];
			$data['building_id']          = $result['building_id'];
			$data['building_number_name'] = $result['building_number'] . ' - ' . $result['building_name'];
			$data['room_id']              = $result['room_id'];
			$data['room_number_name']     = $result['room_number'] . ' - ' . $result['room_name'];
			$data['collection']           = $result['collection'];

			$data['building_number_names_key_value'] = $helper->getBuildingNumberNamesKeyValue($data['parent_location_id']);
			$data['room_number_names_key_value']     = $helper->getRoomNumberNamesKeyValue($data['building_id']);

			$data['images'] = $helper->getImagesKeyValue($data['record_id']);

			new Form('artwork-updateForm', 'update', 'artwork', $data);
		};
		
		/**
		 * Populate and return artwork read form.
		 */
		$this->builder_functions['artwork']['read'] = function($config) {

			$collection = new Collection($config);

			$data = array(
				'record_id' => $_REQUEST['record_id'],
				'images'    => array()
			);

			$query = 'SELECT * FROM v_artwork_listings WHERE record_id = ? LIMIT 1;';

			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
			$result = $statement->fetch();

			$data['title']                = $result['title'];
			$data['type']                 = $result['type'];
			$data['medium']               = $result['medium'];
			$data['description']          = $result['description'];
			$data['artist_full_name']     = $result['first_name'] . ' ' . $result['last_name'];
			$data['parent_location_name'] = $result['parent_location_name'];
			$data['building_number_name'] = $result['building_number'] . ' - ' . $result['building_name'];
			$data['room_number_name']     = $result['room_number'] . ' - ' . $result['room_name'];
			$data['collection']           = $result['collection'];

			$images = $collection->query('SELECT file_name FROM artwork_image WHERE artwork_id = ' . $result['record_id'] . ';');

			foreach($images as $image) {
				$data['images'][] = '<img src="img/' . $image['file_name'] . '">';
			}

			new Form('artwork-readForm', 'read', 'artwork', $data);
		};
	}

	/**
	 * Initialize artist form builder functions.
	 */
	private function initArtistBuilderFunctions() {
		/**
		 * Populate and return artist search form.
		 */
		$this->builder_functions['artist']['search'] = function($config) {

			$collection = new Collection($config);

			$helper = new FormBuilderHelper($collection);

			$data = array(
				'first_name'   => $_REQUEST['first_name'],
				'last_name'    => $_REQUEST['last_name'],
				'phone_number' => $_REQUEST['phone_number'],
				'email'        => $_REQUEST['email'],
				'address'      => $_REQUEST['address'],
				'city'         => $_REQUEST['city'],
				'province'     => $_REQUEST['province'],
				'postal_code'  => $_REQUEST['postal_code']
			);

			$data['first_names'] = $helper->getArtistFirstNames();
			$data['last_names']  = $helper->getArtistLastNames();
			$data['phone_numbers'] = $helper->getArtistPhoneNumbers();
			$data['emails'] = $helper->getArtistEmails();
			$data['addresses'] = $helper->getArtistAddresses();
			$data['cities'] = $helper->getArtistCities();
			$data['provinces'] = $helper->getArtistProvinces();
			$data['postal_codes'] = $helper->getArtistPostalCodes();

			new Form('artist-searchForm', 'search', 'artist', $data);
		};
		
		/**
		 *
		 */
		$this->builder_functions['artist']['create'] = function($config) {
			
			$collection = new Collection($config);
			
			$helper = new FormBuilderHelper($collection);

			$data = array(
				'first_name'   => $_REQUEST['first_name'],
				'last_name'    => $_REQUEST['last_name'],
				'phone_number' => $_REQUEST['phone_number'],
				'email'        => $_REQUEST['email'],
				'address'      => $_REQUEST['address'],
				'city'         => $_REQUEST['city'],
				'province'     => $_REQUEST['province'],
				'postal_code'  => $_REQUEST['postal_code']
			);

			$data['cities'] = $helper->getArtistCities();
			$data['provinces'] = $helper->getArtistProvinces();
			$data['postal_codes'] = $helper->getArtistPostalCodes();

			new Form('artist-createForm', 'create', 'artist', $data);
		};
		
		/**
		 *
		 */
		$this->builder_functions['artist']['update'] = function($config) {

			$collection = new Collection($config);
			
			$helper = new FormBuilderHelper($collection);
			
			$data = array(
					'record_id' => $_REQUEST['record_id']
			);
			
			$data['cities'] = $helper->getArtistCities();
			$data['provinces'] = $helper->getArtistProvinces();
			$data['postal_codes'] = $helper->getArtistPostalCodes();
			
			$query = 'SELECT * FROM artist WHERE id = ? LIMIT 1;';
			
			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
			$result = $statement->fetch();

			$data['first_name']   = $result['first_name'];
			$data['last_name']    = $result['last_name'];
			$data['phone_number'] = $result['phone_number'];
			$data['email']        = $result['email'];
			$data['address']      = $result['address'];
			$data['city']         = $result['city'];
			$data['province']     = $result['province'];
			$data['postal_code']  = $result['postal_code'];
			
			new Form('artist-updateForm', 'update', 'artist', $data);
		};
		
		/**
		 *
		 */
		$this->builder_functions['artist']['read'] = function($config) {

			$collection = new Collection($config);

			$data = array(
					'record_id' => $_REQUEST['record_id'],
			);
			
			$query = 'SELECT * FROM artist WHERE id = ? LIMIT 1;';

			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
			$result = $statement->fetch();
			
			$data['first_name']   = $result['first_name'];
			$data['last_name']    = $result['last_name'];
			$data['phone_number'] = $result['phone_number'];
			$data['email']        = $result['email'];
			$data['address']      = $result['address'];
			$data['city']         = $result['city'];
			$data['province']     = $result['province'];
			$data['postal_code']  = $result['postal_code'];
			
			new Form('artist-readForm', 'read', 'artist', $data);
		};
	}
	
	private function initArtworkTypeBuilderFunctions() {
		/**
		 *
		 */
		$this->builder_functions['artwork_type']['search'] = function($config) {
			$collection = new Collection($config);
			
			$helper = new FormBuilderHelper($collection);
			
			$data = array('type' => $_REQUEST['type']);

			$data['types'] = $helper->getArtworkTypes();

			new Form('artwork_type-searchForm', 'search', 'artwork_type', $data);
		};
	
		/**
		 *
		 */
		$this->builder_functions['artwork_type']['create'] = function($config) {

			$data = array('type' => $_REQUEST['type']);
			
			new Form('artwork_type-createForm', 'create', 'artwork_type', $data);
		};
	
		/**
		 *
		 */
		$this->builder_functions['artwork_type']['update'] = function($config) {

			$collection = new Collection($config);
				
			$helper = new FormBuilderHelper($collection);
				
			$data = array(
					'record_id' => $_REQUEST['record_id']
			);

			$query = 'SELECT * FROM artwork_type WHERE id = ? LIMIT 1;';
				
			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
			$result = $statement->fetch();
			
			$data['type']   = $result['type'];

			new Form('artwork_type-updateForm', 'update', 'artwork_type', $data);
		};
	
		/**
		 *
		 */
		$this->builder_functions['artwork_type']['read'] = function($config) {

			$collection = new Collection($config);
			
			$data = array(
					'record_id' => $_REQUEST['record_id'],
			);
				
			$query = 'SELECT * FROM artwork_type WHERE id = ? LIMIT 1;';
			
			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
			$result = $statement->fetch();
				
			$data['type']   = $result['type'];
			
			new Form('artwork_type-readForm', 'read', 'artwork_type', $data);
		};
	}
	
	private function initArtworkMediumBuilderFunctions() {
		/**
		 *
		 */
		$this->builder_functions['artwork_medium']['search'] = function($config) {
			$collection = new Collection($config);
				
			$helper = new FormBuilderHelper($collection);
				
			$data = array('medium' => $_REQUEST['medium']);
			
			$data['mediums'] = $helper->getArtworkMediums();
			
			new Form('artwork_medium-searchForm', 'search', 'artwork_medium', $data);
		};
	
		/**
		 *
		 */
		$this->builder_functions['artwork_medium']['create'] = function($config) {
			$data = array('medium' => $_REQUEST['medium']);
				
			new Form('artwork_medium-createForm', 'create', 'artwork_medium', $data);
		};
	
		/**
		 *
		 */
		$this->builder_functions['artwork_medium']['update'] = function($config) {
			$collection = new Collection($config);
			
			$helper = new FormBuilderHelper($collection);
			
			$data = array(
					'record_id' => $_REQUEST['record_id']
			);
			
			$query = 'SELECT * FROM artwork_medium WHERE id = ? LIMIT 1;';
			
			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
			$result = $statement->fetch();
				
			$data['medium']   = $result['medium'];
			
			new Form('artwork_medium-updateForm', 'update', 'artwork_medium', $data);
		};
	
		/**
		 *
		 */
		$this->builder_functions['artwork_medium']['read'] = function($config) {

			$collection = new Collection($config);
				
			$data = array(
					'record_id' => $_REQUEST['record_id'],
			);
			
			$query = 'SELECT * FROM artwork_medium WHERE id = ? LIMIT 1;';
				
			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
			$result = $statement->fetch();
			
			$data['medium']   = $result['medium'];
				
			new Form('artwork_medium-readForm', 'read', 'artwork_medium', $data);
		};
	}
	
	private function initParentLocationBuilderFunctions() {
		/**
		 *
		 */
		$this->builder_functions['parent_location']['search'] = function($config) {
			$collection = new Collection($config);
			
			$helper = new FormBuilderHelper($collection);
			
			$data = array(
				'name'         => $_REQUEST['name'],
				'address'      => $_REQUEST['address'],
				'city'         => $_REQUEST['city'],
				'province'     => $_REQUEST['province'],
				'postal_code'  => $_REQUEST['postal_code'],
				'phone_number' => $_REQUEST['phone_number']
			);
				
			$data['names']         = $helper->getParentLocationNames();
			$data['addresses']     = $helper->getParentLocationAddresses();
			$data['cities']        = $helper->getParentLocationCities();
			$data['provinces']     = $helper->getParentLocationProvinces();
			$data['postal_codes']  = $helper->getParentLocationPostalCodes();
			$data['phone_numbers'] = $helper->getParentLocationPhoneNumbers();
				
			new Form('parent_location-searchForm', 'search', 'parent_location', $data);
		};
	
		/**
		 *
		 */
		$this->builder_functions['parent_location']['create'] = function($config) {
			$data = array(
				'name'         => $_REQUEST['name'],
				'address'      => $_REQUEST['address'],
				'city'         => $_REQUEST['city'],
				'province'     => $_REQUEST['province'],
				'postal_code'  => $_REQUEST['postal_code'],
				'phone_number' => $_REQUEST['phone_number']
			);

			new Form('parent_location-createForm', 'create', 'parent_location', $data);
		};
	
		/**
		 *
		 */
		$this->builder_functions['parent_location']['update'] = function($config) {
			$collection = new Collection($config);
				
			$helper = new FormBuilderHelper($collection);
				
			$data = array(
					'record_id' => $_REQUEST['record_id']
			);
				
			$query = 'SELECT * FROM parent_location WHERE id = ? LIMIT 1;';
				
			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
			$result = $statement->fetch();
			
			$data['name']   = $result['name'];
			$data['phone_number'] = $result['phone_number'];
			$data['address']      = $result['address'];
			$data['city']         = $result['city'];
			$data['province']     = $result['province'];
			$data['postal_code']  = $result['postal_code'];
				
			new Form('parent_location-updateForm', 'update', 'parent_location', $data);
		};
	
		/**
		 *
		 */
		$this->builder_functions['parent_location']['read'] = function($config) {
			$collection = new Collection($config);
			
			$data = array(
					'record_id' => $_REQUEST['record_id'],
			);
				
			$query = 'SELECT * FROM parent_location WHERE id = ? LIMIT 1;';
			
			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
			$result = $statement->fetch();
				
			$data['name']         = $result['name'];
			$data['address']      = $result['address'];
			$data['city']         = $result['city'];
			$data['province']     = $result['province'];
			$data['postal_code']  = $result['postal_code'];
			$data['phone_number'] = $result['phone_number'];
			
			new Form('parent_location-readForm', 'read', 'parent_location', $data);
		};
	}

	private function initBuildingBuilderFunctions() {
		/**
		 *
		 */
		$this->builder_functions['building']['search'] = function($config) {
			$collection = new Collection($config);
			
			$helper = new FormBuilderHelper($collection);
			
			$data = array(
				'parent_location_id'   => $_REQUEST['parent_location_names_key_value'],
				'number'               => $_REQUEST['number'],
				'name'                 => $_REQUEST['name'],
			);

			$data['parent_location_names_key_value'] = $helper->getParentLocationNamesKeyValue();
			/*$data['numbers']                         = $helper->getBuildingNumbers($data['parent_location_id']);
			$data['names']                           = $helper->getBuildingNames($data['parent_location_id']);*/

			new Form('building-searchForm', 'search', 'building', $data);
		};
	
		/**
		 *
		 */
		$this->builder_functions['building']['create'] = function($config) {
			$collection = new Collection($config);
			
			$helper = new FormBuilderHelper($collection);
			
			$data = array(
					'parent_location_id' => $_REQUEST['parent_location_names_key_value'],
					'number'             => $_REQUEST['number'],
					'name'               => $_REQUEST['name'],
			);
			
			$data['parent_location_names_key_value'] = $helper->getParentLocationNamesKeyValue();
			
			new Form('building-createForm', 'create', 'building', $data);
		};
	
		/**
		 *
		 */
		$this->builder_functions['building']['update'] = function($config) {
			$collection = new Collection($config);
			
			$helper = new FormBuilderHelper($collection);
			
			$data = array(
					'record_id' => $_REQUEST['record_id']
			);
			
			$query = 'SELECT * FROM v_building_listings WHERE record_id = ? LIMIT 1;';
			
			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
			$result = $statement->fetch();

			$data['parent_location_name'] = $result['parent_location_name'];
			$data['number']               = $result['number'];
			$data['name']                 = $result['name'];

			new Form('building-updateForm', 'update', 'building', $data);
		};
	
		/**
		 *
		 */
		$this->builder_functions['building']['read'] = function($config) {
			$collection = new Collection($config);
				
			$data = array(
					'record_id' => $_REQUEST['record_id'],
			);
			
			$query = 'SELECT * FROM v_building_listings WHERE record_id = ? LIMIT 1;';
				
			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
			$result = $statement->fetch();
			
			$data['parent_location_name'] = $result['parent_location_name'];
			$data['number']               = $result['number'];
			$data['name']                 = $result['name'];

			new Form('building-readForm', 'read', 'building', $data);
		};
	}
	
	private function initRoomBuilderFunctions() {
		/**
		 *
		 */
		$this->builder_functions['room']['search'] = function($config) {
			$collection = new Collection($config);
				
			$helper = new FormBuilderHelper($collection);
				
			$data = array(
				'parent_location_id' => $_REQUEST['parent_location_names_key_value'],
				'building_id'        => $_REQUEST['building_number_names_key_value'],
				'number'             => $_REQUEST['number'],
				'name'               => $_REQUEST['name'],
			);
			
			$data['parent_location_names_key_value'] = $helper->getParentLocationNamesKeyValue();
			$data['building_number_names_key_value'] = $helper->getBuildingNumberNamesKeyValue();
			/*$data['numbers']                         = $helper->getBuildingNumbers($data['parent_location_id']);
			 $data['names']                           = $helper->getBuildingNames($data['parent_location_id']);*/
			
			new Form('room-searchForm', 'search', 'room', $data);
		};
	
		/**
		 *
		 */
		$this->builder_functions['room']['create'] = function($config) {
			$collection = new Collection($config);
				
			$helper = new FormBuilderHelper($collection);
				
			$data = array(
					'parent_location_id' => $_REQUEST['parent_location_names_key_value'],
					'building_id'        => $_REQUEST['building_number_names_key_value'],
					'number'             => $_REQUEST['number'],
					'name'               => $_REQUEST['name'],
			);
				
			$data['parent_location_names_key_value'] = $helper->getParentLocationNamesKeyValue();
			$data['building_number_names_key_value'] = $helper->getBuildingNumberNamesKeyValue();
				
			new Form('room-createForm', 'create', 'room', $data);
		};
	
		/**
		 *
		 */
		$this->builder_functions['room']['update'] = function($config) {
			$collection = new Collection($config);
				
			$helper = new FormBuilderHelper($collection);
				
			$data = array(
					'record_id' => $_REQUEST['record_id']
			);
				
			$query = 'SELECT * FROM v_room_listings WHERE record_id = ? LIMIT 1;';
				
			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
			$result = $statement->fetch();
			
			$data['parent_location_name'] = $result['parent_location_name'];
			$data['building_number']      = $result['building_number'];
			$data['building_name']        = $result['building_name'];
			$data['number']               = $result['number'];
			$data['name']                 = $result['name'];
			
			new Form('room-updateForm', 'update', 'room', $data);
		};
	
		/**
		 *
		 */
		$this->builder_functions['room']['read'] = function($config) {
			$collection = new Collection($config);
			
			$data = array(
					'record_id' => $_REQUEST['record_id'],
			);
				
			$query = 'SELECT * FROM v_room_listings WHERE record_id = ? LIMIT 1;';
			
			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
			$result = $statement->fetch();
				
			$data['parent_location_name'] = $result['parent_location_name'];
			$data['building_number']      = $result['building_number'];
			$data['building_name']        = $result['building_name'];
			$data['number']               = $result['number'];
			$data['name']                 = $result['name'];
			
			new Form('room-readForm', 'read', 'room', $data);
		};
	}

	private function initArtworkCollectionBuilderFunctions() {
		/**
		 *
		 */
		$this->builder_functions['artwork_collection']['search'] = function($config) {

			$collection = new Collection($config);
				
			$helper = new FormBuilderHelper($collection);
				
			$data = array('collection' => $_REQUEST['collection']);
			
			$data['collections'] = $helper->getArtworkCollections();
			
			new Form('artwork_collection-searchForm', 'search', 'artwork_collection', $data);
		};
		
		/**
		 *
		 */
		$this->builder_functions['artwork_collection']['create'] = function($config) {
			$data = array('collection' => $_REQUEST['collection']);
				
			new Form('artwork_collection-createForm', 'create', 'artwork_collection', $data);
		};
		
		/**
		 *
		 */
		$this->builder_functions['artwork_collection']['update'] = function($config) {
			$collection = new Collection($config);
			
			$helper = new FormBuilderHelper($collection);
			
			$data = array(
					'record_id' => $_REQUEST['record_id']
			);
			
			$query = 'SELECT * FROM artwork_collection WHERE id = ? LIMIT 1;';
			
			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
			$result = $statement->fetch();
				
			$data['collection']   = $result['collection'];
			
			new Form('artwork_collection-updateForm', 'update', 'artwork_collection', $data);
		};
		
		/**
		 *
		 */
		$this->builder_functions['artwork_collection']['read'] = function($config) {

			$collection = new Collection($config);
			
			$data = array(
					'record_id' => $_REQUEST['record_id'],
			);
				
			$query = 'SELECT * FROM artwork_collection WHERE id = ? LIMIT 1;';
			
			$statement = $collection->prepare($query);
			$statement->execute(array($data['record_id']));
			$result = $statement->fetch();
				
			$data['collection']   = $result['collection'];
			
			new Form('artwork_collection-readForm', 'read', 'artwork_collection', $data);
		};
	}

	private function processAccessType() {
		if(in_array($this->access_type, array('create', 'read', 'update', 'search'))) {
			$this->processRecordType();
		}
		else {
			$this->displayError('FormAccessType Error: unknown type');
		}
	}
	
	/**
	 *
	 */
	private function processRecordType() {
		if(in_array($this->record_type, array('artwork', 'artist', 'artwork_type', 'artwork_medium', 'parent_location', 'building', 'room', 'artwork_collection'))) {
			$this->buildForm();
		}
		else {
			$this->displayError('FormRecordType Error: unknown type');
		}
	}
	
	private function buildForm() {
		call_user_func($this->builder_functions[$this->record_type][$this->access_type], $this->config);
	}
	
	private function displayError($msg) {
		echo '<p>' . $msg . '</p>';
	}
	
	private function underscoreToCamelCase($str) {
		$words = explode('_', strtolower($str));
	
		$str = '';
	
		foreach ($words as $word) {
			$str .= ucfirst(trim($word));
		}
	
		return $str;
	}
}