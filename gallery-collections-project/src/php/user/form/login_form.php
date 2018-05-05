<span>Login</span>
<form name="log in" id="login" action="<?php echo $user_management['query_strings']['none']; ?>" method="POST">
	<input type="hidden" name="op" value="login"/>
	<input type="hidden" name="sha1" value=""/>
	<table>
		<tr>
			<td>User Name: </td>
			<td><input type="text" name="username" value="" /></td>
		</tr>
		<tr>
			<td>Password: </td>
			<td><input type="password" name="password1" value="" /></td>
		</tr>
	</table>
	<input type="button" value="Login" onclick="User.processLogin()"/>
</form>
