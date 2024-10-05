// semaphore.c
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#define SEMKEY 0x1234
typedef union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
} semun_t;

int initSem(key_t key);
int semLock(int semid);
int semULock(int semid);
void test(int semid);
