//======== Programming Assignment 2: Deadlock Scenario ======================
//===========================================================================
//===========================================================================
//===========================================================================
//===========================================================================

//=============================================================================
#include "Deadlock.h"

pthread_mutex_t mutex_lock=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condvar=PTHREAD_COND_INITIALIZER;

#define PCnt 10 /* Maximum number of threads*/
#define eps .005
float thread_priority[PCnt]={0}; // priority of threads
int active_p=0;			  // detemine the active threas that should be run

//============================================  develope the mutex class here ====================================================
class CS  // The developed Mutex class
{
	private:
		int s;								// mutex status; 0 is free 1 is taken
	public:
		CS ();
		~CS();
		void lock(int p, CS sp[]);
		void unlock(int p);
};

CS::CS() {
	s=0;
}

CS::~CS(){
}


void CS::lock(int p, CS sp[]){
	cout<< "LOCKed by P"<<p;	 // display a message
}

void CS::unlock(int p){
	cout<<"UNLOCKed by P"<<p;	 // display a message
}


//=============================================================================

//=============================================================================

CS sem[PCnt]; // mutexes are instantiated here

//=============================================================================
void * P1(void* arg)
{
	int cnt=1,j;
	//Message msg;
	//cout << ".........P1 thread starts.........";
	while(1){
		pthread_mutex_lock(&mutex_lock);
		do{
			pthread_cond_wait(&condvar,&mutex_lock); // check for a message from ThreadManager
		}while(active_p!=1);				 // check the active thread

		//if(active_p==1)     				 // check the active thread
		{
		//+++++++++++++++++++++++++++++++++++++++++++++
			cout<<"P1->";//<<cnt<<endl;
			if(cnt==1){
				cout << ".....CS1 ..";
				sem[1].lock(1,sem);
			}
			else if(cnt==2){
				cout << ".....CS2 ..";
				sem[2].lock(1,sem);
			}
			else if(cnt==3){
				cout << ".....CS2 ..";
				sem[2].unlock(1);
			}
			else if(cnt==4){
				cout << ".....CS1 ..";
				sem[1].unlock(1);
			}
			else if (cnt == 6){
				//Message msg;
				cout << ".........P1 thread ends.........";
				thread_priority[1]=0; // to remove process 1 from the queue of ThreadManager
				pthread_mutex_unlock(&mutex_lock);
				break;
			}
			cnt++;
		}
		//---------------------------------------------
		//pthread_cond_wait(&condvar,&mutex_lock); // check for a message from ThreadManager
		pthread_mutex_unlock(&mutex_lock);
	}

}
//=============================================================================
void * P2(void* arg)
{

	int cnt=1,j;
	//Message msg;
	//cout << ".........P2 thread starts.........";

	while(1){
		pthread_mutex_lock(&mutex_lock);
		do{
			pthread_cond_wait(&condvar,&mutex_lock); // check for a message from ThreadManager
		}while(active_p!=2);				 // check the active thread

		//if(active_p==2)     				 // check the active thread
		{
		//+++++++++++++++++++++++++++++++++++++++++++++
			cout<<"P2->";//<<cnt<<endl;
			if(cnt==1){
				cout << ".....CS2 ..";
				sem[2].lock(2,sem);
			}
			else if(cnt==2){
				cout << ".....CS1 ..";
				sem[1].lock(2,sem);
			}
			else if(cnt==3){
				cout << ".....CS1 ..";
					sem[1].unlock(2);
			}
			else if(cnt==4){
				cout << ".....CS2 ..";
				sem[2].unlock(2);
			}
			else if (cnt == 6){
				//Message msg;
				cout << ".........P2 thread ends.........";
				thread_priority[2]=0; // to remove process 2 from the queue of ThreadManager
				pthread_mutex_unlock(&mutex_lock);
				break;
			}
			cnt++;
		}
		//---------------------------------------------
		//pthread_cond_wait(&condvar,&mutex_lock); // check for a message from ThreadManager
		pthread_mutex_unlock(&mutex_lock);
	}
}


void ThreadManager(){ // determines that which thread should be run
	float p;
	int i;

	pthread_mutex_lock(&mutex_lock);

		p=-1;
		for(i=1;i<PCnt;i++){ // find the thread with the most priority and set it as active thread
			if(thread_priority[i]>p){
				active_p=i;
				p=thread_priority[i];
			}
		}
		pthread_mutex_unlock(&mutex_lock);
		pthread_cond_broadcast(&condvar); // send the signal to the threads
}

//=============================================================================
//                                 M     A     I   N
//=============================================================================
void run_deadlock(void)
{
	pthread_t P1_ID,P2_ID;       //p1, p2, threads
	int cnt=0,i;

	#ifdef PRIORITY_CEILING
	//set the priority ceiling of the mutexex

//	sem[1].PC=??;
//	sem[2].PC=??;

	#endif


	//creating up a periodic  timer to generate pulses every 1 sec.
	Timer t(1,0,1,0);
	while(1)
	{
		//--------------------------------------------
		pthread_mutex_lock(&mutex_lock);
		 // release P2 at t=0
		if(cnt==0){
			thread_priority[2]=.5;
			pthread_create(&P2_ID , NULL, P2, NULL);
		}
		 // release P1 at t=2
		else if(cnt==2){
			thread_priority[1]=.7;
			pthread_create(&P1_ID , NULL, P1, NULL);
		}
		 // terminate the program at t=30
		else if (cnt == 30){
			break;
		}
		pthread_mutex_unlock(&mutex_lock);
		//---------------------------------------------
		// waite for the timer pulse
		t.wait();
		//+++++++++++++++++++++++++++++++++++++++++++++
		cnt++;
		// message
		cout<<endl<<"tick="<<cnt<</*", active_p="<<active_p<<*/"->";//<<endl;

		ThreadManager(); // to find out and run the active thread

    	//*********************************************
	}
}
//=============================================================================

