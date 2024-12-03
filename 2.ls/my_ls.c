/*************************************************************************
	> File Name: my_ls.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "./common/head.h"
#define MAXNAME 256
#define MAXFILE 1024

int l_flag = 0, a_flag = 0, bg_c, fg_c, nargs;

void do_ls(const char *dir);

int main(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "la")) != -1) {
        switch (opt) {
            case 'l':
                l_flag = 1;
                break;
            case 'a':
                a_flag = 1;
                break;
            default:
                fprintf(stderr, "Usage : %s [-al]!\n", argv[0]);
                exit(1);
        }
    }
    
    DBG(YELLOW"<D>"NONE": optind = %d\n", optind);
    DBG(YELLOW"<D>"NONE": l_flag = %d, a_flag = %d\n", l_flag, a_flag);
    DBG(YELLOW"<D>"NONE": argv[optind] = %s\n", argv[optind]);
    DBG(YELLOW"<D>"NONE": argv[0] = %s\n", argv[0]);
    
    argc -= optind;
    argv += optind;

    nargs = argc;

    if (argc == 0) {
        DBG(YELLOW"<D>"NONE": Doing with The Current Dir .\n");
        do_ls(".");
    } else {
        while (argc--) {
            do_ls(*(argv++));
            DBG(YELLOW"<D>"NONE": Doing with %s\n", *(argv - 1));
        }
    }

    return 0;
}

int cmp_name(const void* _a, const void* _b) {
    char *a = (char *)_a;
    char *b = (char *)_b;
    for (int i = 0; i < strlen(a); i++) {
        a[i] = tolower(a[i]);
    } 
    for (int i = 0; i < strlen(b); i++) {
        b[i] = tolower(b[i]);
    } 
    return strcmp(a, b);
}

void size_window(char names[][MAXNAME], int cnt, int *row, int *col) {
    struct winsize ws;
    int total_len = 0, max = 0, len[MAXFILE] = {0};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) < 0) {
        perror("ioctl");
        exit(1);
    }
    DBG(YELLOW"<D>"NONE": winsize.col = %d, winsize.row = %d\n", ws.ws_col, ws.ws_row);
    for (int i = 0; i < cnt; i++) {
        len[i] = strlen(names[i]);
        if (max < len[i]) max = len[i];
        total_len += len[i] + 2;
    }

    if (max + 2 >= ws.ws_col) {
        *row = cnt;
        *col = 1;
        return ;
    }

    if (total_len < ws.ws_col) {
        *row = 1;
        *col = cnt;
        return ;
    }
    int try_begin = 0;

    for (int i = 0, tmp = 0; i < cnt; i++) {
        tmp += len[i] + 2;
        if (tmp > ws.ws_col) {
            try_begin = i;
            break;
        }
    }

    DBG(YELLOW"<D>"NONE": try_begin is %d.\n", try_begin);

    for (int i = try_begin; ; i--) {
        int *wide = (int *)malloc(sizeof(int) * i);
        int sum = 0;
        memset(wide, 0, sizeof(int) * i);
        *row = ceil(cnt * 1.0 / i);
        for (int x = 0; x < i; x++) {
            for (int y = x * *row; y < (x + 1) * *row && y < cnt; y++) {
                if (wide[x] < len[y]) wide[x] = len[y];
            }
            sum += wide[x] + 2;
        }
        if (sum > ws.ws_col) continue;
        if (sum <= ws.ws_col) {
            *col = i;
            break;
        }
    }
    DBG(YELLOW"<D>"NONE": file will put in %d*%d  window..\n", *row, *col);
}

void update_color(mode_t mode) {
    bg_c = 0;
    fg_c = 37;
    if (mode & (S_IXGRP | S_IXUSR | S_IXOTH)) {
        fg_c = 32;
    }
    switch (mode & S_IFMT) {
        case S_IFDIR:
            fg_c = 34;
            break;
        case S_IFLNK:
            fg_c = 36;
            break;
    }
    return;
}

void show_files(char names[][MAXNAME], int cnt, int row, int col) {
    int wide[col];
    memset(wide, 0, sizeof(int) * col);
    for (int i = 0; i < col; i++) {
        for (int j = i * row; j < (i + 1) * row && j < cnt; j++) {
            if (wide[i] < strlen(names[j])) wide[i] = strlen(names[j]);
        }
    }
    for (int i = 0; i < row; i++) {
        for (int j = i; j < (row * col) + i && j < cnt; j += row) {
            struct stat t_st;
            lstat(names[j], &t_st);
            update_color(t_st.st_mode);
            printf("\033[%d;%dm%-*s\033[0m", bg_c, fg_c, wide[j / row] + 2, names[j]);
        }
        printf("\n");
    }
    
}

const char* time_to_str(time_t timestamp) {
    static char time_str[64];
    struct tm *time_info;

    time_info = localtime(&timestamp);

    if (time_info != NULL) {
        strftime(time_str, sizeof(time_str), "%h %d %H:%M", time_info);
    } else {
        snprintf(time_str, sizeof(time_str), "Invalid time");
    }
    return time_str;
}


const char *uid_to_name(uid_t uid) {
    static char user_name[64];
    struct passwd *pwd = getpwuid(uid);
    if (pwd != NULL) {
        snprintf(user_name, sizeof(user_name), "%s", pwd->pw_name);
    } else {
        snprintf(user_name, sizeof(user_name), "Unknown");
    }
    return user_name;
}

const char *gid_to_name(gid_t gid) {
    static char group_name[64];
    struct group *grp = getgrgid(gid);
    if (grp != NULL) {
        snprintf(group_name, sizeof(group_name), "%s", grp->gr_name);
    } else {
        snprintf(group_name,sizeof(group_name) , "Unknown");
    }
    return group_name;
}


const char * mode_to_str(mode_t mode) {
    static char str[11];
    switch (mode & S_IFMT) {
        case S_IFREG:
            str[0] = '-';
            break;
        case S_IFDIR:
            str[0] = 'd';
            break;
        case S_IFCHR:
            str[0] = 'c';
            break;
        case S_IFBLK:
            str[0] = 'b';
            break;
        case S_IFIFO:
            str[0] = 'p';
            break;
        case S_IFSOCK:
            str[0] = 's';
            break;
        case S_IFLNK:
            str[0] = 'l';
            break;
        default:
            str[0] = '?';
            break;
    }
    str[1] = (mode & S_IRUSR) ? 'r' : '-';
    str[2] = (mode & S_IWUSR) ? 'w' : '-';
    str[3] = (mode & S_IXUSR) ? 'x' : '-';
    str[4] = (mode & S_IRGRP) ? 'r' : '-';
    str[5] = (mode & S_IWGRP) ? 'w' : '-';
    str[6] = (mode & S_IXGRP) ? 'x' : '-';
    str[7] = (mode & S_IROTH) ? 'r' : '-';
    str[8] = (mode & S_IWOTH) ? 'w' : '-';
    str[9] = (mode & S_IXOTH) ? 'x' : '-';

    str[10] = '\0';

    return str;
}

void show_file_with_long_format(const char *filename, struct stat *st) {
    printf("%s ", mode_to_str(st->st_mode));
    printf("%4ld ", st->st_nlink);
    printf("%6s ", uid_to_name(st->st_uid));
    printf("%6s ", gid_to_name(st->st_gid));
    printf("%10ld ", st->st_size);
    printf("%s ", time_to_str(st->st_mtime));
    printf("\033[%d;%dm%s\033[0m\n", bg_c, fg_c, filename);
}

void do_ls(const char *dir) {
    DIR *dirp = NULL;
    struct dirent *direntp;
    if ((dirp = opendir(dir)) == NULL) {
        if (access(dir, R_OK) == 0) {
            struct stat tmp_st;
            lstat(dir, &tmp_st);
            update_color(tmp_st.st_mode);
            if (l_flag == 1) {
                show_file_with_long_format(dir, &tmp_st);
            } else {
                printf("\033[%d;%dm%s\n\033[0m", bg_c, fg_c, dir);
            }
        } else {
            perror(dir);
            exit(1);
        }
    } else {
        char names[MAXFILE][MAXNAME] = {0};
        int cnt = 0;
        if (nargs > 1) printf("%s:\n", dir);
        while ((direntp = readdir(dirp)) != NULL) {
            if (direntp->d_name[0] == '.' && (a_flag == 0)) continue;
            strcpy(names[cnt++], direntp->d_name);
        }
        // sort
        qsort(names, cnt, MAXNAME, cmp_name);
        chdir(dir);
        if (l_flag == 1) {
            for (int i = 0; i < cnt; i++) {
                struct stat t_st;
                lstat(names[i], &t_st);
                update_color(t_st.st_mode);
                show_file_with_long_format(names[i], &t_st);
            }
        } else {
            int row, col;
            size_window(names, cnt, &row, &col);
            //此处修改
            show_files(names, cnt, row, col);
        }
    }
}
