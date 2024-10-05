#include "mMapping.h"

void writeTofile(char *data, int fd, struct stat fileStat, char *addr)
{
    int newSize = fileStat.st_size + strlen(data);

    if (ftruncate(fd, newSize) == -1)
    {
        perror("ftruncate error");
        exit(1);
    }

    memcpy(addr + fileStat.st_size, data, strlen(data));
}

void writeTofileByMsync(fileInfo_t *fInfo, char **argv)
{
    printf("prevData: %s\n", fInfo->prevData);
    strcpy(fInfo->data, argv[1]); // 문자열 상수를 직접 할당하지 않고 복사합니다.

    if ((size_t)strlen(fInfo->prevData) <= (size_t)strlen(fInfo->data))
    {
        ftruncate(fInfo->fd, (size_t)strlen(fInfo->data));
    }

    // 데이터 쓰기
    memcpy(fInfo->prevData, fInfo->data, strlen(fInfo->data));
    if (msync(fInfo->prevData, fInfo->fileStat.st_size, MS_SYNC) == -1)
    {
        perror("msync error");
        exit(1);
    }
}

void initProcess(fileInfo_t **fileInfo)
{
    *fileInfo = (fileInfo_t *)malloc(sizeof(fileInfo_t));
    if (*fileInfo == NULL)
    {
        perror("malloc");
        exit(1);
    }

    (*fileInfo)->data = (char *)malloc(100);
    if ((*fileInfo)->data == NULL)
    {
        perror("malloc");
        free(*fileInfo);
        exit(1);
    }

    (*fileInfo)->prevData = (char *)malloc(100);
    if ((*fileInfo)->prevData == NULL)
    {
        perror("malloc");
        free((*fileInfo)->data);
        free(*fileInfo);
        exit(1);
    }
}

void freeProcess(fileInfo_t *fileInfo)
{
    if (fileInfo->prevData != NULL)
    {
        free(fileInfo->prevData);
    }
    if (fileInfo->data != NULL)
    {
        free(fileInfo->data);
    }
    if (fileInfo->fd != -1)
    {
        close(fileInfo->fd);
    }
    free(fileInfo);
}

void setFileStat(fileInfo_t *fileInfo, char **argv)
{
    fileInfo->fd = open("log", O_RDWR | O_CREAT, 0666);
    if (fileInfo->fd == -1)
    {
        perror("open");
        freeProcess(fileInfo);
        exit(EXIT_FAILURE);
    }

    if (fstat(fileInfo->fd, &fileInfo->fileStat) == -1)
    {
        perror("stat error");
        freeProcess(fileInfo);
        exit(1);
    }

    if (fileInfo->fileStat.st_size == 0)
    {
        // 파일 크기가 0이면 mmap 호출 전에 크기를 설정합니다.
        if (ftruncate(fileInfo->fd, 100) == -1)
        {
            perror("ftruncate error");
            freeProcess(fileInfo);
            exit(1);
        }
        fileInfo->fileStat.st_size = 100;
    }

    fileInfo->prevData = mmap(NULL, (size_t)fileInfo->fileStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fileInfo->fd, 0);
    if (fileInfo->prevData == MAP_FAILED)
    {
        perror("mmap error");
        freeProcess(fileInfo);
        exit(1);
    }
}

void updateFile(fileInfo_t *fInfo)
{
    if ((size_t)strlen(fInfo->prevData) < (size_t)strlen(fInfo->data))
    {
        if (ftruncate(fInfo->fd, (size_t)strlen(fInfo->data)) == -1)
        {
            perror("ftruncate error");
            exit(1);
        }
    }

    if (msync(fInfo->data, fInfo->fileStat.st_size, MS_SYNC) == -1)
    {
        perror("msync error");
        exit(1);
    }
}