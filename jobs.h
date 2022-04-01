#ifndef __JOBS_H_
#define __JOBS_H_

struct processJobState {
    int jobNo;
    char status;
    char *command_name;
    pid_t pid;
};
typedef struct processJobState processJobState;
#define max(a, b) (a > b ? a : b)

bool processExists(pid_t pid);
void checkJobs();
void jobs(bool running, bool stopped);
void sig_sendSignal(int jobNumber, int signalNumber);

#endif