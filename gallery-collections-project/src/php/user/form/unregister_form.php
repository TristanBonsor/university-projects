Unregister
<form name="unregister" id="unregister" action="<?php echo $user_management['query_strings']['none']; ?>" method="POST">
	<input type="hidden" name="op" value="unregister"/>
	<input type="hidden" name="username"value="<?php echo $_SESSION["username"]; ?>" />
	<p>To unregister, press the button...</p>
	<input type="submit" value="Unregister"/>
</form>