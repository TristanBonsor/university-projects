<?php session_start(); ?>
<?php include('forum_lib.php'); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN""http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<!------------------------------------------------------------------------------
  -- File:    demo1.php
  -- Created: Fri, Mar 30 at 20:19:12 PST
  -- Author:  Tristan W. Bonsor, 536 865 512; Mangled by Craig Burnett, 515 766 558
  --
  -- Description: Demonstration of sortable query results.
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
	  <li><a class="link" href="forumMain.php">Back to Forums</a></li>
	</ul>
      </div>

    </div>
    <!-- Content Area -->
    <div id="content">
      <div class="display">
	<table id="forums" class="display_table">
	  <tr class="display_row">
	    <th class="display_head"><a href="demo1.php?c_id=1">Forum Name</a></th>
	    <th class="display_head"><a href="demo1.php?c_id=2">Total Messages</a></th>
	    <th class="display_head"><a href="demo1.php?c_id=3">Newest Message</a></th>
	    <th class="display_head"><a href="demo1.php?c_id=4">Oldest Message</a></th>
	    <th class="display_head"><a href="demo1.php?c_id=5">Messages Per Day</a></th>
	  </tr>
	  <h1>Busiest forums</h1>
	  <p>Limit results to top <input type="text" value="3" name="HowMany" /></p>
	  <?php
 	     $_SESSION['HowMany'] = $_POST['HowMany'];
	     $connect = connectForum();
	     $result  = demo1($connect, $_GET['c_id'], $_SESSION['HowMany']);
	     while($row = mysql_fetch_array($result)){
	       echo '<tr>';
	       echo '<td class="display_cell">' . $row['forum_name'] . '</td>';
	       echo '<td class="display_cell">' . $row['num_messages'] . '</td>';
	       echo '<td class="display_cell">' . $row['NewestMsg'] . '</td>';
	       echo '<td class="display_cell">' . $row['OldestMsg'] . '</td>';
	       echo '<td class="display_cell">' . $row['MsgPerDay'] . '</td>';
	       echo '</tr>';
	     }
	     ?>
	</table>
	<table border="1">
	  <tr>
	    <th>User ID:</th>
	    <th>User Name:</th>
	    <th>Email:</th>
	    <th>First Name:</th>
	    <th>Last Name:</th>
	    <th>Birthdate:</th>
	    <th>Location:</th>
	    <th>Join Date:</th>
	  </tr>
	<?php
	  $connect = connectForum();
	  $query = 'SELECT * FROM User';
	  $result = mysql_query($query, $connect);
	  while($row = mysql_fetch_array($result)){
	    echo '<tr>';
	    echo '<td>' . $row['user_id'] . '</td>';
	    echo '<td>' . $row['user_name'] . '</td>';
	    echo '<td>' . $row['email'] . '</td>';
	    echo '<td>' . $row['first_name'] . '</td>';
	    echo '<td>' . $row['last_name'] . '</td>';
	    echo '<td>' . $row['birthdate'] . '</td>';
	    echo '<td>' . $row['location'] . '</td>';
	    echo '<td>' . $row['join_time'] . '</td>';
	    echo '</tr>';
	  }
	?>
	</table>

      </div>
    </div>
  </div>
</body>
</html>
