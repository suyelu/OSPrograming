/*************************************************************************
	> File Name: 1.count.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "./common/head.h"
int main() {
    int i = 0;
    for (; i < 100000; i++) {
        printf("%d\n", i);
        sleep(1);
    }
    return 0;
}
