#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <signal.h>
typedef struct shmInfo
{
    key_t key;
    int shmid;
    void *shmaddr;
    size_t size;
    int serverPid;
    int clientPid;
    char buffer[4096];
} shmInfo_t;
