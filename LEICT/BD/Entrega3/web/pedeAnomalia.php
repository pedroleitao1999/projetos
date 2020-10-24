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

  echo("Inserir uma anomalia");

	pg_close($connection);

?>

<form action="insertAnomalias.php">
  Id da anomalia: <input type="text" name="anomalia_id"><br>
  Zona: <input type="text" name="zona"><br>
  Imagem: <input type="text" name="imagem"><br>
  Lingua: <input type="text" name="lingua"><br>
  Timestamp de captura: <input type="text" name="ts"><br>
  Descrição da anomalia: <input type="text" name="descricao_anomalia"><br>
  Tem anomalia de redação? <input type="text" name="tem_anomalia_redacao"><br>
  <input type="submit" value="Submit">
</form>

</body>
</html>
