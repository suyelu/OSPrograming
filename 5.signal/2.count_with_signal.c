/*************************************************************************
	> File Name: 1.count.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "./common/head.h"
int i = 0;
void restart(int signum){
     i = 0;
}
int main() {
    signal(SIGINT, restart);
    for (; i < 100000; i++) {
        printf("%d\n", i);
        sleep(1);
    }

    return 0;
}
