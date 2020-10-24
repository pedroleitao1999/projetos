<html>
    <body style="font-family:Arial; font-size:20px;">
      <head>
    	   <meta charset="UTF-8">
      </head>
    <h3>Inserir Correção</h3>
    <form action="inserePropostaCorrecao.php" method="post">
      Email: <input type="visible" name="email" value="<?=$_REQUEST['email']?>"><br>
      Texto: <input type="visible" name="texto" value="<?=$_REQUEST['texto']?>"><br>
      Data e Hora: <input type="visible" name="data_hora" value="<?=$_REQUEST['data_hora']?>"><br>
      <input type="submit" value="Submit">
    </form>
  </body>
</html>
