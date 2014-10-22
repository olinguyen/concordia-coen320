#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <time.h>
#include <signal.h>

#include "NavigationTimer.h"

void* dap(void* a); // Thread function that will simulate DAP (Data Acquisition process)
void* dd(void* a); // Thread function that will simulate DD (Data display)

#define DAP_TIMEOUT 2
#define DD_TIMEOUT 6
const int g = 9.8;
double X = 0.0, Y = 0.0, Z = 0.0, Vx = 0.0, Vy = 0.0, Vz = 0.0;
pthread_mutex_t *mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t *cond = PTHREAD_COND_INITIALIZER;

int main(int argc, char *argv[]) {
	timer_t dap_timer, dd_timer;
	struct sigevent dap_pulse, dd_pulse;
	pthread_t dap_thread, dd_thread;

	// TODO: Pass info to threads?
	pthread_create(&dap_thread, NULL, dap, NULL);
	pthread_create(&dd_thread, NULL, dd, NULL);

	return EXIT_SUCCESS;
}

void* dap(void* a)
{

	// Get accelerometer data.
	double Ax = -2.0;
	double Ay = 4.0;
	double Az = 10.5;
	double T = 2.0;
	
	// Start timer
	NavigationTimer dapTimer(DAP_TIMEOUT, 0, DAP_TIMEOUT, 0);
	dapTimer.Start();
	while(1)
	{
		// wait for pulse
		
		// Lock mutex
		pthread_mutex_lock(mutex);	
		
		// Update velocity and position.
		Vx = Vx + Ax * T;
		Vy = Vy + Ay * T;
		Vz = Vz + (Az - g) * T;
		X = X + Vx * T;
		Y = Y + Vy * T;
		Z = Z + Vz * T;
		
		// The actual calculations typically take longer on the target processor.
		// Here we simulate the extra time by adding a FOR loop.
		for(int i =0; i < 100000; ++i)
		{
			i = i+1;
			i = i-1;
		}
		
		// Release mutex
		pthread_mutex_unlock(mutex);
	}
	return NULL;
}

void* dd(void* a)
{
	// start timer
	NavigationTimer ddTimer(DD_TIMEOUT, 0, DD_TIMEOUT, 0);
	ddTimer.Start();
	
	while (1) 
	{
		// wait for pulse
		
		// Lock mutex
		pthread_mutex_lock(mutex);	

		// Display velocity and position
		cout << "Current velocity: (" << Vx << ", " << Vy << ", " << Vz << ")" << endl;
		cout << "Current position: (" << X << ", " << Y << ", " << Z << ")" << endl;
		
		// Release mutex
		pthread_mutex_unlock(mutex);
	}

	return NULL;
}
