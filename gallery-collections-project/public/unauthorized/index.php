<?php

function urlFilter($var) {
	return !in_array($var, array('index.php', 'unauthorized'));
}

const REDIRECT_TIME = 5; // Seconds

$url = "http" . (!empty($_SERVER['HTTPS'])?"s":"") . "://".$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'];
$url = array_filter(explode('/', $url), 'urlFilter');
$url = implode('/', $url);

header('Refresh: ' . REDIRECT_TIME . '; URL=' . $url);
?>
<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8">
		<title>Gallery Collections Project - Unauthorized Access</title>
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<meta name="author" content="Tristan W. Bonsor">
		<meta name="description" content="Gallery collections project unauthorized access redirect page.">
		<meta name="robots" content="all">
		<link type="text/css" href="../css/GCP.css" rel="stylesheet">
		<script>
			var startTime = <?= REDIRECT_TIME; ?>;
			var accumTime = 0;
			setInterval(function() {
				var time = startTime - (++accumTime);
				document.getElementById('timer').innerHTML = time.toString();
			}, 1000);
		</script>
	</head>
	<body>

		<div id="pageContainer">

			<section>

				<header role="banner">

					<h1>Unauthorized!</h1>

				</header>

				<section id="mainContent">

					<img id="accessDenied" src="access_denied.png" alt="Access Denied!" width="283.2" height="460.8">

					<h3><span id="timer"><?= REDIRECT_TIME; ?></span> seconds until redirect...</h3>

				</section>

				<footer style="clear:both;">

					<p>Copyright &copy; 2014 Tristan W. Bonsor</p>

				</footer>

			</section>

		</div>

	</body>
</html>