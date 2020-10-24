/*
//  PROJETO DE SO - EXERCICIO 3
//
//  PEDRO LEITAO - 90764
//  TIAGO NEVES - 90778
*/

/*
* =============================================================================
*
* CircuitRouter-Client.c
*
* =============================================================================
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>

#define COMMAND_EXIT "exit"
#define COMMAND_RUN "run"

#define BUFFER_SIZE 1024


int main (int argc, char** argv)
{

	char *pipename;
	char pipenameClient[BUFFER_SIZE];
	char buffer[BUFFER_SIZE];
	char msg[BUFFER_SIZE]="";
	char msg1[BUFFER_SIZE];
	int fdTemp = -1;

	if(argv[1] != NULL)
		pipename = argv[1];

	strcat(pipename,".pipe"); //sufixado por .pipe

	memset(pipenameClient,0,sizeof(pipenameClient));
	strncpy(pipenameClient,"/tmp/fileXXXXXX",sizeof("/tmp/fileXXXXXX"));

	int p = open(pipename, O_WRONLY);

	unlink(pipenameClient);

	if((fdTemp = mkstemp(pipenameClient)) < 1){
		perror("Erro ao abrir o ficheiro temporario do cliente\n");
		exit(EXIT_FAILURE);
	}

	strcat(msg,pipenameClient);


	if (p==-1){
		perror("Erro ao abrir o pipe");
		exit(EXIT_FAILURE);
	}

	printf("\nWelcome to CircuitRouter-Client\n");

	while(1)
	{
		fgets(msg1,BUFFER_SIZE,stdin);
		strtok(msg1,"\n");

		strcat(msg1, " ");
		strcat(msg1,msg);

		if(write(p,msg1,sizeof(msg1)) == -1){
			perror("Erro ao escrita");
			exit(EXIT_FAILURE);
		}

		sleep(2.5);

		int p2 = open(pipenameClient, O_RDONLY);

		if (p2==-1){
			perror("Erro ao abrir o pipe client");
			exit(EXIT_FAILURE);
		}

		if (read (p2, buffer, BUFFER_SIZE) == -1){
			perror("Erro na leitura");
			exit(EXIT_FAILURE);
		}

		printf("%s\n", buffer);
		close(p2);
	}
	close(p);


	unlink(pipenameClient);


	return EXIT_SUCCESS;

}
