/*************************************************************************
	> File Name: 5.freopen.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
int main() {
    if (freopen(".grades.txt", "r", stdin) == NULL) {
        perror(".grades.txt");
        exit(1);
    }
    if (freopen(".results.txt", "w", stdout) == NULL) {
        perror(".results.txt");
        exit(1);
    }
    int grade;
    int cnt = 0;
    int sum = 0;

    while (scanf("%d", &grade) != EOF) {
        sum += grade;
        printf("[%dth] is %d\n", cnt++, grade);
    }

    printf("Avg is %d.\n", sum / cnt);

    return 0;
}
