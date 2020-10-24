<html>
<body>
<?php
	$user="ist190764";		// -> replace by the user name
	$host="db.ist.utl.pt";	        // -> server where postgres is running
	$port=5432;			// -> default port where Postgres is installed
	$password="aonl4691";	        // -> replace with the passoword
	$dbname = $user;		// -> by default the name of the database is the name of the user

	$connection = pg_connect("host=$host port=$port user=$user password=$password dbname=$dbname") or die(pg_last_error());

	$sql = "SELECT * FROM proposta_de_correcao;";

	echo($sql);

	$result = pg_query($sql) or die('ERROR: ' . pg_last_error());

	echo('<table border="5">');
	echo("<tr><td>email</td><td>texto</td><td>data e hora</td><td>nro</td></tr>");
	while ($row = pg_fetch_assoc($result))
	{
		echo("<tr><td>");
		echo($row["email"]);
		echo("</td><td>");
		echo($row["texto"]);
    echo("</td><td>");
    echo($row["data_hora"]);
    echo("</td><td>");
    echo($row["nro"]);
		echo("</td></tr>");
	}
	echo("</table>");

	$result = pg_free_result($result) or die('ERROR: ' . pg_last_error());

	pg_close($connection);

?>
</body>
</html>
