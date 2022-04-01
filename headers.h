#ifndef __HEAD_H_
#define __HEAD_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>
#include <dirent.h>
#include <grp.h>
#include <errno.h>
#include <time.h>
#include <langinfo.h>
#include <signal.h>

#include "process.h"
#include "prompt.h"
#include "ls.h"
#include "cd.h"
#include "signal.h"
#include "command.h"
#include "fgbg.h"
#include "echo.h"
#include "history.h"
#include "pwd.h"
#include "jobs.h"
#include "pipeRedirect.h"

#define MAX_SZ 4100
#define MAX_LEN 265
#define MAX_ARGS 265

void sFree(void*);
extern struct passwd *PWD;
extern uid_t uid;
extern char user_name[MAX_LEN], host_name[MAX_LEN];
extern char current_work_dir[MAX_SZ], home_dir[MAX_SZ];
extern char *buff, *delim;
extern char buff2[MAX_SZ], buff3[MAX_SZ];
extern size_t home_dir_len, buff_len;
extern long cmd_len;
extern bool prompt_done;
extern int STD_INP_FD;
extern int STD_OUT_FD;

#endif

