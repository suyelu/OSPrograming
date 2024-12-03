/*************************************************************************
	> File Name: 6.cp.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created Time: Mon 06 May 2024 09:25:26 PM CST
 ************************************************************************/

#include "./common/head.h"

void copy_file(const char *src, const char *dest) {
    int src_fd, dest_fd;
    if ((src_fd = open(src, O_RDONLY)) < 0) {
        perror("open src");
        exit(1);
    }
    if ((dest_fd = open(dest, O_RDWR | O_CREAT, 0600)) < 0) {
        perror("open dest");
        exit(1);
    }
    const size_t BUFFSIZE = 5;
    char buff[BUFFSIZE + 1];
    ssize_t rsize = 0;
    memset(buff, 0, BUFFSIZE + 1);

    while ((rsize = read(src_fd, buff, BUFFSIZE)) > 0) {
        write(dest_fd, buff, rsize);
    }
    close(src_fd);
    close(dest_fd);

}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage : %s <source_file> <destination_file>\n", argv[0]);
        exit(1);
    }

    copy_file(argv[1], argv[2]);

    return 0;
}

