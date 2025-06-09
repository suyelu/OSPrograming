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
    pthread_cond_t cond;
    int need_init;
} shared_memory_t;

void init_sync_data(shared_memory_t *shm) {
    if (shm->need_init == NEED) return;

    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shm->mutex, &mutex_attr);

    pthread_condattr_t cond_attr;
    pthread_condattr_init(&cond_attr);
    pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
    pthread_cond_init(&shm->cond, &cond_attr);

    memset(shm->msg, 0, MSG_SIZE);
    shm->need_init = NEED;
}

int main(int argc, char **argv) {
    if (argc != 2 || (strcmp(argv[1], "read") != 0 && strcmp(argv[1], "write") != 0)) {
        fprintf(stderr, "Usage: %s [read|write]\n", argv[0]);
        exit(1);
    }

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
    if (strcmp(argv[1], "write") == 0) {
        while (1) {
            char input[MSG_SIZE];
            printf("请输入数据(q退出):\n");
            fgets(input, MSG_SIZE, stdin);
            if (strncmp(input, "q", 1) == 0) break;

            pthread_mutex_lock(&shm_ptr->mutex);
            strncpy(shm_ptr->msg, input, MSG_SIZE);
            pthread_cond_signal(&shm_ptr->cond);
            pthread_mutex_unlock(&shm_ptr->mutex);
        }
    } else {
        while (1) {
            pthread_mutex_lock(&shm_ptr->mutex);
            while (!strlen(shm_ptr->msg)) {
                pthread_cond_wait(&shm_ptr->cond, &shm_ptr->mutex);
            }

            printf("RECV: %s", shm_ptr->msg);
            memset(&shm_ptr->msg, 0, MSG_SIZE);
            pthread_mutex_unlock(&shm_ptr->mutex);
        }
    }
    shmdt(shm_ptr);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
