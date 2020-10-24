<html>
<meta charset="utf-8">
<body>

<?php
	$user="ist190764";		// -> replace by the user name
	$host="db.ist.utl.pt";	        // -> server where postgres is running
	$port=5432;			// -> default port where Postgres is installed
	$password="aonl4691";	        // -> replace with the passoword
	$dbname = $user;		// -> by default the name of the database is the name of the user

	$connection = pg_connect("host=$host port=$port user=$user password=$password dbname=$dbname") or die(pg_last_error());

  echo("Inserir um local");

	pg_close($connection);

?>

<form action="insertLocais.php">
  Latitude: <input type="text" name="latitude"><br>
  Longitude: <input type="text" name="longitude"><br>
  Nome: <input type="text" name="nome"><br>
  <input type="submit" value="Submit">
</form>

</body>
</html>
