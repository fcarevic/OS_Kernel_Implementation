#ifndef _MOJ_EVENT
#define _MOJ_EVENT
#include "KernEv.h"
#include "IVTEnt.h"
class Event{

public:
Event(IVTNo n){ myEv=new KernelEvent(n);}
~Event(){delete myEv;}
void wait(){myEv->wait();}
void signal(){myEv->signal();}


private:
KernelEvent* myEv;
};




#define PREPAREENTRY(numEntry,callOld)\
	void interrupt inter##numEntry(...);\
	IVTEntry IVTEntry##numEntry(numEntry,inter##numEntry);\
	void interrupt inter##numEntry(...){\
	   (KernelEvent::nizEventova[numEntry])->signal();\
	   if(callOld) (IVTEntry::allEntries[numEntry])->call();}
#endif
