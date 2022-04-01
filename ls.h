#ifndef __LIST_DIR_H_
#define __LIST_DIR_H_

#ifndef MAX_LEN
    #define MAX_LEN 265
#endif

#ifndef MAX_SZ
    #define MAX_SZ 4100
#endif

struct element {
    char Name[MAX_LEN], Path[MAX_SZ];
};

int get_total(char *);
void flag_l(char *, char *, struct stat *);
void ls_dir(char *);
void list_dir_contents (char *);

#endif
