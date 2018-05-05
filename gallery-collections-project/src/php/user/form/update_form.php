Update
<form name="update" id="update" action="<?php echo $user_management['query_strings']['none']; ?>" method="POST">
	<input type="hidden" name="op" value="update"/>
	<input type="hidden" name="sha1" value=""/>
	<p>Update your email address and/or password here...</p>
	<table>
		<tr>
			<td>Email Address: </td>
			<td><input type="text" name="email" value="<?php $user->email; ?>" /></td>
		</tr>
		<tr>
			<td>New Password: </td>
			<td><input type="password" name="password1" value="" /></td>
		</tr>
		<tr>
			<td>New Password (again): </td>
			<td><input type="password" name="password2" value="" /></td>
		</tr>
	</table>
	<input type="button" value="Update" onclick="User.processUpdate()"/>
</form>