#include "headers.h"

char *fname = "./history.txt";
char last_cmds[45][MAX_SZ], check_null[MAX_SZ];
int nLines;
void initialize_history() {
    FILE *fptr = fopen(fname, "r");
    int j = 21; 
    nLines = 0;
    if (fptr == NULL) {
        perror("Unable to create history.txt"); exit(EXIT_FAILURE);
    }
    char *curr_line = NULL; size_t len = 0; ssize_t read;
    while (true) {
        read = getline(&curr_line, &len, fptr);
        if (read == -1) 
            break;
        strcpy(last_cmds[j], curr_line);
        nLines++; ++j;
        if (nLines > 20) break;
    }
    fclose(fptr);
    for (int i = j, c = 20 - nLines + 1; i > 20; --i) {
        strcpy(last_cmds[c], last_cmds[i]); c++;
    }
    if (curr_line != NULL) free(curr_line);
}

void append_history(char * command) {
    strcpy(check_null, command);
    char *tkn = strtok(check_null, delim);
    int written = 0;
    while (tkn != NULL) {
        written++;
        tkn = strtok(NULL, delim);
    }
    if (written == 0) return;
    FILE *fptr = fopen(fname, "a+");
    if (nLines == 0) {
        fprintf(fptr, "%s", command);
        nLines++;
        strcpy(last_cmds[20], command);
    } else {
        if (strcmp(last_cmds[20], command) == 0) {
            fclose(fptr);
            return;
        }
        for (int i = 0; i < nLines; ++i) {
            strcpy(last_cmds[20 - i + nLines], last_cmds[20 - i]);
        }
        strcpy(last_cmds[20], command);
        for (int i = 20, j = 19; i + nLines > 20; --i) {
            if (j == 1) break;
            strcpy(last_cmds[j], last_cmds[i + nLines]); j--;
        }
        if (nLines == 20) {
            fclose(fptr);
            fptr = fopen(fname, "w");
            for (int i = 1; i <= 20; ++i) {
                fprintf(fptr, "%s", last_cmds[i]);
            }
            fclose(fptr); return;
        } else {
            nLines++;
            fprintf(fptr, "%s", command);
        }
    }
    fclose(fptr);
}

void history(int n) {
    if (n == -1) {
        int lim = (nLines < 10) ? (20 - nLines) : 10;
        for (int i = lim + 1; i <= 20; ++i) {
            printf("%s", last_cmds[i]);
        }
    } else {
        if (n >= 20) {
            printf("Nash: history: invalid argument. <num> < 20\n");
            return;
        }
        if (n > nLines) {
            printf("Nash: history: total commands stored are %d. Argument value too large\n", nLines);
            return;
        }
        for (int i = 20 - n + 1; i <= 20; ++i) {
            printf("%s", last_cmds[i]);
        }
    }
}