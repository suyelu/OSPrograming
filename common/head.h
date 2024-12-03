/*************************************************************************
	> File Name: head.h
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created Time: Mon 06 May 2024 08:50:07 PM CST
 ************************************************************************/

#ifndef _HEAD_H
#define _HEAD_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <utime.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <math.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdarg.h>

#include "common.h"
#include "color.h"


#ifdef _D
#define DBG(fmt, args...) printf(fmt, ##args)
#else
#define DBG(fmt, args...) 
#endif

#endif
