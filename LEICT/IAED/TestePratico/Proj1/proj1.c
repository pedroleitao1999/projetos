/* Pedro Leitao
	 90764
	 LeicT
	 Projecto 1 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 1000
#define DIA 10
#define NADA ""

struct evento{
	char descricao[63];        /*Elemento 0 da string*/
	int data;                  /*Elemento 1 da string*/
	int hora;                  /*Elemento 2 da string*/
	int duracao;               /*Elemento 3 da string*/
	int sala;                  /*Elemento 4 da string*/
	char participantes[4][63]; /*Elementos 5, 6, 7 e 8 da string*/
  int dia;
  int mes;
  int ano;
	int inicio;
  int final;
  int ordem;
	int numParticip;
  char stringHora[4];
};

void ordenaEventos(struct evento todosEventos[MAX], int num){
/* Ordena os eventos cronologicamente */

	int i, j, min = 0;
	struct evento aux;

	for(i = 0; i < num; i++)
		todosEventos[i].ordem = todosEventos[i].ano*100000000 + todosEventos[i].mes*1000000\
      + todosEventos[i].dia*10000 + todosEventos[i].hora;

	for(i = 0; i < num - 1; i++){
		min = i;
		for(j = i+1; j < num; j++){
			if(todosEventos[j].ordem < todosEventos[min].ordem)
				min = j;
			if(todosEventos[j].ordem == todosEventos[min].ordem && todosEventos[j].sala < todosEventos[min].sala)
				min = j;
		}
		aux = todosEventos[i];
		todosEventos[i] = todosEventos[min];
		todosEventos[min] = aux;
	}

}

void printEvento(struct evento ev){
/* Recebe um dado evento e imprime o respetivo output */

	int p;

  ev.stringHora[4] = '\0';
  if(ev.dia < DIA)
    printf("%s 0%d %s %d Sala%d %s\n", ev.descricao, ev.data, ev.stringHora, ev.duracao, ev.sala, ev.participantes[0]);
  else
    printf("%s %d %s %d Sala%d %s\n", ev.descricao, ev.data, ev.stringHora,ev.duracao, ev.sala, ev.participantes[0]);

  printf("* ");
	for(p = 1; p < ev.numParticip; p++){
		printf("%s", ev.participantes[p]);
		if(p < ev.numParticip - 1)
			printf(" ");
	}
	printf("\n");

}

int addEvento(char comando, char info[9][63], struct evento todosEventos[MAX], int num, int i){

  int m, p;
	int numPart = 0, todosPart = 0, numOrden = 0;
	int t;
  int dataIgual = 0, coincide = 0, salaOcupada = 0, participanteOcupado = 0;
  char participantesOcup[4][63];
	char participantesOrden[4][63];
	int horaNovo = 60*(atoi(info[2])/100) + atoi(info[2])%100;

  for(m = 0; m < num; m++){
    if(atoi(info[1]) == todosEventos[m].data)
      dataIgual = 1;
    if(dataIgual == 1){
      if(horaNovo + atoi(info[3]) <= todosEventos[m].final && horaNovo + atoi(info[3]) > todosEventos[m].inicio)
        coincide = 1;
      if(horaNovo < todosEventos[m].final && horaNovo >= todosEventos[m].inicio)
        coincide = 1;
			if(horaNovo <= todosEventos[m].inicio && horaNovo + atoi(info[3]) >= todosEventos[m].final)
	       coincide = 1;
    }
    if(coincide == 1)
      if(atoi(info[4]) == todosEventos[m].sala){
        salaOcupada = 1;
        break;
      }
    if(coincide == 1 && salaOcupada == 0)
      for(p = 5; p < i; p++)
        for(t = 0; t < todosEventos[m].numParticip; t++)
          if(strcmp(info[p], todosEventos[m].participantes[t]) == 0){
            strcpy(participantesOcup[numPart], info[p]);
            participanteOcupado = 1;
						numPart++;
          }
    dataIgual = 0;
    coincide = 0;
  }

  if(salaOcupada == 1)
    printf("Impossivel agendar evento %s. Sala%d ocupada.\n", info[0], atoi(info[4]));

	if(salaOcupada == 0 && participanteOcupado == 1)
		for(p = 5; p < i; p++)
			for(m = 0; m < 4; m++)
				if(strcmp(info[p], participantesOcup[m]) == 0){
					strcpy(participantesOrden[numOrden], participantesOcup[m]);
					numOrden++;
					break;
				}

  if(salaOcupada == 0 && participanteOcupado == 1 && comando == 'A')
		for(p = 0; p < numOrden; p++)
			printf("Impossivel adicionar participante. Participante %s tem um evento sobreposto.\n", participantesOrden[p]);

	if(salaOcupada == 0 && participanteOcupado == 1 && comando != 'A')
		for(p = 0; p < numOrden; p++)
			printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", info[0], participantesOrden[p]);

  if(salaOcupada == 0 && participanteOcupado == 0){
    strcpy(todosEventos[num].descricao, info[0]);
    todosEventos[num].data = atoi(info[1]);
    todosEventos[num].hora = atoi(info[2]);
    todosEventos[num].duracao = atoi(info[3]);
    todosEventos[num].sala = atoi(info[4]);
    for(p = 5; p < i; p++){
			strcpy(todosEventos[num].participantes[p-5], info[p]);
			todosPart++;
		}
    todosEventos[num].dia = atoi(info[1])/1000000;
    todosEventos[num].mes = (atoi(info[1])/10000)%100;
    todosEventos[num].ano = atoi(info[1])%10000;
		todosEventos[num].inicio = 60*(atoi(info[2])/100) + atoi(info[2])%100;
    todosEventos[num].final = 60*(atoi(info[2])/100) + atoi(info[2])%100 + atoi(info[3]);
		todosEventos[num].numParticip = todosPart;
    strcpy(todosEventos[num].stringHora, info[2]);
    num++;
  }

  return num;

}

void listaTodosEventos(struct evento todosEventos[MAX], int num){

	int i;

	/* Ordena os eventos cronologicamente antes de mostrar cada um no output */
	ordenaEventos(todosEventos, num);
	for(i = 0; i < num; i++)
		printEvento(todosEventos[i]);

}

void listaEventosSala(char sala[1][63], struct evento todosEventos[MAX], int num){

	int i;
	int salaInt = atoi(sala[0]);

	/* Ordena todos os eventos antes de mostrar os eventos numa dada sala */
	ordenaEventos(todosEventos, num);
	for(i = 0; i < num; i++)
		if(todosEventos[i].sala == salaInt)
			printEvento(todosEventos[i]);

}

int removeEvento(char descricao[63], struct evento todosEventos[MAX], int num){

	int i, j;
	int existe = 0;

	for(i = 0; i < num; i++){
		if(strcmp(todosEventos[i].descricao, descricao) == 0){
			existe = 1;
			for(j = i; j < num - 1; j++)
				todosEventos[j] = todosEventos[j + 1];
		}
	}

	if(existe == 1)
		num--;

	if(existe == 0)
		printf("Evento %s inexistente.\n", descricao);

	return num;

}

/* As funcoes que alteram algum atributo de um evento comecam por encontrar
um evento com a descricao indicada e se encontrarem removem o mesmo
adicionando o evento com o atributo alterado e se nao for possivel
adicionam o evento antigo */

void alteraHora(char comando, char info[2][63], struct evento todosEventos[MAX], int num, int i){

	int m, existe = 0;
	int numNovo, numApag;
	char novo_inicio[4];
	struct evento evAntigo;
	char evNovo[9][63];

	strcpy(novo_inicio, info[1]);

	for(m = 0; m < num; m++){
		if(strcmp(todosEventos[m].descricao, info[0]) == 0){
			existe = 1;
			evAntigo = todosEventos[m];
			numApag = removeEvento(info[0], todosEventos, num);
			break;
		}
	}

	if(existe == 0)
		printf("Evento %s inexistente.\n", info[0]);

	if(existe == 1){
		strcpy(evNovo[0], info[0]);
		sprintf(evNovo[1], "%d", evAntigo.data);
		strcpy(evNovo[2], novo_inicio);
		sprintf(evNovo[3], "%d", evAntigo.duracao);
		sprintf(evNovo[4], "%d", evAntigo.sala);
		for(i = 5; i < evAntigo.numParticip + 5; i++)
			strcpy(evNovo[i], evAntigo.participantes[i-5]);
		numNovo = addEvento(comando, evNovo, todosEventos, numApag, i);
		if(numNovo == numApag){
			strcpy(evNovo[2], evAntigo.stringHora);
			addEvento(comando, evNovo, todosEventos, numNovo, i);
		}
	}

}

void alteraDuracao(char comando, char info[2][63], struct evento todosEventos[MAX], int num, int i){

	int m, existe = 0;
	int numNovo, numApag;
	int nova_duracao = atoi(info[1]);
	struct evento evAntigo;
	char evNovo[9][63];

	for(m = 0; m < num; m++)
		if(strcmp(todosEventos[m].descricao, info[0]) == 0){
			existe = 1;
			evAntigo = todosEventos[m];
			numApag = removeEvento(info[0], todosEventos, num);
			break;
		}

	if(existe == 0)
		printf("Evento %s inexistente.\n", info[0]);

	if(existe == 1){
		strcpy(evNovo[0], info[0]);
		sprintf(evNovo[1], "%d", evAntigo.data);
		strcpy(evNovo[2], evAntigo.stringHora);
		sprintf(evNovo[3], "%d", nova_duracao);
		sprintf(evNovo[4], "%d", evAntigo.sala);
		for(i = 5; i < evAntigo.numParticip + 5; i++)
			strcpy(evNovo[i], evAntigo.participantes[i-5]);
		numNovo = addEvento(comando, evNovo, todosEventos, numApag, i);
		if(numNovo == numApag){
			sprintf(evNovo[3], "%d", evAntigo.duracao);
			addEvento(comando, evNovo, todosEventos, numNovo, i);
		}
	}

}

void mudaSala(char comando, char info[2][63], struct evento todosEventos[MAX], int num, int i){

	int m, existe = 0;
	int numNovo, numApag;
	int nova_sala = atoi(info[1]);
	struct evento evAntigo;
	char evNovo[9][63];

	for(m = 0; m < num; m++)
		if(strcmp(todosEventos[m].descricao, info[0]) == 0){
			existe = 1;
			evAntigo = todosEventos[m];
			numApag = removeEvento(info[0], todosEventos, num);
			break;
		}

	if(existe == 0)
		printf("Evento %s inexistente.\n", info[0]);

	if(existe == 1){
		strcpy(evNovo[0], info[0]);
		sprintf(evNovo[1], "%d", evAntigo.data);
		strcpy(evNovo[2], evAntigo.stringHora);
		sprintf(evNovo[3], "%d", evAntigo.duracao);
		sprintf(evNovo[4], "%d", nova_sala);
		for(i = 5; i < evAntigo.numParticip + 5; i++)
			strcpy(evNovo[i], evAntigo.participantes[i-5]);
		numNovo = addEvento(comando, evNovo, todosEventos, numApag, i);
		if(numNovo == numApag){
			sprintf(evNovo[4], "%d", evAntigo.sala);
			addEvento(comando, evNovo, todosEventos, numNovo, i);
		}
	}

}

void addParticipante(char comando, char info[2][63], struct evento todosEventos[MAX], int num, int i){

	int m, p;
	int existe = 0, partExiste = 0, maxPart = 0;
	int numNovo = 0, numApag = 0;
	char participante[63];
	struct evento evAntigo;
	char evNovo[9][63];

	strcpy(participante, info[1]);

	for(m = 0; m < num; m++)
		if(strcmp(todosEventos[m].descricao, info[0]) == 0){
			existe = 1;
			evAntigo = todosEventos[m];
			numApag = removeEvento(info[0], todosEventos, num);
			break;
		}


	if(existe == 0){
		printf("Evento %s inexistente.\n", info[0]);
	}

	else{
		if(evAntigo.numParticip >= 4){
			maxPart = 1;
			printf("Impossivel adicionar participante. Evento %s ja tem 3 participantes.\n", info[0]);
		}
		if(maxPart == 0)
			for(p = 0; p < evAntigo.numParticip; p++)
				if(strcmp(evAntigo.participantes[p], participante) == 0)
					partExiste = 1;
	}

	if(existe == 1 && maxPart == 0){
		strcpy(evNovo[0], info[0]);
		sprintf(evNovo[1], "%d", evAntigo.data);
		strcpy(evNovo[2], evAntigo.stringHora);
		sprintf(evNovo[3], "%d", evAntigo.duracao);
		sprintf(evNovo[4], "%d", evAntigo.sala);
		for(i = 5; i < evAntigo.numParticip + 5; i++)
			strcpy(evNovo[i], evAntigo.participantes[i-5]);
		if(partExiste == 0){
			strcpy(evNovo[evAntigo.numParticip + 5], participante);
			evAntigo.numParticip++;
			i++;
		}
		numNovo = addEvento(comando, evNovo, todosEventos, numApag, i);
		if(numNovo == numApag){
			evAntigo.numParticip--;
			i--;
			strcpy(evNovo[evAntigo.numParticip + 5], NADA);
			addEvento(comando, evNovo, todosEventos, numNovo, i);
		}
	}

}

void removeParticipante(char comando, char info[2][63], struct evento todosEventos[MAX], int num, int i){

	int m, p;
	int existe = 0, partExiste = 0, minPart = 0;
	int numNovo, numApag;
	char participante[63];
	struct evento evAntigo;
	char evNovo[9][63];
	int partNumero = 0;
	char partEvento[4][63];

	strcpy(participante, info[1]);

	for(m = 0; m < num; m++)
		if(strcmp(todosEventos[m].descricao, info[0]) == 0){
			existe = 1;
			evAntigo = todosEventos[m];
			numApag = removeEvento(info[0], todosEventos, num);
			break;
		}

	if(existe == 0){
		printf("Evento %s inexistente.\n", info[0]);
	}

	else{
		for(p = 0; p < evAntigo.numParticip; p++){
			strcpy(partEvento[p], evAntigo.participantes[p]);
			if(strcmp(evAntigo.participantes[p], participante) == 0){
				partExiste = 1;
				partNumero = p;
			}
		}
		if(evAntigo.numParticip <= 2 && partExiste == 1){
			minPart = 1;
			printf("Impossivel remover participante. Participante %s e o unico participante no evento %s.\n", participante, info[0]);
		}
	}

	if(partExiste == 0)
		todosEventos[num] = evAntigo;

	if(existe == 1){
		strcpy(evNovo[0], info[0]);
		sprintf(evNovo[1], "%d", evAntigo.data);
		strcpy(evNovo[2], evAntigo.stringHora);
		sprintf(evNovo[3], "%d", evAntigo.duracao);
		sprintf(evNovo[4], "%d", evAntigo.sala);
		for(i = 5; i < evAntigo.numParticip + 5; i++)
			strcpy(evNovo[i], evAntigo.participantes[i-5]);
		if(partExiste == 1 && minPart == 0){
			for(p = partNumero; p < evAntigo.numParticip; p++)
				strcpy(evNovo[p+5], evNovo[p+6]);
			strcpy(evNovo[evAntigo.numParticip+5], NADA);
			evAntigo.numParticip--;
			i--;
		}
		numNovo = addEvento(comando, evNovo, todosEventos, numApag, i);
		if(numNovo == numApag){
			evAntigo.numParticip++;
			i++;
			for(p = 0; p < evAntigo.numParticip; p++)
				strcpy(evNovo[p+5], partEvento[p]);
			addEvento(comando, evNovo, todosEventos, numNovo, i);
		}
	}

}

void comandoZ(struct evento todosEventos[MAX], int num){
	int i, total = 0;
	for(i = 0; i < num; i++){
		if(todosEventos[i].duracao < 60){
			total++;
		}
	}
	printf("%d\n", total);
}

void listaComandos(char comando[3], struct evento todosEventos[MAX], int num){
/* Analisa os elementos introduzidos no input */

	char instrucao[MAX];
	int tamanho;
	const char s[2] = ":";
	char *token;
	char info[10][63];
	int i = 0, numNovo;
	char d[3];

	if(comando[0] != 'l' && comando[0] != 'x' && comando[0] != 'Z'){
		fgets(instrucao, MAX, stdin);
		tamanho = strlen(instrucao);
		instrucao[--tamanho] = 0;
		if(comando[0] != 'r'){
			token = strtok(instrucao, s);
			while(token != NULL) {
				strcpy(info[i], token);
				token = strtok(NULL, s);
				i++;
			}
		}
	}

	switch(comando[0]){

		case 'a':
			numNovo = addEvento(comando[0], info, todosEventos, num, i);
			fgets(d, 3, stdin);
			listaComandos(d, todosEventos, numNovo);
			break;

		case 'l':
			listaTodosEventos(todosEventos, num);
			fgets(d, 3, stdin);
			listaComandos(d, todosEventos, num);
			break;

		case 's':
			listaEventosSala(info, todosEventos, num);
			fgets(d, 3, stdin);
			listaComandos(d, todosEventos, num);
			break;

		case 'r':
			numNovo = removeEvento(instrucao, todosEventos, num);
			fgets(d, 3, stdin);
			listaComandos(d, todosEventos, numNovo);
			break;

		case 'i':
			alteraHora(comando[0], info, todosEventos, num, i);
			fgets(d, 3, stdin);
			listaComandos(d, todosEventos, num);
			break;

		case 't':
			alteraDuracao(comando[0], info, todosEventos, num, i);
			fgets(d, 3, stdin);
			listaComandos(d, todosEventos, num);
			break;

		case 'm':
			mudaSala(comando[0], info, todosEventos, num, i);
			fgets(d, 3, stdin);
			listaComandos(d, todosEventos, num);
			break;

		case 'A':
			addParticipante(comando[0], info, todosEventos, num, i);
			fgets(d, 3, stdin);
			listaComandos(d, todosEventos, num);
			break;

		case 'R':
			removeParticipante(comando[0], info, todosEventos, num, i);
			fgets(d, 3, stdin);
			listaComandos(d, todosEventos, num);
			break;

		case 'Z':
			comandoZ(todosEventos, num);
			fgets(d, 3, stdin);
			listaComandos(d, todosEventos, num);
			break;

		case 'x':
			exit(0);

	}

}


int main(){

	char c[3];
	struct evento todosEventos[MAX];
	int numerotodosEventos = 0;

	fgets(c, 3, stdin);
	listaComandos(c, todosEventos, numerotodosEventos);

	return 0;

}
