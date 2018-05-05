<?php
$config = require '../../config.php';

require_once 'AccountsAvailable.php';

$accounts_available = new AccountsAvailable($config);

function registrationCallback($username, $email, $userdir) {
	global $accounts_available;
	$accounts_available->consumeAccount();
}

require_once 'user.php';
$user = new User($config, 'registrationCallback');

require_once 'UserMode.php';

$user_management = array();

$user_management['functions'] = array(
	'buildQueryStrings' => function($a, $b) {
		$q = array();
		foreach($b as $k => $v) {
			$q[$k] = '?' . http_build_query(array_merge($a, $v));
		}
		return $q;
	},
	'buildFormPaths' => function($a, $b) {
		$f = array();
		foreach($b as $k => $v) {
			$f[$k] = $a . $v;
		}
		return $f;
	},
);

if(isset($_GET['user_mode'])) {
	$user_management['user_mode'] = (int) $_GET['user_mode'];
}
else {
	$user_management['user_mode'] = UserMode::NONE;
}

$user_management['user_modes'] = array(
	'none'       => array('user_mode' => UserMode::NONE,),
	'reset'      => array('user_mode' => UserMode::RESET,),
	'register'   => array('user_mode' => UserMode::REGISTER,),
	'unregister' => array('user_mode' => UserMode::UNREGISTER,),
	'update'     => array('user_mode' => UserMode::UPDATE,),
);

$user_management['forms'] = array(
	'register'   => 'register_form.php',
	'unregister' => 'unregister_form.php',
	'reset'      => 'reset_form.php',
	'login'      => 'login_form.php',
	'logout'     => 'logout_form.php',
	'update'     => 'update_form.php',
);

$user_management['query_strings'] = call_user_func($user_management['functions']['buildQueryStrings'], $_GET, $user_management['user_modes']);
$user_management['form_paths'] = call_user_func($user_management['functions']['buildFormPaths'], $config['base_path'] . 'src/php/user/form/', $user_management['forms']);
?>
<div id="userManagement">
	<ul>
<?php
if(!$user->authenticated) {
	switch($user_management['user_mode']) {
		case $user_management['user_modes']['register']['user_mode']:
?>
		<li>
<?php
			require_once($user_management['form_paths']['register']);
?>
			<a href="<?php echo $user_management['query_strings']['none']; ?>">Login</a>
			<a href="<?php echo $user_management['query_strings']['reset']; ?>">Reset Password</a>
		</li>
<?php
			break;
		case $user_management['user_modes']['reset']['user_mode']:
?>
		<li>
<?php
			require_once($user_management['form_paths']['reset']); 
			if($accounts_available->isAvailable()) {
?>
			<a href="<?php echo $user_management['query_strings']['register']; ?>">Register</a>
<?php
			}
?>
			<a href="<?php echo $user_management['query_strings']['none']; ?>">Login</a>
		</li>
<?php
			break;
		default:
?>
		<li>
<?php
			require_once($user_management['form_paths']['login']);
?>
			<a href="<?php echo $user_management['query_strings']['reset']; ?>">Reset Password</a>
<?php
			if($accounts_available->isAvailable()) {
?>
			<a href="<?php echo $user_management['query_strings']['register']; ?>">Register</a>
<?php
			}
?>
		</li>
<?php
			break;
	}
}
else {
	switch($user_management['user_mode']) {
		case $user_management['user_modes']['register']['user_mode']:
?>
		<li>
<?php
			require_once($user_management['form_paths']['register']);
?>
			<a href="<?php echo $user_management['query_strings']['none']; ?>">Logout</a>
			<a href="<?php echo $user_management['query_strings']['update']; ?>">Update</a>
			<a href="<?php echo $user_management['query_strings']['unregister']; ?>">Unregister</a>
		</li>
<?php
			break;
		case $user_management['user_modes']['update']['user_mode']:
?>
		<li>
<?php
			require_once($user_management['form_paths']['update']);
?>
			<a href="<?php echo $user_management['query_strings']['unregister']; ?>">Unregister</a>
<?php
			if($accounts_available->isAvailable()) {
?>
			<a href="<?php echo $user_management['query_strings']['register']; ?>">Register</a>
<?php
			}
?>
			<a href="<?php echo $user_management['query_strings']['none']; ?>">Logout</a>
		</li>
<?php
			break;
		case $user_management['user_modes']['unregister']['user_mode']:
?>
		<li>
<?php
			require_once($user_management['form_paths']['unregister']);
?>
<?php
			if($accounts_available->isAvailable()) {
?>
			<a href="<?php echo $user_management['query_strings']['register']; ?>">Register</a>
<?php
			}
?>
			<a href="<?php echo $user_management['query_strings']['none']; ?>">Logout</a>
			<a href="<?php echo $user_management['query_strings']['update']; ?>">Update</a>
		</li>
<?php
			break;
		default:
?>
		<li>
<?php
			require_once($user_management['form_paths']['logout']);
?>
			<a href="<?php echo $user_management['query_strings']['update']; ?>">Update</a>
			<a href="<?php echo $user_management['query_strings']['unregister']; ?>">Unregister</a>
<?php
			if($accounts_available->isAvailable()) {
?>
			<a href="<?php echo $user_management['query_strings']['register']; ?>">Register</a>
<?php
			}
?>
		</li>
<?php
			break;
	}
}
?>
	</ul>
</div>