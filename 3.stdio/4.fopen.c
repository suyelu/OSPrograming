/*************************************************************************
	> File Name: 4.fopen.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp = NULL;
    fp = fopen("./.input.txt", "r");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }
    char buffer[1024] = {0};
    //fread(buffer, 1, 1024, fp);
    int a, b;
    fscanf(fp, "%d%d", &a, &b);
    printf("a+b = %d\n", a+b);
    fclose(fp);
    return 0;
}
