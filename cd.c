#include "headers.h"

char prev_dir[MAX_SZ], oldpwd[MAX_SZ];
bool old;

void call_CD(char *path) {
    old = false;
    path = strtok(NULL, delim);
    if (path == NULL) {
        strcpy(buff2, home_dir);
    } else {
        if (strcmp("-", path) == 0) {
            if (oldpwd[0] == '\0') {
                printf("Nash: cd: OLDPWD not set\n"); 
                return;
            }
            old = true;
            strcpy(buff2, oldpwd);
        } else if (path[0] == '~') {
            strcpy(buff2, home_dir);
            strcat(buff2, path + 1);
        } else {
            strcpy(buff2, path);
        }
        path = strtok(NULL, delim);
    }
    getcwd(prev_dir, MAX_SZ);
    if (path != NULL) {
        printf("Nash: cd: too many arguments\n"); return;
    }
    int ret = chdir(buff2);
    if (ret == -1)
        printf("Nash: cd: %s: No such file or directory\n", buff2);
    else {
        if (old) {
            present_working_directory();
        }
        strcpy(oldpwd, prev_dir);
    }
    memset(buff2, '\0', strlen(buff2));
}