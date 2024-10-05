#include "signalControl.h"
#include <signal.h>
#include <stdio.h>

#define MAX_COUNT 10000

void sigusr1Handler(int signal)
{
    printf("SIGUSR1 received\n");
}

int main()
{
    SignalControl_t signalControl;
    SignalControl_init(&signalControl);
    SignalControl_signalFunction(&signalControl);

    printf("Signal Control\n");
    printf("--------------\n");
    while (1)
    {
        char c = getchar();
        if (c == '\n')
        {
            raise(SIGUSR1);
        }
        static int count = 0;
        count++;
        printf("count: %d\n", count);
    }
    return 0;
}