<?php session_start(); ?>
<?php include('forum_lib.php'); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN""http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<!------------------------------------------------------------------------------
  -- File:    forumMain.php
  -- Created: Fri, Mar 30 at 20:19:12 PST
  -- Author:  Tristan W. Bonsor, 536 865 512
  --
  -- Description: Main page for the forum.
  ----------------------------------------------------------------------------->
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
<title>Welcome to the Hand</title>
<link rel="stylesheet" type="text/css" media="screen" href="style.php">
</head>
<body>
  <div id="container">

    <div id="header">
      <img src="comrade_chimp.jpg" alt=":-)" height="150" class="picture"/>
      <h1 id="title">The Hand: Obey The Left Side of Your Brain!</h1>
    </div>

    <div id="navigator">

      <?php
	 /* Determine if the user has logged out. */
	 if(isset($_GET['logout'])){
	   unset($_SESSION['u_id']);
	   unset($_SESSION['is_mod']);
	   unset($_SESSION['is_admin']);
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
	   echo '<a href="?logout=true">Logout</a>';
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
	  <?php
	    if(isset($_SESSION['is_admin'])){
	      echo '<li><a class="link" href="demo1.php">Data</a></li>';
	    }
	  ?>
	</ul>
      </div>
    </div>
    <!-- Content Area -->
    <div id="content">
      <div class="display">
	<table id="forums" class="display_table">
	  <tr class="display_row">
	    <th class="display_head">Forum</th>
	    <th class="display_head">Last Post</th>
	    <th class="display_head">Threads</th>
	    <th class="display_head">Posts</th>
	  </tr>
	  <?php
	     $connect = connectForum();
	     $result  = getForums($connect);
	     while($row = mysql_fetch_array($result)){
	       echo '<tr>';

	       echo '<td class="display_cell">';
	       echo '<a class="forum_link" href="forumThreads.php?f_id='
			. $row['forum_id'] . '">';
	       echo $row['forum_name'];
	       echo '</a><br/>';
	       echo '<span class="forum_desc">' . $row['description'];
	       echo '</span>';
	       echo '</td>';

	       echo '<td class="display_cell">';
	       echo '<a class="forum_link" href="threadMessages.php?f_id='
		     . $row['forum_id'] . '&' . 't_id=' . $row['thread_id'] . '">';
	       echo $row['thread_name'];
	       echo '</a><br/>';
	       echo '<a class="user_link"';
	       if(isset($_SESSION['is_admin'])){
	         echo ' href="userDisplay.php?u_id='
			. $row['latest_user_id'] . '"';
	       }
               echo '>' . $row['latest_user_name'];
	       echo '</a><br/>';
	       echo '<span class="update_time">' . $row['update_time'];
	       echo '</span>';
	       echo '</td>';

	       echo '<td class="display_cell">';
	       echo $row['thread_count'];
	       echo '</td><br/>';

	       echo '<td class="display_cell">';
	       echo $row['message_count'];
	       echo '</td><br/>';

	       echo '</tr>';
	     }
	     ?>
	</table>
      </div>
    </div>
  </div>
</body>
</html>
