#include "signalControl.h"
SignalControl_t sigInfo;

void SignalControl_init(SignalControl_t *self)
{
    memset(&self->sigAct, 0, sizeof(self->sigAct));
    sigemptyset(&self->sigAct.sa_mask);
    self->signint = SIGUSR1;
    self->functionNum = 2;
    self->logFile = NULL;
    self->handler = SignalControl_writeLog;
    self->sigAct.sa_handler = self->handler;
    sigInfo = *self;
}

void SignalControl_signalFunction(SignalControl_t *self)
{
    int functionNum = self->functionNum;
    int signalIndex = self->signint;
    if (functionNum == 1)
    {
        printf("using sigset\n");
        signal(signalIndex, self->handler);
    }
    else if (functionNum == 2)
    {
        printf("using sigaction\n");
        sigaction(signalIndex, &self->sigAct, NULL);
    }
}

void SignalControl_writeLog(int signal)
{
    int functionNum = sigInfo.functionNum;
    fflush(stdout);
    fflush(stderr);

    int stdoutFd = fileno(stdout);
    int stderrFd = fileno(stderr);

    int logFd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (logFd == -1)
    {
        perror("Failed to open log file");
        return;
    }
    else
    {
        dup2(logFd, stdoutFd);
        dup2(logFd, stderrFd);
    }

    FILE *logFile = fopen("log.txt", "a");

    if (logFile != NULL)
    {
        fprintf(logFile, "Function number: %d\n", functionNum);
        fclose(logFile);
    }
    else
    {
        perror("Failed to open log file");
    }
    close(logFile);
}