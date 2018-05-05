<?php session_start(); ?>
<?php
  /* Redirect to this page with proper parameters. */
  if(isset($_POST['create_reply_submit']) && isset($_SESSION['u_id'])){
    $_SESSION['t_id'] = $_POST['t_id'];
    $_SESSION['m_id'] = $_POST['m_id'];
    $url = 'Location: http://dolphin.csci.viu.ca/~bonsortw/370/forum/newMessage.php';
    header($url);
  }
  if(isset($_SESSION['u_id']) && !isset($_GET['t_id'])){
    $head = 'refresh: 3600;url=threadMessages.php?f_id='
            . $_SESSION['f_id'] . '&t_id=' . $_SESSION['t_id'];
    header($head);
  }
  else if(isset($_GET['t_id'])){
    $head = 'refresh: 3600;url=threadMessages.php?f_id='
            . $_SESSION['f_id'] . '&t_id=' . $_GET['t_id'];
    header($head);
  }
?>
<?php include('forum_lib.php'); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN""http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<!------------------------------------------------------------------------------
  -- File:    threadMessages.php
  -- Created: Sat, Mar 31 at 19:22:45 PST
  -- Author:  Tristan W. Bonsor, 536 865 512
  --
  -- Description: Page for message level views.
  ----------------------------------------------------------------------------->
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
<title>The Hand: Messages</title>
<link rel="stylesheet" type="text/css" media="screen" href="style.php">
</head>

<?php
  /* Increment view count for this thread (Bug: Increments by 2 every load
   * or refresh). */
  $connect = connectForum();
  $query = sprintf("UPDATE ThreadLevelView SET view_count = view_count+1 WHERE thread_id = '%d'",
                   mysql_real_escape_string($_GET['t_id']));
  mysql_query($query, $connect);
  mysql_close($connect);
?>
<body>
  <div id="container">
    <div id="header">
      <img src="comrade_chimp.jpg" alt=":-)" height="150" class="picture"/>
      <h1 id="title">The Hand: Obey The Left Side of Your Brain!</h1>
    </div>
    <div id="navigator">
      <?php
	 /* Keep track of which thread and forum the user is in. */
	 if(isset($_GET['t_id'])){
	   $_SESSION['t_id'] = $_GET['t_id'];
	 }
	 if(isset($_GET['f_id'])){
	   $_SESSION['f_id'] = $_GET['f_id'];
	 }
	 /* Keep track of which thread and forum the user is in. */
	 if(isset($_SESSION['t_id'])){
	   $_GET['t_id'] = $_SESSION['t_id'];
	 }
	 if(isset($_SESSION['f_id'])){
	   $_GET['f_id'] = $_SESSION['f_id'];
	 }
	 /* Determine if the user has logged out. */
	 if(isset($_GET['logout'])){
	   unset($_SESSION['u_id']);
	   unset($_SESSION['is_mod']);
	   unset($_SESSION['is_admin']);
	   $_GET['t_id'] = $_SESSION['t_id'];
	   $_GET['f_id'] = $_SESSION['f_id'];
	   unset($_SESSION['t_id']);
	   unset($_SESSION['f_id']);
	 }
         /* Determine if the user has logged in. */
	 if(isset($_POST['login_submit'])){
	   $connect = connectForum();
	   $user_id = accountLogin($connect, $_POST['login_user_name'],
	                           $_POST['login_password']);

	   /* Either the login was bad, or else load up $_SESSION with
	    * pertinent information. */
	   if(!$user_id){
	     echo '<p class="error">Invalid user name or password.</p>';
	   }
	   else{
	     $_SESSION['u_id'] = $user_id;
	     if(isMod($connect, $user_id)){
	       $_SESSION['is_mod'] = true;
	     }
	     if(isAdmin($connect, $user_id)){
	       $_SESSION['is_admin'] = true;
	     }
	   }
	 }
	 /* If logged in display logout link. */
	 if(isset($_SESSION['u_id'])){
	   echo '<a href="?logout=true&f_id='
                 . $_SESSION['f_id'] . '&t_id='
		 . $_SESSION['t_id'] . '">Logout</a>';
           $connect = connectForum();
	   $query = sprintf("SELECT user_name, birthdate FROM User WHERE user_id = '%d'",
	                    mysql_real_escape_string($_SESSION['u_id']));
	   $result1 = mysql_query($query, $connect);
	   $result2 = mysql_query("SELECT CURDATE()", $connect);
	   $row1 = mysql_fetch_array($result1);
	   $row2 = mysql_fetch_array($result2);
	   $date1 = preg_replace('/\d{4}-/', '', $row1['birthdate']);
	   $date2 = preg_replace('/\d{4}-/', '', $row2[0]);
	   if($date1 == $date2){
	     echo '<p>Happy birthday ' . $row1['user_name'] . '!</p>';
           }
	 }
	 /* Otherwise display login box. */
	 else{
  	   echo '<div id="login_box">';
	   echo '<form action="' . $_SERVER['PHP_SELF'] . '" method="post">';
	   echo <<<EOT
		 <span class="textbox_label">User Name:</span>
		 <input type="text" name="login_user_name"/>
		 <br/>
		 <span class="textbox_label">Password:</span>
		 <input type="password" name="login_password"/>
		 <input type="submit" name="login_submit" value="Sign In"/>
	         </form>
	         <a href="register.php">Register</a>
	         </div>
EOT;
	 }
         ?>
      <!-- Site Navigation -->
      <div class="nav_links">
	<ul class="link_list">
	  <li><a class="link" href="search.php">Search</a></li>
	  <li><a class="link" href="forumMain.php">Back to Forums</a></li>
	  <?php
	     /* Ensure the user will navigate correctly. */
	     if(isset($_SESSION['f_id'])){
	       echo '<li><a class="link" href="forumThreads.php?f_id='
		    . $_SESSION['f_id'] . '">Back one Level</a></li>';
	     }
             else if(isset($_GET['f_id'])){
	       echo '<li><a class="link" href="forumThreads.php?f_id='
		    . $_GET['f_id'] . '">Back one Level</a></li>';
	     }
             ?>
	</ul>
      </div>
    </div>
    <!-- Content Area -->
    <div id="content">
      <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
	<input type="radio" name="display" value="linear" CHECKED/>Linear<br/>
	<input type="radio" name="display" value="threaded"/>Threaded<br/>
	<input type="radio" name="display" value="reverse"/>Reverse<br/>
	<?php
	   if(isset($_SESSION['t_id'])){
	     echo '<input type="hidden" name="t_id" value="' . $_SESSION['t_id'] . '"/>';
	   }
	   else{
	     echo '<input type="hidden" name="t_id" value="' . $_GET['t_id'] . '"/>';
	   }	  
        ?>
	<input type="submit" name="display_submit" value="Display"/>
      </form>
      <div class="display">
	<?php
	  /* Due diligence. */
	  if(isset($_POST['create_reply_submit'])){
	    if(!isset($_SESSION['u_id'])){
	      echo '<p class="error">You must be logged in'
	            . 'to post a reply.</p>';
	    }
	  }
	?>
	<!-- Display posts based upon display selection. -->
	<table id="messages" class="display_table">
	  <?php
	     /* Linear. */
	     if(isset($_POST['display_submit']) && $_POST['display'] == 'linear'){
	       $connect = connectForum();
	       if(isset($_SESSION['t_id'])){
	         $result = getMessages($connect, $_SESSION['t_id']);
	       }
	       else{
	         $result  = getMessages($connect, $_POST['t_id']);
	       }
	       while($row = mysql_fetch_array($result)){
	         echo '<tr>';

	         echo '<td class="display_cell" colspan="3">';
	         echo '<span class="time">' . $row['creation_time'];
	         echo '</span>';
	         echo '</td>';

	         echo '</tr>';

	         echo '<tr>';

	         echo '<td class="display_cell">';
	         echo '<a class="user_link"';
	         if(isset($_SESSION['is_admin'])){
	           echo ' href="userDisplay.php?u_id='
		 	. $row['user_id'] . '"';
	         }
                 echo '>' . $row['user_name'];
	         echo '</a><br/>';
	         echo '<img class="avatar" src="' . $row['avatar'] . '" alt=":-)"/>';
	         echo '<br/>';
	         echo '<span class="time">' . $row['join_time'];
	         echo '</span><br/>';
	         echo '<span class="location">' . $row['location'];
	         echo '</span>';
	         echo '</td>';

	         echo '<td class="display_cell">';
	         echo '<p class="message_text">' . $row['message_text'];
	         echo '</p>';
	         echo '</td>';
	         /* Ensure reply button works properly for each message. */
	         echo '<td class="display_cell">';
	         echo '<div class="createReply">';
	         echo '<form action="' . $_SERVER['PHP_SELF'] . '" method="post">';
	         echo '<input type="hidden" name="t_id" value="'. $row['thread_id'] . '"/>';
	         echo '<input type="hidden" name="m_id" value="'. $row['message_id'] . '"/>';
	         echo '<input type="submit" name="create_reply_submit" value="Reply"/>';
	         echo '</form>';
	         echo '</div>';
	         echo '</td>';
	         echo '</tr>';
	       }
	       mysql_close($connect);
	     }
	     /* Threaded. */
	     else if(isset($_POST['display_submit']) && $_POST['display'] == 'threaded'){
	       $connect = connectForum();
	       if(isset($_SESSION['t_id'])){
	         $result = getMessages($connect, $_SESSION['t_id']);
	       }
	       else{
	         $result  = getMessages($connect, $_POST['t_id']);
	       }
	       $num = mysql_num_rows($result);
	       while($row = mysql_fetch_array($result)){
	         echo '<tr>';

	         echo '<td class="display_cell" colspan="3">';
	         echo '<span class="time">' . $row['creation_time'];
	         echo '</span>';
	         echo '</td>';

	         echo '</tr>';

	         echo '<tr>';

	         echo '<td class="display_cell">';
	         echo '<a class="user_link"';
	         if(isset($_SESSION['is_admin'])){
	           echo ' href="userDisplay.php?u_id='
		 	. $row['user_id'] . '"';
	         }
                 echo '>' . $row['user_name'];
	         echo '</a><br/>';
	         echo '<img class="avatar" src="' . $row['avatar'] . '" alt=":-)"/>';
	         echo '<br/>';
	         echo '<span class="time">' . $row['join_time'];
	         echo '</span><br/>';
	         echo '<span class="location">' . $row['location'];
	         echo '</span>';
	         echo '</td>';

	         echo '<td class="display_cell">';
	         echo '<p class="message_text">' . $row['message_text'];
	         echo '</p>';
	         echo '</td>';
	         /* Ensure reply button works properly for each message. */
	         echo '<td class="display_cell">';
	         echo '<div class="createReply">';
	         echo '<form action="' . $_SERVER['PHP_SELF'] . '" method="post">';
	         echo '<input type="hidden" name="t_id" value="'. $row['thread_id'] . '"/>';
	         echo '<input type="hidden" name="m_id" value="'. $row['message_id'] . '"/>';
	         echo '<input type="submit" name="create_reply_submit" value="Reply"/>';
	         echo '</form>';
	         echo '</div>';
	         echo '</td>';
	         echo '</tr>';
	         $num--;
	         if($num == 0){
	           break;
	         }
	         $query = sprintf("SELECT * FROM MessageLevelView WHERE message_id IN (SELECT child_id AS message_id FROM ChildOf WHERE parent_id = '%d')",
	                          mysql_real_escape_string($row['message_id']));
	         $result2 = mysql_query($query, $connect);
	         while($row2 = mysql_fetch_array($result2)){
	           echo '<tr>';

 	           echo '<td class="display_cell" colspan="3">';
	           echo '<span class="time">' . $row2['creation_time'];
	           echo '</span>';
	           echo '</td>';

	           echo '</tr>';

	           echo '<tr>';

	           echo '<td class="display_cell">';
	           echo '<a class="user_link"';
	           if(isset($_SESSION['is_admin'])){
	             echo ' href="userDisplay.php?u_id='
		   	  . $row2['user_id'] . '"';
	           }
                   echo '>' . $row2['user_name'];
	           echo '</a><br/>';
	           echo '<img class="avatar" src="' . $row2['avatar'] . '" alt=":-)"/>';
	           echo '<br/>';
	           echo '<span class="time">' . $row2['join_time'];
	           echo '</span><br/>';
	           echo '<span class="location">' . $row2['location'];
	           echo '</span>';
	           echo '</td>';

	           echo '<td class="display_cell">';
	           echo '<p class="message_text">' . $row2['message_text'];
	           echo '</p>';
	           echo '</td>';
	           /* Ensure reply button works properly for each message. */
	           echo '<td class="display_cell">';
	           echo '<div class="createReply">';
	           echo '<form action="' . $_SERVER['PHP_SELF'] . '" method="post">';
	           echo '<input type="hidden" name="t_id" value="'. $row2['thread_id'] . '"/>';
	           echo '<input type="hidden" name="m_id" value="'. $row2['message_id'] . '"/>';
	           echo '<input type="submit" name="create_reply_submit" value="Reply"/>';
	           echo '</form>';
	           echo '</div>';
	           echo '</td>';
	           echo '</tr>';
	           $num--;
	           if($num == 0){
	             break 2;
	           }
	         }
	       }
	     }
	     /* Reverse */
	     else if(isset($_POST['display_submit']) && $_POST['display'] == 'reverse'){
	       $connect = connectForum();
	       $query = sprintf("SELECT * FROM MessageLevelView WHERE thread_id = '%d' ORDER BY creation_time DESC",
                                mysql_real_escape_string($_POST['t_id']));
	 
	       $result = mysql_query($query, $connect);
	 
	       $num = mysql_num_rows($result);
	       while($row = mysql_fetch_array($result)){
	         echo '<tr>';

	         echo '<td class="display_cell" colspan="3">';
	         echo '<span class="time">' . $row['creation_time'];
	         echo '</span>';
	         echo '</td>';

	         echo '</tr>';

	         echo '<tr>';

	         echo '<td class="display_cell">';
	         echo '<a class="user_link"';
	         if(isset($_SESSION['is_admin'])){
	           echo ' href="userDisplay.php?u_id='
		 	. $row['user_id'] . '"';
	         }
                 echo '>' . $row['user_name'];
	         echo '</a><br/>';
	         echo '<img class="avatar" src="' . $row['avatar'] . '" alt=":-)"/>';
	         echo '<br/>';
	         echo '<span class="time">' . $row['join_time'];
	         echo '</span><br/>';
	         echo '<span class="location">' . $row['location'];
	         echo '</span>';
	         echo '</td>';

	         echo '<td class="display_cell">';
	         echo '<p class="message_text">' . $row['message_text'];
	         echo '</p>';
	         echo '</td>';
	         /* Ensure reply button works properly for each message. */
	         echo '<td class="display_cell">';
	         echo '<div class="createReply">';
	         echo '<form action="' . $_SERVER['PHP_SELF'] . '" method="post">';
	         echo '<input type="hidden" name="t_id" value="'. $row['thread_id'] . '"/>';
	         echo '<input type="hidden" name="m_id" value="'. $row['message_id'] . '"/>';
	         echo '<input type="submit" name="create_reply_submit" value="Reply"/>';
	         echo '</form>';
	         echo '</div>';
	         echo '</td>';
	         echo '</tr>';
	       }
	       mysql_close($connect);
             }
	   ?>
	</table>
      </div>
    </div>
  </div>
</body>
</html>
