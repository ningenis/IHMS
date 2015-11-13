<?php 
    // Mulai koneksi MySQL
    include('dbconnect.php');
?>

<html>
<head>
    <!-- Judul -->
    <title>Sistem Pengelolaan Intelligent Home</title>
    <link rel="icon" type="image/png" href="img/logo.png">
    <!-- Include -->
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width">
    <link rel="stylesheet" href="css/bootstrap.min.css">
    <script type="text/javascript" src="js/bootstrap.min.js"></script>
    <link href="css/style.css" rel="stylesheet">
    <link href="css/table.css" rel="stylesheet">
</head>

<body>
    <!-- Header -->
    <div class="header">
        <ul class="nav nav-pills pull-right">
          <li role="presentation" class="active">
            <a href="index.html">Home</a>
          </li>
        </ul>
        <h3 class="text-muted">42K4</h3>
    </div>
    <!-- Container --> 
    <div class="container">
        <div class="jumbotron">
        <h1><p style="font-size:36px">Status Rumah</p></h1>
<?php
    // Retrieve record terbaru
    mysql_select_db('ihms');
    $result = mysql_query("SELECT * FROM ihms ORDER BY id DESC LIMIT 1");
    // Proses record dan menampilkannya
    while( $row = mysql_fetch_array($result) )
    {
        echo '<div class="well">';
        echo date("F j, Y g:i a", strtotime($row["event"]));
        echo '</div>';
        echo '<div class="panel panel-default">';
        echo '<div class="panel-heading">';
        echo '<h3 class="panel-title">Suhu Ruangan</h3>';
        echo '</div>';
        echo '<div class="panel-body">';
        echo $row["temperature"];
        echo ' C';
        echo '</div>';
        echo '</div>';
        if ($row["temperature"] < 16.00) {
            $situasi = 'dingin.';
        }
        if ($row["temperature"] > 16.00 && $row["temperature"] <= 20.00) {
            $situasi = 'agak dingin.';
        }
        if ($row["temperature"] > 20.00 && $row["temperature"] <= 26.00) {
            $situasi = 'sejuk.';
        }
        if ($row["temperature"] > 26.00 && $row["temperature"] <= 30.00) {
            $situasi = 'hangat.';
        }
        if ($row["temperature"] > 30.00) {
            $situasi = 'panas.';
        }
        echo '<div class="panel-footer">';
        echo 'Suasana rumah saat ini ';
        echo $situasi;
        echo '</div>';
        echo '<br>';
        echo '<div class="panel panel-default">';
            echo '<div class="panel-heading">';
                echo '<h3 class="panel-title">Status Peralatan Elektronik</h3>';
            echo '</div>';
        echo '</div>';
        echo '<div class="row">';
            echo '<div class="col-sm-6 col-md-6 col-lg-6">';
                echo '<div class="panel panel-default">';
                    echo '<div class="panel-heading">';
                        echo '<h3 class="panel-title">Lampu</h3>';
                    echo '</div>';
                    echo '<div class="panel-body">';
                        if ($row["lampu"] == 0) {
                            echo 'OFF';
                        } else if ($row["lampu"] == 1) {
                            echo 'ON';
                        } else if ($row["lampu"] == 2) {
                            echo 'Redup';
                        }
                    echo '</div>';
                echo '</div>';
            echo '</div>';
            echo '<div class="col-sm-6 col-md-6 col-lg-6">';
                echo '<div class="panel panel-default">';
                    echo '<div class="panel-heading">';
                        echo '<h3 class="panel-title">AC</h3>';
                    echo '</div>';
                    echo '<div class="panel-body">';
                    if ($row["temperature"] < 26.00 && $row["temperature"] > 20.00) {
                        echo 'OFF';
                    } else {
                        echo 'ON';
                    }
                    echo '</div>';
                echo '</div>';
            echo '</div>';
        echo '<div class="btn-group">';
        echo        '<a href="data_review.php"><button class="btn btn-default">Refresh</button></a>';
        echo '</div>';
        echo '</div>';

    }
?>
        </div>
    </div>
    <!-- Footer -->
    <div class="footer">
      <p>&copy; 42K4 2015</p>
    </div>
</body>

</html>