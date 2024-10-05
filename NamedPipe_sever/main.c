#include "pipe.h"

int main(void)
{
    int pd, n;
    char msg[] = "hello , I am Server";

    if (mkfifo("./SERVER", 0666) == -1)
    {
        perror("open");
        printf("mkfifo failed\n");
        exit(1);
    }
    printf("================= Server\n");

    if ((pd = open("./SERVER", O_WRONLY)) == -1)
    {
        perror("open");
    }
    n = write(pd, msg, sizeof(msg));
    if (n == -1)
    {
        perror("write");
        exit(1);
    }

    close(pd);

    return 0;
}