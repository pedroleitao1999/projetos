<html>
    <body style="font-family:Arial; font-size:20px;">
      <head>
    	   <meta charset="UTF-8">
      </head>
    <h3>Remover Correção</h3>
    <form action="removeCorrecao.php" method="post">
      Email: <input type="visible" name="email" value="<?=$_REQUEST['email']?>"><br>
      Anomalia(ID): <input type="visible" name="anomalia_id" value="<?=$_REQUEST['anomalia_id']?>"><br>
      <input type="submit" value="Submit">
    </form>
  </body>
</html>
