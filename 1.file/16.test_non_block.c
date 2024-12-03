/*************************************************************************
	> File Name: 16.test_non_block.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "./common/head.h"
int main() {
    int age;
    make_nonblock(0); // 0 1 2
    int ret = scanf("%d", &age);
    if (ret < 0) {
        perror("scanf");
        exit(1);
    }
    printf("suyelu is %d years old!\n", age);
    return 0;
}
