/*************************************************************************
	> File Name: 14.fcntl_lock.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "./common/head.h"

void write_to_log(const char* log_message){
    int fd = open("./13.log", O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("Failed to open log file");
        exit(2);
    }
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;

    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Failed to lock the file");
        close(fd); 
        exit(1);
    }

    if (lseek(fd, 0, SEEK_END) == -1) {
        perror("Failed ti seek to eof");
        close(fd);
        exit(1);
    }
    ssize_t wnum = write(fd, log_message, strlen(log_message));
    if(wnum == -1) {
        perror("Failed to write the log");
        close(fd);
        exit(1);
    }
    write(fd, "\n", 1);
    printf("Write to log file success!\n");
    sleep(5);
    
    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Failed to unlock the file");
        close(fd);
        exit(1);
    }
    close(fd);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s log_message\n", argv[0]);
        exit(1);
    }
    write_to_log(argv[1]);
    return 0;
}
