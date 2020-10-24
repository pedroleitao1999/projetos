/*
Pedro de Oliveira Rosa Alves Leitao
90764
LEIC-T
*/



propaga([Terms|_],Pos,PosicoesOrdenadas) :-                    % Mostra o que precisamos de preencher num termometro, dada uma posicao.
  propaga_2(Terms,Pos,Posicoes),
  sort(Posicoes,PosicoesOrdenadas).


propaga_2([Term1|_],Pos,Posicoes) :-                           % Procura o termometro que contem a posicao escolhida.
   member(Pos,Term1),
   propaga_3(Term1,Pos,Posicoes).

propaga_2([Term1|TermsRestantes],Pos,Posicoes) :-
  \+member(Pos,Term1),
  propaga_2(TermsRestantes,Pos,Posicoes).


propaga_3([Pos|_],Pos,[Pos]).                                  % Constroi a lista que contem as posicoes do termometro a preencher.

propaga_3([NaoPos|Resto],Pos,[NaoPos|NovoResto]) :-
  propaga_3(Resto,Pos,NovoResto).



nao_altera_linhas_anteriores([(Linha,_)|Resto],L,Ja_Preenchidas) :-         % Verifica se as novas posicoes escolhidas nao alteram as linhas anteriores a linha que escolhemos.
  Linha>=L,
  nao_altera_linhas_anteriores(Resto,L,Ja_Preenchidas).

nao_altera_linhas_anteriores([(Linha,Coluna)|Resto],L,Ja_Preenchidas) :-
  Linha<L,
  nao_altera_linhas_anteriores_2([(Linha,Coluna)|Resto],L,Ja_Preenchidas).

nao_altera_linhas_anteriores([], _, _).

nao_altera_linhas_anteriores_2([Pos1|Resto],L,Ja_Preenchidas) :-            % Verifica se cada nova posicao pertencente a uma linha anterior a escolhida ja foi anteriormente preenchida.
  member(Pos1,Ja_Preenchidas),
  nao_altera_linhas_anteriores(Resto,L,Ja_Preenchidas).



verifica_parcial([_,_,[LimColuna|LimsResto]],Ja_Preenchidas,Dim,Poss)  :-       % Verifica se, juntamente com as posicoes que escolhemos, os limites das colunas nao foram ultrapassados.
  NumColunas is 0,
  union(Ja_Preenchidas,Poss,Posicoes),
  length([LimColuna|LimsResto],LimitesColunas),
  Coluna is Dim-LimitesColunas+1,
  verifica_parcial_2([_,_,[LimColuna|LimsResto]],Dim,Coluna,Posicoes,Ja_Preenchidas,Poss,NumColunas).

verifica_parcial([_,_,[]],_,_,_).

verifica_parcial_2([_,_,[LimColuna|LimsResto]],Dim,Coluna,[(_,ColunaAtual)|PosResto],Ja_Preenchidas,Poss,NumColunas) :-  % Conta o numero de posicoes diferentes na mesma coluna.
  ColunaAtual=:=Coluna,
  NovoNumColunas is NumColunas+1,
  verifica_parcial_2([_,_,[LimColuna|LimsResto]],Dim,Coluna,PosResto,Ja_Preenchidas,Poss,NovoNumColunas).

verifica_parcial_2([_,_,[LimColuna|LimsResto]],Dim,Coluna,[(_,ColunaAtual)|PosResto],Ja_Preenchidas,Poss,NumColunas) :-
  ColunaAtual=\=Coluna,
  verifica_parcial_2([_,_,[LimColuna|LimsResto]],Dim,Coluna,PosResto,Ja_Preenchidas,Poss,NumColunas).

verifica_parcial_2([_,_,[LimColuna|LimsResto]],Dim,_,[],Ja_Preenchidas,Poss,NumColunas) :-
  NumColunas=<LimColuna,
  verifica_parcial([_,_,LimsResto],Ja_Preenchidas,Dim,Poss).
