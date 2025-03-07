/*************************************************************************
	> File Name: 4.orphan.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "./common/head.h"

int main() {
    pid_t pid;
    pid = fork();

    if (pid > 0) {
        sleep(20);
    } else {
        sleep(1000);
    }

    return 0;
}
