#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proj2.h"

char *strdup(const char *src) {
  /* Funcao que duplica uma string, retornando-a. */
  char *dst = malloc(strlen (src) + 1);
  if (dst == NULL) return NULL;
  strcpy(dst, src);
  return dst;
}

int hash(Key value, int m){
  /* Funcao que retorna a posicao de um dado elemento na Hash Table. */
  return value%m;
}

void STinit(int m){
  /* Cria uma Hash Table. */
  int i;
  M = m;
  heads = malloc(M*sizeof(link));
  for(i = 0; i < M; i++)
    heads[i] = NULL;
}

void STinsert(Item item){
  /* Adiciona um elemento na Hash Table. */
  int i = hash(key(item), M);
  link point = malloc(sizeof(struct node));
  if(heads[i]!=NULL){
    for(point = heads[i]; point != NULL; point = point->next)
      if(point->next == NULL){
        point->next = malloc(sizeof(struct node));
        point->next->item = item;
        break;
      }
  }
  else{
    point->item = item;
    heads[i] = point;
  }
}

Item STsearch(Key v) {
  /* Procura um elemento na Hash Table, retornando o contacto respetivo. */
  int i = hash(v, M);
  link point = malloc(sizeof(struct node));
  Item this = (Item)malloc(sizeof(struct contacto));
  if(heads[i] != NULL){
    for(point = heads[i]; point != NULL; point = point->next){
      if(point->item != NULL && point->item->idNumber == v){
        this = point->item;
        break;
      }
    }
  }
  return this;
}

void freeItem(Item a){
  /* Liberta um Item da Hash Table. */
  free(a->nome);
  free(a->email);
  free(a->telefone);
  free(a->dominio);
  free(a);
}

void printItem(Item a){
  /* Apresenta um dado contacto no output. */
  if(a->conjugue != NULL){
    printf("%s %s %s %s\n", a->nome, a->email, a->telefone, a->conjugue);
  }
  else{
    printf("%s %s %s\n", a->nome, a->email, a->telefone);
  }
}

void leDominio(Item item, char*email){
  /* A partir do email de um contacto, esta funcao le o seu dominio,
  adicionando-o a estrutura. */
  int tamanho;
  const char s[2] = "@";
	char *token;
  char todo[511];
  int i = 0;
  strcpy(todo, email);
  tamanho = strlen(todo);
  todo[--tamanho] = 0;
  token = strtok(todo, s);
  while(token != NULL) {
    if(i == 1)
      item->dominio = strdup(token);
    token = strtok(NULL, s);
    i++;
  }
}

Item newItem(char*nome, char*email, char*telefone, int idNumber){
  /* Cria um novo contacto. */
  Item new = (Item)malloc(sizeof(struct contacto));
  new->idNumber = idNumber;
  new->nome = strdup(nome);
  new->email = strdup(email);
  new->telefone = strdup(telefone);
  leDominio(new, new->email);
  return new;
}

int verificacao(char*nome, int num){
  /* Verifica se ja existe um contacto na Hash Table,
  retornando a sua veracidade. */
  int n, existe = 0;
  Item itemNovo = (Item)malloc(sizeof(struct contacto));
  for(n = 0; n < num; n++){
    itemNovo = STsearch(n);
    if(itemNovo->nome != NULL && strcmp(itemNovo->nome, nome) == 0){
      existe = 1;
      break;
    }
  }
  return existe;
}

void contaOcorrencias(char*dominio, int num){
  /* Recebe um dominio e conta o numero de contactos que o contem. */
  int n, total = 0;
  char dom[511];
  Item itemNovo;
  for(n = 0; n < num; n++){
    itemNovo = STsearch(n);
    if(itemNovo->nome != NULL){
      strcpy(dom, dominio);
      dom[strlen(dom)-1] = '\0';
      if(strcmp(dom, itemNovo->dominio) == 0)
        total++;
    }
  }
  printf("%s:%d\n", dominio, total);
}

void comandoP(char*pref, int num){
  int n, l, total = 0;
  Item itemNovo;
  for(n = 0; n < num; n++){
    itemNovo = STsearch(n);
    if(itemNovo->nome != NULL){
      for(l = 0; l < strlen(itemNovo->telefone); l++){
        if(pref[l] != itemNovo->telefone[l] && pref[l] != '\0'){
          break;
        }
        if(pref[l-1] == itemNovo->telefone[l-1] && pref[l] == '\0'){
          total++;
        }
      }
    }
  }
  printf("%d\n", total);
}

void comandoC(char*n1, char*n2, int num){
  int n, l, n1Existe = 0, n2Existe = 0, head1, head2;
  Item itemNovo, itemConjugue;
  for(n = 0; n < num; n++){
    itemNovo = STsearch(n);
    if(itemNovo->nome != NULL){
      if(strcmp(itemNovo->nome, n1) == 0){
        n1Existe = 1;
        head1 = n;
        break;
      }
    }
  }
  for(l = 0; l < num; l++){
    itemConjugue = STsearch(l);
    if(itemConjugue->nome != NULL){
      if(strcmp(itemConjugue->nome, n2) == 0){
        n2Existe = 1;
        head2 = l;
        break;
      }
    }
  }
  if(n1Existe == 1 && n2Existe == 1){
    heads[head1]->item->conjugue = strdup(n2);
    heads[head2]->item->conjugue = strdup(n1);
  }
}

void listaComandos(char instrucao[MAX], int num){

	int tamanho;
  const char s[2] = " ";
	char *token;
	char info[3][1023];
  int i = 0, n;
	char d[MAX];
  Item itemNovo;

  if(instrucao[0] != 'l' && instrucao[0] != 'x'){
    tamanho = strlen(instrucao);
    instrucao[--tamanho] = 0;
    token = strtok(instrucao, s);
    while(token != NULL) {
      strcpy(info[i], token);
      token = strtok(NULL, s);
      i++;
    }
  }

  switch(instrucao[0]){

		case 'a':
      if(verificacao(info[1], num) == 0){
  			itemNovo = newItem(info[1], info[2], info[3], num);
        STinsert(itemNovo);
        num++;
      }
      else{
        printf("Nome existente.\n");
      }
			fgets(d, MAX, stdin);
			listaComandos(d, num);
			break;

    case 'l':
  		for(n = 0; n < num; n++){
        itemNovo = STsearch(n);
        if(itemNovo->nome != NULL)
          printItem(itemNovo);
      }
  		fgets(d, MAX, stdin);
  		listaComandos(d, num);
  		break;

    case 'p':
      if(verificacao(info[1], num) == 1){
        for(n = 0; n < num; n++){
          itemNovo = STsearch(n);
          if(itemNovo->nome != NULL && strcmp(itemNovo->nome, info[1]) == 0){
            printItem(itemNovo);
            break;
          }
        }
      }
      else{
        printf("Nome inexistente.\n");
      }
      fgets(d, MAX, stdin);
      listaComandos(d, num);
      break;

    case 'r':
      if(verificacao(info[1], num) == 1){
        for(n = 0; n < num; n++){
          itemNovo = STsearch(n);
          if(itemNovo->nome != NULL && strcmp(itemNovo->nome, info[1]) == 0){
            freeItem(itemNovo);
            itemNovo->nome = NULL;
            break;
          }
        }
      }
      else{
        printf("Nome inexistente.\n");
      }
      fgets(d, MAX, stdin);
      listaComandos(d, num);
      break;

    case 'e':
      if(verificacao(info[1], num) == 1){
        for(n = 0; n < num; n++){
          itemNovo = STsearch(n);
          if(itemNovo->nome != NULL && strcmp(itemNovo->nome, info[1]) == 0){
            itemNovo->email = strdup(info[2]);
            leDominio(itemNovo, itemNovo->email);
            break;
          }
        }
      }
      else{
        printf("Nome inexistente.\n");
      }
      fgets(d, MAX, stdin);
      listaComandos(d, num);
      break;

    case 'c':
      contaOcorrencias(info[1], num);
      fgets(d, MAX, stdin);
      listaComandos(d, num);
      break;

    case 'P':
      comandoP(info[1], num);
      fgets(d, MAX, stdin);
      listaComandos(d, num);
      break;

    case 'C':
      comandoC(info[1], info[2], num);
      fgets(d, MAX, stdin);
      listaComandos(d, num);
      break;

    case 'x':
      exit(0);

	}

}

int main(){
  char instrucao[MAX];
  int numContactos = 0;
  STinit(53);
  fgets(instrucao, MAX, stdin);
  listaComandos(instrucao, numContactos);
  return 0;
}
