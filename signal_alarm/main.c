#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void hanlder(int signal, siginfo_t *info, ucontext_t *context)
{
    printf("Signal: %d\n", signal);
    printf("Signal code: %d\n", info->si_code);
    printf("Signal value: %d\n", info->si_value.sival_int);
    psiginfo(info, "Signal info: ");
}

int main()
{
    sigset(SIGALRM, hanlder);
    alarm(1);
    printf("alarm\n");
    sleep(10);
    return 0;
}