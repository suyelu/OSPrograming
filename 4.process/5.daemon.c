/*************************************************************************
	> File Name: 5.daemon.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "./common/head.h"

void init_daemon() {
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        perror("Fork error");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        sleep(2);
        exit(EXIT_SUCCESS);
    }
    //子进程需要做的事情

    if (setsid() < 0) {
        perror("setsid");
        exit(EXIT_FAILURE);
    }

    if (chdir("/") < 0) {
        perror("chdir");
        exit(EXIT_FAILURE);
    }

    FILE *pid_file = fopen("/run/tbd.pid", "w");
    freopen("/dev/null", "r", stdin);
    freopen("/dev/null", "w", stdout);
    freopen("/dev/null", "w", stderr);
}

void broadcast_time() {
    DIR *dir;
    struct dirent *entry;
    char *dev_path = "/dev/pts";
    char full_path[512];
    time_t now;
    struct tm *timeinfo;
    char msg[512] = {0};
    int fd;
    time(&now);
    timeinfo = localtime(&now);
    snprintf(msg, sizeof(msg) - 1, "\n您好，当前时间是：%02d:%02d:%02d\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    dir = opendir(dev_path);
    if (dir == NULL) {
        perror("Failed to open dev_path");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] < '0' || entry->d_name[0] > '9')  continue;
        memset(full_path, 0, sizeof(full_path));
        snprintf(full_path, sizeof(full_path), "%s/%s", dev_path, entry->d_name);
        fd = open(full_path, O_WRONLY | O_NONBLOCK);
        if (fd < 0) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        write(fd, msg, strlen(msg));
        close(fd);
    } 
    closedir(dir);

}

int main() {
    //实现一个守护进程
    init_daemon();

    while (1) {
        time_t now;
        struct tm *timeinfo;
        time(&now);
        timeinfo = localtime(&now);

        if (timeinfo->tm_min == 0 && timeinfo->tm_sec == 0) {
        //if (timeinfo->tm_sec == 0) {
            //Do  broadcast
            //printf("整点！\n");
            broadcast_time();
        }
        sleep(1);
    }

    return 0;
}
