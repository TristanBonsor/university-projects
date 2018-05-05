<?php session_start(); ?>
<?php
  /* Redirect to this page with proper parameters. */
  if(isset($_POST['create_thread_submit']) && isset($_SESSION['u_id'])){
    $url = 'Location: http://dolphin.csci.viu.ca/~bonsortw/370/forum/newThread.php';
    header($url);
  }
  if(isset($_SESSION['u_id']) && !isset($_GET['f_id'])){
    $head = 'refresh: 3600;url=forumThreads.php?f_id='
            . $_SESSION['f_id'];
    header($head);
  }
  else if(isset($_GET['f_id'])){
    $head = 'refresh: 3600;url=forumThreads.php?f_id='
            . $_GET['f_id'];
    header($head);
  }
?>
<?php include('forum_lib.php'); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN""http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<!------------------------------------------------------------------------------
  -- File:    forumThreads.php
  -- Created: Sat, Mar 31 at 16:20:54 PST
  -- Author:  Tristan W. Bonsor, 536 865 512
  --
  -- Description: Page for thread level views.
  ----------------------------------------------------------------------------->
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
<title>The Hand: Threads</title>
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
	 /* Keep track of which forum the user is in. */
	 if(isset($_GET['f_id'])){
	   $_SESSION['f_id'] = $_GET['f_id'];
	 }
	 /* Determine if the user has logged out. */
	 if(isset($_GET['logout'])){
	   unset($_SESSION['u_id']);
	   unset($_SESSION['is_mod']);
	   unset($_SESSION['is_admin']);
	   $_GET['f_id'] = $_SESSION['f_id'];
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
	     if(isset($_POST['f_id'])){
	       $_SESSION['f_id'] = $_POST['f_id'];
	     }
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
	   echo '<a id="logout" href="?logout=true&f_id='
		 . $_SESSION['f_id'] . '">Logout</a>';
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
EOT;
		 /* Ensure login works in the right forum. */
		 if(isset($_POST['f_id'])){
		   echo '<input type="hidden" name="f_id" value="'
			 . $_POST['f_id'] . '"/>';
		 }
		 else if(isset($_GET['f_id'])){
		   echo '<input type="hidden" name="f_id" value="'
			 . $_GET['f_id'] . '"/>';
		 }
		 else if(isset($_SESSION['f_id'])){
		   echo '<input type="hidden" name="f_id" value="'
			 . $_SESSION['f_id'] . '"/>';
		 }
	  echo <<<EOT
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
	</ul>
      </div>
    </div>
    <!-- Content Area -->
    <div id="content">
      <div class="display">
        <div id="createThread">
	  <form action="<?php $_SERVER['PHP_SELF']; ?>" method="post">
	    <input type="submit" name="create_thread_submit" value="Create Thread"/>
	    <?php
	      /* Ensure create thread works in the right forum. */
	      if(isset($_SESSION['f_id'])){
	        echo '<input type="hidden" name="f_id" value="'
		      . $_SESSION['f_id'] . '"/>';
	      }
	      else{
	        echo '<input type="hidden" name="f_id" value="'
		      . $_GET['f_id'] . '"/>';
	      }
	      /* Due diligence. */
	      if(isset($_POST['create_thread_submit'])){
	        if(!isset($_SESSION['u_id'])){
	          echo '<p class="error">You must be logged in'
	                . 'to create a new thread.</p>';
	        }
	      }
	    ?>
	  </form>
	</div>
	<!-- Display threads -->
	<table id="threads" class="display_table">
	  <tr class="display_row">
	    <th class="display_head">Thread</th>
	    <th class="display_head">Last Post</th>
	    <th class="display_head">Replies</th>
	    <th class="display_head">Views</th>
	  </tr>
	  <?php
	     $connect = connectForum();
	     if(isset($_SESSION['f_id'])){
	       $result = getThreads($connect, $_SESSION['f_id']);
	     }
	     else{
	       $result  = getThreads($connect, $_GET['f_id']);
	     }
	     while($row = mysql_fetch_array($result)){
	       echo '<tr>';

	       echo '<td class="display_cell">';
	       echo '<a class="thread_link" href="threadMessages.php?t_id='
			. $row['thread_id'] . '">';
	       echo $row['thread_name'];
	       echo '</a><br/>';
	       echo '<a class="user_link"';
	       if(isset($_SESSION['is_admin'])){
	         echo ' href="userDisplay.php?u_id='
			. $row['original_user_id'] . '"';
	       }
               echo '>' . $row['original_user_name'];
	       echo '</a><br/>';
	       echo '<span class="time">' . $row['creation_time'];
	       echo '</span>';
	       echo '</td>';

	       echo '<td class="display_cell">';
	       echo '<span class="time">' . $row['update_time'];
	       echo '</span><br/>';
	       echo '<span>by</span>&nbsp';
	       echo '<a class="user_link"';
	       if(isset($_SESSION['is_admin'])){
	         echo ' href="userDisplay.php?u_id='
			. $row['latest_user_id'] . '"';
	       }
               echo '>' . $row['latest_user_name'];
	       echo '</a><br/>';
	       echo '</td>';

	       echo '<td class="display_cell">';
	       echo $row['reply_count'];
	       echo '</td><br/>';

	       echo '<td class="display_cell">';
	       echo $row['view_count'];
	       echo '</td><br/>';

	       echo '</tr>';
	     }
	     mysql_close($connect);
	     ?>
	</table>
      </div>
    </div>
  </div>
</body>
</html>
