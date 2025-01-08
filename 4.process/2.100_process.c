/*************************************************************************
	> File Name: 2.100_process.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "./common/head.h"
int main() {
    pid_t pid;
    int no;
    for (int i = 0; i < 100; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork()");
            exit(1);
        } else if (pid == 0) {
            no = i;
            break;
        }
    }
    if (pid == 0)  {
        printf("<%d>th child, pid = %d.\n", no, getpid());
    } else {
        printf("This is parent!\n");
    }
    return 0;
}
