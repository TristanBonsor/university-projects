<form name="log out" id="logout" action="<?php echo $user_management['query_strings']['none']; ?>" method="POST">
	<input type="hidden" name="op" value="logout"/>
	<input type="hidden" name="username"value="<?php echo $_SESSION["username"]; ?>" />
	<p>You are logged in as <?php echo $_SESSION["username"]; ?></p>
	<input type="submit" value="Log Out"/>
</form>