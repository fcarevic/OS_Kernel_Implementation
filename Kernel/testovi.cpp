/*
 * testovi.cpp
 *
 *  Created on: May 11, 2019
 *      Author: OS1
 */
/*
#include "thread.h"
#include "KernelS.h"
#include <iostream.h>
#include <stdio.h>
#include "IVTEnt.h"
#include "KernEv.h"
#include <stdarg.h>
#include <stdlib.h>

extern Thread* running;
extern int requested_context_switch;
extern int context_switch_disabled;

#define Semaphore KernelSem
#define Event KernelEvent
void interrupt timer_interrupt(...);
#define lock context_switch_disabled++;
#define unlock if(context_switch_disabled){context_switch_disabled--; if(!context_switch_disabled) if(requested_context_switch)timer_interrupt();};

int syncPrintf(const char *format, ...)
 {
	int res;
	va_list args;
	lock;
va_start(args, format);
res = vprintf(format, args);
va_end(args); unlock;
return res; }



const int n = 200;

void tick(){}

class TestThread : public Thread
{
public:

	TestThread(): Thread(2096,2) {};
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{

	int buffer=2;

	//for(int i=0;i<32000;i++)
	{
		buffer = 4096/2048;
		//for (int j = 0; j < 1024; j++)
		{
			buffer = buffer*2;
			if(buffer%2)
				buffer = 2;
		}
	}

}




/*class NitA:public Thread{

public:
	NitA():Thread(512,2){}
	virtual void run(){
		for(int i = 0;i<10000;i++){
			//lock
					//cout<<"Nit a\n";
			//unlock

		if(i==7)i=0;
		}
	}

};






Thread* temp=new NitA();


class NitB:public Thread{

public:
	NitB():Thread(512,3){}
	virtual void run(){
		for(int i = 0;i<10000;i++){
		//	lock
		//	cout<<"Nit B\n";
		//	unlock
			if(i==7)i=0;

		}
	}

};






int UserMain(){


  lock
	cout<<"usao\n";
  unlock;
NitA* a=new NitA();

//cout<<"aa\n";
  NitB* b= new NitB();

lock
  cout<<"b\n";
unlock
  a->start();
  lock
 cout<<"astart\n";

unlock
   //for(int i=0;i<10000;i++){


//requested_context_switch=1;

//timer_interrupt();
  b->start();
  a->waitToComplete();//promeni ovo,kako da cekas niti ???
  b->waitToComplete();




return 0;
}

*/

