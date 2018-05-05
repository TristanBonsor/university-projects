<?php session_start(); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN""http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<!------------------------------------------------------------------------------
  -- File:    register.php
  -- Created: Sat, Mar 31 at 12:56:00 PST
  -- Author:  Tristan W. Bonsor, 536 865 512
  --
  -- Description: Registration page.
  ----------------------------------------------------------------------------->
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
<title>The Hand: Registration</title>
<link rel="stylesheet" type="text/css" media="screen" href="style.php">
</head>

<?php
$errors = array();
/* Sticky registration form. */
function displayRegisterForm(){
  global $errors;
  echo '<div id="register_box">';
  echo '<form action="' . $_SERVER['PHP_SELF'] . '" method="post">';

  echo '<h6 class="register_head">Required Information</h6>';

  echo '<span class="textbox_label">User Name:</span>';
  echo '<input type="text" name="user_name" value="'
	.$_POST['user_name'] . '"/>';
  echo '<p class="error">' . $errors['user_name'] . '</p><br/>';

  echo '<span class="textbox_label">Password:</span>';
  echo '<input type="password" name="password1"/>';
  echo '<p class="error">' . $errors['password1'] . '</p><br/>';

  echo '<span class="textbox_label">Confirm Password:</span>';
  echo '<input type="password" name="password2"/>';
  echo '<p class="error">' . $errors['password2'] . '</p><br/>';

  echo '<span class="textbox_label">Email Address:</span>';
  echo '<input type="text" name="email1" value="'
	. $_POST['email1'] . '"/>';
  echo '<p class="error">' . $errors['email1'] . '</p><br/>';

  echo '<span class="textbox_label">Confirm Email Address:</span>';
  echo '<input type="text" name="email2" value="'
	. $_POST['email2'] . '"/>';
  echo '<p class="error">' . $errors['email2'] . '</p><br/>';

  echo '<h6 class="register_head">Optional Information</h6>';

  echo '<span class="textbox_label">First Name:</span>';
  echo '<input type="text" name="fname" value="'
	. $_POST['fname'] . '"/>';
  echo '<p class="error">' . $errors['fname'] . '</p><br/>';

  echo '<span class="textbox_label">Last Name:</span>';
  echo '<input type="text" name="lname" value="'
	. $_POST['lname'] . '"/>';
  echo '<p class="error">' . $errors['lname'] . '</p><br/>';

  echo '<span class="textbox_label">Birthdate:</span><br/>';

  initDateBox();
  echo '<br/>';

  echo '<span class="textbox_label">Location:</span>';
  echo '<input type="text" name="location" value="'
	. $_POST['location'] . '"/>';
  echo '<p class="error">' . $errors['location'] . '</p><br/>';

  echo '<span class="textbox_label">Avatar:</span>';
  echo '<input type="text" name="avatar" value="'
	. $_POST['avatar'] . '"/>';
  echo '<p class="error">' . $errors['avatar'] . '</p><br/>';

  echo '<input type="submit" name="register_submit" value="Register"/>';

  echo '</form>';
  echo '</div>';
}

function initDateBox(){
  $month = array(
 		 array("01","Jan"),
		 array("02","Feb"),
		 array("03","Mar"),
		 array("04","Apr"),
		 array("05","May"),
		 array("06","Jun"),
		 array("07","Jul"),
		 array("08","Aug"),
		 array("09","Sep"),
		 array("10","Oct"),
		 array("11","Nov"),
		 array("12","Dec"));

  echo '<div id="date_box">';

  echo '<span class="textbox_label">Month:</span>';
  echo '<select name="month">';
  echo '<option value="00">--';
  for($i = 0; $i < 12; $i++){
    echo '<option value="'. $month[$i][0] . '">' . $month[$i][1] . '</option>';
  }
  echo '</select>';

  echo '<span class="textbox_label">Day:</span>';
  echo '<select name="day">';
  echo '<option value="00">--';
  for($i = 1; $i < 32; $i++){
    echo '<option value="'. $i . '">' . $i . '</option>';
  }
  echo '</select>';

  echo '<span class="textbox_label">Year:</span>';
  echo '<select name="year">';
  echo '<option value="0000">----';
  for($i = 2012; $i > 1900; $i--){
    echo '<option value="'. $i . '">' . $i . '</option>';
  }
  echo '</select>';

  echo '</div>';
}

function procBirth($year, $month, $day){
  if($day < 10){
    $day = "0" . $day;
  }
  return ($year . "-" . $month . "-" . $day);
}

function validateReg(){
  global $errors;

  if(preg_match('/(^\s+$|^$|^\s+\w+|^\w+\s+)/', $_POST['user_name'])){
    $errors['user_name'] = 'Invalid user name.';
  }
  if(preg_match('/(^\s+$|^$)/', $_POST['password1'])){
    $errors['password1'] = 'Invalid password. ';
  }
  if(preg_match('/(^\s+$|^$)/', $_POST['email1'])){
    $errors['email1'] = 'Invalid email address. ';
  }
  else if(!preg_match('/^\w+@\w+\.\w{2,3}$/',$_POST['email1'])){
    $errors['email1'] = 'Invalid email address. ';
  }
  if($_POST['password1'] != $_POST['password2']){
    $errors['password1'] = $errors['password1'] . 'Password does not match.';
    $errors['password2'] = $errors['password2'] . 'Password does not match.';
  }
  if($_POST['email1'] != $_POST['email2']){
    $errors['email1'] = $errors['email1'] . 'Email address does not match.';
    $errors['email2'] = $errors['email2'] . 'Email address does not match.';
  }
}
?>

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
	<?php
	  require_once('forum_lib.php');
	  if(isset($_POST['register_submit'])){
	    validateReg();
	    if(count($errors) != 0){
	      displayRegisterForm();
	    }
	    else{
	      $insert_err = "";
	      $connect = connectForum();
	      $birthdate = procBirth($_POST['year'], $_POST['month'], $_POST['day']);
	      if(createAccount($insert_err, $connect, $_POST['user_name'],
	         $_POST['password1'], $_POST['email1'], $_POST['fname'],
	         $_POST['lname'], $birthdate, $_POST['location'],
	         $_POST['avatar'])){
	        echo '<p>Thanks. Account succsessfully created.</p><br/>';
	      }
	      else{
	        echo '<p class="error">' . $insert_err . '</p><br />';
	        displayRegisterForm();
	      }
	      mysql_close($connect);
	   }
	 }
	 else{
	   displayRegisterForm();
	 }
      ?>
      </div>
    </div>
  </div>
</body>
</html>
