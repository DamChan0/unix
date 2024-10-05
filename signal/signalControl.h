#ifndef SIGNALCONTROL_H
#define SIGNALCONTROL_H

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

typedef struct SignalControl
{
    FILE *logFile;
    int signint;
    int functionNum;
    void (*handler)(int);
    struct sigaction sigAct;
} SignalControl_t;

void SignalControl_init(SignalControl_t *self);
void SignalControl_signalFunction(SignalControl_t *self);
void SignalControl_writeLog(int signal);

#endif // SIGNALCONTROL_H