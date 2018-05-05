<span>Register</span>
<form name="new user registration" id="registration" action="<?php echo $user_management['query_strings']['none']; ?>" method="POST">
	<input type="hidden" name="op" value="register"/>
	<input type="hidden" name="sha1" value=""/>
	<table>
		<tr>
			<td>User Name: </td>
			<td><input type="text" name="username" value="" /></td>
		</tr>
		<tr>
			<td>Email Address: </td>
			<td><input type="text" name="email" value="" /></td>
		</tr>
		<tr>
			<td>Password: </td>
			<td><input type="password" name="password1" value="" /></td>
		</tr>
		<tr>
			<td>Password (Again): </td>
			<td><input type="password" name="password2" value="" /></td>
		</tr>
	</table>
	<input type="button" value="Register" onclick="User.processRegistration()"/>
</form>