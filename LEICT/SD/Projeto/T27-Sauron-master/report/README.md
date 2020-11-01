# Relatório do projeto Sauron

Sistemas Distribuídos 2019-2020, segundo semestre


## Autores

**Grupo T27**



| Número | Nome              | Utilizador                             | Correio eletrónico                                   |
| -------|-------------------|----------------------------------------| -----------------------------------------------------|
| 84757  | Rafael Cabral     | <https://github.com/raffiti1997>       | <mailto:rafael.castro.cabral@tecnico.ulisboa.pt>     |
| 90764  | Pedro Leitão      | <https://github.com/pedroleitao1999>   | <mailto:pedro.o.leitao@gmail.com>                    | 
| 90782  | Tomás Gomes       | <https://github.com/4ThoseAbout2Rock>  | <mailto:tomas.gomes@tecnico.ulisboa.pt>              |

*(usar imagens com 150px de altura; e depois apagar esta linha)*  
![Rafael](rafael.png) ![Pedro](pedro.png) ![Tomás](tomas.png)


## Melhorias da primeira parte

_(que correções ou melhorias foram feitas ao código da primeira parte -- incluir link para commits no GitHub onde a alteração foi feita)_

- [Correção dos argumentos da linha de comandos do Eye](https://github.com/tecnico-distsys/T27-Sauron/commit/e81a5db2cd38e2786f0b7d6da02dc191e3a2e92c)
- [Correção dos argumentos da linha de comandos do Spotter](https://github.com/tecnico-distsys/T27-Sauron/commit/e81a5db2cd38e2786f0b7d6da02dc191e3a2e92c)


## Modelo de faltas

_(que faltas são toleradas, que faltas não são toleradas)_


## Solução

_(Figura da solução de tolerância a faltas)_

_(Breve explicação da solução, suportada pela figura anterior)_


## Protocolo de replicação

_(Explicação do protocolo)_
- O protocolo Gossip implementado é uma variante do protocolo gossip architecture com coerência fraca.
- Neste protocolo, um dado cliente do tipo Eye precisa apenas de contactar uma determinada réplica para 
fazer uma atualização.
- Este protocolo serve para se conseguir passar informação de uma réplica do servidor para outra, 
trocando mensagens entre réplicas por cada intervalo de tempo determinado,
garantindo que um cliente do tipo Spotter conseguirá consultar todas as
informações pretendidas independentemente da réplica do servidor onde o cliente estiver. 

_(descrição das trocas de mensagens)_
- As mensagens são trocadas via gRPC, através das funções update e response, presentes no objeto Gossip 
que criámos, onde o update envia observações registadas pelo Eye para as outras réplicas, e o receive recebe as
observações das outras replicas, juntando-as à lista de observações já presente na réplica.
- As mensagens contêm o timestamp vetorial de cada replica, sendo então comparado e atualizado se necessário, e 
uma lista de observações. 

## Opções de implementação

_(Descrição de opções de implementação, incluindo otimizações e melhorias introduzidas)_



## Notas finais

_(Algo mais a dizer?)_
