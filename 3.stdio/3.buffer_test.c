/*************************************************************************
	> File Name: 3.buffer_test.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "../common/head.h"

#define BUFFER_SIZE 123
#define TEST_SIZE 1000000

void buffered_write(int fd, const char *data, size_t count) {
    char *buffer = malloc(BUFFER_SIZE);
    size_t buffer_pos = 0;
    for (size_t i = 0; i < count; i++) {
        buffer[buffer_pos++] = data[0];
        if (buffer_pos == BUFFER_SIZE) {
            write(fd, buffer, buffer_pos);
            buffer_pos = 0;
        }
    }
    if (buffer_pos > 0) {
        write(fd, buffer, buffer_pos);
    }
    free(buffer);
}

void test_buffered_io() {
    int fd = open(".test_buffer.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return ;
    }
    char data = 'a';
    buffered_write(fd, &data, TEST_SIZE);
    close(fd);
}


void test_unbuffered_io() {
    int fd = open(".test_unbuffer.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return ;
    }
    char data = 'a';
    for (int i = 0; i < TEST_SIZE; i++) {
        write(fd, &data, 1);
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    int use_buffer = 0;
    int opt;
    //获取选项
    while ((opt = getopt(argc, argv, "b")) != -1) {
        switch (opt) {
            case 'b':
                use_buffer = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-b]\n", argv[0]);
                exit(1);
        }
    }
    if (use_buffer) {
        printf("执行缓冲I/O测试...\n");
        test_buffered_io();
    } else {
        printf("执行无缓冲I/O测试...\n");
        test_unbuffered_io();
    }
    return 0;
}
