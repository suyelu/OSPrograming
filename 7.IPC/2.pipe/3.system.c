/*************************************************************************
	> File Name: 3.system.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2025
 ************************************************************************/

#include "./common/head.h"
int main() {
    int ret = system(NULL);
    printf("ret = %d\n", ret);

    system("ls -a");

    system("sleep 20");

    system("echo Done");



    return 0;
}
