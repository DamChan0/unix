
#include "msg.h"

int main()
{
    char *msg = "Hello, I am Client";
    int msgid = msgSend(msg);
    msgRecv();
    // msgQDelet(msgid);

    return 0;
}