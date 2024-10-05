#include "semaphoreSYSV.h"

void test(int semid)
{
    union semun sem_union;
    sem_union.val = 1;
    if (semctl(semid, 0, SETVAL, sem_union) == -1)
    {
        perror("semctl SETVAL failed");
    }
    else
    {
        printf("Semaphore value set to 1\n");
    }

    // GETVAL: 지정된 세마포어의 현재 값을 가져옵니다.
    int sem_value = semctl(semid, 0, GETVAL);
    if (sem_value == -1)
    {
        perror("semctl GETVAL failed");
    }
    else
    {
        printf("Current semaphore value: %d\n", sem_value);
    }

    // IPC_STAT: 세마포어 집합의 상태를 가져옵니다.
    struct semid_ds semid_ds;
    sem_union.buf = &semid_ds;
    if (semctl(semid, 0, IPC_STAT, sem_union) == -1)
    {
        perror("semctl IPC_STAT failed");
    }
    else
    {
        printf("Last semop time: %ld\n", semid_ds.sem_otime);
        printf("Last change time: %ld\n", semid_ds.sem_ctime);
    }

    // GETPID: 세마포어를 마지막으로 조작한 프로세스의 PID를 가져옵니다.
    int last_pid = semctl(semid, 0, GETPID);
    if (last_pid == -1)
    {
        perror("semctl GETPID failed");
    }
    else
    {
        printf("Last semop process PID: %d\n", last_pid);
    }

    // GETNCNT: 세마포어 값을 증가시키기 위해 기다리는 프로세스의 수를 가져옵니다.
    int waiting_processes = semctl(semid, 0, GETNCNT);
    if (waiting_processes == -1)
    {
        perror("semctl GETNCNT failed");
    }
    else
    {
        printf("Processes waiting for increase: %d\n", waiting_processes);
    }

    // GETZCNT: 세마포어 값이 0이 되기를 기다리는 프로세스의 수를 가져옵니다.
    int zero_waiting_processes = semctl(semid, 0, GETZCNT);
    if (zero_waiting_processes == -1)
    {
        perror("semctl GETZCNT failed");
    }
    else
    {
        printf("Processes waiting for zero: %d\n", zero_waiting_processes);
    }

    return 0;
}

int initSem(key_t key)
{
    semun_t semun;
    int status = 0; // 세마포어 초기화 상태
    int semid;
    semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid == -1)
    {
        if (errno == EEXIST)
        {
            semid = semget(key, 1, 0666);
        }
        perror("semget");
        return -1;
    }
    else
    {
        semun.val = 1;
        status = semctl(semid, 0, SETVAL, semun);
    }

    if (status == -1 || semid == -1)
    {
        perror("semctl");
        return -1;
    }

    // 결과 확인
    return semid;
}

int semLock(int semid)
{
    struct sembuf p_op;
    p_op.sem_num = 0;
    p_op.sem_op = -1; // -1: P operation
    p_op.sem_flg = SEM_UNDO;

    if (semop(semid, &p_op, 1) == -1) //
    {
        perror("semop");
        return -1;
    }

    printf("semval = %d\n", semctl(semid, 0, GETVAL, NULL));
    // 결과 확인

    return 0;
}

int semULock(int semid)
{
    struct sembuf v_op;
    v_op.sem_num = 0;
    v_op.sem_op = 1;         // 1: V operation
    v_op.sem_flg = SEM_UNDO; // SEM_UNDO: 프로세스가 종료되면 세마포어를 해제
    if (semop(semid, &v_op, 1) == -1)
    {
        perror("semop");
        return -1;
    }
    printf("sem_value : %d\n", semctl(semid, 0, GETVAL, NULL));
}