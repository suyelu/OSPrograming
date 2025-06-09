/*************************************************************************
	> File Name: 5.popen_write.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2025
 ************************************************************************/

#include "./common/head.h"
#define MAX_LINE 256

int main() {
    FILE *fp;
    char line[MAX_LINE];
    fp = popen("sort", "w");
    if (fp == NULL) {
        perror("popen");
        exit(1);
    }
    printf("请输入单词，使用CTRL+D结束:\n");

    while (fgets(line, sizeof(line), stdin) != NULL) {
        fprintf(fp, "%s", line);
    }
    printf("\n");

    pclose(fp);

    return 0;
}
