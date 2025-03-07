/*************************************************************************
	> File Name: ccode.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "./common/head.h"

#define LOG_FILE "compile.log"

void handle_signal(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

time_t get_file_mod_time(const char *filename) {
    struct stat file_stat;
    if (stat(filename, &file_stat) == 0) {
        return file_stat.st_mtime;
    }
    return 0;
} 

int compile_and_run(const char * filename) {
    char *compiler = strstr(filename, ".cpp") ? "g++" : "gcc";
    char outfilename[256] = {0};
    snprintf(outfilename, sizeof(outfilename) - 1, "%s.out", filename);
    DBG(L_BLUE"[D] 编译器: %s, 输出文件: %s"NONE"\n", compiler, outfilename);
    DBG(L_BLUE"[编译中] %s -> %s"NONE"\n", filename, outfilename);
    pid_t c_pid = fork();
    if (c_pid == 0) {
        FILE * log = freopen(LOG_FILE, "w", stderr);
        execlp(compiler, compiler, filename, "-o", outfilename, NULL);
        fclose(log);
        exit(1);
    }
    int status;
    waitpid(c_pid, &status, 0);

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        DBG(L_BLUE"[编译成功] 运行 %s"NONE"\n", outfilename);
        pid_t r_pid = fork();
        if (r_pid == 0) {
            execl(outfilename, outfilename, NULL);
            exit(1);
        }
        waitpid(r_pid, NULL, 0);
        return 0;
        
    } else {
        DBG(L_RED"[编译失败] 详细请查看%s"NONE"\n", LOG_FILE);
        for (int i = 5; i > 0; i--) {
            printf(YELLOW" %d 秒后进入编辑模式..."NONE"\r", i);
            fflush(stdout);
            sleep(1);
        }
        printf("\n");
        return 1;
    }
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return 1;
    }
    DBG(L_CYAN"[D] 启动C代码编辑器"NONE"\n");
    signal(SIGCHLD, handle_signal);
    while (1) {
        time_t last_mod_time = get_file_mod_time(argv[1]);
        DBG(L_CYAN"[编辑模式] 打开%s"NONE"\n", argv[1]);
        pid_t edit_pid = fork();
        if (edit_pid == 0) {
            execlp("vim", "vim", argv[1], NULL);
            exit(1);
        }
        waitpid(edit_pid, NULL, 0);
        time_t new_mod_time = get_file_mod_time(argv[1]);

        if (new_mod_time == last_mod_time) {
            DBG(L_RED"[退出] 检测到文件%s并未被修改"NONE"\n", argv[1]);
            exit(0);
        }
        if (compile_and_run(argv[1]) == 0) break;
    }

    return 0;
}
