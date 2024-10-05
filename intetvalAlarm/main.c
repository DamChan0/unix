#include "IntervalAlarm.h"

void handler(void)
{
    printf("Alarm\n");
}

int main()
{
    struct itimerval itval;

    sigset(SIGALRM, handler);

    itval.it_value.tv_sec = 3;
    itval.it_value.tv_usec = 0;
    itval.it_interval.tv_sec = 2;
    itval.it_interval.tv_usec = 0;

    if (setitimer(ITIMER_REAL, &itval, NULL) == -1)
    {
        perror("setitimer");
        return 1;
    }

    while (1)
    {
        if (getitimer(ITIMER_REAL, &itval) == -1)
        {
            perror("getitimer");
            return 1;
        }

        printf("sec : %d , usec : %d\n", itval.it_value.tv_sec, itval.it_value.tv_usec);
        sleep(1);
    }
    return 0;
}