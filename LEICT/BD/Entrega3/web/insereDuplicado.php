<html>
    <body>
    <form>
  <input type="button" value="voltar" href="index.html" onclick="history.go(-2)">
  </form>
  <head>
       <meta charset="UTF-8">
  </head>

  <?php
    $item1=$_REQUEST['item1'];
    $item2=$_REQUEST['item2'];

    try
    {
        $user="ist190764";		// -> replace by the user name
        $host="db.ist.utl.pt";	        // -> server where postgres is running
        $port=5432;			// -> default port where Postgres is installed
        $password="aonl4691";	        // -> replace with the password
        $dbname = $user;
        $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        $sql = "INSERT INTO duplicado VALUES (:item1, :item2 );";

        $frase = "<p>Duplicado inserido</p>";

        $result = $db->prepare($sql);
        $result->execute([':item1' => $item1, ':item2' => $item2]);

        $db = null;

    }

    catch (PDOException $e)
    {
      $frase = "<p>Duplicado não registado</p><p>ERROR: {$e->getMessage()}</p>";
    }

    echo("<p>$frase</p>");
  ?>
  
  </body>

</html>
