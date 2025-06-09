/*************************************************************************
	> File Name: 1.chatroom.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "./common/head.h"

#define CHAT_FILE "/tmp/chat_file.txt"
#define MAX_MSG_LEN 512

void get_time_str(char *buff, size_t size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buff, size, "[%H:%M:%S]", tm_info);
}

void send_msg(const char *username, const char *msg) {
    int fd = open(CHAT_FILE, O_CREAT | O_WRONLY | O_APPEND, 0666);
    if (fd == -1) {
        perror("打开聊天文件错误");
        return;
    }

    flock(fd, LOCK_EX);
    FILE *fp  = fdopen(fd, "a");
    if (fp == NULL) {
        perror("文件指针创建失败");
        close(fd);
        return;
    }
    char timestr[16];
    get_time_str(timestr, sizeof(timestr));
    fprintf(fp, "%s %s: %s\n", timestr, username, msg);
    fflush(fp);
    fclose(fp);

}

void recv_chat() {
    FILE *fp = fopen(CHAT_FILE, "r");
    if (fp == NULL) {
        perror("打开聊天文件错误");
        return;
    }
    char line[MAX_MSG_LEN];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage : %s <username>\n", argv[0]);
        return 1;
    }
    
    const char *username = argv[1];
    char msg[MAX_MSG_LEN];

    printf(L_RED"欢迎您进入聊天室，输入消息，按回车发送！\n"NONE);

    while (1) {
        printf("> ");
        fflush(stdout);
        memset(msg, 0, sizeof(msg));
        if (fgets(msg, sizeof(msg), stdin) == NULL) {
            printf("退出聊天室!\n");
            break;
        }
        msg[strcspn(msg, "\n")] = '\0';
        if (strcmp(msg, "/recv") == 0) {
            printf("接收中...");
            recv_chat();
            continue;
        }

        send_msg(username, msg);
        recv_chat();
    }
    return 0;
}
