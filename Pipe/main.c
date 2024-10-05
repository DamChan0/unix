#define __USE_POSIX2
#include "pipe.h"
int fd_value = 1;

Finfo_t Finfo = (Finfo_t){.fd = &fd_value, .file = NULL, .buf = NULL};

int main()
{
    // makePipe(&Finfo);

    pWrite();
    pRead();

    // pComplex();
}

void makePipe(Finfo_t *fInfo)
{
    fInfo->fd = popen("data", "r");
}

void pWrite()
{
    FILE *fp;

    fp = popen("wc -l", "w");

    if (fp == NULL)
    {
        fprintf(stderr, "open fail\n");
        exit(1);
    }

    for (int a = 0; a < 100; a++)
    {
        fprintf(fp, "line %d\n", 1);
        fprintf(fp, "Hello, World!\n");
        fprintf(fp, "This is a test.\n");
    }

    pclose(fp);
}

void pRead()
{
    FILE *fp;
    char buf[256];
    fp = popen("date", "r");
    if (fp == NULL)
    {
        fprintf(stderr, "open failed\n");
    }

    if (fgets(buf, sizeof(buf), fp) == NULL)
    {
        fprintf(stderr, "No data");
        exit(1);
    }
    printf("line data :%s \n", buf);
    pclose(fp);
}

void pComplex(void)
{
    int fd[2];
    pid_t pid;
    char buf[256];
    int len, status;

    if (pipe(fd) == -1)
    {
        printf("open failed\n");
        exit(1);
    }

    pid = fork();
    switch (pid)
    {
    case -1:
        printf("open failed\n");
        exit(1);

        break;
    case 0:           // 자식 프로세스
        close(fd[1]); // close write end
        write(1, "child process\n", 14);
        len = read(fd[0], buf, sizeof(buf));
        printf("reading data : %s\n", buf);
        if (len > 0)
        {
            buf[len] = '\0';    // Null-terminate the string
            write(1, buf, len); // Print the data read from the pipe
        }
        close(fd[0]);
        exit(0);
        break;
    default:
        close(fd[0]);
        buf[0] = '\0';
        char data[] = "Test pipe communication";
        write(fd[1], data, sizeof(data));
        close(fd[1]);
        waitpid(pid, &status, 0);
        break;
    }
}