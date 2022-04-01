#ifndef __PROC_H_
#define __PROC_H_

struct process {
    char *name; pid_t pid; int JobNo;
    struct process* next;
};
typedef struct process process;

extern process* process_ptr;
extern int total_processes;

void initProcess(process**, pid_t, char*);
void insertProcess(pid_t, char*);
void deleteProcess(pid_t);

#endif