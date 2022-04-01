#include "headers.h"

void child_process_handler(int signum, siginfo_t *info, void *ucontext) {
	int wstatus;
    pid_t id;
	while (true) {
		id = waitpid(-1, &wstatus, WNOHANG | WUNTRACED);
		if (id <= 0) return;
		for (process* currPtr = process_ptr; currPtr != NULL; currPtr = currPtr->next) {
			if (id != currPtr->pid) continue;
			if (WIFEXITED(wstatus)) {
				fprintf(stderr, "%s with pid %d exited normally\n", currPtr->name, id);
				deleteProcess(id);
			} else {
				if (!WIFSTOPPED(wstatus) && !WIFCONTINUED(wstatus)) {
					fprintf(stderr, "%s with pid %d exited abnormally\n", currPtr->name, id);	
					deleteProcess(id);
				}
			}
			break;
		}
		prompt_done = true;
		prompt(); 
		fflush(stderr);
		fflush(stdout);
	}
}

void signal_handler(int signum, siginfo_t *info, void *ucontext) {
	// 
	checkJobs();
}

void initSignal() {
	static struct sigaction sigAct, childSig;
    sigAct.sa_sigaction = signal_handler;
    childSig.sa_sigaction = child_process_handler;
    sigAct.sa_flags = SA_RESTART | SA_SIGINFO;
    childSig.sa_flags = SA_RESTART | SA_SIGINFO;
    sigemptyset(&sigAct.sa_mask);
	sigemptyset(&childSig.sa_mask);
    sigaction(SIGINT, &sigAct, NULL);
    sigaction(SIGTSTP, &sigAct, NULL);
	sigaction(SIGCHLD, &childSig, NULL);
}

