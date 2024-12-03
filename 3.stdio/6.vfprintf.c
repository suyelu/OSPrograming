/*************************************************************************
	> File Name: 6.vfprintf.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include <stdio.h>
#include <stdarg.h>
void m_log(FILE *fp, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(fp, format, args);
    va_end(args);
    fflush(fp);
}

int main() {
    FILE *fp = fopen("./.test.txt", "w");
    if (fp == NULL) {
        perror("fopen");
        return 1;
    }
    m_log(fp, "Hello World\n");
    m_log(fp, "%d+%d\n", 1, 2);
    return 0;
}
