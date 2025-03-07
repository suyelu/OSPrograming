/*************************************************************************
	> File Name: 6.wait.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "./common/head.h"

int main() {
    pid_t pid;
    for (int i = 0; i < 10; i++) {
        pid  = fork();
        if (pid == 0) break;
    }
    
    if (pid == 0) printf("I'm the child!\n");

    if (pid > 0) {
        for (int i = 0; i < 10; i++)
            wait(NULL);
        printf("I'm the Parent!\n");
    }

    return 0;
}
