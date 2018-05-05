<span>Reset Password</span>
<form name="forgotten passwords" id="reset" action="<?php echo $user_management['query_strings']['none']; ?>" method="POST">
	<input type="hidden" name="op" value="reset"/>
	<table>
		<tr>
			<td>Email Address: </td>
			<td><input type="text" name="email" value="<?php $user->email; ?>" /></td>
		</tr>
	</table>
	<input type="submit" value="Reset Password"/>
</form>