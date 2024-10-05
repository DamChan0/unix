#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int setSharedMemory(int key, size_t size);
int removeSharedMemory(int shmid);
char *linkSharedMemory(int shmid);
void writeMemory(char *mAddr, char *data, size_t size);
void readMemory(char *mAddr, size_t size);