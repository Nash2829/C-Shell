#include "headers.h"

process* process_ptr, *last;
int total_processes;

void initProcess(process** node, pid_t pid, char* cmd_name) {
    *node = NULL;
    *node = (process*)malloc(sizeof(process));
    (*node)->pid = pid;
    (*node)->JobNo = -1;
    (*node)->name = strdup(cmd_name);
    (*node)->next = NULL;
}

void insertProcess(pid_t pid, char* cmd_name) {
    if (!process_ptr) {
        initProcess(&process_ptr, pid, cmd_name);
        last = process_ptr;
    } else {
        initProcess(&(last->next), pid, cmd_name);
        last = last->next;
    }
    total_processes++;   
}

void deleteProcess(pid_t pid) {
    process *prevPtr, *currPtr;
    prevPtr = currPtr = process_ptr;
    if (currPtr->pid == pid) {
        // printf("deleted: %s\n", currPtr->name);
        process_ptr = process_ptr->next;
        free(prevPtr->name); 
        free(prevPtr); 
        total_processes--;
        return;
    }
    while (currPtr != NULL) {
        if (currPtr->pid != pid) goto iter;
        // printf("deleted: %s\n", currPtr->name);
        prevPtr->next = currPtr->next;
        free(currPtr->name);
        free(currPtr);
        total_processes--; 
        return;
    iter:
        prevPtr = currPtr;
        currPtr = currPtr->next;
    }
}