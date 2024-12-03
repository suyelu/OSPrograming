/*************************************************************************
	> File Name: 9.logger.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "../common/head.h"

typedef enum{
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

static FILE *log_file = NULL;

int init_logger(const char *filename) {
    log_file = fopen(filename, "a");
    return log_file != NULL;
}

void close_logger() {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

const char* get_level_str(LogLevel level) {
    switch (level) {
        case LOG_INFO:
            return "INFO";
        case LOG_WARNING:
            return "WARNING";
        case LOG_ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

void log_message(LogLevel level, const char *format, ...) {
    if (!log_file) return;

    time_t now;
    time(&now);
    char timestr[26];
    ctime_r(&now, timestr);
    timestr[24] = '\0';
    fprintf(log_file, "[%s] [%s] ", timestr, get_level_str(level));

    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);
    fprintf(log_file, "\n");
    fflush(log_file);
}

int main() {
    // 初始化日志系统
    if (!init_logger("app.log")) {
        printf("无法创建日志文件!\n");
        return 1;
    }

    // 记录不同级别的日志
    log_message(LOG_INFO, "系统启动成功");
    log_message(LOG_WARNING, "磁盘空间不足: %d%%", 15);
    log_message(LOG_ERROR, "数据库连接失败: %s", "Connection timeout");

    // 关闭日志系统
    close_logger();
    return 0;
}

