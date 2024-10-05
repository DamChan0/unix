#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <omp.h>
#include <sys/time.h>
#define BUFFER_SIZE 10
#define SIZE 1000000
int buffer[BUFFER_SIZE];
int count = 0;
pthread_mutex_t lock;
int max = 5;
int consumer1_first = 1; // 1번 소비자가 먼저 가져가도록 플래그 설정
pthread_cond_t cond;

int paraller()
{
    struct timeval start, end;
    long seconds, useconds;
    double mtime;
    gettimeofday(&start, NULL);

    int array[SIZE];
    long long sum = 0;

    // 배열 초기화
    for (int i = 0; i < SIZE; i++)
    {
        array[i] = i;
    }

    // OpenMP를 사용한 병렬 합산
    // #pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < SIZE; i++)
    {
        sum += array[i];
    }

    printf("Parallel Sum: %lld\n", sum);
    gettimeofday(&end, NULL);

    seconds = end.tv_sec - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    mtime = ((seconds) * 1000 + useconds / 1000.0);
    printf("Execution time: %f ms\n", mtime);
    return 0;
}

void *producer(void *arg)
{
    int produced_item = 0;
    while (produced_item < 10)
    {
        pthread_mutex_lock(&lock);
        if (count < BUFFER_SIZE)
        {
            if (count == 0)
            {
                // 버퍼가 비어 있다가 처음으로 채워질 때만 신호를 보낸다
                pthread_cond_broadcast(&cond);
            }
            buffer[count++] = produced_item;
            printf("Produced: %d (Buffer count: %d)\n", produced_item, count);
            produced_item++;
        }
        pthread_mutex_unlock(&lock);
        sleep(1); // 생산 시간 시뮬레이션
    }
    return NULL;
}

void *consumer1(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&lock);
        while (count == 0 || !consumer1_first)
        { // 1번 소비자가 우선 접근
            pthread_cond_wait(&cond, &lock);
        }
        int consumed_item = buffer[--count];
        printf("Consumed by 1: get %d (remain items in Buffer: %d)\n", consumed_item, count);
        consumer1_first = 0;           // 2번 소비자에게 기회 제공
        pthread_cond_broadcast(&cond); // 다른 소비자에게 신호
        pthread_mutex_unlock(&lock);
        sleep(1); // 소비 시간 시뮬레이션
    }
    return NULL;
}

void *consumer2(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&lock);
        while (count == 0 || consumer1_first)
        { // 1번 소비자가 우선 접근할 때 대기
            pthread_cond_wait(&cond, &lock);
        }
        int consumed_item = buffer[--count];
        printf("Consumed by 2: get %d (remain items in Buffer: %d)\n", consumed_item, count);
        consumer1_first = 1;           // 1번 소비자에게 기회 제공
        pthread_cond_broadcast(&cond); // 1번 소비자에게 신호
        pthread_mutex_unlock(&lock);
        sleep(1); // 소비 시간 시뮬레이션
    }
    return NULL;
}

int main()
{
    pthread_t producer_thread, consumer_thread1, consumer_thread2;

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread1, NULL, consumer1, NULL);
    pthread_create(&consumer_thread2, NULL, consumer2, NULL);

    pthread_join(producer_thread, NULL);
    pthread_cancel(consumer_thread1); // 프로그램 종료
    pthread_cancel(consumer_thread2);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    printf("OpenMP==================================\n");
    paraller();
    return 0;
}
