/*
 * PROJETO DE SO - EXERCICIO 2
 *
 * PEDRO LEITAO - 90764
 * TIAGO NEVES - 90778
 *
 * =============================================================================
 *
 * CircuitRouter-ParSolver.c
 *
 * =============================================================================
 */


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include "lib/list.h"
#include "maze.h"
#include "router.h"
#include "lib/timer.h"
#include "lib/types.h"

enum param_types {
    PARAM_BENDCOST = (unsigned char)'b',
    PARAM_XCOST    = (unsigned char)'x',
    PARAM_YCOST    = (unsigned char)'y',
    PARAM_ZCOST    = (unsigned char)'z',
};

enum param_defaults {
    PARAM_DEFAULT_BENDCOST = 1,
    PARAM_DEFAULT_XCOST    = 1,
    PARAM_DEFAULT_YCOST    = 1,
    PARAM_DEFAULT_ZCOST    = 2,
};

bool_t global_doPrint = TRUE;
char* global_inputFile = NULL;
long global_params[256]; /* 256 = ascii limit */

int NUMTAREFAS;


/* =============================================================================
 * displayUsage
 * =============================================================================
 */
static void displayUsage (const char* appName){
    printf("Usage: %s [options] input_filename\n", appName);
    puts("\nOptions:                            (defaults)\n");
    printf("    b <INT>    [b]end cost          (%i)\n", PARAM_DEFAULT_BENDCOST);
    printf("    x <UINT>   [x] movement cost    (%i)\n", PARAM_DEFAULT_XCOST);
    printf("    y <UINT>   [y] movement cost    (%i)\n", PARAM_DEFAULT_YCOST);
    printf("    z <UINT>   [z] movement cost    (%i)\n", PARAM_DEFAULT_ZCOST);
    printf("    h          [h]elp message       (false)\n");
    exit(1);
}


/* =============================================================================
 * setDefaultParams
 * =============================================================================
 */
static void setDefaultParams (){
    global_params[PARAM_BENDCOST] = PARAM_DEFAULT_BENDCOST;
    global_params[PARAM_XCOST]    = PARAM_DEFAULT_XCOST;
    global_params[PARAM_YCOST]    = PARAM_DEFAULT_YCOST;
    global_params[PARAM_ZCOST]    = PARAM_DEFAULT_ZCOST;
}


/* =============================================================================
 * parseArgs
 * =============================================================================
 */
static void parseArgs (long argc, char* const argv[]){
    long opt;

    opterr = 0;

    setDefaultParams();

    while ((opt = getopt(argc, argv, "hb:x:y:t:z:")) != -1) {
        switch (opt) {
            case 'b':
            case 'x':
            case 'y':
            case 't':
            case 'z':
                global_params[(unsigned char)opt] = atol(optarg);
                break;
            case '?':
            case 'h':
                displayUsage(argv[0]);
            default:
                break;
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Missing input file\n");
        displayUsage(argv[0]);
    }

    global_inputFile = argv[optind];
}

/* =============================================================================
 * outputFile
 * =============================================================================
 */
FILE * outputFile() {
    FILE *fp;

    char result_outputFile[strlen(global_inputFile) + strlen(".res") + 1];
    sprintf(result_outputFile, "%s.res", global_inputFile);

    if (access(result_outputFile, F_OK) == 0) {
        char old_outputFile[strlen(global_inputFile) + strlen(".res.old") + 1];
        sprintf(old_outputFile, "%s.res.old", global_inputFile);
        if (rename(result_outputFile, old_outputFile) == -1) {
            perror("Error renaming output file");
            exit(EXIT_FAILURE);;
        }
    }
    fp = fopen(result_outputFile, "wt");
    if (fp == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }
    return fp;
}

int verify(char * string)
{
    int x = 0;
    int len = strlen(string);

    while(x < len) {
           if(!isdigit(*(string+x)))
           return 1;

           ++x;
    }

    return 0;
}

/* =============================================================================
 * main
 * =============================================================================
 */
int main(int argc, char** argv){
    /*
     * Initialization
     */

	if(argc>1)
	{
		if(strcmp(argv[1],"-t") == 0)
		{
			if(argc>2)
			{
				if(verify(argv[2])==0)
				{
					if(atoi(argv[2])>0)
							NUMTAREFAS=atoi(argv[2]);
					else
					{
						fprintf(stderr, "O numero de tarefas nao e positivo\n");
						exit(0);
					}
				}
				else
				{
					fprintf(stderr, "Nao inseriu o numero de tarefas\n");
					exit(0);
				}
			}
			else
			{
				fprintf(stderr, "Nao inseriu o numero de tarefas\n");
				exit(0);
			}
		}
	}
	else
	{
		fprintf(stderr, "Não inseriu o argumento adicional obrigatorio (-t NUMTAREFAS)\n");
		exit(0);
	}

  parseArgs(argc, argv);
  FILE* resultFp = outputFile();
  maze_t* mazePtr = maze_alloc();
  assert(mazePtr);
  long numPathToRoute = maze_read(mazePtr, global_inputFile, resultFp);
  router_t* routerPtr = router_alloc(global_params[PARAM_XCOST],
                                     global_params[PARAM_YCOST],
                                     global_params[PARAM_ZCOST],
                                     global_params[PARAM_BENDCOST]);
  assert(routerPtr);
  list_t* pathVectorListPtr = list_alloc(NULL);
  assert(pathVectorListPtr);

  router_solve_arg_t routerArg = {routerPtr, mazePtr, pathVectorListPtr};
  TIMER_T startTime;
  TIMER_READ(startTime);

  // INICIA A CRICAO DAS TAREFAS
  int num=0;
	pthread_t tarefas[NUMTAREFAS];


  for(int i=0; i<NUMTAREFAS; i++){
		if(pthread_create (&tarefas[i], 0, router_solve, &routerArg)==0)
			num++;
		else {
			fprintf(stderr, "ERRO - pthread_create\n");
			exit(1);
		}
	}

	for(int j=0; j<num;j++)
	{
		if((pthread_join(tarefas[j], NULL))!=0)
		{
			fprintf(stderr, "ERRO - pthread_join\n");
			exit(1);
		}
	}

  TIMER_T stopTime;
  TIMER_READ(stopTime);

  long numPathRouted = 0;
  list_iter_t it;
  list_iter_reset(&it, pathVectorListPtr);
  while (list_iter_hasNext(&it, pathVectorListPtr)) {
      vector_t* pathVectorPtr = (vector_t*)list_iter_next(&it, pathVectorListPtr);
      numPathRouted += vector_getSize(pathVectorPtr);
  }
  fprintf(resultFp, "Paths routed    = %li\n", numPathRouted);
  fprintf(resultFp, "Elapsed time    = %f seconds\n", TIMER_DIFF_SECONDS(startTime, stopTime));

  /*
   * Check solution and clean up
   */
  assert(numPathRouted <= numPathToRoute);
  bool_t status = maze_checkPaths(mazePtr, pathVectorListPtr, resultFp, global_doPrint);

  assert(status == TRUE);
  fputs("Verification passed.\n",resultFp);

  maze_free(mazePtr);
  router_free(routerPtr);

  list_iter_reset(&it, pathVectorListPtr);
  while (list_iter_hasNext(&it, pathVectorListPtr)) {
      vector_t* pathVectorPtr = (vector_t*)list_iter_next(&it, pathVectorListPtr);
      vector_t* v;
      while((v = vector_popBack(pathVectorPtr))) {
          // v stores pointers to longs stored elsewhere; no need to free them here
          vector_free(v);
      }
      vector_free(pathVectorPtr);
  }
  list_free(pathVectorListPtr);

  fclose(resultFp);
  exit(0);
}


/* =============================================================================
 *
 * End of CircuitRouter-SeqSolver.c
 *
 * =============================================================================
 */
