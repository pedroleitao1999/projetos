/*
    PROJETO DE SO - EXERCICIO 1

    PEDRO LEITAO - 90764
    TIAGO NEVES - 90778

    TESTE PRATICO - EXERCICIO 1

    PEDRO LEITAO - 90764
*/


#include <sys/types.h>
#include <sys/wait.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

int CHILD = 0;  //Numero de processos filho
int MAXCHILDREN = INT_MAX;  //Limite de processos filho
int NUMBER = 0; //Numero total de filhos lancados



/*----------------------------
    Para verificar se o numero de processos
    filho chegou ao limite
  ----------------------------*/

int limitChildProcesses()
{
  if(CHILD<MAXCHILDREN)
    return 1;
  else
    return 0;
}


/*----------------------------
    Comando run
  ----------------------------*/

int runInputFile()
{

  pid_t pid;
  char inputfile[3000];

  if(limitChildProcesses()==1)
  {


    pid=fork();

    if(pid<0)
      printf("Unable to fork\n"); //Erro
    else
    {
      if(pid==0)
      {
        //Processo filho
        scanf("%s",inputfile);
        execl("/bin/bash","/bin/bash", "callSeqSolver.sh", inputfile, (char *)NULL);
      }
    }
  }

  return pid; //Retorna o ID do processo filho
}


/*----------------------------
    Comando exit
  ----------------------------*/
void exitStatus(int id_pid[])
{

  int state, i;
  pid_t pid;



  for(i=0;i<CHILD;i++)  //Percorre todos os processos filho
  {
    if((pid=waitpid(id_pid[i],&state,WNOHANG))==-1) //A verificar se um determinado processo saiu
    {
      perror("wait() error"); //Erro do waitpid()
    }
    else
    {
      if(pid!=0)
      {
        if(WIFEXITED(state))
          printf("CHILD EXITED (PID=%i; return OK)\n", id_pid[i]);  //Saiu com sucesso
        else
          printf("CHILD EXITED (PID=%i; return NOK)\n", id_pid[i]); //Saiu sem sucesso
      }
    }
  }

  printf("END.\n");
}



/*----------------------------
    Lista de comandos
  ----------------------------*/
void commandList()
{

    char command;
    static int id_pid[3000], id;

    scanf("%s",&command);

    while(strcmp(&command, "exit")!=0)
    {

      if(strcmp(&command,"run")==0)
      {

        id=runInputFile(); //Recebe um ID do processo filho

        if(id>0)    //Verificar se e filho
        {
          id_pid[CHILD]=id;
          CHILD++;
        }

        NUMBER++;
      }

      if(strcmp(&command, "info")==0)
      {
        printf("%d\n",NUMBER);
      }

      scanf("%s",&command);
    }

    if(strcmp(&command,"exit")==0)
    {

      exitStatus(id_pid);
      exit(0);
    }
}


int main(int argc, char** argv)
{

  if(argc==2)
  {
    if(atoi(argv[1])>0) //Verificar se o argumento e maior que zero (MAXCHILDREN)
      MAXCHILDREN=atoi(argv[1]);
    else
      printf("Erro - MAXCHILDREN\n");
  }

  commandList();
}
