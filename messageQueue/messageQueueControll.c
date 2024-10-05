#include "msg.h"

void msgQDelet(int msgid)
{
    // Delete the message queue
    system("ipcs -q");
    printf("Deleting message queue...\n");
    if (msgctl(msgid, IPC_RMID, NULL) == -1)
    {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Message queue deleted successfully.\n");
    }
    system("ipcs -q");
}
