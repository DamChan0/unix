#include "mMapping.h"

int main(int argc, char **argv)
{
    fileInfo_t *fileInfo;
    initProcess(&fileInfo);
    setFileStat(fileInfo, argv);
    writeTofileByMsync(fileInfo, argv);
    close(fileInfo->fd);

    return 0;
}
