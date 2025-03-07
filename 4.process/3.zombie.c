/*************************************************************************
	> File Name: 3.zombie.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "./common/head.h"
int main() {
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        exit(1);
    }
    sleep(10000);
    return 0;
}
