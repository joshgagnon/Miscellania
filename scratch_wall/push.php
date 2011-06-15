
<?php 
    session_start();
     $con = mysql_connect("localhost","root","scratch");
  
    if(! $con){	
	die("lol" . mysql_error());		
    }
  mysql_select_db('scratch');	    

if(! isset($_SESSION['id'])){
    $_SESSION['id'] = 1;
}
$text = mysql_query('update scratch set text = "'.mysql_real_escape_string($_POST['delta']).'" where id = '.$_SESSION['id']);
?>
