# Guião de Demonstração

## 1. Preparação do Sistema

Para testar a aplicação e todos os seus componentes, é necessário preparar um ambiente com dados para proceder à verificação dos testes.

### 1.1. Compilar o Projeto

Primeiramente, é necessário instalar as dependências necessárias para o *silo* e os clientes (*eye* e *spotter*) e compilar estes componentes.

Para isso, basta ir à diretoria *root* do projeto e correr o seguinte comando:

```
$ mvn clean install -DskipTests
```

Com este comando já é possível analisar se o projeto compila na íntegra.

### 1.2. *Silo*

Para proceder aos testes, é preciso o servidor *silo* estar a correr. 

Para isso basta ir à diretoria *silo-server* e executar:

```
$ mvn exec:java
```

Este comando vai colocar o *silo* no endereço *localhost* e na porta *8081*.

Para colocar o *silo* numa outra porta, devemos escolher um dado número de réplica, executando assim:

```
$ mvn exec:java -Dinstance=(Número de replica)
```

Como por exemplo:

```
$ mvn exec:java -Dinstance=2
```

Para definir um temporizador especifico para correr o Gossip, executamos assim:


```
$ mvn exec:java -Dtime=(Temporizador)
```

Como por exemplo:

```
$ mvn exec:java -Dtime=50000
```

Se não definirmos o temporizador, associa ao temporizador 30000 (30 segundos).

Para terminarmos um servidor, clicamos em ENTER.

### 1.3. *Eye*

Vamos registar 3 câmeras e as respetivas observações. 

Cada câmera vai ter o seu ficheiro de entrada próprio com observações já definidas.

Para isso basta ir à diretoria *eye* e correr os seguintes comandos:

```
$ ./target/appassembler/bin/eye localhost 2181 Tagus 38.737613 -9.303164 < eye1.txt
$ ./target/appassembler/bin/eye localhost 2181 Lisbon 43.123333 89.432368 2 < eye2.txt
```
**Notas:** Para correr o script *eye* é ter `mvn install` feito e utilizar diretamente os executáveis gerados na diretoria `target/appassembler/bin/`.
O ultimo argumento é o numero de réplica do servidor ao qual o Eye se liga e é opcional. Correndo o primeiro comando do exemplo, o Eye liga-se a um servidor aleatório de entre os encontrados. Correndo o segundo comando do exemplo, o Eye liga-se ao servidor no porto 8082, visto que o número de replica introduzido foi 2. 

Depois de executar os comandos acima já temos o que é necessário para testar o sistema. 

## 2. Teste das Operações

Nesta secção vamos correr os comandos necessários para testar todas as operações. 

Cada subsecção é respetiva a cada operação presente no *silo*.

### 2.1. *cam_join*

Esta operação já foi testada na preparação do ambiente, no entanto ainda é necessário testar algumas restrições.

2.1.1. Teste das câmeras com nome duplicado e coordenadas diferentes.  
O servidor deve rejeitar esta operação. 
Para isso basta executar um *eye* com o seguinte comando:

```
$ eye localhost 8080 Tagus 10.0 10.0
```

2.1.2. Teste do tamanho do nome.  
O servidor deve rejeitar esta operação. 
Para isso basta executar um *eye* com o seguinte comando:

```
$ eye localhost 8080 ab 10.0 10.0
$ eye localhost 8080 abcdefghijklmnop 10.0 10.0
```

### 2.2. *cam_info*

Esta operação não tem nenhum comando específico associado e para isso é necessário ver qual o nome do comando associado a esta operação. 
Para isso precisamos instanciar um cliente *spotter*, presente na diretoria com o mesmo nome:

```
$ mvn exec:java
```

De seguida, corremos o comando *help* e, **assumindo** que o comando se chama *info* e recebe um nome, corremos os seguintes testes:

```
> help
```

2.2.1. Teste para uma câmera existente.  
O servidor deve responder com as coordenadas de localização da câmera *Tagus* (38.737613 -9.303164):

```
> info Tagus
```

2.2.2. Teste para câmera inexistente.  
O servidor deve rejeitar esta operação:

```
> info Inexistente
```

### 2.3. *report*

Esta operação já foi testada acima na preparação do ambiente.

No entanto falta testar o sucesso do comando *zzz*. 
Na preparação foi adicionada informação que permite testar este comando.
Para testar basta abrir um cliente *spotter* e correr o comando seguinte:

```
> trail car 00AA00
```

O resultado desta operação deve ser duas observações pela câmera *Tagus* com intervalo de mais ou menos 5 segundos.

### 2.4. *track*

Esta operação vai ser testada utilizando o comando *spot* com um identificador.

2.4.1. Teste com uma pessoa (deve devolver vazio):

```
> spot person 14388236
```

2.4.2. Teste com uma pessoa:

```
> spot person 123456789
person,123456789,<timestamp>,Alameda,30.303164,-10.737613
```

2.4.3. Teste com um carro:

```
> spot car 20SD21
car,20SD21,<timestamp>,Alameda,30.303164,-10.737613
```

### 2.5. *trackMatch*

Esta operação vai ser testada utilizando o comando *spot* com um fragmento de identificador.

2.5.1. Teste com uma pessoa (deve devolver vazio):

```
> spot person 143882*
```

2.5.2. Testes com uma pessoa:

```
> spot person 111*
person,111111000,<timestamp>,Tagus,38.737613,-9.303164

> spot person *000
person,111111000,<timestamp>,Tagus,38.737613,-9.303164

> spot person 111*000
person,111111000,<timestamp>,Tagus,38.737613,-9.303164
```

2.5.3. Testes com duas ou mais pessoas:

```
> spot person 123*
person,123111789,<timestamp>,Alameda,30.303164,-10.737613
person,123222789,<timestamp>,Alameda,30.303164,-10.737613
person,123456789,<timestamp>,Alameda,30.303164,-10.737613

> spot person *789
person,123111789,<timestamp>,Alameda,30.303164,-10.737613
person,123222789,<timestamp>,Alameda,30.303164,-10.737613
person,123456789,<timestamp>,Alameda,30.303164,-10.737613

> spot person 123*789
person,123111789,<timestamp>,Alameda,30.303164,-10.737613
person,123222789,<timestamp>,Alameda,30.303164,-10.737613
person,123456789,<timestamp>,Alameda,30.303164,-10.737613
```

2.5.4. Testes com um carro:

```
> spot car 00A*
car,00AA00,<timestamp>,Tagus,38.737613,-9.303164

> spot car *A00
car,00AA00,<timestamp>,Tagus,38.737613,-9.303164

> spot car 00*00
car,00AA00,<timestamp>,Tagus,38.737613,-9.303164
```

2.5.5. Testes com dois ou mais carros:

```
> spot car 20SD*
car,20SD20,<timestamp>,Alameda,30.303164,-10.737613
car,20SD21,<timestamp>,Alameda,30.303164,-10.737613
car,20SD22,<timestamp>,Alameda,30.303164,-10.737613

> spot car *XY20
car,66XY20,<timestamp>,Lisboa,32.737613,-15.303164
car,67XY20,<timestamp>,Alameda,30.303164,-10.737613
car,68XY20,<timestamp>,Tagus,38.737613,-9.303164

> spot car 19SD*9
car,19SD19,<timestamp>,Lisboa,32.737613,-15.303164
car,19SD29,<timestamp>,Lisboa,32.737613,-15.303164
car,19SD39,<timestamp>,Lisboa,32.737613,-15.303164
car,19SD49,<timestamp>,Lisboa,32.737613,-15.303164
car,19SD59,<timestamp>,Lisboa,32.737613,-15.303164
car,19SD69,<timestamp>,Lisboa,32.737613,-15.303164
car,19SD79,<timestamp>,Lisboa,32.737613,-15.303164
car,19SD89,<timestamp>,Lisboa,32.737613,-15.303164
car,19SD99,<timestamp>,Lisboa,32.737613,-15.303164
```

### 2.6. *trace*

Esta operação vai ser testada utilizando o comando *trail* com um identificador.

2.6.1. Teste com uma pessoa (deve devolver vazio):

```
> trail person 14388236
```

2.6.2. Teste com uma pessoa:

```
> trail person 123456789
person,123456789,<timestamp>,Alameda,30.303164,-10.737613
person,123456789,<timestamp>,Alameda,30.303164,-10.737613
person,123456789,<timestamp>,Tagus,38.737613,-9.303164

```

2.6.3. Teste com um carro (deve devolver vazio):

```
> trail car 12XD34
```

2.6.4. Teste com um carro:

```
> trail car 00AA00
car,00AA00,<timestamp>,Tagus,38.737613,-9.303164
car,00AA00,<timestamp>,Tagus,38.737613,-9.303164
```

## 3. Replicação e tolerância a faltas

Para conseguir correr o projeto, é necessario no inicio correr o Zookeeper, com o comando:

```
$ ./zkServer.sh start
```

A seguir, para corrermos o servidor, ao executarmos o seguinte comando, corremos um servidor no porto 8081:

```
$ mvn exec:java
```

Para lançarmos outras réplicas do servidor, executamos o comando:

```
$ mvn exec:java -Dinstance=(Número de replica)
```

Onde o (Número de replica) é um inteiro de 0 a 9. Neste exemplo, lançamos uma réplica do servidor na porta 8082, com o número de réplica 2:

```
$ mvn exec:java -Dinstance=2
```

Para definir um temporizador especifico para correr o Gossip, executamos assim:


```
$ mvn exec:java -Dtime=(Temporizador)
```

Como por exemplo:

```
$ mvn exec:java -Dtime=50000
```

Se não definirmos o temporizador, associa ao temporizador 30000 (30 segundos).

Para terminarmos um servidor, clicamos em ENTER.

Forncecemos dados aos servidores graças ao Eye, que corremos da seguinte forma:

```
$ ./target/appassembler/bin/eye (zooHost) (zooPort) (nome) (latitude) (longitude) (número de réplica opcional)
```

Por exemplo:

```
$ ./target/appassembler/bin/eye localhost 2181 Tagus 4.53 8.51
```

Caso queiramos ligar o Eye a uma replica do servidor aleatória, ou:

```
$ ./target/appassembler/bin/eye localhost 2181 Tagus 4.53 8.51 3
```

Caso queiramos ligar o Eye a uma replica do servidor específica, ou ainda:

```
$ ./target/appassembler/bin/eye localhost 2181 Tagus 38.737613 -9.303164 < eye1.txt
$ ./target/appassembler/bin/eye localhost 2181 Lisbon 43.123333 89.432368 2 < eye2.txt
```

Caso queiramos fornecer logo os dados através de um ficheiro de texte, quando lançamos um Eye.

A seguir, para fornecermos os dados, fazemos por exemplo:

```
> #Dados1
> person,123
> person,234
> person,5000
> car,56BO23

> #Dados2
> person,6678
> car,98HU23

```

Onde os comandos começados por '#' são comentários, person,(personId) para fazer report de uma pessoa, car,(carId) para fazer report de um carro.

As observações são todas lançadas assim que damos '\n' mais uma vez.

Fazemos interrogações graças ao Spotter, que lançamos da seguinte forma:

```
$ ./target/appassembler/bin/spotter (zooHost) (zooPort) (número de replica opciona)
```

Por exemplo:

```
$ ./target/appassembler/bin/spotter localhost 2181
```

Para ligarmos a uma réplica de servidor aleatória, ou:

```
$ ./target/appassembler/bin/spotter localhost 2181 4
```

Para ligarmos a uma réplica em específico, ou ainda:

```
$ ./target/appassembler/bin/spotter localhost 2181 4 < spotter1.txt
```

Para fazermos as interrogações presentes num ficheiro de texto.

Fazemos as interrogações da seguinte forma.

```
> spot person 123
person,123,<timestamp>,Tagus,36.213,80.43
> spot car 12BA*
car,12BA32,<timestamp>,Tagus,36.213,80.43
car,12BA65,<timestamp>,Lisbon,44.12,4.49
> spot car *
car,12BA32,<timestamp>,Tagus,36.213,80.43
car,12BA65,<timestamp>,Lisbon,44.12,4.49
car,33OD56,<timestamp>,Porto,21.32,54.63
> move to 2
> trail person 4534
person,4534,<timestamp>,Tagus,36.213,80.43
person,4534,<timestamp>,Lisbon,44.12,4.49
```

O comando spot consulta uma pessoa ou carro através do seu id, ou parte dele (se usarmos '*' e mostra todas as pessoas ou carros começadas ou acabadas por parte do id, dependendo de '*' estar no final ou no inicio do id, ou todas as pessoas ou carros, se o id inserido for apenas '*'.

O comando trail mostra todas as observações feitas a uma dada pessoa ou carro.

O comando move to transporta o spotter até uma nova réplica do servidor.

## 4. Considerações Finais

Apesar de não serem avaliados os comandos de controlo, o comando *help* deve ser minimamente informativo e deve indicar todas as operações existentes no *spotter*.
Estes testes não cobrem tudo, pelo que devem ter sempre em conta os testes de integração e o código.
