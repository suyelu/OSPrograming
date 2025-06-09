/*************************************************************************
	> File Name: 1.chatroom.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "./common/head.h"

#define CHAT_FILE "/tmp/chat_file.txt"
#define MAX_MSG_LEN 512

int running = 1;

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

void signal_handler(int sig) {
    running = 0;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage : %s <username>\n", argv[0]);
        return 1;
    }
   
    signal(SIGINT, signal_handler);

    const char *username = argv[1];
    char msg[MAX_MSG_LEN];

    printf(L_RED"欢迎您进入聊天室，输入消息，按回车发送！\n"NONE);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        while(running) {
            system("clear");
            recv_chat();
            printf(">");
            fflush(stdout);
            sleep(1);
        }
        exit(0);
    } else {
        while (running) {
            if (fgets(msg, sizeof(msg), stdin) == NULL) {
                printf("退出聊天室\n");
                break;
            }
            msg[strcspn(msg, "\n")] = '\0';
            if (strcmp(msg, "/exit") == 0) {
                printf("聊天室马上关闭...\n");
                sleep(2);
                break;
            }
            if (strlen(msg)) send_msg(username, msg);
        }
        kill(pid, SIGINT);
        wait(NULL);
        printf("子进程资源已销毁，聊天室结束。");
    }

    return 0;
}
