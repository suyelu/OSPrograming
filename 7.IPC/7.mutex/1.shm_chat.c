/*************************************************************************
	> File Name: 1.shm_chat.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2025
 ************************************************************************/

#include "./common/head.h"

#define SHM_KEY_PATH "/tmp/myshm.key"
#define SHM_PROJ_ID 666
#define MSG_SIZE 1024
#define NEED 1314
//进程是独立的，线程互斥锁需不需要放在共享内存里面。

typedef struct {
    char msg[MSG_SIZE];
    pthread_mutex_t mutex;
    int need_init;
} shared_memory_t;

void init_sync_data(shared_memory_t *shm) {
    if (shm->need_init == NEED) return;
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shm->mutex, &mutex_attr);
    memset(shm->msg, 0, MSG_SIZE);
    shm->need_init = NEED;
}

int main() {
    key_t key = ftok(SHM_KEY_PATH, SHM_PROJ_ID);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    //创建或者获取共享内存

    int shmid = shmget(key, sizeof(shared_memory_t), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    //映射共享内存
    shared_memory_t *shm_ptr = (shared_memory_t *)shmat(shmid, NULL, 0);
    if (shm_ptr == (void *)-1) {
        perror("shmat");
        exit(1);
    }
    init_sync_data(shm_ptr);
    char cmd;
    int ch;
    while (1) {
        printf("请输入指令[r|w|q]:\n");
        scanf("%c", &cmd);
        while ((ch = getchar()) != '\n' && ch != EOF);
        if (cmd == 'r') {
            pthread_mutex_lock(&shm_ptr->mutex);
            printf("MSG:%s\n", shm_ptr->msg);
            pthread_mutex_unlock(&shm_ptr->mutex);
        } else if (cmd == 'w') {
            printf("请输入内容:\n");
            int ret = pthread_mutex_lock(&shm_ptr->mutex);
            printf("ret = %d\n", ret);
            if (fgets(shm_ptr->msg, MSG_SIZE - 1, stdin) != NULL) {
                printf(GREEN"写入成功!\n"NONE);
            }
            pthread_mutex_unlock(&shm_ptr->mutex);
        } else if (cmd == 'q') {
            break;
        } else {
            printf(RED"无效的操作！\n"NONE);
        }
    }
    shmdt(shm_ptr);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
