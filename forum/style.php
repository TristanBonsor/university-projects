<?php header("Content-type: text/css"); ?>
<!------------------------------------------------------------------------------
  -- File:    style.php
  -- Created: Sat, Mar 31 at 00:04:26 PST
  -- Author:  Tristan W. Bonsor, 536 865 512
  --
  -- Description:
  ----------------------------------------------------------------------------->
<?php
$HTML_MARGIN  = '0px';
$HTML_PADDING = '0px';

$BODY_MARGIN  = '0px';
$BODY_PADDING = '0px';
$BODY_BG_COLOR = '#6699CC';

$cDISPLAY_TABLE_BORDER = '1px solid black';
$cDISPLAY_TABLE_BG_COLOR = '#99CCCC';
$cDISPLAY_TABLE_WIDTH = '100%';

$cERROR_COLOR = 'red';

$cDISPLAY_CELL_BORDER = '1px solid black';

$iCONTAINER_MARGIN = '0px';
$iCONTAINER_PADDING = '1px';
$iCONTAINER_BG_COLOR = '#6699CC';

$iHEADER_MARGIN = '0px';
$iHEADER_PADDING = '0px';

$iTITLE_MARGIN = '0px';
$iTITLE_PADDING = '0px';

$iCONTENT_MARGIN = '0px';
$iCONTENT_PADDING = '0px';
?>

html{
  margin: <?=$HTML_MARGIN?>;
  padding: <?=$HTML_PADDING?>;
}

body{
  margin: <?=$BODY_MARGIN?>;
  padding: <?=$BODY_PADDING?>;
  background-color: <?=$BODY_BG_COLOR?>;
}

.error{
  color: <?=$cERROR_COLOR?>;
}

.display_table{
  border: <?=$cDISPLAY_TABLE_BORDER?>;
  background-color: <?=$cDISPLAY_TABLE_BG_COLOR?>;
  width: <?=$cDISPLAY_TABLE_WIDTH?>;
}

.display_cell{
  border: <?=$cDISPLAY_CELL_BORDER?>;
}

#container{
  margin: <?=$iCONTAINER_MARGIN?>;
  padding: <?=$iCONTAINER_PADDING?>;
  background-color: <?=$iCONTAINER_BG_COLOR?>;
}

#header{
  margin: <?=$iHEADER_MARGIN?>;
  padding: <?=$iHEADER_PADDING?>;
}

#title{
  margin: <?=$iTITLE_MARGIN?>;
  padding: <?=$iTITLE_PADDING?>;
}

#content{
  margin: <?=iCONTENT_MARGIN?>;
  padding: <?=iCONTENT_PADDING?>;
}