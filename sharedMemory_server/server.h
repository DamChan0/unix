#include "include/type.h"

void initServer();
void readMemory(shmInfo_t *shmInfo);
void setSignal(sigset_t mask, shmInfo_t *shmInfo);