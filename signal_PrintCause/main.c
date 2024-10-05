#define _POSIX_C_SOURCE 200809L // POSIX 기능 사용을 위한 매크로 정의

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

// 시그널 핸들러 함수
void handler(int signum, siginfo_t *info, void *context)
{
    printf("Received signal %d\n", signum);
    if (info != NULL)
    {
        printf("process id: %d\n", getpid());
        printf("Signal sent by process %d\n", info->si_pid);
        psiginfo(info, "Signal info: ");
        printf("si_code: %d\n", info->si_code);

        if (signum == SIGQUIT)
        {
            printf("SIGQUIT received, ignoring it for now.\n");
        }
    }
    printf("ho\n");
    sleep(3);
}

int main()
{
    struct SignalControl
    {
        struct sigaction sigAct;
    } self;

    printf("using sigaction\n");

    // 시그널 핸들러 설정
    memset(&self.sigAct, 0, sizeof(self.sigAct)); // Zero out the structure
    sigemptyset(&self.sigAct.sa_mask);
    sigaddset(&self.sigAct.sa_mask, SIGQUIT); // Block SIGQUIT during handler 385146execution
    sigaddset(&self.sigAct.sa_mask, SIGUSR1); // Block SIGUSR1 during handler execution
    self.sigAct.sa_sigaction = handler;
    self.sigAct.sa_flags = SA_SIGINFO;

    int signalIndex = SIGINT; // Example signal to handle, SIGINT (Ctrl+C)
    if (sigaction(signalIndex, &self.sigAct, NULL) < 0)
    {
        perror("sigaction");
        printf("aaaaaa\n");
        return 1;
    }

    // SIGQUIT 시그널에 대한 핸들러 설정 (옵션)
    if (sigaction(SIGQUIT, &self.sigAct, NULL) < 0)
    {
        perror("sigaction");
        printf("aaaaaa\n");
        return 1;
    }

    if (sigaction(SIGUSR1, &self.sigAct, NULL) < 0)
    {
        perror("sigaction");
        printf("aaaaaa\n");
        return 1;
    }

    // 프로그램 실행 중
    while (1)
    {
        printf("Running... Press Ctrl+C to interrupt. Press Ctrl+\\ for SIGQUIT (won't be processed during handler).\n");
        sleep(1);
    }

    return 0;
}
