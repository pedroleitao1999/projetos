<html>
    <body>
    <form>
  <input type="button" value="voltar" href="index.html" onclick="history.go(-2)">
  </form>
  <head>
       <meta charset="UTF-8">
  </head>

  <?php
    $anomalia_id=$_REQUEST['anomalia_id'];

    try
    {
        $user="ist190764";		// -> replace by the user name
        $host="db.ist.utl.pt";	        // -> server where postgres is running
        $port=5432;			// -> default port where Postgres is installed
        $password="aonl4691";	        // -> replace with the password
        $dbname = $user;
        $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        $sql= "DELETE FROM anomalia WHERE anomalia_id=:anomalia_id;";

        $frase = "<p>Anomalia apagada</p>";

        $result = $db->prepare($sql);
        $result->execute([':anomalia_id' => $anomalia_id]);

        $db = null;

    }

    catch (PDOException $e)
    {
      $frase = "<p>Anomalia não apagada</p><p>ERROR: {$e->getMessage()}</p>";
    }

    echo("<p>$frase</p>");
  ?>

  </body>

</html>
