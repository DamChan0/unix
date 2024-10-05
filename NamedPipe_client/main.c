#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
    int pd, n;
    char msg[100];

    if ((pd = open("../../NamedPipe_sever/build/SERVER", O_RDONLY)) == -1)
    {
        perror("open");
        exit(1);
    }
    // Null-terminate the message

    printf("================= client\n");

    // Debugging output

    // Write the message to stdout
    if (write(1, msg, n) == -1)
    {
        perror("write");
        exit(1);
    }

    while ((n = read(pd, msg, sizeof(msg) - 1)) > 0)
    { // Read from pipe
        printf("Read %d bytes: %s\n", n, msg);
        msg[n] = '\0'; // Null-terminate the message
        if (write(1, msg, n) == -1)
        { // Write to stdout
            perror("write");
            exit(1);
        }
    }

    if (n == -1)
    {
        perror("read");
        exit(1);
    }

    write(1, "\n", 1);
    close(pd);

    return 0;
}