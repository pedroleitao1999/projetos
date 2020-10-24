struct contacto{
  char*nome;
  char*email;
  char*telefone;
  char*dominio;
  unsigned long int idNumber;
};

typedef struct contacto* Item;

typedef struct node{
  Item item;
  struct node*next;
}*link;

static link *heads;
static int M;
typedef unsigned long Key;
#define key(a) (a->idNumber)
#define MAX 1605

char *strdup(const char *src);
int hash(Key value, int m);
void STinit(int m);
void STinsert(Item item);
Item STsearch(Key v);
void freeItem(Item a);
void printItem(Item a);
void leDominio(Item item, char*email);
Item newItem(char*nome, char*email, char*telefone, int idNumber);
int verificacao(char*nome, int num);
void contaOcorrencias(char*dominio, int num);
void listaComandos(char instrucao[MAX], int num);
