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
	$anomalia_id = $_REQUEST['anomalia_id'];

	try
	{
		$host="db.ist.utl.pt";
		$user="ist190764";
		$password= "aonl4691";
		$dbname=$user;

		$db= new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
		$db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    $sql= "DELETE FROM correcao WHERE email=:email AND anomalia_id=:anomalia_id;";

    $frase = "<p>Correção Removida!</p>";

		$db->beginTransaction();

		$result=$db->prepare($sql);

		$result-> execute([':email' => $email ,':anomalia_id' => $anomalia_id]);

	 	$db->commit();
		$db=null;
	}
	catch(PDOException $e){
		echo("<p>ERROR: {$e->getMessage()}</p>");
	}
  echo("<p>$frase</p>");
?>

</html>
