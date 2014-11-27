#ifndef TIMER_H_
#define TIMER_H_

#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <sys/neutrino.h>

class Timer
{
	private:
		struct sigevent pulseEvent;
		timer_t timer;
		struct itimerspec it_val;
		int chid, connectionID;
		char msg[100];
	public:
		Timer(int sec, int ns, int intervalSec, int intervalNsec);
		void start();
		void wait();

};

#endif /* condvarTIMER_H_ */
