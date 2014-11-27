//=======================PRIORITY INVERSION==================================
//===========================================================================
//===========================================================================
//===========================================================================
#include "PriorityInversion.h"
//=============================================================================

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;

#define PCnt 10 /* Maximum number of threads*/
#define eps .005
#define RELEASE_TIME_P1 4
#define RELEASE_TIME_P2 2
#define RELEASE_TIME_P3 0
#define PRIORITY_P1	0.7
#define PRIORITY_P2	0.6
#define PRIORITY_P3	0.5

// indicate the algorithm here

#define PRIORITY_CEILING
//#define PRIORITY_INHERITANCE

float priority[PCnt]={0}; // priority of threads
int active_process=0;			  // detemine the active thread that should be run

#ifdef PRIORITY_CEILING
//=======================    PRIORITY_CEILING  ================================
class Sem  // The Priority Ceiling Mutex class
{
	public:
		float PC;		// priority ceiling of the mutex
		Sem ();
		~Sem();
		void lock(int p, Sem sp[]);
		void unlock(int p);
};

Sem::Sem() {
}

Sem::~Sem(){
}

void Sem::lock(int p, Sem sp[]){
    // if requesting ps priotity > pc
    pthread_mutex_destroy(&lock);

    // else suspend requesting process
    // priority holding process = priority questing process
}

void Sem::unlock(int p){
}

#endif
//=============================================================================

#ifdef PRIORITY_INHERITANCE
//=============================================================================
class Sem  // The Priority Inheritance Mutex class
{
	private:
		bool isTaken;
		int holding_process; // Process holding the

	public:
		Sem ();
		~Sem();
		void lock(int p, Sem sp[]);
		void unlock(int p);
};

Sem::Sem() {
}

Sem::~Sem(){
}

void Sem::lock(int p, Sem sp[]){
    // if holding process == NULL
    pthread_mutex_destroy(&lock);

    // if there is holding process & priority of holding process < requesting process
    // holding process priority = priority requesting process + delta
    // save initial priority

}

void Sem::unlock(int p){
    // if requesting process = null
    // return to initial priority if priority of low ps changed
    pthread_mutex_unlock(&lock);
}

#endif
//=============================================================================


Sem s[PCnt]; // mutexes are instantiated here

//=============================================================================
void * process1(void* arg)
{
	int cnt=0,j;
	while(1){
		pthread_mutex_lock(&mutex);
		do{
			pthread_cond_wait(&cond,&mutex); // check for a message from ThreadManager
		}while(active_process!=1);				 // check the active thread

		//+++++++++++++++++++++++++++++++++++++++++++++
			cout<<"P1->";//<<cnt<<endl;
			if(cnt==1){			// Try to acquire mutex after running for 1 unit
				cout << ".....Attempting to Lock Semaphore ..";
				s[0].lock(1,s);
			}
			else if(cnt==3){		// Release mutex after running for 3 units
				cout << ".....Unlocking Semaphore ..";
				s[0].unlock(1);
			}
			else if (cnt == 4){		// Finish after 4 units
				//Message msg;
				cout << ".........P1 thread ends.........";
				priority[1]=0; // to remove process 1 from the queue of ThreadManager
				pthread_mutex_unlock(&mutex);
				break;
			}
			cnt++;
		//---------------------------------------------
		pthread_mutex_unlock(&mutex);
	}

}

//=============================================================================
void * process2(void* arg)
{
	int cnt=0,j;

	while(1){
		pthread_mutex_lock(&mutex);
		do{
			pthread_cond_wait(&cond,&mutex); // check for a message from ThreadManager
		}while(active_process!=2);				 // check the active thread

		//+++++++++++++++++++++++++++++++++++++++++++++
			cout<<"P2->";//<<cnt<<endl;
			if (cnt == 6){
				cout << ".........P2 thread ends.........";
				priority[2]=0; // to remove process 2 from the queue of ThreadManager
				pthread_mutex_unlock(&mutex);
				break;
			}
			cnt++;
		//---------------------------------------------
		pthread_mutex_unlock(&mutex);
	}
}


//=============================================================================
void * process3(void* arg)
{

	int cnt=0,j;

	while(1){
		pthread_mutex_lock(&mutex);
		do{
			pthread_cond_wait(&cond,&mutex); // check for a message from ThreadManager
		}while(active_process!=3);				 // check the active thread

		//+++++++++++++++++++++++++++++++++++++++++++++
			cout<<"process3->";//<<cnt<<endl;
			if(cnt==1){
				cout << ".....Attempting to Lock Semaphore ..";
				s[0].lock(3,s);
			}
			else if(cnt==3){
				cout << ".....Unlocking Semaphore ..";
					s[0].unlock(3);
			}
			else if (cnt == 5){
				cout << ".........process3 thread ends.........";
				priority[3]=0; // to remove process 3 from the queue of ThreadManager
				pthread_mutex_unlock(&mutex);
				break;
			}
			cnt++;
		//---------------------------------------------
		pthread_mutex_unlock(&mutex);
	}
}


void ThreadManager1(){ // determines that which thread should be run
	float p;
	int i;

	pthread_mutex_lock(&mutex);

		p=-1;
		for(i=1;i<PCnt;i++){ // find the thread with the most priority and set it as active thread
			if(priority[i]>p){
				active_process=i;
				p=priority[i];
			}
		}
		pthread_mutex_unlock(&mutex);
		pthread_cond_broadcast(&cond); // send the signal to the threads
}

//=============================================================================
//                                 M     A     I   N
//=============================================================================
void run_priority_inversion(void)
{
	pthread_t P1_ID,P2_ID, P3_ID;       //p1, p2, p3 threads
	int cnt=0,i;


	//creating up a periodic  timer to generate pulses every 1 sec.
	Timer t(1,0,1,0);
	while(1)
	{
		//--------------------------------------------
		pthread_mutex_lock(&mutex);
		 // release P1 t= 4
		if(cnt==RELEASE_TIME_P1){
			priority[1]=PRIORITY_P1;
			pthread_create(&P1_ID , NULL, process1, NULL);
		}
		 // release P2 at t=2
		if(cnt==RELEASE_TIME_P2){
			priority[2]=PRIORITY_P2;
			pthread_create(&P2_ID , NULL, process2, NULL);
		}
		 // release P3 at t=0
		if(cnt==RELEASE_TIME_P3){
			priority[3]=PRIORITY_P3;
			pthread_create(&P3_ID , NULL, process3, NULL);
		}
		 // terminate the program at t=30
		if (cnt == 30){
			break;
		}
		pthread_mutex_unlock(&mutex);
		//---------------------------------------------
		// wait for the timer pulse
		t.wait();
		//+++++++++++++++++++++++++++++++++++++++++++++
		// message
		cout<<endl<<"tick="<<cnt<</*", active_process="<<active_process<<*/"->";//<<endl;

		ThreadManager1(); // to find out and run the active thread
		cnt++;

    	//*********************************************
	}
}
//=============================================================================

