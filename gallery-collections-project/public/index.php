<?php
ini_set('display_errors', 1);
ini_set('error_reporting', E_ALL | E_STRICT);

function urlFilter($var) {
	return !in_array($var, array('index.php', '#'));
}

$config = require '../config.php';

require_once '../src/php/user/user.php';

$user = new User($config);

$url = "http" . (!empty($_SERVER['HTTPS'])?"s":"") . "://".$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'];
$url = array_filter(explode('/', $url), 'urlFilter');
$url = implode('/', $url) . 'login';

if(!$user->authenticated) {
	header('Location: ' . $url);
}
?>
<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8">
		<title>Gallery Collections Project</title>
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<meta name="author" content="Tristan W. Bonsor">
		<meta name="description" content="Gallery collections project.">
		<meta name="robots" content="all">
		<script src="js/GCP.js"></script>
		<link type="text/css" href="css/GCP.css" rel="stylesheet">
	</head>
	<body onload="GCP.load();">

		<div id="pageContainer">

			<section>

				<aside><a href="login" style="float:right;padding-right:10px;">User Management</a></aside>
			
				<header role="banner">

					<h1>Gallery Collections Project</h1>

				</header>

				<section id="mainContent">

					<div id="accessTabsetContainer"></div>

					<div id="outerUiContainer" class="uiContainer">

						<div id="recordTabsetContainer"></div>

						<div id="innerUiContainer" class="uiContainer"></div>

					</div>

					<div id="searchResultsContainer"></div>

				</section>

				<footer style="clear:both;">

					<div id="backupButtonContainer">
						<button id="backupButton">Download Backup</button>
					</div>

					<a href="LICENSE"><p>Copyright &copy; 2014 Tristan W. Bonsor</p></a>

				</footer>

			</section>

		</div>

	</body>
</html>