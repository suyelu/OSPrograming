/* ************************************************************************
    > File Name:     13.lseek.c
    > Author:        suyelu
    > mail:          suyelu@hotmail.com
    > Created Time:  Thu 31 Oct 2024 06:03:59 PM CST
    > Description:   
 ************************************************************************/

#include "./common/head.h"
#define MAX_SIZE 512
int main() {
    int fd = open("./test.txt", O_RDONLY);
    if (fd == -1) {
        perror("Failed to open the file");
        exit(EXIT_FAILURE);
    }

    off_t len = lseek(fd, 0, SEEK_END);
    if (len == -1) {
        perror("Failed to get the length of the file");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    printf("The length of the file is %ld\n", len);

    off_t mid_pos = len / 2;

    //lseek(fd, mid_pos, SEEK_SET);
    if (lseek(fd, mid_pos, SEEK_SET) == -1) {
        perror("Failed to move the cursor");
        close(fd);
        exit(EXIT_FAILURE);
    }

    char buff[MAX_SIZE + 5] = {0};
    ssize_t nread = read(fd, buff, MAX_SIZE);
    if (nread < 0) {
        perror("Failed to read the file");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    buff[nread] = '\0';
    printf("The content of the file is %s\n", buff);

    close(fd);
    return 0;
}
