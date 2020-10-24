<html>
    <body style="font-family:Arial; font-size:20px;">
      <head>
    	   <meta charset="UTF-8">
      </head>
    <h3>Alterar Proposta de Correção</h3>
    <form action="alteraProposta.php" method="post">
      Nro da Proposta a ser alterada: <input type="visible" name="nro" value="<?=$_REQUEST['nro']?>"><br>
      Nova Data e Hora: <input type="visible" name="data_hora" value="<?=$_REQUEST['data_hora']?>"><br>
      Novo Texto: <input type="visible" name="texto" value="<?=$_REQUEST['texto']?>"><br>
      <input type="submit" value="Submit">
    </form>
  </body>
</html>
