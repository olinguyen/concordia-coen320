#include "Timer.h"

Timer::Timer(int timeout_s, int timeout_ns, int perSec, int perNsec)
{
	//Channel setup
	chid = ChannelCreate(_NTO_CHF_FIXED_PRIORITY);
	connectionID = ConnectAttach(0, 0, chid, 0, 0);
	SIGEV_PULSE_INIT(&pulseEvent, connectionID, SIGEV_PULSE_PRIO_INHERIT, _PULSE_CODE_MINAVAIL, NULL);

	// create timer

	if (timer_create(CLOCK_REALTIME, &pulseEvent, &timer) == -1) {
		std::cout << "Could not create timer\n";
	}

	it_val.it_value.tv_sec = timeout_s;
	it_val.it_value.tv_nsec = timeout_ns;

	it_val.it_interval.tv_sec = perSec;
	it_val.it_interval.tv_nsec = perNsec;
}


// Start Timer
void Timer::start()
{
	std::cout<< "Timer starting with timeout " << it_val.it_value.tv_sec << "s\n";
	if (timer_settime(timer, 0, &it_val, NULL) == -1) {
		std::cout << "Could not start timer\n";
	}
}

// Wait for pulse
void Timer::wait()
{
#if DEBUG
	std::cout << "Waiting for pulse." << std::endl;
#endif
	MsgReceive(chid, &msg, sizeof(msg), NULL);
#if DEBUG
	std::cout << "Pulse received." << std::endl;
#endif
}
