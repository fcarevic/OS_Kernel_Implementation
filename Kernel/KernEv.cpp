#include "KernEv.h"
#include "SCHEDULE.H"
#include "IVTEnt.h"
extern Thread* running;
KernelEvent* KernelEvent::nizEventova[256]={0};
extern int requested_context_switch;
extern int context_switch_disabled;
void interrupt timer_interrupt(...);

#define lock context_switch_disabled++;
#define unlock if(context_switch_disabled){context_switch_disabled--; if(!context_switch_disabled) if(requested_context_switch)timer_interrupt();};


KernelEvent::KernelEvent(IVTNo n):num(n),blocked(0){
	lock
	creator=running;
	creatorpid= running->getId();
	nizEventova[num]=this;
	unlock
}
KernelEvent:: ~KernelEvent()
{



nizEventova[num]=0;

	}

void KernelEvent::wait()
{if(running == creator)
{ lock
	blocked=1;creator->myPCB->status=PCB::BLOCKED;
unlock
running->dispatch();}}


void KernelEvent::signal()
{if(Thread::getThreadById(creatorpid) == 0) return;

	if (blocked) {
	lock
	blocked=0;
	creator->myPCB->status=PCB::READY;
	Scheduler::put(creator->myPCB);
	unlock
	if(!context_switch_disabled) running->dispatch();}} //da li se dispatch radi uvek??


