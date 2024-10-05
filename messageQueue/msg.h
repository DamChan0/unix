#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
typedef struct msgbuf
{
    long mtype;
    char mtext[100];
} msgbuf_t;

void msgQDelet(int msgid);
void msgRecv();
int msgSend(char *msg);