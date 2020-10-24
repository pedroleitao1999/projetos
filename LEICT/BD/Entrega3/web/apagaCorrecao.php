<html>
    <body>
    <form>
  <input type="button" value="voltar" href="index.html" onclick="history.go(-2)">
  </form>
  <head>
       <meta charset="UTF-8">
  </head>

  <?php
    $email=$_REQUEST['email'];
    $nro=$_REQUEST['nro'];

    try
    {
        $user="ist190764";		// -> replace by the user name
        $host="db.ist.utl.pt";	        // -> server where postgres is running
        $port=5432;			// -> default port where Postgres is installed
        $password="aonl4691";	        // -> replace with the password
        $dbname = $user;
        $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        $sql= "DELETE FROM correcao WHERE email=:email AND nro=:nro;";

        $frase = "<p>Correção apagada</p>";

        $result = $db->prepare($sql);
        $result->execute([':email' => $email, ':nro' => $nro]);

        $db = null;

    }

    catch (PDOException $e)
    {
      $frase = "<p>Correção não apagada</p><p>ERROR: {$e->getMessage()}</p>";
    }

    echo("<p>$frase</p>");
  ?>

  </body>

</html>
