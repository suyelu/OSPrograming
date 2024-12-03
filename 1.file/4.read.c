/*************************************************************************
	> File Name: 4.read.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created Time: Mon 06 May 2024 08:52:03 PM CST
 ************************************************************************/

#include "./common/head.h"

int main() {
    int fd;
    if ((fd = open("./x.c", O_RDWR | O_CREAT, 0644)) < 0) {
        perror("open");
        exit(1);
    }
    char buff[10] = {0};
    int buff_size = sizeof(buff);
    while (1) {
        memset(buff, 0, buff_size);
        ssize_t rsize = read(fd, buff, buff_size - 1);
        if (rsize < 0) {
            perror("read");
            exit(1);
        } else if (rsize == 0) {
            break;
        } else {
            printf("%s", buff);
        }
        
    }
    return 0;
}
