#include "headers.h"

struct passwd *PWD;
uid_t uid;
char user_name[MAX_LEN], host_name[MAX_LEN];
char current_work_dir[MAX_SZ], home_dir[MAX_SZ];
char *buff, *delim;
char buff2[MAX_SZ], buff3[MAX_SZ];
size_t home_dir_len, buff_len;
long cmd_len;
bool prompt_done;
int STD_INP_FD, STD_OUT_FD;

void sFree(void *__ptr) {
	if (__ptr != NULL)
		free(__ptr);
}

int main() {
	initSignal();
	STD_INP_FD = dup(STDIN_FILENO);
	STD_OUT_FD = dup(STDOUT_FILENO);
	User_and_System_Name();
	call_command("clear", false);
	fflush(stdout);
	initialize_history();
	while (true) {
		if (!prompt_done)
			prompt();
		prompt_done = false;
        buff = NULL;
        cmd_len = getline(&buff, &buff_len, stdin);
		if ((cmd_len == -1 || strlen(buff) == 0) && feof(stdin)) {
			puts("");
			if (errno == EINTR) {
				continue;
			}
			sFree(buff); return 0;
		}
        if (cmd_len == -1) {
            perror(""); 
            exit(EXIT_FAILURE);
        }
        call_command(buff, true);
        sFree(buff);
	}
	return 0;
}

