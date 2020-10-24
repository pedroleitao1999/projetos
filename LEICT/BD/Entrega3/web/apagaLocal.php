<html>
    <body>
    <form>
  <input type="button" value="voltar" href="index.html" onclick="history.go(-2)">
  </form>
  <head>
       <meta charset="UTF-8">
  </head>

  <?php
    $latitude=$_REQUEST['latitude'];
    $longitude=$_REQUEST['longitude'];

    try
    {
        $user="ist190764";		// -> replace by the user name
        $host="db.ist.utl.pt";	        // -> server where postgres is running
        $port=5432;			// -> default port where Postgres is installed
        $password="aonl4691";	        // -> replace with the password
        $dbname = $user;
        $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        $sql= "DELETE FROM local_publico WHERE (latitude=:latitude AND longitude=:longitude);";

        $result = $db->prepare($sql);
        $result->execute([':latitude' => $latitude, ':longitude' => $longitude]);

        $db = null;

        $frase = "<p>Local apagado</p>";

    }

    catch (PDOException $e)
    {
      $frase = "<p>Local não apagado</p><p>ERROR: {$e->getMessage()}</p>";
    }

    echo("<p>$frase</p>");
  ?>

  </body>

</html>
