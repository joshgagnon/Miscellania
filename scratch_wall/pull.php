<?php 
    session_start();
      	
$con = mysql_connect("localhost","root","scratch");

if(! $con){	
    die("lol");		
}	    
mysql_select_db('scratch');		 

if(! isset($_SESSION['id'])){
    $_SESSION['id'] = 1;
}
$result = mysql_query('SELECT text from scratch where id='.$_SESSION['id']);
if($row=mysql_fetch_assoc($result)){
    echo $row['text'];
}
mysql_free_result($result);
?>
