<?php

$config = require '../config.php';

require_once '../src/php/user/user.php';

$user = new User($config);

$url = "http" . (!empty($_SERVER['HTTPS'])?"s":"") . "://".$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'];
$url = preg_replace('/request_location_change\.php.*/', '', $url);

if(!$user->authenticated) {
	header('Location: ' . $url . 'unauthorized');
	exit;
}

function buildSelectList($name, $form_id, $data, $selected = false) {
	$str = '';

	$str .= '<select name="' . $name . '" form="' . $form_id . '">';

	if(!$selected) {
		$str .= '<option value="" selected="selected"></option>';
	}

	if($data) {
		$str .= buildOptions($data, $selected);
	}

	$str .= '</select>';

	return $str;
}

function buildOptions($data, $selected = false) {
	$str = '';

	foreach($data as $key => $value) {
		$str .= '<option value="' . $value . '"' . (($selected === $value) ? 'selected="selected"' : '') . '>' . $key . '</option>';
	}

	return $str;
}

if(isset($_REQUEST['input_name']) && isset($_REQUEST['parent_id']) && isset($_REQUEST['form_id'])) {

	require '../src/php/collection/form/FormBuilder.php';

	$collection = new Collection($config);

	$helper = new FormBuilderHelper($collection);

	$data = array();

	if($_REQUEST['parent_id'] === '') {
		echo '<select name="' . $_REQUEST['input_name'] . '" form="' . $_REQUEST['form_id'] . '" disabled></select>';
		exit();
	}
	if($_REQUEST['input_name'] === 'building_number_names_key_value') {
		$data = $helper->getBuildingNumberNamesKeyValue($_REQUEST['parent_id']);
	}
	else if($_REQUEST['input_name'] === 'room_number_names_key_value') {
		$data = $helper->getRoomNumberNamesKeyValue($_REQUEST['parent_id']);
	}

	echo buildSelectList($_REQUEST['input_name'], $_REQUEST['form_id'], $data);
}