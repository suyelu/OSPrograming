/*************************************************************************
	> File Name: 4.popen.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2025
 ************************************************************************/

#include "./common/head.h"
int main() {
    FILE *fp;
    char buff[512] = {0};

    fp = popen("ls -1", "r");
    if (fp == NULL) {
        perror("popen()");
        exit(1);
    }

    printf("命令正在执行中,输出如下:\n");
    while (fgets(buff, sizeof(buff) - 1, fp) != NULL) {
        printf("%s", buff);
        memset(buff, 0, sizeof(buff));
    }

    pclose(fp);

    return 0;
}
