#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    struct tms start_buf, end_buf;
    clock_t start_time, end_time;
    long ticks;
    char buffer[1024];
    memset(buffer, 'A', sizeof(buffer));

    // 초당 클록 틱 수를 가져옴
    ticks = sysconf(_SC_CLK_TCK);

    // 시작 시점의 시간 측정
    start_time = times(&start_buf);
    if (start_time == (clock_t)-1)
    {
        perror("times");
        return 1;
    }

    // 임의의 작업 수행 (예: for loop)
    for (int i = 0; i < 100000000; i++)
        ;

    // 시스템 시간을 소모하는 파일 입출력 작업
    int fd = open("test.txt", O_CREAT | O_RDWR, 0644);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    for (int i = 0; i < 1000; i++)
    {
        if (write(fd, buffer, sizeof(buffer)) == -1)
        {
            perror("write");
            close(fd);
            return 1;
        }
    }

    // 파일 닫기
    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }

    for (int i = 0; i < 100000000; i++)
    {
        void *ptr = malloc(1024);
        if (ptr == NULL)
        {
            perror("malloc");
            return 1;
        }
        free(ptr);
    }
    // 자식 프로세스 생성
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        // 자식 프로세스
        for (int i = 0; i < 100000000; i++)
            ;

        return 0;
    }
    else
    {
        for (int i = 0; i < 100000000; i++)
            ;
        // 부모 프로세스: 자식 프로세스가 종료될 때까지 기다림
        if (wait(NULL) == -1)
        {
            perror("wait");
            return 1;
        }

        // 종료 시점의 시간 측정
        end_time = times(&end_buf);
        if (end_time == (clock_t)-1)
        {
            perror("times");
            return 1;
        }

        // 사용자 시간과 시스템 시간 계산
        clock_t user_time = end_buf.tms_utime - start_buf.tms_utime;
        clock_t system_time = end_buf.tms_stime - start_buf.tms_stime;
        clock_t child_user_time = end_buf.tms_cutime - start_buf.tms_cutime;
        clock_t child_system_time = end_buf.tms_cstime - start_buf.tms_cstime;

        // 시간 출력 (초 단위로 변환)
        printf("User time: %lf seconds\n", (double)user_time / ticks);
        printf("System time: %lf seconds\n", (double)system_time / ticks);
        printf("Child user time: %lf seconds\n", (double)child_user_time / ticks);
        printf("Child system time: %lf seconds\n", (double)child_system_time / ticks);
        printf("Total elapsed time: %lf seconds\n", (double)(end_time - start_time) / ticks);
    }

    return 0;
}
