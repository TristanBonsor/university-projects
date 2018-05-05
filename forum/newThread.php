<?php session_start(); ?>
<?php
  function connectForum(){
    /* The last two parameters allow php to catch MySQL procedure output
     * parameters without MySQLi and PDO extensions. */
    $con = mysql_connect("cscidb.csci.viu.ca", "bonsortw", "tr1t0n15#!", false, 65536);
    mysql_select_db("csci370", $con);
    return $con;
  }

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

  if(isset($_POST['thread_submit'])
     && !preg_match('/(^\s+$|^$)/', $_POST['thread_title'])
     && !preg_match('/(^\s+$|^$)/', $_POST['message'])){


    insertThread($_POST['f_id'], $_POST['thread_title'], $_POST['u_id'],
                 $_POST['message']);

     $url = 'Location: forumThreads.php?f_id=' . $_POST['f_id'];
     header($url);
  }
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN""http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<!------------------------------------------------------------------------------
  -- File:    newThread.html
  -- Created: Sat, Mar 31 at 22:02:06 PST
  -- Author:  Tristan W. Bonsor, 536 865 512
  --
  -- Description: Page for creating a new thread.
  ----------------------------------------------------------------------------->
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
<title>The Hand: Create A New Thread</title>
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
	  <?php
	     if(isset($_SESSION['f_id'])){
	       echo '<li><a class="link" href="forumThreads.php?f_id='
		    . $_SESSION['f_id'] . '">Back one Level</a></li>';
	     }
             else{
	       echo '<li><a class="link" href="forumThreads.php?f_id='
		    . $_GET['f_id'] . '">Back one Level</a></li>';
	     }
             ?>
	</ul>
      </div>
    </div>

    <div id="content">

      <div id="display">
	<?php
	  if(isset($_POST['thread_submit'])){
	    if(preg_match('/(^\s+$|^$)/', $_POST['thread_title'])){
	      echo '<p class="error">Cannot submit a blank thread title.';
	      echo '</p><br />';
	    }
	    else if(preg_match('/(^\s+$|^$)/', $_POST['message'])){
	      echo '<p class="error">Cannot submit a blank message.';
	      echo '</p><br />';
	    }
	  }
        ?>
	<form action="<?php echo $_SERVER['PHP_SELF'];?>" method="post">
	  <span class="textbox_label">Thread Title:</span><br/>
	  <input type="text" name="thread_title" value="<?php echo $_POST['thread_title'];?>"/><br/>
	  <span class="textbox_label">Message:</span><br/>
	  <textarea rows="20" cols="120" name="message"><?php echo $_POST['message'];?></textarea>
	  <input type="hidden" name="f_id" value="<?php echo $_SESSION['f_id'];?>"/>
	  <input type="hidden" name="u_id" value="<?php echo $_SESSION['u_id'];?>"/>
	  <input type="submit" name="thread_submit" value="Post Thread"/>
	  <input type="reset" value="Clear"/>
	</form>
      </div>

    </div>

  </div>
</body>
</html>
