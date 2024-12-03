/*************************************************************************
	> File Name: 1.std_file_stream.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include <stdio.h>
#include <errno.h>
int main() {
    int a, b;
    scanf("%d%d", &a, &b);
    printf("a+b=%d\n", a + b);
    errno = 13;
    fprintf(stderr, "This is  Error Message!\n");
    return 0;
}
