#include "client.h"
#include <sys/stat.h> // Add this line to include the necessary header file
#include <signal.h>   // Include the signal handling definitions
typedef unsigned long sigset_t;
void _signalHandler(int signum)
{
    printf("server recived\n");
}

void writeMemory(shmInfo_t *shmInfo)
{
    printf("start to write shared memory\n");

    strcpy(shmInfo->buffer, "Hello from client");

    shmInfo->clientPid = getpid();
    printf("write: %s\n", shmInfo->buffer);

    kill(shmInfo->serverPid, SIGUSR1);

    sigset_t mask;
    sigdelset(&mask, SIGUSR2);
    signal(SIGUSR1, _signalHandler);
    sigsuspend(&mask);
}

void initClient(shmInfo_t *shmInfo)
{
    shmInfo = (shmInfo_t *)malloc(sizeof(shmInfo_t));
    const char *key_path = "KEY";
    struct stat buffer;

    // "KEY" 파일이 존재하는지 확인
    if (stat(key_path, &buffer) != 0)
    {
        // 파일이 존재하지 않으면 생성
        FILE *file = fopen(key_path, "w");
        if (file == NULL)
        {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        fclose(file);
    }

    // ftok 함수 호출
    key_t key = 124;
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    shmInfo->shmaddr = shmat(shmid, NULL, 0);

    shmInfo_t *data = (shmInfo_t *)shmInfo->shmaddr;

    writeMemory(data);
}

int main()
{
    shmInfo_t *shmInfo;
    initClient(shmInfo);
    return 0;
}