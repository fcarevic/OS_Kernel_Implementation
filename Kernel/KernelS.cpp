/*
 * KernelS.cpp
 *
 *  Created on: May 9, 2019
 *      Author: OS1
 */
#include "KernelS.h"
#include "SCHEDULE.H"
extern int context_switch_disabled;
extern int requested_context_switch;
extern Thread* running;
void interrupt timer_interrupt(...);
#define false 0;
#define true 1;
#define lock() context_switch_disabled++;
#define unlock() if(context_switch_disabled){context_switch_disabled--; if(!context_switch_disabled) if(requested_context_switch)timer_interrupt();};

Queue KernelSem::allSems;

KernelSem::KernelSem(int n):val(n){
	allSems.add(this);
}

int KernelSem::value(){return val;}

int KernelSem::wait(int n){
	lock();
	if(n==0)n=-1;
	if(--val<0){
	TimePCB* t=new TimePCB(running->myPCB,n); //OVO TREBA DA OSTANE NA STEKU KADA SE AKTIVIRA BLOKIRANA NIT
	//AKO NE RADI, ONDA FLAG NEKI DODAJ U PCB DA TI KAZE KAKO JE NIT DEBLOKIRANA
	blocked.add(t);
	running->myPCB->status=PCB::BLOCKED;
	running->dispatch();
	unlock();
	int i=1;;
	if(t->time_left==0)i=0;
	delete t; return i;}
	unlock();
	return 1;
}

int KernelSem::signal(int n){
	if(n<0)return n;
	lock();
	if(!n || n==1){
		if(++val<=0){
		TimePCB* t=(TimePCB*)blocked.remove();
		t->pcb->status=PCB::READY;
		Scheduler::put(t->pcb);
		//delete t; //U WAIT SE BRISE t
		}
		unlock();
		return 1;
	}else {
		val+=n;
		for(int i=0;i<n;i++){
			TimePCB* t=(TimePCB*)blocked.remove();
			if(!t){ unlock(); return i;}
			t->pcb->status=PCB::READY;
			Scheduler::put(t->pcb);

			}
		unlock(); return n;
	}
}


void KernelSem::signalTimerinterrupt(){// NE TREBA LOCK JER SE IZVRSAVA U PREKIDNOJ RUTINI
	int i=0;
	while(allSems.getInfo(i)){

		KernelSem* sem= (KernelSem*)(allSems.getInfo(i));
		int j=0;
		while((sem->blocked).getInfo(j)){
		TimePCB* t=(TimePCB*)((sem->blocked).getInfo(j));
		if(t->time_left!=-1){
			if(!(--(t->time_left))){
				    sem->val++;
				    t->pcb->status=PCB::READY;
					Scheduler::put(t->pcb);
					(sem->blocked).deletee(t);// U WAIT KAD IZADJES SE BRISE t

			}else j++;
		}else j++;

		}
			i++;
	}


}



KernelSem::~KernelSem(){
	lock();
	while(1){
		TimePCB* t=(TimePCB*)blocked.remove();
		if(!t)break;
		t->pcb->status=PCB::READY;
		Scheduler::put(t->pcb);

	}
	allSems.deletee(this);
	unlock();
}



