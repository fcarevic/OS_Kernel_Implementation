#ifndef _MOJ_EVENT_
#define _MOJ_EVENT_
#include "thread.h"


typedef unsigned int IVTNo;

class KernelEvent {

public:
	KernelEvent(IVTNo n);
    ~KernelEvent();
	void wait();
	void signal();

static KernelEvent* nizEventova [256];
private:
int  blocked;
int creatorpid;
Thread* creator;
IVTNo num;
};



#endif
