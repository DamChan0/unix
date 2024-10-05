#ifndef MMAPPING_H
#define MMAPPING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

typedef struct fileInfo
{
    char *data;
    char *prevData;
    int fd;
    struct stat fileStat;
} fileInfo_t;

void initProcess(fileInfo_t **fileInfo);
void freeProcess(fileInfo_t *fileInfo);
void setFileStat(fileInfo_t *fileInfo, char **argv);
void writeTofileByMsync(fileInfo_t *fInfo, char **argv);
void writeTofile(char *data, int fd, struct stat fileStat, char *addr);
#endif // MMAPPING_H