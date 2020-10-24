<html>
  <body>
  <form>
    <input type="button" value="voltar" href="index.html" onclick="history.go(-2)">
  </form>
  <head>
    <meta charset="UTF-8">
  </head>

  <?php

    $latitude1=$_REQUEST['latitude1'];
    $longitude1=$_REQUEST['longitude1'];
    $latitude2=$_REQUEST['latitude2'];
    $longitude2=$_REQUEST['longitude2'];

    try
    {
        $user="ist190764";		// -> replace by the user name
        $host="db.ist.utl.pt";	        // -> server where postgres is running
        $port=5432;			// -> default port where Postgres is installed
        $password="aonl4691";	        // -> replace with the password
        $dbname = $user;
        $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        $sql = "SELECT anomalia_id, zona, imagem, lingua, ts, descricao_anomalia, tem_anomalia_redacao
        FROM anomalia
        NATURAL JOIN incidencia
        NATURAL JOIN item
        NATURAL JOIN local_publico
        WHERE ((latitude BETWEEN :latitude1 AND :latitude2) OR (latitude BETWEEN :latitude2 AND :latitude1))
        AND ((longitude BETWEEN :longitude1 and :longitude2) OR (longitude BETWEEN :longitude2 and :longitude1))
        AND (latitude != :latitude1 AND latitude != :latitude2 AND longitude != :longitude1 AND longitude != :longitude2);";

        echo("<p>Anomalias</p>");

        $result = $db->prepare($sql);

        $result->execute([':latitude1'=>$latitude1, ':longitude1'=>$longitude1, ':latitude2'=>$latitude2, ':longitude2'=>$longitude2]);

        echo('<table border="5">');
        echo("<tr><td>anomalia_id</td><td>zona</td><td>imagem</td><td>lingua</td><td>ts</td><td>descricao_anomalia</td><td>tem_anomalia_redacao</td></tr>");

        foreach ($result as $row)
        {
          echo("<tr><td>");
          echo($row['anomalia_id']);
          echo("</td><td>");
          echo($row['zona']);
          echo("</td><td>");
          echo($row['imagem']);
          echo("</td><td>");
          echo($row['lingua']);
          echo("</td><td>");
          echo($row['ts']);
          echo("</td><td>");
          echo($row['descricao_anomalia']);
          echo("</td><td>");
          echo($row['tem_anomalia_redacao']);
          echo("</td></tr>");
        }
        echo("</table>\n");

        $db = null;
    }

    catch (PDOException $e)
    {
      echo("<p>Não foi possivel verificar</p><p>ERROR: {$e->getMessage()}</p>");
    }

  ?>

  </body>

</html>
