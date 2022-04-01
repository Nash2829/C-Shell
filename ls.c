#include "headers.h"
#include "ls.h"

int comp_str(const void *a, const void *b) {
    return strcmp((const char*)a, (const char*)b);
}

int comp_struct_element(const void *a, const void *b) {
    return strcmp((const char*)((struct element*)a)->Name, (const char*)((struct element*)b)->Name);
}

int no_of_files, no_of_dirs, err_no;
char directories[105][MAX_SZ], 
     files[105][MAX_SZ], 
     error_msg[105][MAX_SZ],
     rwx[11],
     ForPath[MAX_SZ],
     date_n_time[30],
     invalid_opt;
struct element all_elems[MAX_SZ];
bool ls_a, ls_l;

int get_total(char *path) {
    struct dirent *dent; DIR* dir = opendir(path);
    unsigned long long int total = 0;
    struct stat sbf;
    int elems = 0;
    while (true) {
        dent = readdir(dir);
        if (!dent) break;

        if (ls_a || dent->d_name[0] != '.') {
            sprintf(all_elems[elems].Path, "%s/%s", path, dent->d_name);
            stat(all_elems[elems].Path, &sbf);
            strcpy(all_elems[elems].Name, dent->d_name);
            ++elems;
            total += sbf.st_blocks;
        }
    }
    closedir(dir);
    if (ls_l)
        printf("total %llu\n", (total / 2));
    qsort(all_elems, elems, sizeof(struct element), comp_struct_element);
    return elems;
}

void flag_l(char * path, char * name, struct stat *ptr_st_bf) {
    rwx[0] = (S_ISDIR(ptr_st_bf->st_mode) ? 'd' : '-');
    rwx[1] = (ptr_st_bf->st_mode & S_IRUSR) ? 'r' : '-';
    rwx[2] = (ptr_st_bf->st_mode & S_IWUSR) ? 'w' : '-';
    rwx[3] = (ptr_st_bf->st_mode & S_IXUSR) ? 'x' : '-';
    rwx[4] = (ptr_st_bf->st_mode & S_IRGRP) ? 'r' : '-';
    rwx[5] = (ptr_st_bf->st_mode & S_IWGRP) ? 'w' : '-';
    rwx[6] = (ptr_st_bf->st_mode & S_IXGRP) ? 'x' : '-';
    rwx[7] = (ptr_st_bf->st_mode & S_IROTH) ? 'r' : '-';
    rwx[8] = (ptr_st_bf->st_mode & S_IWOTH) ? 'w' : '-';
    rwx[9] = (ptr_st_bf->st_mode & S_IXOTH) ? 'x' : '-';
    printf("%s %3lu %8.8s %8.8s %12ld", rwx, ptr_st_bf->st_nlink, (getpwuid(ptr_st_bf->st_uid)->pw_name), (getgrgid(ptr_st_bf->st_uid)->gr_name),
    ptr_st_bf->st_size);
    struct tm *t = localtime(&(ptr_st_bf->st_mtime));
    strftime(date_n_time, 30, nl_langinfo(D_T_FMT), t);

    date_n_time[16] = '\0';
    printf(" %s %s\n", date_n_time + 4, name);

}

void ls_dir(char * path) {
    struct dirent *dirent_obj;
    DIR *dir = opendir(path);
    if (!dir) {
        perror(""); 
    }
    if (no_of_files + no_of_dirs + err_no > 1) {
        printf("%s:\n", path);
    }
    struct stat stat_buffer;
    stat(path, &stat_buffer);
    int n = get_total(path);
    for (int i = 0; i < n; ++i) {
        if (ls_l) {
            stat(all_elems[i].Path, &stat_buffer);
            flag_l(all_elems[i].Path, all_elems[i].Name, &stat_buffer);
        } else {
            printf("%s\n", all_elems[i].Name);
        }
    }
}

void list_dir_contents (char *string) {
    no_of_dirs = no_of_files = err_no = 0;
    ls_a = ls_l = false;
    string = strtok(NULL, delim);
    while (string != NULL) {
        bool invalid_arg = false;
        if (string[0] == '-') {
            int n = strlen(string);
            for (int i = 1; i < n; ++i) {
                if (string[i] == 'a') 
                    ls_a = true;
                else if (string[i] == 'l') 
                    ls_l = true;
                else {
                    printf("ls: invalid option -- '%c'\n", string[i]);
                    return;
                }
            }
        } else {
            if (string[0] == '~') {
                sprintf(ForPath, "%s%s", home_dir, &string[1]);
            } else {
                strcpy(ForPath, string); 
            }
            struct stat st_bff;
            if (stat(ForPath, &st_bff) == -1) {
                sprintf(error_msg[err_no], "ls: cannot access '%s': No such file or directory\n", ForPath);
                err_no++;
                string = strtok(NULL, delim);
                continue;
            }
            if (S_ISREG(st_bff.st_mode)) {
                strcpy(files[no_of_files], ForPath); 
                no_of_files++;
            } else {
                strcpy(directories[no_of_dirs], ForPath); 
                no_of_dirs++;
            }
        }
        string = strtok(NULL, delim);
    }
    for (int i = 0; i < err_no; ++i) {
        printf("%s", error_msg[i]);
    }
    if (no_of_dirs == 0 && no_of_files == 0 && err_no == 0) {
        ls_dir(".");
    } else {
        qsort(files, no_of_files, MAX_SZ, comp_str);
        for (int i = 0; i < no_of_files; ++i) {
            if (ls_l) {
                struct stat sbf; stat(files[i], &sbf);
                flag_l(files[i], files[i], &sbf);
            } else {
                printf("%s\n", files[i]);
            }
            if (i == no_of_files - 1 && no_of_dirs > 0) printf("\n");
        }
        qsort(directories, no_of_dirs, MAX_SZ, comp_str);
        for (int i = 0; i < no_of_dirs; ++i) {
            ls_dir(directories[i]);
            if (i < no_of_dirs - 1) printf("\n");
        }
    }
}