/*************************************************************************
	> File Name: 1.pipe.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "./common/head.h"
int main() {
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if (pid >0 ) {
        //写
        close(fd[0]);
        char msg[] = "Hello World!";
        write(fd[1], msg, strlen(msg));
        close(fd[1]);
    } else if (pid == 0) {
        //读
        close(fd[1]);
        char buff[256] = {0};
        int n = read(fd[0], buff, sizeof(buff));
        printf("子进程收到: %s\n", buff);
        close(fd[0]);
    }
    return 0;
}
