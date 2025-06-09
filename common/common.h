/*************************************************************************
	> File Name: common.h
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H
static int max_fd = 0;
static pid_t *child_pid = NULL;
int make_block(int fd);
int make_nonblock(int fd);
void init_child_pid();
FILE *mypopen(const char *cmd, const char *type);
int mypclose(FILE *fp);
#endif
