#ifndef CIRCUITROUTER_SHELL_H
#define CIRCUITROUTER_SHELL_H

#include "lib/vector.h"
#include <sys/types.h>

typedef struct {
    pid_t pid;
    int status;
    int time;
} child_t;

void waitForChild(vector_t *children, int v[][2], int runningChildren);
void printChildren(vector_t *children);
void handler(int sig);

#endif /* CIRCUITROUTER_SHELL_H */
