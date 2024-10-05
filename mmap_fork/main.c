#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

typedef struct
{
    int fd;
    char *data;
    char *prevData;
    struct stat fileStat;
    pthread_mutex_t lock;
} fileInfo_t;

void fileAlloc(fileInfo_t **fInfo);
void childProcessControl(fileInfo_t *fInfo);
void *childThread(void *arg);
void *parentThread(void *arg);

fileInfo_t *fileInfo;

int main()
{
    fileAlloc(&fileInfo);

    fileInfo->fd = open("log.txt", O_RDWR | O_CREAT, 0666);
    if (fileInfo->fd == -1)
    {
        perror("open");
        free(fileInfo->prevData);
        free(fileInfo->data);
        free(fileInfo);
        exit(EXIT_FAILURE);
    }

    if (stat("log.txt", &fileInfo->fileStat) == -1)
    {
        perror("stat");
        close(fileInfo->fd);
        free(fileInfo->prevData);
        free(fileInfo->data);
        free(fileInfo);
        exit(EXIT_FAILURE);
    }

    if (fileInfo->fileStat.st_size == 0)
    {
        fprintf(stderr, "File size is 0, cannot mmap\n");
        close(fileInfo->fd);
        free(fileInfo->prevData);
        free(fileInfo->data);
        free(fileInfo);
        exit(EXIT_FAILURE);
    }

    fileInfo->prevData = mmap(NULL, (size_t)fileInfo->fileStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fileInfo->fd, 0);
    if (fileInfo->prevData == MAP_FAILED)
    {
        perror("mmap");
        close(fileInfo->fd);
        free(fileInfo->data);
        free(fileInfo);
        exit(EXIT_FAILURE);
    }

    pthread_mutex_init(&fileInfo->lock, NULL);

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        munmap(fileInfo->prevData, (size_t)fileInfo->fileStat.st_size);
        close(fileInfo->fd);
        free(fileInfo->prevData);
        free(fileInfo->data);
        free(fileInfo);
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    { // 자식 프로세스
        pthread_t child_tid;
        pthread_create(&child_tid, NULL, childThread, (void *)fileInfo);
        pthread_join(child_tid, NULL);
    }
    else
    { // 부모 프로세스
        pthread_t parent_tid;
        pthread_create(&parent_tid, NULL, parentThread, (void *)fileInfo);
        pthread_join(parent_tid, NULL);

        wait(NULL);
    }

    pthread_mutex_destroy(&fileInfo->lock);
    munmap(fileInfo->prevData, (size_t)fileInfo->fileStat.st_size);
    close(fileInfo->fd);
    free(fileInfo->prevData);
    free(fileInfo->data);
    free(fileInfo);

    return 0;
}

void fileAlloc(fileInfo_t **fInfo)
{
    *fInfo = (fileInfo_t *)malloc(sizeof(fileInfo_t));
    if (*fInfo == NULL)
    {
        perror("malloc");
        exit(1);
    }

    (*fInfo)->data = (char *)malloc(100);
    (*fInfo)->prevData = (char *)malloc(100);
}

void *childThread(void *arg)
{
    fileInfo_t *fInfo = (fileInfo_t *)arg;
    while (1)
    {
        pthread_mutex_lock(&fInfo->lock);

        // 파일의 끝으로 이동
        lseek(fInfo->fd, 0, SEEK_END);

        // 자식 스레드에서 데이터를 쓰는 작업 수행
        printf("Child thread accessing data\n");
        strncpy(fInfo->data, "Child Data\n", 11);
        write(fInfo->fd, fInfo->data, strlen(fInfo->data));

        // msync(fInfo->data, 100, MS_SYNC); // 데이터 동기화
        pthread_mutex_unlock(&fInfo->lock);
        sleep(1);
    }
    return NULL;
}

void *parentThread(void *arg)
{
    fileInfo_t *fInfo = (fileInfo_t *)arg;
    while (1)
    {
        pthread_mutex_lock(&fInfo->lock);

        // 파일의 끝으로 이동
        lseek(fInfo->fd, 0, SEEK_END);

        // 부모 스레드에서 데이터를 쓰는 작업 수행
        printf("Parent thread accessing data\n");
        strncpy(fInfo->data, "Parent Data\n", 12);
        write(fInfo->fd, fInfo->data, strlen(fInfo->data));

        // msync(fInfo->data, 100, MS_SYNC); // 데이터 동기화
        pthread_mutex_unlock(&fInfo->lock);
        sleep(1);
    }
    return NULL;
}
