<?php session_start(); ?>
<?php include('forum_lib.php'); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN""http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<!------------------------------------------------------------------------------
  -- File:    userDisplay.php
  -- Created: Mon, Apr 2 at 08:36:27 PST
  -- Author:  Tristan W. Bonsor, 536 865 512
  --
  -- Description: User info display page.
  ----------------------------------------------------------------------------->
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
<title>The Hand: User Display</title>
<link rel="stylesheet" type="text/css" media="screen" href="style.php">
</head>
<body>
  <div id="container">

    <div id="header">
      <img src="comrade_chimp.jpg" alt=":-)" height="150" class="picture"/>
      <h1 id="title">The Hand: Obey The Left Side of Your Brain!</h1>
    </div>

    <div id="navigator">
      <div class="nav_links">
	<ul class="link_list">
	  <li><a class="link" href="forumMain.php">Back to Forums</a></li>
	</ul>
      </div>
    </div>

    <div id="content">

      <div class="display">
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
	  $result = getUserInfo($connect, $_GET['u_id']);
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
