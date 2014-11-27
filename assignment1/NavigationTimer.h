#ifndef NAVIGATIONTIMER_H_
#define NAVIGATIONTIMER_H_

#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <sys/neutrino.h>

class NavigationTimer 
{	
	private:
		struct sigevent pulseEvent;
		timer_t timer;
		struct itimerspec it_val;
		
	public:
		void NavigationTimer(int sec, int ns, int intervalSec, int intervalNsec);
		void waitPulse();

};

#endif /* NAVITIMER_H_ */