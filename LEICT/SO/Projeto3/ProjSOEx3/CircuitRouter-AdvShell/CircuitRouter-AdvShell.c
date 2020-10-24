/*
//  PROJETO DE SO - EXERCICIO 3
//
//  PEDRO LEITAO - 90764
//  TIAGO NEVES - 90778
*/

/*
* =============================================================================
*
* CircuitRouter-AdvShell.c
*
* =============================================================================
*/

#include "lib/commandlinereader.h"
#include "lib/vector.h"
#include "CircuitRouter-AdvShell.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>

#define COMMAND_EXIT "exit"
#define COMMAND_RUN "run"

#define MAXARGS 3
#define BUFFER_SIZE 100

int wasSignal = 0;

// Funcao para ler os argumentos escritos pelo stdin e pelo pipe
int readLineArguments1(char **argVector, int vectorSize, char *buffer, int bufferSize)
{

  int numTokens = 0;
  char *s = " \r\n\t";

  int i;

  char *token;
  if (argVector == NULL || buffer == NULL || vectorSize <= 0 || bufferSize <= 0)
    return 0;

  /* get the first token */
  token = strtok(buffer, s);
  /* walk through other tokens */
  while( numTokens < vectorSize-1 && token != NULL )
  {
    argVector[numTokens] = token;
    numTokens ++;

    token = strtok(NULL, s);
  }

  for (i = numTokens; i<vectorSize; i++)
    argVector[i] = NULL;

  return numTokens;

}

// Funcao que espera pela terminacao do processo filho
void waitForChild(vector_t *children, int v[][2], int runningChildren)
{

  while (1)
  {
    child_t *child = malloc(sizeof(child_t));
    if (child == NULL) {
      perror("Error allocating memory");
      exit(EXIT_FAILURE);
    }
    child->pid = wait(&(child->status));
    if (child->pid < 0) {
      if (errno == EINTR) {
      /* Este codigo de erro significa que chegou signal que interrompeu a
          espera pela terminacao de filho; logo voltamos a esperar */
          free(child);
          continue;
      }
      else
      {
        perror("Unexpected error while waiting for child.");
        exit (EXIT_FAILURE);
      }
    }
    int exit;

    exit = time(NULL); //Acaba a contar o tempo

    for(int i=0; i<runningChildren;i++)
    {
      if(v[i][0]==child->pid)
      {
        child->time = exit - v[i][1];
        break;
      }
    }

    vector_pushBack(children, child);
    return;
  }

}

// Imprime no terminal o id, o modo como terminou e o tempo gasto do processo
void printChildren(vector_t *children)
{

  for(int i = 0; i < vector_getSize(children); ++i)
  {
    child_t *child = vector_at(children, i);
    int status = child->status;
    pid_t pid = child->pid;
    int time = child->time;
    if (pid != -1) {
      const char* ret = "NOK";
      if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        ret = "OK";
      printf("CHILD EXITED: (PID=%d; return %s; %d s)\n", pid, ret, time);
    }
  }

  puts("END.");

}

void handler(int sig)
{

  int *_wasSignal;

  _wasSignal=&wasSignal;	//altera a variavel global
  *_wasSignal=1;

}

int main (int argc, char** argv)
{

  char *args[MAXARGS + 1];
  int MAXCHILDREN = -1;
  vector_t *children;
  int runningChildren = 0;
  int start;
  char *pipename = "/tmp/AdvShell.pipe";
  int rv;
  int wasStdin;

  if(argv[1] != NULL)
    MAXCHILDREN = atoi(argv[1]);

  int v[MAXCHILDREN][2];	//onde ira a ser armazenado os ids e tempos

  children = vector_alloc(MAXCHILDREN);

  unlink(pipename);

  if(mkfifo(pipename,0644) == -1)
  {
      perror("Erro no mkfifo");
      exit(EXIT_FAILURE);
  }

  int p = open(pipename,O_RDWR);

  if(p==-1)
  {
      perror("Erro ao abrir o pipe");
      exit(EXIT_FAILURE);
  }

  fd_set read_fds; //descritor de ficheiro

  FD_ZERO(&read_fds);

  char buffer[BUFFER_SIZE];

  printf("Welcome to CircuitRouter-AdvShell\n\n");

  while (1)
  {
      wasStdin = 0;

      FD_SET(fileno(stdin),&read_fds);
      FD_SET(p,&read_fds);

      while((rv = select(p+1,&read_fds,NULL,NULL,NULL)) == -1 && errno == EINTR)
        continue;

      if(rv==-1)
      {
        perror("Select failed");
        exit(EXIT_FAILURE);
      }

      if(FD_ISSET(fileno(stdin),&read_fds))
      {

        if(fgets(buffer,BUFFER_SIZE,stdin)==NULL)
        {
          perror("Erro no fgets");
          exit(EXIT_FAILURE);
        }
        else
        {
          wasStdin = 1;  //Recebeu atraves da stdin 
        }
      }

      if(FD_ISSET(p,&read_fds))
      {
        rv = read(p,buffer,BUFFER_SIZE);
        if(rv == -1)
        {
          perror("Read failed");
          exit(EXIT_FAILURE);
        }
      }

      int numArgs;

      numArgs = readLineArguments1(args, MAXARGS+1, buffer, BUFFER_SIZE);


      /* EOF (end of file) do stdin ou comando "sair" */
      if (numArgs < 0 || (numArgs > 0 && (strcmp(args[0], COMMAND_EXIT) == 0))) {
        printf("CircuitRouter-AdvShell will exit.\n--\n");

        /* Espera pela terminacao de cada filho */
        while (runningChildren > 0) {
          waitForChild(children, v, runningChildren);
          runningChildren --;
        }

        printChildren(children);
        printf("--\nCircuitRouter-AdvShell ended.\n");

        if(wasStdin != 1) //Verifica se nao foi a stdin
        {
          unlink(args[1]);

          if(mkfifo(args[1],0644) == -1)
          {
            perror("Erro no mkfifo");
            exit(EXIT_FAILURE);
          }

          int p2 = open(args[1],O_WRONLY);

            if(p2==-1)
            {
              perror("Erro ao abrir o pipe");
              exit(EXIT_FAILURE);
            }

            if (write (p2, "Command not supported",sizeof("Command not supported")) == -1)
            {
              perror("Erro ao escrever no pipe");
              exit(EXIT_FAILURE);
            }

            close(p2);
            unlink(args[2]);

            wasStdin = 0;
        }

        break;
      }

      else if (numArgs > 0 && strcmp(args[0], COMMAND_RUN) == 0)
      {
        int pid;
        if (numArgs < 2)
          continue;

        if (MAXCHILDREN != -1 && runningChildren >= MAXCHILDREN)
        {
          waitForChild(children, v, runningChildren);
          runningChildren--;
        }
        signal(SIGCHLD,handler);

        if(wasSignal == 1)
        {
          int *aux;
          waitForChild(children,v, runningChildren);
          runningChildren--;
          aux=&wasSignal;
          *aux=0;
        }
        pid = fork();
        if (pid < 0)
        {
          perror("Failed to create new process.");
          exit(EXIT_FAILURE);
        }

        if (pid > 0)
        {
          start = time(NULL); 			//Comeca a contar o tempo
          v[runningChildren][0]=pid;
          v[runningChildren][1]=start;
          runningChildren++;
          printf("%s: background child started with PID %d.\n\n", COMMAND_RUN, pid);
          continue;
        }
        else
        {
          char seqsolver[] = "../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver";
          char *newArgs[3] = {seqsolver, args[1], args[2]};

          execv(seqsolver, newArgs);
          perror("Error while executing child process"); // Nao deveria chegar aqui
          exit(EXIT_FAILURE);
        }
      }

      else if (numArgs == 0)
      {
	      /* Nenhum argumento ignora e volta a pedir */
	      continue;
      }
    }

    close(p);
    unlink(pipename);

    for (int i = 0; i < vector_getSize(children); i++)
      free(vector_at(children, i));

    vector_free(children);

    return EXIT_SUCCESS;

}
