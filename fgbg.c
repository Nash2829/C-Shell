#include "headers.h"

int argc, bg_no;
char *argv[265];
bool background;

void get_args(char *token) {
    argc = 0;
    background = false;
    while (token != NULL) {
        argv[argc] = token;
        token = strtok(NULL, delim);
        argc++;
    }
    argv[argc] = NULL;
    if (strcmp(argv[argc - 1], "&") == 0) {
        background = true;
        argv[argc - 1] = NULL;
        argc--;
    }
}   

void fgbg(char * tkn) {
    get_args(tkn);

    pid_t pid = fork();
    
    if (pid == -1) {
        perror("Nash: fork");
    } else if (pid == 0) {
        setpgid(0, 0);
        if (execvp(argv[0], argv) == -1) {
            printf("%s: command not found\n", argv[0]);
        }
        _exit(EXIT_SUCCESS);
    } else {
        insertProcess(pid, argv[0]);
        setpgid(pid, pid);
        if (!background) {
            int status;
            signal(SIGTTOU, SIG_IGN);
            signal(SIGTTIN, SIG_IGN);
            tcsetpgrp(STDIN_FILENO, getpgid(pid));
            tcsetpgrp(STDOUT_FILENO, getpgid(pid));

            waitpid(pid, &status, WUNTRACED);

            tcsetpgrp(STDIN_FILENO, getpgrp());
            tcsetpgrp(STDOUT_FILENO, getpgrp());
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);

            if (!WIFSTOPPED(status)) {
                deleteProcess(pid);
            }
        } else {
            printf("%d\n", pid);
        }
    }
    for (int i = 0; i < argc; ++i) {
        argv[i] = NULL;
    }
} 

void _foreground(int jobNo) {
    checkJobs();
    bool jobNoSpecified = true;
    if (jobNo == 0) {
        jobNoSpecified = false;
        for (process* curr = process_ptr; curr != NULL; curr = curr->next) {
            jobNo = max(jobNo, curr->JobNo);
        }
    }
    pid_t childProcessPID = -1;
    bool validJob = false;
    char *name;
    for (process* curr = process_ptr; curr != NULL; curr = curr->next) {
        if (curr->JobNo == jobNo) {
            validJob = true;
            childProcessPID = curr->pid; 
            name = curr->name;
            break;
        }
    } 
    if (!validJob) {
        printf("Nash: fg: ");
        if (jobNoSpecified) {
            printf("%d: ", jobNo);
        } else {
            printf("current: ");
        }
        printf("No such job\n");
        return;
    }
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    tcsetpgrp(STDIN_FILENO, getpgid(childProcessPID));
    tcsetpgrp(STDOUT_FILENO, getpgid(childProcessPID));
    int status, err = 0;
    if (kill(childProcessPID, SIGCONT) == -1) {
        err = 1;
        perror("Nash: fg");
    } else {
        printf("%s\n", name);
        waitpid(childProcessPID, &status, WUNTRACED);
    }
    tcsetpgrp(STDIN_FILENO, getpgrp());
    tcsetpgrp(STDOUT_FILENO, getpgrp());
    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);

    if (!WIFSTOPPED(status) && err) {
        deleteProcess(childProcessPID);
    }
}

void _background(int jobNo) {
    checkJobs();
    bool jobNoSpecified = true;
    if (jobNo == 0) {
        jobNoSpecified = false;
        for (process* curr = process_ptr; curr != NULL; curr = curr->next) {
            jobNo = max(jobNo, curr->JobNo);
        }
    }
    pid_t childProcessPID = -1;
    bool validJob = false;
    for (process* curr = process_ptr; curr != NULL; curr = curr->next) {
        if (curr->JobNo == jobNo) {
            validJob = true;
            childProcessPID = curr->pid; break;
        }
    } 
    if (!validJob) {
        printf("Nash: bg: ");
        if (jobNoSpecified) {
            printf("%d: ", jobNo);
        } else {
            printf("current: ");
        }
        printf("No such job\n");
        return;
    }
    if (kill(childProcessPID, SIGCONT) == -1) {
        perror("Nash: bg");
    }
}
