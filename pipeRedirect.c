#include "headers.h"

char* getFileName(int start, int *end, char *cmd) {
    if (start == -1) return NULL;
    char *File = NULL;
    int l, r;
    for (l = start + 1; cmd[l] == ' ' || cmd[l] == '\n' || cmd[l] == '\t'; ++l);
    for (r = l + 1; cmd[r] && cmd[r] != ' ' && cmd[r] != '\n' && cmd[r] != '\t'; ++r);
    File = (char*)calloc((r - l + 1), sizeof(char));
    for (int i = l; i < r; ++i) 
        File[i - l] = cmd[i];
    *end = r;
    return File;
}

bool isPipeOrRedirect(char *cmd) {
    bool append = false;
    int inpPos = -1, outPos = -1, nPipes = 0, inpPos2 = -1, outPos2 = -1;
    for (int i = 0; cmd[i]; ++i) {
        if (cmd[i] == '<') {
            inpPos = i;
        } else if (cmd[i] == '>') {
            outPos = i;
            if (i > 0 && cmd[i - 1] == '>') append = true;
        } else if (cmd[i] == '|') {
            nPipes++;
        }
    }
    if (inpPos == -1 && outPos == -1 && nPipes == 0) {
        return false;
    }
    char *inputFile = getFileName(inpPos, &inpPos2, cmd);
    char *outputFile = getFileName(outPos, &outPos2, cmd);
    int FDin, FDout;

    if (outputFile) {
        FDout = open(outputFile, O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644); 
        close(FDout);
    } 


    if (nPipes == 0) {
        if (outputFile) {
            FDout = open(outputFile, O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644); 
        } else {
            FDout = dup(STD_OUT_FD);
        }
        if (inputFile) {
            FDin = open(inputFile, O_RDONLY);
            if (FDin < 0) {
                char msg[1000]={};
                sprintf(msg, "Nash: %s", inputFile);
                perror(msg);
                close(FDout);
                dup2(STD_INP_FD, STDIN_FILENO);
                dup2(STD_OUT_FD, STDOUT_FILENO);
                return true;
            } 
        } else {
            FDin = dup(STD_INP_FD);
        }
        cmd = strtok(cmd, "><");
        dup2(FDout, STDOUT_FILENO);
        dup2(FDin, STDIN_FILENO);
        close(FDout);
        close(FDin);
        execute(cmd);
        dup2(STD_INP_FD, STDIN_FILENO);
        dup2(STD_OUT_FD, STDOUT_FILENO);
        return true;
    }
    char *tkn = strtok(cmd, "|");
    FDin = dup(STD_INP_FD);
    for (int cmdNo = 0; tkn != NULL; cmdNo++) {
        bool LESS = false, GREAT = false;
        int len = strlen(tkn);
        for (char *c = tkn; *c; ++c) {
            if (*c == '<') {
                LESS = true; *c = '\0'; len = c - tkn;
            }
            if (*c == '>') {
                GREAT = true; *c = '\0'; len = c - tkn;
            }
        }
        char *curr_cmd = (char*)calloc(len + 1, sizeof(char));
        strcpy(curr_cmd, tkn);
        tkn = strtok(NULL, "|");

        dup2(FDin, STDIN_FILENO);
        close(FDin);
        if (LESS) {
            FDin = open(inputFile, O_RDONLY);
            if (FDin < 0) {
                char msg[1000];
                sprintf(msg, "Nash: %s", inputFile);
                perror(msg);
            } else {
                dup2(FDin, STDIN_FILENO);
                close(FDin);
            }
        }
        if (tkn == NULL) {
            if (outputFile && GREAT) {
                FDout = open(outputFile, O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644);
            } else {
                FDout = dup(STD_OUT_FD);
            }
        } else {
            int fdPipe[2]={};
            pipe(fdPipe);
            FDout = fdPipe[1];
            FDin = fdPipe[0];
        }
        dup2(FDout, STDOUT_FILENO);
        close(FDout);
        
        pid_t pid = fork();
        if (pid == -1) {
            perror("Nash: fork");
        } else if (pid == 0) {
            execute(curr_cmd);
            _exit(EXIT_SUCCESS);
        }
        waitpid(pid, NULL, WUNTRACED);
        dup2(STD_INP_FD, STDIN_FILENO);
        dup2(STD_OUT_FD, STDOUT_FILENO);
    }
    return true;
}

