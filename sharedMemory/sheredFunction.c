#include <sharedFuntion.h>
int _checkPage(size_t size)
{
    size_t page_size = sysconf(_SC_PAGE_SIZE);
    printf("Page size: %zu bytes\n", page_size);

    if (size % page_size != 0)
    {
        size = ((size / page_size) + 1) * page_size;
        printf("Adjusted size to page boundary: %zu bytes\n", size);
    }
    return size;
}

int setSharedMemory(int key, size_t size)
{
    int shmid;
    size = _checkPage(size);
    if ((shmid = shmget(key, size, IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        exit(1);
    }
    return shmid;
}

int removeSharedMemory(int shmid)
{
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        return 1;
        // exit(1);
    }
    return 0;
}

char *linkSharedMemory(int shmid)
{
    char *shmaddr;
    if ((shmaddr = shmat(shmid, NULL, 0)) == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }
    return shmaddr;
}

void writeMemory(char *mAddr, char *data, size_t size)
{
    memcpy(mAddr, data, size);
}

void readMemory(char *mAddr, size_t size)
{
    printf("Data: %s\n", mAddr);
}
