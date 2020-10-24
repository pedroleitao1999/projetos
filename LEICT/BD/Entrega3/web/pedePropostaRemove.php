<html>
    <body style="font-family:Arial; font-size:20px;">
      <head>
    	   <meta charset="UTF-8">
      </head>
    <h3>Remover Proposta de Correção</h3>
    <form action="removePropostaCorrecao.php" method="post">
      Email: <input type="visible" name="email" value="<?=$_REQUEST['email']?>"><br>
      nro: <input type="visible" name="nro" value="<?=$_REQUEST['nro']?>"><br>
      <input type="submit" value="Submit">
    </form>
  </body>
</html>
