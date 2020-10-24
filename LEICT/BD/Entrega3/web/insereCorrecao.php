<html>
    <body>
    <form>
    <input type="button" value="Voltar" href="index.html" onclick="history.go(-2)">
  </form>
  <head>
       <meta charset="UTF-8">
  </head>

  <?php

    $email = $_REQUEST['email'];
    $anomalia_id = $_REQUEST['anomalia_id'];
    $nro = $_REQUEST['nro'];

    try
    {
      $user="ist190764";        // -> replace by the user name
      $host="db.ist.utl.pt";            // -> server where postgres is running
      $port=5432;            // -> default port where Postgres is installed
      $password="aonl4691";            // -> replace with the passoword
      $dbname = $user;
      $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
      $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

      $sql = "INSERT INTO correcao VALUES (:email, :nro,:anomalia_id);";

      $frase = "<p>Correção Inserida</p>";

      $result = $db->prepare($sql);
      $result->execute([':email' => $email , ':nro' => $nro, ":anomalia_id" => $anomalia_id]);

      $db = null;
    }
    catch (PDOException $e)
    {
      $frase = "<p>Correção não inserida</p><p>ERROR: {$e->getMessage()}</p>";
    }
    echo("<p>$frase</p>");
  ?>
  </body>

</html>
