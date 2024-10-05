
#include "msg.h"

void msgRecv()
{
    msgbuf_t msg;
    int msgid;
    key_t key;
    int len;

    key = ftok("../msgq", 1);
    printf("Generated key: %d\n", key);

    msgid = msgget(key, IPC_CREAT | 0644);

    if (msgid < 0)
    {
        perror("msgget");
        exit(1);
    }

    len = msgrcv(msgid, &msg, sizeof(msgbuf_t) - sizeof(long), 0, 0);

    printf("msg: %s\n", msg.mtext);
    return;
}