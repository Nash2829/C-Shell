#include "headers.h"

void call_command(char *cmd, bool append_to_hist) {
    if (cmd == NULL) return;
    int nCmds = 1;
    char *curr_cmd = strdup(cmd);
    char **store_cmd = (char**)calloc(nCmds, sizeof(char*));
    cmd = strtok(cmd, ";");
    if (cmd == NULL) return;
    if (append_to_hist) {
        append_history(curr_cmd);
        for (char *ptr = cmd; *ptr; ++ptr) 
            if (*ptr == ';') 
                ++nCmds;
    }
    sFree(curr_cmd);
    int z = 0;
    while (cmd != NULL) {
        store_cmd[z] = strdup(cmd); ++z;
        cmd = strtok(NULL, ";");
    }
    for (int i = 0; i < z; ++i) {
        if (!isPipeOrRedirect(store_cmd[i])) {
            execute(store_cmd[i]);
        }
        sFree(store_cmd[i]);
    }
    sFree(store_cmd);
}


void execute(char *cmd) {
    char *token = strtok(cmd, delim);
    if (token == NULL) {
        return;
    }
    if (strcmp("exit", token) == 0 || strcmp("quit", token) == 0) {
        exit(EXIT_SUCCESS);
    } else if (strcmp("repeat", token) == 0) {
        token = strtok(NULL, delim);
        long a = 0, m = 1, digs = strlen(token);
        for (int i = digs - 1; i >= 0; --i) {
            a += m * (token[i] - '0');
            m *= 10;
        }
        token = strtok(NULL, delim);
        char command[MAX_SZ]={};
        strcpy(command, token);
        token = strtok(NULL, delim);

        while (token != NULL) {
            command[strlen(command)] = ' ';
            strcat(command, token);
            token = strtok(NULL, delim);
        }
        char temp[MAX_SZ]={}; 
        while (a--) {  
            strcpy(temp, command);
            char *s = &temp[0];
            call_command(s, false);
        }
    } else if (strcmp("echo", token) == 0) {
        call_echo(token);
    } else if (strcmp("pwd", token) == 0) {
        present_working_directory();
    } else if (strcmp("cd", token) == 0) {
        call_CD(token);
    } else if (strcmp("ls", token) == 0) {
        list_dir_contents(token);
    } else if (strcmp("history", token) == 0) {
        token = strtok(NULL, delim);
        int argc = 0, a = 0;
        while (token != NULL) {
            ++argc;
            int  m = 1, digs = strlen(token);
            for (int i = digs - 1; i >= 0; --i) {
                if (i == 0 && token[0] == '-') {
                    a = -a;
                } else {
                    a += m * (token[i] - '0');
                    m *= 10;
                }
            }   
            token = strtok(NULL, delim);
        }
        if (argc > 1) {
            printf("Nash: history: too many arguments\n");
        } else {
            if (argc == 0) 
                history(-1);
            else {
                if (a >= 0) {
                    history(a);
                } else {
                    printf("Nash: history: invalid option\n");
                }
            }
        }
    } else if (strcmp("jobs", token) == 0) {
        bool procStopped = false, procRunning = false, valid  = true; 
        while (token != NULL) {
            if (token[0] == '-') {
                for (int i = 1; token[i] != '\0'; ++i) {
                    if (token[i] == 'r') {
                        procRunning = true;
                    } else if (token[i] == 's') {
                        procStopped = true;
                    } else {
                        valid = false;
                        printf("Nash: jobs: %c: invalid option\n", token[i]); 
                        break;
                    }
                }
            }
            token = strtok(NULL, delim);
        }
        if (valid) {
            jobs(procRunning, procStopped);
        }
    } else if (strcmp("sig", token) == 0) {
        token = strtok(NULL, delim);
        if (token == NULL) {
            printf("sig: Usage: sig [job number] [signal number]\n");
            return;
        }
        int jobNumber = atoi(token);
        token = strtok(NULL, delim);
        if (token == NULL) {
            printf("sig: Usage: sig [job number] [signal number]\n");
            return;
        }
        int sigNumber = atoi(token);
        token = strtok(NULL, token);
        if (token != NULL) {
            printf("Nash: sig: too many arguments\n");
        } else {
            sig_sendSignal(jobNumber, sigNumber);
        }
    } else if (strcmp("fg", token) == 0) {
        token = strtok(NULL, delim);
        int jNo = 0, args = 0;
        while (token != NULL) {
            args++;
            jNo = atoi(token);
            token = strtok(NULL, delim);
        }
        if (args >= 1) {
            if (args > 1) {
                printf("Nash: fg: too many arguments\n");
                return;
            } else if (jNo <= 0) {
                printf("Nash: fg: invalid job number\n");
                return;
            } else {
                _foreground(jNo);
            }
        } else {
            _foreground(0);
        }
    } else if (strcmp("bg", token) == 0) {
        token = strtok(NULL, delim);
        int jNo = 0, args = 0;
        while (token != NULL) {
            args++;
            jNo = atoi(token);
            token = strtok(NULL, delim);
        }
        if (args >= 1) {
            if (args > 1) {
                printf("Nash: bg: too many arguments\n");
                return;
            } else if (jNo <= 0) {
                printf("Nash: bg: invalid job number\n");
                return;
            } else {
                _background(jNo);
            }
        } else {
            _background(0);
        }
    } else {
        fgbg(token);
    }
}