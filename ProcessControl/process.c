#include "process.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int createProcess()
{
    pid_t pid;
    pid = fork();
    int status;

    switch (pid)
    {
    case -1: // fork() 실패
        fprintf(stderr, "Fork Failed\n");
        exit(-1);
        break;

    case 0: // 자식 프로세스
        printf("Child Process\n");
        // 폴더 생성
        if (system("mkdir ./test") == -1)
        {
            perror("mkdir failed");
            exit(1);
        }

        // 폴더 내용 확인
        if (system("ls") == -1)
        {
            perror("ls failed");
            exit(1);
        }

        // 폴더 삭제
        if (system("rm -rf ./test") == -1)
        {
            perror("rm failed");
            exit(1);
        }
        system("ls");
        destroyProcess(&pid, &status);
        break;

    default: // 부모 프로세스
        printf("Parent Process\n");
        syncProcess(pid, &status);
        break;
    }
    return pid;
}

void destroyCallback()
{
    printf("Destroy Process\n");
}

void destroyProcess(int *pid, int *status)
{

    printf("Child Process pid : %d\n", *pid);
    atexit(destroyCallback);
    exit(0);
}