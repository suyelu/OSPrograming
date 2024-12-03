/*************************************************************************
	> File Name: 15.fcntl_lock_part.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "./common/head.h"

int main(){
    int fd = open("15.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 100;

    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("lock");
        close(fd);
        exit(1);
    }
    DBG("First 100 bytes of the file are locked. Press Enter to continue and  unlock...\n");
    getchar();

    char buff[105];
    lseek(fd, 0, SEEK_SET);
    ssize_t rnum = read(fd, buff, 100);
    if (rnum == -1) {
        perror("read");
        close(fd);
        exit(1);
    }
    buff[rnum] = '\0';
    DBG("First 100 bytes is : \n%s\n", buff);

    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("unlock");
        close(fd);
        exit(1);
    }
    DBG("File is unlocked!\n");
    close(fd);
    return 0;
}

