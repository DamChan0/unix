#include "semaphoreSYSV.h"
#include <omp.h>

void semhandle(key_t key)
{
    int pid = getpid();
    int semid = initSem(key);

    printf("Process %d attempting to lock semaphore...\n", pid);
    semLock(semid);

    printf("Lock acquired by process %d\n", pid);
    printf("Current semaphore value (before sleep): %d\n", semctl(semid, 0, GETVAL));

    // 세마포어 잠금을 해제하기 전에 대기
    sleep(5);

    test(semid);

    printf("Process %d releasing semaphore...\n", pid);
    semULock(semid);
}

int main()
{
    key_t key = 12345;
    int num_procs = 3;
    for (int i = 0; i < num_procs; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("Fork failed");
            exit(1);
        }
        else if (pid == 0)
        {
            // 자식 프로세스에서 세마포어 처리

            semhandle(key);
            exit(0);
        }
    }
    wait(NULL);
    return 0;
}