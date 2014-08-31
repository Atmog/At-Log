<?php 
$host = '';
$user = '';
$pass = '';
$dbname = '';

if($_POST['action'] == "insert")
{
	mysql_connect($host,$user,$pass);
	mysql_select_db($dbname);
	mysql_query("SET NAMES UTF8"); 

	$type = htmlspecialchars(addslashes($_POST['type']));
	$appname = htmlspecialchars(addslashes($_POST['appname']));
	$appversion = htmlspecialchars(addslashes($_POST['appversion']));
	$address = htmlspecialchars(addslashes($_POST['address']));
	$username = htmlspecialchars(addslashes($_POST['username']));
	$content = htmlspecialchars(addslashes($_POST['content']));

	mysql_query("INSERT INTO log VALUES('', NOW(), '$type', '$appname', '$appversion', '$address', '$username', '$content')");
	mysql_close();
}
else
{
	if(!isset($_POST['action']) OR $_POST['action'] == "read")
	{
		mysql_connect($host,$user,$pass);
		mysql_select_db($dbname);
		mysql_query("SET NAMES UTF8");

		$req = mysql_query("SELECT * FROM log ORDER BY id DESC");

                while ($rep = mysql_fetch_array($req))
		{ 
			echo $rep['date'] . " ";
			echo $rep['type'] . " ";
			echo $rep['appname'] . " ";
			echo $rep['appversion'] . " ";
			echo $rep['address'] . " ";
			echo $rep['username'] . " ";
			echo "::: " . $rep['content'];
		}
		mysql_close();
	}
	// Here I can add more 'action' and more GET variables to see only logs I want for examples only see logs from one user, or one day...
}
?>
