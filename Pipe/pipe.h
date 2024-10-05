#define __USE_POSIX2
#ifndef _PIPE_H_
#define _PIPE_H_
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct Finfo
{
    int *fd;
    FILE *file;
    char *buf;
} Finfo_t;
void makePipe(Finfo_t *fInfo);
void pRead(void);
void pWrite(void);
void pComplex();
#endif