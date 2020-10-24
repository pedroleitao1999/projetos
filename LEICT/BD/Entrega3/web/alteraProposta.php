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
  $data_hora = $_REQUEST['data_hora'];
  $texto = $_REQUEST['texto'];

	try{


		$host="db.ist.utl.pt";
		$user="ist190764";
		$password= "aonl4691";
		$dbname=$user;

		$db= new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
		$db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

		$sql= "UPDATE proposta_de_correcao SET data_hora=:data_hora WHERE nro=:nro; ";
    $sql2= "UPDATE proposta_de_correcao SET texto=:texto WHERE nro=:nro; ";

    $frase = "<p>Proposta de Correção Editada!</p>";

    $db->beginTransaction();
		$result = $db->prepare($sql);
    $result2 = $db->prepare($sql2);

    $result->execute([':data_hora' => $data_hora, ':nro' => $nro]);
    $result2->execute([':texto' => $texto, ':nro' => $nro]);

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
