<?php
    // Koneksi ke MySQL
    include("dbconnect.php");
    $tem = $_GET["temperature"];
    $lam = $_GET["lampu"];
	$SQL = "INSERT INTO ihms (temperature, lampu) VALUES ('$tem', '$lam')"; // SQL Query    
    // Eksekusi pernyataan SQL
	mysql_query($SQL);
    // Menuju file review_data.php
    header("Location: data_review.php");
?>