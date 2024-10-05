#include "process.h"
#include <sys/wait.h>

void syncProcess(int pid, int *pStatus)
{
    while (pid != wait(pStatus))
    {
        if (wait(pStatus) == -1)
        {
            perror("wait failed");
            exit(1);
        }
        printf("wait\n");
    }

    printf("%d process is destroyed\n", pid);
}