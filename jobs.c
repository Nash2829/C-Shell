#include "headers.h"

const char *run = "Running", *stop = "Stopped";
processJobState JOBS[1000];
int nJobs ;

int compare_jobs(const void *a, const void *b) {
    return strcmp((const char*)((processJobState*)a)->command_name, (const char*)((processJobState*)b)->command_name);
}

bool processExists (pid_t pid) {
    return (bool)(kill(pid, 0) == 0);
}

void checkJobs() {
    int z = 0;
    nJobs = 0;
    char process_stat_path[MAX_SZ]={};
    for (process* curr = process_ptr; curr != NULL; curr = curr->next) {
        if (processExists(curr->pid)) {
            z = max(z, curr->JobNo);
        } else {
            deleteProcess(curr->pid);
        }
    }
    ++z;
    for (process* curr = process_ptr; curr != NULL; curr = curr->next) {
        sprintf(process_stat_path, "/proc/%d/stat", curr->pid);
        FILE *file = fopen(process_stat_path, "r");
        if (file == NULL) continue;
        char executable[MAX_LEN]={};
        fscanf(file, "%d %s %c", &JOBS[nJobs].pid, executable, &JOBS[nJobs].status);
        fclose(file);
        if (curr->JobNo == -1) {
            curr->JobNo = JOBS[nJobs].jobNo = z; ++z;
        } else {
            JOBS[nJobs].jobNo = curr->JobNo;
        }
        JOBS[nJobs].command_name = strdup(curr->name); 
        nJobs++;
    }
}

void jobs(bool running, bool stopped) {
    checkJobs();
    qsort(JOBS, nJobs, sizeof(processJobState), compare_jobs);
    for (int i = 0; i < nJobs; ++i) {
        if (running && JOBS[i].status == 'T') continue;
        if (stopped && JOBS[i].status != 'T') continue;
        printf("[%d] %s %s [%d]\n", (JOBS[i].jobNo), (JOBS[i].status == 'T' ? stop : run), JOBS[i].command_name, JOBS[i].pid);
        free(JOBS[i].command_name);
    }
}

void sig_sendSignal(int jobNumber, int signalNumber) {
    checkJobs();
    bool validJobNo = false;
    for (process* curr = process_ptr; curr != NULL; curr = curr->next) {
        if (curr->JobNo == jobNumber) {
            int ret = kill(curr->pid, signalNumber);
            if (ret < 0) {
                perror("Nash: sig");
            }
            validJobNo = true;
            break;
        }
    }
    for (process* curr = process_ptr; curr != NULL; curr = curr->next) {
        if (!processExists(curr->pid)) {
            deleteProcess(curr->pid);
        }
    }
    if (!validJobNo) {
        printf("Nash: sig: %d: No such job\n", jobNumber);
    }
}