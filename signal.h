#ifndef __SIGNAL_H
#define __SIGNAL_H

void signal_handler(int, siginfo_t*, void*);
void child_process_handler(int, siginfo_t*, void*);
void initSignal();

#endif
