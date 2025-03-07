/*************************************************************************
	> File Name: 7.wait_status.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "./common/head.h"

int main() {
    pid_t pid;
    int wstatus;
    if ((pid = fork()) < 0) {
        perror("fork()");
        exit(1);
    }
    if (pid > 0) {
        pid_t wpid = wait(&wstatus);
        printf("return value is %d, wstatus = %d\n", wpid, wstatus);
        printf("the return value of child process is %d \n", WEXITSTATUS(wstatus));
    } else {
        printf("I'm the child with pid = %d\n", getpid());
        return 3;
    }

    return 0;
}
