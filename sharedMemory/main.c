#include "sharedFuntion.h"

int main()
{
    size_t size = 123123123;
    int key = 0;
    key = ftok("test", 51);
    int shmid = setSharedMemory(key, size);
    system("ipcs -m");

    char *shmaddr = linkSharedMemory(shmid);

    readMemory(shmaddr, (size_t)5);
    writeMemory(shmaddr, "Hello", (size_t)5);
    readMemory(shmaddr, (size_t)5);

    removeSharedMemory(shmid);
}