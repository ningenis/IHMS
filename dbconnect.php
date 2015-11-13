<?php
$MyUsername = "root";  // Username MySQL
$MyPassword = "";  // Password MySQL 
$MyHostname = "localhost";	// Host
$Database = "ihms";    // Nama basis data
$dbh = mysql_connect($MyHostname , $MyUsername, $MyPassword); // Lakukan koneksi ke MySQL
mysql_select_db($Database); // Pilih basis data
if (!$dbh) {
    die("Connection failed: " . mysqli_connect_error());
}
?>