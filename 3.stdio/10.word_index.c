/*************************************************************************
	> File Name: 10.word_index.c
	> Author: suyelu
	> Mail: suyelu@hotmail.com
	> Created in HangZhou, Year: 2024
 ************************************************************************/

#include "../common/head.h"

#define MAX_WORD_LEN 50
#define MAX_LINE_LEN 1024
#define MAX_WORDS 1000

typedef struct {
    char word[MAX_WORD_LEN];
    int line_numbers[MAX_LINE_LEN];
    int cnt;
} IndexItem;

typedef struct {
    IndexItem items[MAX_WORDS];
    int size;
} IndexDB;

void init_index(IndexDB *db) {
    db->size = 0;
}

void add_to_index(IndexDB *db, const char *word, int line_num) {
    //单词是不是已经在索引中
    for (int i = 0; i < db->size; i++) {
        if (strcmp(word, db->items[i].word) == 0) {
            if (db->items[i].line_numbers[db->items[i].cnt-1] != line_num) {
                db->items[i].line_numbers[db->items[i].cnt++] = line_num;
            } 
            return ;
        }
    }

    if (db->size < MAX_WORDS) {
        strcpy(db->items[db->size].word, word);
        db->items[db->size].cnt = 1;
        db->items[db->size].line_numbers[0] = line_num;
        db->size++;
    }
}


void build_index(IndexDB *db, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return;
    char line[MAX_LINE_LEN];
    char word[MAX_WORD_LEN];
    int line_num = 0;
    while (fgets(line, sizeof(line), fp)) {
        line_num++;
        char *ptr = line;
        int word_len = 0;
        while (*ptr) {
            if (isalnum(*ptr)) {
                word[word_len++] = tolower(*ptr);
            } else {
                if (word_len > 0) {
                    word[word_len] = '\0';
                    add_to_index(db, word, line_num);
                    word_len = 0;
                }
            }
            ptr++;
        }
        if (word_len > 0) {
            word[word_len] = '\0';
            add_to_index(db, word, line_num);
        }
    }
    fclose(fp);
}


void search_word(const IndexDB *db, const char *word) {
    for (int i = 0; i < db->size; i++) {
        if (strcmp(db->items[i].word, word) == 0) {
            printf("%s 已找到！存在于以下行：\n", word);
            for (int j = 0; j < db->items[i].cnt; j++) {
                printf("%d ", db->items[i].line_numbers[j]);
            }
            printf("\n");
            return;
        }
    }
    printf("%s 未找到！\n", word);
    return ;
}


int main(int argc, char **argv) {
    IndexDB db;
    init_index(&db);
    build_index(&db, ".words.txt");
    for (int i = 0; i < argc - 1; i++) {
        search_word(&db, argv[i+1]);
    }
    return 0;
}
