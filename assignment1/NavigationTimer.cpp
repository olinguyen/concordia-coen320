#include "NavigationTimer.h"

NavigationTimer::NavigationTimer(int timeout_s, int timeout_ns, int perSec, int perNsec)
{
	//Channel setup
	chid = ChannelCreate(_NTO_CHF_FIXED_PRIORITY);
	connectionID = ConnectAttach(0, 0, chid, 0, 0);
	SIGEV_PULSE_INIT(&pulseEvent, connectionID, SIGEV_PULSE_PRIO_INHERIT, _PULSE_CODE_MINAVAIL, NULL);

	// create timer

	if (timer_create(CLOCK_REALTIME, &pulseEvent, &timer) == -1) {
		std::cout << "Could not create timer\n";
		return NULL;
	}
	
	std::cout<< "Timer" << (long)*timer << " starting with timeout " << (long)timeout_s << "s\n";
	it_val.it_value.tv_sec = timeout_s;
	it_val.it_value.tv_nsec = timeout_ns;
	
	it_val.it_interval.tv_sec = perSec;
	it_val.it_interval.tv_nsec = perNsec;
}

NavigationTimer::~NavigationTimer()
{
}

// Start Timer
void NavigationTimer::start()
{
	#if DEBUG
	cout << "Debug -> DDTimer: Timer Started." << endl;
	#endif
	if (timer_settime(*timer, 0, &it_val, NULL) == -1) {
		std::cout << "Could not start timer\n";
	}
}

// Wait for pulse
void NavigationTimer::waitForPulse()
{
#if DEBUG
	cout << "Debug -> DDTimer: Waiting." << endl;
#endif
	MsgReceive(chid, &msg, sizeof(msg), NULL);
#if DEBUG
	cout << "Debug -> DDTimer: Message Received." << endl;
#endif
}
