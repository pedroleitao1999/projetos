<html>
<meta charset="utf-8">
<body>

<?php
  $latitude=$_REQUEST['latitude'];
  $longitude=$_REQUEST['longitude'];
  $nome=$_REQUEST['nome'];

  try
  {
      $user="ist190764";		// -> replace by the user name
      $host="db.ist.utl.pt";	        // -> server where postgres is running
      $port=5432;			// -> default port where Postgres is installed
      $password="aonl4691";	        // -> replace with the passoword
      $dbname = $user;
      $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
      $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

      $sql = "INSERT INTO local_publico VALUES (:latitude, :longitude, :nome);";

      $frase = "<p>Local inserido</p>";

      $result = $db->prepare($sql);
      $result->execute([':latitude' => $latitude, ':longitude' => $longitude, ':nome' => $nome]);

      $db = null;
  }
  catch (PDOException $e)
  {
    $frase = "<p>Local não inserido</p><p>ERROR: {$e->getMessage()}</p>";
  }
  echo("<p>$frase</p>");
?>

<form action="index.html">
  <input type="submit" value="Voltar">
</form>

</body>
</html>
