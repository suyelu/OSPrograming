/*************************************************************************
	> File Name: 2.fifo.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2025
 ************************************************************************/

#include "./common/head.h"
#define FIFO_PATH "fifo.txt"
#define BUFF_SIZE 256
void write_mode() {
    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd < 0) {
        perror("打开FIFO错误");
        exit(1);
    }
    char buf[BUFF_SIZE + 5] = {0};
    printf("请输入数据,按CTRL+D结束:\n");
    while (fgets(buf, BUFF_SIZE, stdin)) {
        write(fd, buf, strlen(buf));
    }
    close(fd);
}

void read_mode() {
    printf("准备打开管道文件中!\n");
    int fd = open(FIFO_PATH, O_RDONLY);
    printf("已经打开管道文件!\n");
    if (fd < 0) {
        perror("打开FIFO错误");
        exit(1);
    }
    char buf[BUFF_SIZE + 5] = {0};
    printf("等待数据中:\n");
    while (1) {
        ssize_t nread = read(fd, buf, BUFF_SIZE);
        if (nread > 0) {
            printf("收到:%s", buf);
        } else if (nread == 0) {
            printf("对端已关闭!\n");
            break;
        } else {
            fprintf(stderr, "读取失败!\n");
            break;
        }
        memset(buf, 0, BUFF_SIZE + 5);
    }

    close(fd);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [read|write]\n", argv[0]);
        exit(1);
    }
    if (access(FIFO_PATH, F_OK) == -1) {
        if (mkfifo(FIFO_PATH, 0666) != 0) {
            perror("创建FIFO错误");
            exit(1);
        }
    } 

    if (strcmp(argv[1], "write") == 0) {
        write_mode();
    } else if (strcmp(argv[1], "read") == 0) {
        read_mode();
    } else {
        fprintf(stderr, "Usage: %s [read|write]\n", argv[0]);
        exit(1);
    }
    return 0;
}
