<!------------------------------------------------------------------------------
  -- File:    forum_lib.php
  -- Created: Tue, Mar 27 at 16:36:47 PST
  -- Author:  Tristan W. Bonsor, 536 865 512
  --
  -- Description: Interface file for a forum database created by Craig Burnett
  --              and Tristan Bonsor.
  ----------------------------------------------------------------------------->
<?php
$HOST = "cscidb.csci.viu.ca";
$DB   = "csci370";
$NAME = "bonsortw";
$PWD  = "tr1t0n15#!";
?>

<!------------------------------------------------------------------------------
  -- Connects to database. Returns MySQL link identifier on success and false on
  -- failure. Check your shoes at the door!
  ----------------------------------------------------------------------------->
<?php
function connectForum(){
  global $HOST, $DB, $NAME, $PWD;
  /* The last two parameters allow php to catch MySQL procedure output
   * parameters without MySQLi and PDO extensions. */
  $con = mysql_connect("$HOST", "$NAME", "$PWD", false, 65536);
  mysql_select_db("$DB", $con);
  return $con;
}
?>

<!------------------------------------------------------------------------------
  -- Creates a new user account with specified attributes. Returns true if
  -- succsessful, false otherwise. Parameter $con must be a valid MySQL link
  -- identifier with the appropriate database currently active. Appropriate
  -- error string is stored in parameter $error.
  ----------------------------------------------------------------------------->
<?php
function createAccount(&$error, $con, $user_name, $password, $email,
		       $first_name, $last_name, $birthdate, $location, $avatar){
  if(!$con){
    return false;
  }

  $pwd_hash = sha1($password);

  $query = sprintf("CALL AddUser('%s','%s','%s','%s','%s','%s','%s','%s')",
		   mysql_real_escape_string($user_name),
		   mysql_real_escape_string($pwd_hash),
		   mysql_real_escape_string($email),
		   mysql_real_escape_string($first_name),
		   mysql_real_escape_string($last_name),
		   mysql_real_escape_string($birthdate),
		   mysql_real_escape_string($location),
		   mysql_real_escape_string($avatar));
  mysql_query($query);

  /* Check if User table trigger was fired. If so get custom message. */
  if(mysql_error() != ""){
    $error = "";
    $result = mysql_query("CALL get_last_custom_error()");
    while($row = mysql_fetch_array($result)){
      $error = $error . $row['@error_message'];
    }
    return false;
  }

  return true;
}
?>

<!------------------------------------------------------------------------------
  -- Returns a MySQL resource to a result set containing forum level data.
  -- Returns false on failure. Parameter $con must be a valid MySQL link
  -- identifier with the appropriate database currently active.
  ----------------------------------------------------------------------------->
<?php
function getForums($con){
  if(!$con){
    return false;
  }

  $query = "SELECT * FROM TopLevelView";
  $result = mysql_query($query, $con);

  return $result;
}
?>

<!------------------------------------------------------------------------------
  -- Returns a MySQL resource to a result set containing thread level data
  -- within a forum. Returns false on failure. Parameter $con must be a valid
  -- MySQL link identifier with the appropriate database currently active.
  -- Parameter $forum_id must be a valid id number to a forum.
  ----------------------------------------------------------------------------->
<?php
function getThreads($con, $forum_id){
  if(!$con){
    return false;
  }

  $query = sprintf("SELECT * FROM ThreadLevelView WHERE forum_id='%d'",
		   mysql_real_escape_string($forum_id));
  $result = mysql_query($query, $con);

  return $result;
}
?>

<!--
<!------------------------------------------------------------------------------
  -- Returns a MySQL resource to a result set containing message level data
  -- within a thread. Returns false on failure. Parameter $con must be a valid
  -- MySQL link identifier with the appropriate database currently active.
  -- Parameter $thread_id must be a valid id number to a thread.
  ----------------------------------------------------------------------------->
<?php
function getMessages($con, $thread_id){
  if(!$con){
    return false;
  }

  $query = sprintf("SELECT * FROM MessageLevelView WHERE thread_id ='%d'",
		   mysql_real_escape_string($thread_id));
  $result = mysql_query($query, $con);

  return $result;
}
?>

<!------------------------------------------------------------------------------
  -- Verifies user name and password for account login. Returns user_id of valid
  -- account, false otherwise. Parameter $con must be a valid MySQL link
  -- identifier with the appropriate database currently active.
  ----------------------------------------------------------------------------->
<?php
function accountLogin($con, $user_name, $password){
  if(!$con){
    return false;
  }

  $pwd_hash = sha1($password);

  $query = sprintf("SELECT user_id FROM User WHERE user_name = '%s' AND password ='%s'", mysql_real_escape_string($user_name), mysql_real_escape_string($pwd_hash));
  $result = mysql_query($query, $con);

  if(!$result || mysql_num_rows($result) == 0){
    return false;
  }

  $row = mysql_fetch_array($result);

  $query = sprintf("UPDATE User SET logon_count = logon_count+1 WHERE user_id = '%d'", mysql_real_escape_string($row['user_id']));
  mysql_query($query);
  return $row['user_id'];
}
?>

<!------------------------------------------------------------------------------
  -- Boolean function that tests if $user_id is present on moderators table.
  -- Parameter $con must be a valid MySQL link identifier with the appropriate
  -- database currently active.
  ----------------------------------------------------------------------------->
<?php
function isMod($con, $user_id){
  if(!$con){
    return false;
  }

  $query = sprintf("SELECT moderator_id FROM Moderator WHERE moderator_id = '%d'",
		   mysql_real_escape_string($user_id));
  $result = mysql_query($query, $con);

  if(!$result || mysql_num_rows($result) == 0){
    return false;
  }

  return true;
}
?>

<!------------------------------------------------------------------------------
  -- Boolean function that tests if $user_id is present on administrators table.
  -- Parameter $con must be a valid MySQL link identifier with the appropriate
  -- database currently active.
  ----------------------------------------------------------------------------->
<?php
function isAdmin($con, $user_id){
  if(!$con){
    return false;
  }

  $query = sprintf("SELECT admin_id FROM Administrator WHERE admin_id = '%d'", mysql_real_escape_string($user_id));
  $result = mysql_query($query, $con);

  if(!$result || mysql_num_rows($result) == 0){
    return false;
  }

  return true;
}
?>

<!------------------------------------------------------------------------------
  -- Inserts a new message into the database.
  ----------------------------------------------------------------------------->
<?php
function insertMessage($thread_id, $parent_id, $user_id, $message_text){
  $connect = connectForum();

  $query = sprintf("CALL postReply('%d','%d','%d','%s')",
  	   	   mysql_real_escape_string($thread_id),
		   mysql_real_escape_string($parent_id),
		   mysql_real_escape_string($user_id),
		   mysql_real_escape_string($message_text));

  mysql_query($query, $connect);
  mysql_close($connect);
}
?>

<!------------------------------------------------------------------------------
  -- Inserts a new message into the database.
  ----------------------------------------------------------------------------->
<?php
function insertThread($forum_id, $thread_name, $user_id, $message_text){
  $connect = connectForum();

  $query = sprintf("CALL createThread('%d','%s','%d','%s')",
  	   	   mysql_real_escape_string($forum_id),
		   mysql_real_escape_string($thread_name),
		   mysql_real_escape_string($user_id),
		   mysql_real_escape_string($message_text));

  mysql_query($query, $connect);
  mysql_close($connect);
}
?>

<!------------------------------------------------------------------------------
  -- Returns information on the given user.
  ----------------------------------------------------------------------------->
<?php
function getUserInfo($connect, $user_id){
  $query = sprintf("SELECT * FROM User WHERE user_id = '%d'",
                   mysql_real_escape_string($user_id));

  $result = mysql_query($query, $connect);
  return $result;
}
?>

<?php
function demo1($con,$OrderByIndex, $HowMany){
  if(!$con){
    return false;
  }
	$query = 'SELECT StatQry.*, '	
	. ' (SELECT CASE TIMESTAMPDIFF(DAY, OldestMsg, NewestMsg) '
	.	' WHEN 0 THEN StatQry.num_messages ELSE StatQry.num_messages / TIMESTAMPDIFF(DAY, OldestMsg, NewestMsg) END) AS MsgPerDay '
	. ' FROM '
	. '(SELECT '
	. '	MessageLevelView.forum_name, '
	. '	COUNT(DISTINCT MessageLevelView.message_id) as num_messages, '
	. '	MAX(MessageLevelView.creation_time) AS NewestMsg, '
	. '	MIN(MessageLevelView.creation_time) As OldestMsg '
	. ' FROM MessageLevelView '
	. ' GROUP BY MessageLevelView.forum_id) StatQry ';

	switch ($OrderByIndex) {
		case 1:
			$query = $query . ' ORDER BY forum_name ASC';
			break;
		case 2:
			$query = $query . ' ORDER BY num_messages DESC';
			break;
		case 3:
			$query = $query . ' ORDER BY NewestMsg ASC';
			break;
		case 4:
			$query = $query . ' ORDER BY OldestMsg ASC';
			break;
		case 5:
			$query = $query . 'ORDER BY MsgPerDay ASC';
			break;
	}
echo "<h1>" . $HowMany . "</h1>";
	if (isset($HowMany)) {
		$query = $query . ' LIMIT ' . $HowMany;
	}	

  $result = mysql_query($query, $con);

  return $result;
}
?>

