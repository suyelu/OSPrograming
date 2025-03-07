/*************************************************************************
	> File Name: 3.signal_action.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "./common/head.h"
int i = 0;

void restart(int signum) {
    i = 0;
}

int main() {
    struct sigaction sa;
    sa.sa_handler = restart;

    sigaction(SIGINT, &sa, NULL);

    for(; i < 10000; i++) {
        printf("%d\n", i);
        sleep(1);
    }
    
    return 0;
}
