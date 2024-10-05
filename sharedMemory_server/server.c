#include "server.h"
int signalOn = 0;
typedef unsigned long sigset_t;

void _signalHandler(int signum)
{
    printf("Caught signal %d\n", signum);
    printf("start to read shared memory\n");
}

void setSignal(sigset_t mask, shmInfo_t *shmInfo)
{
    sigfillset(&mask);
    sigdelset(&mask, SIGUSR1);
    signal(SIGUSR1, _signalHandler);

    printf("waiting for signal\n");
    sigsuspend(&mask);

    if (shmInfo->clientPid != NULL)
    {
        signalOn = 1;
    }
    else
    {
        sigsuspend(&mask);
    }
}

void readMemory(shmInfo_t *shmInfo)
{
    printf("start to read shared memory\n");
    printf("read: %s\n", shmInfo->buffer);
    if (shmInfo->buffer != NULL)
    {
        kill(shmInfo->clientPid, SIGUSR2);
    }
}

shmInfo_t *wakeupClient(shmInfo_t *shmInfo)
{
    key_t key = 124;
    shmInfo = (shmInfo_t *)malloc(sizeof(shmInfo_t));
    // 공유 메모리 세그먼트 생성 또는 얻기
    int shmid = shmget(key, sizeof(shmInfo_t), 0666 | IPC_CREAT);

    // 공유 메모리 연결
    shmInfo->shmaddr = shmat(shmid, NULL, 0);
    if (shmInfo->shmaddr == (void *)-1)
    {
        perror("shmat failed");
        exit(1);
    }
    shmInfo_t *data = (shmInfo_t *)shmInfo->shmaddr;

    data->serverPid = getpid();
    data->key = key;
    data->shmid = shmid;

    // 공유 메모리에 데이터 복사
}

void initServer(shmInfo_t *shmInfo)
{
    printf("server pid: %d\n", getpid());
    shmInfo_t *shdata = wakeupClient(shmInfo);

    sigset_t mask;
    setSignal(mask, shdata);

    if (signalOn)
    {

        readMemory(shdata);
    }
}

int main()
{
    shmInfo_t *shmInfo;
    initServer(shmInfo);
    return 0;
}