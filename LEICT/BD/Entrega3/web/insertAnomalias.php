<html>
<meta charset="utf-8">
<body>

<?php
  $anomalia_id=$_REQUEST['anomalia_id'];
  $zona=$_REQUEST['zona'];
  $imagem=$_REQUEST['imagem'];
  $lingua=$_REQUEST['lingua'];
  $ts=$_REQUEST['ts'];
  $descricao_anomalia=$_REQUEST['descricao_anomalia'];
  $tem_anomalia_redacao=$_REQUEST['tem_anomalia_redacao'];

  try
  {
      $user="ist190764";		// -> replace by the user name
      $host="db.ist.utl.pt";	        // -> server where postgres is running
      $port=5432;			// -> default port where Postgres is installed
      $password="aonl4691";	        // -> replace with the passoword
      $dbname = $user;
      $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
      $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

      $sql = "INSERT INTO anomalia VALUES (:anomalia_id, :zona, :imagem, :lingua, :ts, :descricao_anomalia, :tem_anomalia_redacao);";

      $frase = "<p>Anomalia inserido</p>";

      $result = $db->prepare($sql);
      $result->execute([':anomalia_id' => $anomalia_id, ':zona' => $zona, ':imagem' => $imagem, ':lingua' => $lingua, ':ts' => $ts, ':descricao_anomalia' => $descricao_anomalia, ':tem_anomalia_redacao' => $tem_anomalia_redacao]);

      $db = null;
  }
  catch (PDOException $e)
  {
    $frase = "<p>Anomalia não inserida</p><p>ERROR: {$e->getMessage()}</p>";
  }
  echo("<p>$frase</p>");
?>

<form action="index.html">
  <input type="submit" value="Voltar">
</form>

</body>
</html>
