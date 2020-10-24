<html>
    <body>
    <form>
  <input type="button" value="voltar" href="index.html" onclick="history.go(-2)">
  </form>
  <head>
       <meta charset="UTF-8">
  </head>

  <?php

    $user="ist190764";		// -> replace by the user name
    $host="db.ist.utl.pt";	        // -> server where postgres is running
    $port=5432;			// -> default port where Postgres is installed
    $password="aonl4691";	        // -> replace with the passoword
    $dbname = $user;		// -> by default the name of the database is the name of the user

    $connection = pg_connect("host=$host port=$port user=$user password=$password dbname=$dbname") or die(pg_last_error());

    echo("Apagar um item");

    pg_close($connection);

  ?>

  <form action="apagaItem.php">
    Id do item a apagar: <input type="text" name="item_id"><br>
    <input type="submit" value="Submit">
  </form>

  </body>
</html>
