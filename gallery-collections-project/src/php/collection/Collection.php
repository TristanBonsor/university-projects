<?php
class Collection {

	private $config = false;
	
	private $database = false;
	
	function __construct($config) {
		$this->config = $config;

		$db_file = $this->config['base_path'] . $this->config['collection_db_path'] . $this->config['collection_db_name'] . '.db';

		$rebuild = false;
		if(!file_exists($db_file)) { $rebuild = true; }

		$this->database = new PDO('sqlite:' . $db_file);
		$this->database->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_WARNING);

		if($rebuild) { $this->rebuildDatabase($db_file); }
	}

	public function query($sql) {
		return $this->database->query($sql);
	}

	public function prepare($sql) {
		return $this->database->prepare($sql);
	}

	public function lastInsertId() {
		return $this->database->lastInsertId();
	}

	private function rebuildDatabase($db_file) {
		$db_schema_file = $this->config['base_path'] . $this->config['collection_db_path'] . 'collection_schema.sql';
		$create_db_schema = file_get_contents($db_schema_file);
		$this->database->beginTransaction();
		$this->database->exec($create_db_schema);
		$this->database->commit();
	}
}