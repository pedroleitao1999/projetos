<html>
	<body style="font-family:Arial; font-size:20px;">
    <form>
    <input type="button" value="Voltar" href="index.html" onclick="history.go(-2)">
    </form>
    <head>
       <meta charset="UTF-8">
    </head>

<?php

	$email = $_REQUEST['email'];
  $nro = $_REQUEST['nro'];

	try{
		$host="db.ist.utl.pt";
		$user="ist190764";
		$password= "aonl4691";
		$dbname=$user;

		$db= new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
		$db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    $sql1= "DELETE FROM correcao WHERE nro=:nro AND email=:email;";
		$sql2= "DELETE FROM proposta_de_correcao WHERE nro=:nro AND email=:email;";

    $frase = "<p>Proposta de Correção removida!</p>";

		$db->beginTransaction();

		$result=$db->prepare($sql1);
		$result2=$db->prepare($sql2);

		$result-> execute([':nro' => $nro, ':email' => $email]);
		$result2-> execute([':nro' => $nro,':email' => $email]);

	 	$db->commit();
		$db=null;
	}
	catch(PDOException $e){
		echo("<p>ERROR: {$e->getMessage()}</p>");
	}
  echo("<p>$frase</p>");
?>

</html>
