/*************************************************************************
	> File Name: common.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "../common/head.h"

void init_child_pid(){
    max_fd = sysconf(_SC_OPEN_MAX); 
    child_pid = calloc(max_fd, sizeof(pid_t));
}

int make_nonblock(int fd) {
    int flag;
    if ((flag = fcntl(fd, F_GETFL)) < 0) {
        return -1;
    }
    flag |= O_NONBLOCK;
    return fcntl(fd, F_SETFL, flag);
}

int make_block(int fd) {
    int flag;
    if ((flag = fcntl(fd, F_GETFL)) < 0) {
        return -1;
    }
    flag &= ~O_NONBLOCK;
    return fcntl(fd, F_SETFL, flag);
}



FILE *mypopen(const char *cmd, const char *type) {
    if (!child_pid) init_child_pid();
    if (!cmd || !type || (*type != 'r' && *type != 'w')) return NULL;
    int pipefd[2];
    if (pipe(pipefd) < 0) {
        return NULL;
    }
    //fork()
    pid_t pid = fork();
    if (pid < 0) return NULL;
    if (pid == 0) {
        //子进程
        if (*type == 'r') {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
        } else {
            close(pipefd[1]);
            dup2(pipefd[0], STDOUT_FILENO);
            close(pipefd[0]);
            
        }
        execl("/bin/sh", "sh", "-c", cmd, (char *)NULL);
    } else {
        //父进程
        FILE *fp;
        if (*type == 'r') {
            close(pipefd[1]);
            fp = fdopen(pipefd[0], "r");
        } else {
            close(pipefd[0]);
            fp = fdopen(pipefd[1], "w");
        }
        if (fp == NULL) return NULL;
        //进程和文件怎么关联fp->pid
        int fd = fileno(fp);
        if (fd >= 0 && fd < max_fd) { 
            child_pid[fd] = pid;

        }
        return fp;
    }
}

int mypclose(FILE *fp) {
    if (fp == NULL) return -1;
    int fd = fileno(fp);
    if (fd < 0 || fd >= max_fd || child_pid[fd] == 0) return -1;

    pid_t pid = child_pid[fd];
    child_pid[fd] = 0;
    fclose(fp);

    int status;
    struct rusage usage;
    if (wait4(pid, &status, 0,&usage) < 0) return -1;
    DBG(GREEN"User time: %ld.%06ld", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
    return status;
}

