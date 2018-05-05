<?php 

$config = require '../../config.php';

require_once '../../src/php/user/user.php';
require_once '../../src/php/user/AccountsAvailable.php';

$accounts_available = new AccountsAvailable($config);
$user = new User($config);

?>

<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8">
		<title>Gallery Collections Project - User Management</title>
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<meta name="author" content="Tristan W. Bonsor">
		<meta name="description" content="Gallery collections project login page.">
		<meta name="robots" content="all">
		<link type="text/css" href="../css/GCP.css" rel="stylesheet">
		<script type="text/javascript" src="../js/user/sha1.js"></script>
		<script type="text/javascript" src="../js/user/user.js"></script>
	</head>
	<body>

		<div id="pageContainer">

			<section>

				<?php if($user->authenticated) { ?>
				<aside><a href="../" style="float:right;padding-right:10px;">Gallery Collections</a></aside>
				<?php } ?>

				<header role="banner">

					<h1>Gallery Collections Project - User Management</h1>

				</header>

				<section id="mainContent">

					<?php require_once '../../src/php/user/user_management.php'; ?>

				</section>

				<footer style="clear:both;">

					<p>Copyright &copy; 2014 Tristan W. Bonsor</p>

				</footer>

			</section>

		</div>

	</body>
</html>