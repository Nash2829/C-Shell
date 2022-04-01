#include "headers.h"

void extract_curr_dir() {
    if (strncmp(home_dir, current_work_dir, home_dir_len) == 0) {
        buff2[0] = '~';
        strcpy(buff2 + 1, current_work_dir + home_dir_len);
        strcpy(current_work_dir, buff2);
    }
}

void User_and_System_Name() {
    delim = " \t\n";
    getcwd(home_dir, MAX_SZ);
    home_dir_len = strlen(home_dir);
    buff_len = 100000UL; 
}

void prompt() {
    uid = geteuid();
    PWD = getpwuid(uid);
    if (PWD == NULL) {
        perror("");
    }
    strcpy(user_name, PWD->pw_name);
    gethostname(host_name, 256);
    getcwd(current_work_dir, MAX_SZ);
    extract_curr_dir();
    fflush(stdout);
    printf(CYAN "<" boldYELLOW "%s@%s" WHITE ":" boldBLUE "%s" CYAN "> " RESET , user_name, host_name, current_work_dir);
    // fflush(stdout);
}
