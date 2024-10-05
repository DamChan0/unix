#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int createProcess();

void destroyCallback();

void destroyProcess(int *pid, int *status);
void syncProcess(int pid, int *pStatus);