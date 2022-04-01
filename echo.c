#include "headers.h"

void call_echo(char *p1) {
    p1 = strtok(NULL, delim);
    int z = 0;
    buff2[0] = '\0';
    while (p1 != NULL)
    {
        strcat(buff2, p1);
        z = strlen(buff2);
        buff2[z] = ' ';
        p1 = strtok(NULL, delim);
    }
    printf("%s\n", buff2);
    memset(buff2, '\0', z);
}