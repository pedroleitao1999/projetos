<html>
<meta charset="utf-8">
<body>

<?php
  $item_id=$_REQUEST['item_id'];
  $descricao_item=$_REQUEST['descricao_item'];
  $localizacao=$_REQUEST['localizacao'];
  $latitude=$_REQUEST['latitude'];
  $longitude=$_REQUEST['longitude'];

  try
  {
      $user="ist190764";		// -> replace by the user name
      $host="db.ist.utl.pt";	        // -> server where postgres is running
      $port=5432;			// -> default port where Postgres is installed
      $password="aonl4691";	        // -> replace with the passoword
      $dbname = $user;
      $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
      $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

      $sql = "INSERT INTO item VALUES (:item_id, :descricao_item, :localizacao, :latitude, :longitude);";

      $frase = "<p>Item inserido</p>";

      $result = $db->prepare($sql);
      $result->execute([':item_id' => $item_id, ':descricao_item' => $descricao_item, ':localizacao' => $localizacao, ':latitude' => $latitude, ':longitude' => $longitude]);

      $db = null;
  }
  catch (PDOException $e)
  {
      $frase = "<p>Item não inserido</p><p>ERROR: {$e->getMessage()}</p>";
  }
  echo("<p>$frase</p>");
?>

<form action="index.html">
  <input type="submit" value="Voltar">
</form>

</body>
</html>
