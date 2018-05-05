<?php session_start(); ?>
<?php include('forum_lib.php'); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN""http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<!------------------------------------------------------------------------------
  -- File:    search.php
  -- Created: Mon, Apr 2 at 14:47:35 PST
  -- Author:  Tristan W. Bonsor, 536 865 512
  --
  -- Description: Search page.
  ----------------------------------------------------------------------------->
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
<title>The Hand: Search</title>
<link rel="stylesheet" type="text/css" media="screen" href="style.php">
</head>

<body>
  <div id="container">

    <div id="header">
      <img src="comrade_chimp.jpg" alt=":-)" height="150" class="picture"/>
      <h1 id="title">The Hand: Obey The Left Side of Your Brain!</h1>
    </div>

  <div class="nav_links">
    <ul class="link_list">
      <li><a class="link" href="forumMain.php">Back to Forums</a></li>
    </ul>
  </div>

  <form action="<?php echo $SERVER['PHP_SELF']; ?>" method="post">
    <span class="textbox_label">Search Term:</span>
    <input type="text" name="search_term"/>
    <input type="submit" name="search_submit"/>
  </form>

  <?php
    if(isset($_POST['search_submit']) && !preg_match('/(^\s+$|^$)/', $_POST['search_term'])){
      $connect = connectForum();

      $term = '%' . $_POST['search_term'] . '%';

      $query = sprintf("SELECT * FROM ThreadLevelView NATURAL LEFT OUTER JOIN MessageLevelView WHERE thread_name LIKE '%s' OR message_text LIKE '%s'",
                       mysql_real_escape_string($term),
                       mysql_real_escape_string($term));

      $result = mysql_query($query, $connect);

      if($result != false && mysql_num_rows($result) != 0){
        echo '<p>Term found in:</p>';
        while($row = mysql_fetch_array($result)){
          echo '<a class="forum_link" href="threadMessages.php?f_id='
		. $row['forum_id'] . '&t_id=' . $row['thread_id'] . '">';
          echo $row['thread_name'];
          echo '</a><br/>';
        }
      }
      else{
        echo '<p class="error">No matches found.</p>';
      }
      mysql_close($connect);
    }
    else if(isset($_POST['search_submit']) && preg_match('/(^\s+$|^$)/', $_POST['search_term'])){
      echo '<p class="error">Blank search terms not permitted.</p>';
    }
  ?>

  </div>
</body>
</html>
