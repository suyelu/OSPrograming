/*************************************************************************
	> File Name: 1.shm_chat.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2025
 ************************************************************************/

#include "./common/head.h"

#define SHM_KEY_PATH "/tmp/myshm.key"
#define SHM_PROJ_ID 666
#define SHM_SIZE 1024

int main() {
    key_t key = ftok(SHM_KEY_PATH, SHM_PROJ_ID);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    //创建或者获取共享内存

    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    //映射共享内存
    char *shm_ptr = shmat(shmid, NULL, 0);
    if (shm_ptr == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    char cmd;
    char buff[SHM_SIZE];
    int ch;
    while (1) {
        printf("请输入指令[r|w|q]:\n");
        scanf("%c", &cmd);
        while ((ch = getchar()) != '\n' && ch != EOF);
        if (cmd == 'r') {
            printf("MSG:%s\n", shm_ptr);
        } else if (cmd == 'w') {
            printf("请输入内容:\n");
            if (fgets(shm_ptr, SHM_SIZE - 1, stdin) != NULL) {
                printf(GREEN"写入成功!\n"NONE);
            }
        } else if (cmd == 'q') {
            break;
        } else {
            printf(RED"无效的操作！\n"NONE);
        }
    }
    shmdt(shm_ptr);

    return 0;
}
