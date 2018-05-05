<?php session_start(); ?>
<?php
  function connectForum(){
    /* The last two parameters allow php to catch MySQL procedure output
     * parameters without MySQLi and PDO extensions. */
    $con = mysql_connect("cscidb.csci.viu.ca", "bonsortw", "tr1t0n15#!", false, 65536);
    mysql_select_db("csci370", $con);
    return $con;
  }

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
  if(isset($_POST['reply_submit'])
     && !preg_match('/(^\s+$|^$)/', $_POST['message'])){

    insertMessage($_POST['t_id'], $_POST['m_id'], $_POST['u_id'],
                 $_POST['message']);

     $url = 'Location: threadMessages.php?t_id=' . $_POST['t_id'];
     header($url);
  }
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN""http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<!------------------------------------------------------------------------------
  -- File:    newMessage.html
  -- Created: Sat, Mar 31 at 22:02:19 PST
  -- Author:  Tristan W. Bonsor, 536 865 512
  --
  -- Description: Page for creating a reply.
  ----------------------------------------------------------------------------->
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
<title>The Hand: Create A Reply</title>
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
	     if(isset($_SESSION['t_id'])){
	       echo '<li><a class="link" href="threadMessages.php?t_id='
		    . $_SESSION['t_id'] . '">Back one Level</a></li>';
	     }
             else{
	       echo '<li><a class="link" href="threadMessages.php?t_id='
		    . $_GET['t_id'] . '">Back one Level</a></li>';
	     }
          ?>
	</ul>
      </div>
    </div>

    <div id="content">

      <div id="display">
	<?php
	  if(isset($_POST['reply_submit'])){
	    if(preg_match('/(^\s+$|^$)/', $_POST['message'])){
	      echo '<p class="error">Cannot submit a blank message.';
	      echo '</p><br />';
	    }
	  }
        ?>
	<form action="<?php echo $_SERVER['PHP_SELF'];?>" method="post">
	  <span class="textbox_label">Message:</span><br/>
	  <textarea rows="20" cols="120" name="message"><?php echo $_POST['message'];?></textarea>
	  <input type="hidden" name="t_id" value="<?php echo $_SESSION['t_id'];?>"/>
	  <input type="hidden" name="m_id" value="<?php echo $_SESSION['m_id'];?>"/>
	  <input type="hidden" name="u_id" value="<?php echo $_SESSION['u_id'];?>"/>
	  <input type="submit" name="reply_submit" value="Post Reply"/>
	  <input type="reset" value="Clear"/>
	</form>
      </div>

    </div>

  </div>
</body>
</html>
