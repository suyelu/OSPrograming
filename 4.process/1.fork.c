/*************************************************************************
	> File Name: 1.fork.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "./common/head.h"

int main() {
    pid_t pid;
    printf("this is before fork()\n");
    if ((pid = fork()) < 0) {
        perror("fork()");
        exit(1);
    }
    if (pid > 0) {
        printf("I'm the parent process! <%d>\n", pid);
        exit(12);
    } else {
        printf("I'm the child process! <pid = %d>\n", getpid());
        exit(1);
    }
    return 0;
}
