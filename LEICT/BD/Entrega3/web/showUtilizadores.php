<html>
<body>
	<form>
		<input type="button" value="Voltar" href="index.html" onclick="history.go(-1)">
	</form>
<?php
	$user="ist190764";		// -> replace by the user name
	$host="db.ist.utl.pt";	        // -> server where postgres is running
	$port=5432;			// -> default port where Postgres is installed
	$password="aonl4691";	        // -> replace with the passoword
	$dbname = $user;		// -> by default the name of the database is the name of the user

	$connection = pg_connect("host=$host port=$port user=$user password=$password dbname=$dbname") or die(pg_last_error());

	$sql = "SELECT * FROM utilizador;";

	echo("Utilizadores");

	$result = pg_query($sql) or die('ERROR: ' . pg_last_error());

	echo('<table border="5">');
	echo("<tr><td>email</td><td>password</td></tr>");
	while ($row = pg_fetch_assoc($result))
	{
		echo("<tr><td>");
		echo($row["email"]);
		echo("</td><td>");
		echo($row["password"]);
		echo("</td></tr>");
	}
	echo("</table>");

	$result = pg_free_result($result) or die('ERROR: ' . pg_last_error());

	pg_close($connection);

?>
</body>
</html>
