#include "thread.h"
#include "KernelS.h"
#include <iostream.h>


extern int requested_context_switch;
extern int context_switch_disabled;
void interrupt timer_interrupt(...);

#define lock context_switch_disabled++;
#define unlock if(context_switch_disabled){context_switch_disabled--; if(!context_switch_disabled) if(requested_context_switch)timer_interrupt();};






Queue waitingToDie;



class Killer: public Thread{

public:
	void run();


   ~Killer(){}

};

void Killer::run(){
  cout<<"killer \n";
	while(1){
		lock
		int i=0;
			while(waitingToDie.getInfo(i)){
				Thread* t=(Thread*)(waitingToDie.getInfo(i));
				t->myPCB->status=PCB::FINISHED;
				myPCB->status=PCB::FINISHED;//da bi mogao da posalje signal 1 ocu niti t jer ne salje signal nit t vec nit killer
				 for(int j=0 ; j< 16; j++)
					t->unregisterAllHandelrs(j);
					t->notifyAllEndWaiting();

					if(Thread::getThreadById(t->myPCB->ppid))
					 if(t->parent){
						   	    t->parent->signal(1);
					 	 	 }

					all_threads.deletee(t->myPCB);
					delete [] t->myPCB->stack;
					delete t->myPCB;
					t->myPCB=0;
					delete [] t->handlers;
					delete [] t->signalMask;






						i++;

			}
			waitingToDie.clear();
			myPCB->status=PCB::IDLE;
				cout<<"Izlazi iz killera\n";
			unlock
			dispatch();

	}
}

Thread* killer;

void addKiller(){

	killer= new Killer();

}
