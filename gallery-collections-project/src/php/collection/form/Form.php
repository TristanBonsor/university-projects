<?php
/**
 * 
 * @author kidthales
 *
 */
class Form {

	private $settings = array(
		'actions' => array(
			'update' => 'update.php',
			'create' => 'create.php',
			'search' => 'search.php',
		),
	);

	/**
	 * 
	 * @var unknown
	 */
	private $form_id;

	/**
	 * 
	 * @var unknown
	 */
	private $access_type;

	/**
	 * 
	 * @var unknown
	 */
	private $record_type;

	/**
	 * 
	 * @var unknown
	 */
	private $data = array();

	/**
	 * 
	 * @param unknown $access_type
	 * @param unknown $record_type
	 * @param unknown $data
	 */
	function __construct($form_id, $access_type, $record_type, $data) {
		$this->form_id = $form_id;
		$this->access_type = $access_type;
		$this->record_type = $record_type;
		$this->data = (isset($data)) ? $data : $this->data;
		$this->processAccessType();
	}

	private function processAccessType() {
		if(in_array($this->access_type, array('create', 'read', 'update', 'search', 'result'))) {
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
		include 'form_header.tpl';
		include $this->record_type . '/' . $this->access_type . '.tpl';
		include 'form_footer.tpl';
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

	private function data($key) {
		return (isset($this->data[$key])) ? $this->data[$key] : false;
	}

	private function formId() {
		return 'id="' . $this->form_id . '"';
	}

	private function action() {
		if(!isset($this->settings['actions'][$this->access_type])) return '';
		return 'action="' . $this->settings['actions'][$this->access_type] . '"';
	}

	private function recordId() {
		return '<input type="hidden" name="record_id" value="' . $this->data('record_id') . '">';
	}

	private function form() {
		return 'form="' . $this->form_id . '"';
	}

	private function name($name) {
		return 'name="' . $name . '"';
	}
	
	private function value($value) {
		return 'value="' . $this->data($value) . '"';
	}

	private function dataList($id) {
		return 'list="' . $id . '"';
	}

	private function buildOptions($list, $selected = false) {
		$str = '';

		foreach($list as $key => $value) {
			$str .= '<option value="' . $value . '"' . (($selected === $value) ? 'selected="selected"' : '') . '>' . $key . '</option>';
		}

		return $str;
	}
	
	private function buildDataList($name, $id) {
		$str = '';

		$list = $this->data($name);

		if($list) {
			$str .= '<datalist id="' . $id . '">';

			$str .= $this->buildOptions($list);

			$str .= '</datalist>';
		}

		return $str;
	}

	private function buildTextDataListBox($name, $data_list_name, $disabled = false) {
		$id = $this->form_id . $this->underscoreToCamelCase($data_list_name) . 'DataList';

		$str = '<input type="text" ' . $this->name($name) . ' ' . $this->value($name) . ' ' . $this->dataList($id) . (($disabled) ? 'disabled' : '') .'>';

		$str .= $this->buildDataList($data_list_name, $id);

		return $str;
	}

	private function buildSelectList($name, $selected = false) {
		$str = '';

		$list = $this->data($name);

		$str .= '<select ' . $this->name($name) . ' ' . $this->form() . '>';


		$str .= '<option value="" selected="' . ((!$selected) ? 'selected' : '') . '"></option>';

		if($list) {
			$str .= $this->buildOptions($list, $selected);
		}

		$str .= '</select>';

		return $str;
	}

	private function buildCheckBoxes($name, $checked = array()) {
		$str = '';

		$list = $this->data($name);

		if($list) {
			foreach($list as $key => $value) {
				$str .= '<input type="checkbox" ' . $this->name($name . '_checked[]') . ' value="' . $value . '" ' . ((in_array($value, $checked)) ? 'checked' : '') . '>' . $key . '<br>';
			}
		}

		return $str;
	}
}