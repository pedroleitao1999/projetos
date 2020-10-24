<html>
    <body style="font-family:Arial; font-size:20px;">
      <head>
    	   <meta charset="UTF-8">
      </head>
    <h3>Alterar Proposta de Correção</h3>
    <form action="alteraCorrecao.php" method="post">
      Nro da Proposta a ser alterada: <input type="visible" name="nro" value="<?=$_REQUEST['nro']?>"><br>
      Nova Anomalia associada(ID): <input type="visible" name="anomalia_id" value="<?=$_REQUEST['anomalia_id']?>"><br>
      <input type="submit" value="Submit">
    </form>
  </body>
</html>
