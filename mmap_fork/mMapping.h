#ifndef MMAPPING_H
#define MMAPPING_H

#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

typedef struct fileInfo
{
    char *data;
    char *prevData;
    int fd;
    struct stat fileStat;
} fileInfo_t;

#endif // MMAPPING_H