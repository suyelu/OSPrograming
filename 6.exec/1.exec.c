/*************************************************************************
	> File Name: 1.exec.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#define _GNU_SOURCE
#include "./common/head.h"
int main() {
    extern char **environ;
    for (int i = 0; environ[i] != NULL; i++) {
        printf("%s\n", environ[i]);
    }

    char *args[] = {
        "ls111",
        ".",
        "/",
        NULL
    };

    char *env[] = {
        "PATH=.",
        NULL
    };

    printf("pid is %d\n", getpid());

    execvpe("testexec", args, env);

    printf("Program is Over!\n");
    return 0;
}
