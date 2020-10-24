<html>
	<body style="font-family:Arial; font-size:20px;">
	<form>
  <input type="button" value="Voltar" href="index.html" onclick="history.go(-2)">
</form>
<head>
	<meta charset="UTF-8">
</head>

<?php

	$nro= $_REQUEST['nro'];
  $anomalia_id = $_REQUEST['anomalia_id'];

	try{


		$host="db.ist.utl.pt";
		$user="ist190764";
		$password= "aonl4691";
		$dbname=$user;

		$db= new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
		$db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

		$sql= "UPDATE correcao SET anomalia_id=:anomalia_id WHERE nro = :nro; ";

    $frase = "<p>Correção Editada!</p>";

    $db->beginTransaction();
		$result = $db->prepare($sql);

    $result->execute([':anomalia_id' => $anomalia_id, ':nro' => $nro]);

    $db->commit();

		$db=null;

	}
	catch (PDOException $e){
        echo("<p>ERROR: {$e->getMessage()}</p>");
  }
  echo("<p>$frase</p>");
?>
</body>
</html>
