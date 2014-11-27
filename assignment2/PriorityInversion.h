#ifndef DEADLOCK_H_
#define DEADLOCK_H_

#include <stdio.h>
#include <iostream.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sync.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <sys/syspage.h>
#include "Timer.h"

void ThreadManager1();
void * process1(void* arg);
void * process2(void* arg);
void * process3(void* arg);
void run_priority_inversion(void);

#endif /* DEADLOCK_H_ */
