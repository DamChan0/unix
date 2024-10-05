#include "msg.h"
int msgSend(char *msg)
{
    key_t key;
    int msgid;

    msgbuf_t msgingfo;

    key = ftok("../msgq", 1);
    printf("Generated key: %d\n", key);

    msgid = msgget(key, IPC_CREAT | 0644);
    if (msgid == -1)
    {
        perror("msgget");
        exit(1);
    }

    msgingfo.mtype = 1;
    strcpy(msgingfo.mtext, msg);
    if (msgsnd(msgid, &msgingfo, sizeof(msgbuf_t) - sizeof(long), 0) == -1)
    {
        perror("msgsnd");
        exit(1);
    }
    return msgid;
}