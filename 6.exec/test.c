/*************************************************************************
	> File Name: test.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "./common/head.h"
int main(int argc, char **argv){
    pid_t pid;
    pid = getpid();
    printf("pid = %d\n", pid);
    printf("argv[0] = %s\n", argv[0]);
    extern char **environ;
    for (int i = 0; environ[i] != NULL; i++) {
        printf("%s\n", environ[i]);
    } 

    return 0;
}
