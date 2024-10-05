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

int initSem(key_t key)
{
    semun_t semun;
    int status = 0;
    int semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid == -1)
    {
        if (errno == EEXIST)
        {
            semid = semget(key, 1, 0666);
        }
        perror("semget");
        return -1;
    }
    else
    {
        semun.val = 1;
        status = semctl(semid, 0, SETVAL, semun); 
        //
    }

    if (status == -1 || semid == -1)
    {
        perror("semctl");
        return -1;
    }

    return 0;
}

int main()
{
    key_t key = 12345;
    initSem(key);
}