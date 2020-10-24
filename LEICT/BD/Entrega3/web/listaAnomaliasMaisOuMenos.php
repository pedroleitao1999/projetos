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
    $grauX=$_REQUEST['grauX'];
    $grauY=$_REQUEST['grauY'];

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
        WHERE ((latitude BETWEEN CAST(:latitude AS numeric(9,6)) - CAST(:grauX AS numeric(9,6)) AND CAST(:latitude AS numeric(9,6)) + CAST(:grauX As numeric(9,6)))
        AND (longitude BETWEEN CAST(:longitude AS numeric(9,6)) - CAST(:grauY AS numeric(9,6)) AND CAST(:longitude AS numeric(9,6)) + CAST(:grauY AS numeric(9,6))))
        AND (DATE_TRUNC('day', ts) >= (DATE_TRUNC('day', NOW()) - INTERVAL '3 MONTH') );";

        echo("<p>Anomalias</p>");

        $result = $db->prepare($sql);

        $result->execute( [':latitude'=> $latitude, ':longitude' => $longitude, ':grauX'=> $grauX, ':grauY'=> $grauY]);

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
