/*
 * thread.cpp
 *
 *  Created on: Apr 21, 2019
 *      Author: OS1
 */
#include "thread.h"
#include "SCHEDULE.H"
#include <dos.h>
#include <iostream.h>//obrisi
extern Thread* running;
extern Thread* mainThread;
extern int requested_context_switch;
extern int context_switch_disabled;
void interrupt timer_interrupt(...);

#define lock context_switch_disabled++;
#define unlock if(context_switch_disabled){context_switch_disabled--; if(!context_switch_disabled) if(requested_context_switch)timer_interrupt();};

Queue Thread::all_threads;



Thread::Thread (StackSize stackSize , Time timeSlice )
{
	///////////////////////
	//DEO ZA SIOGNALE
	signalMask=new SignalId[numOfSignals];
	 parent=running;
	 handlers=new Queue[numOfSignals];
	for(int i=0;i<numOfSignals;i++) {
		signalMask[i]=1;
		if(parent)
		handlers[i].copy(parent->handlers[i]);
	}
	    int ppid=-1;
		ppid=running->myPCB->pid;



	/////////////////////
	myPCB= new PCB(stackSize,timeSlice,ppid);
	create_context();
	all_threads.add(this);
}

void Thread::create_context(){

	if(myPCB->stack_size==0){myPCB->status=PCB::RUNNING;return;} //ovo je za main nit samo
	lock

	myPCB->stack= new unsigned [myPCB->stack_size];//8 bajtova????
#ifndef BCC_BLOCK_IGNORE
	myPCB->stack[myPCB->stack_size -1]= 0x200;
	myPCB->stack[myPCB->stack_size -2]= FP_SEG(Thread::wrapper);
	myPCB->stack[myPCB->stack_size -3]= FP_OFF(Thread::wrapper);
	myPCB->SP=FP_OFF(myPCB->stack+ (myPCB->stack_size-12));
	myPCB->SS=FP_SEG(myPCB->stack+(myPCB->stack_size-12));
	myPCB->BP=myPCB->SP;
	myPCB->status=PCB::NEW;
#endif
	unlock
}

//////////////////////////////////////
void Thread::dispatch(){

  requested_context_switch=1;
  context_switch_disabled=0;
  timer_interrupt();

  //asm int 1Ch
}
//////////////////////////////////////
ID Thread::getId(){
	return myPCB->pid;
}
ID Thread::getRunningId(){
	return running->getId();

}

 Thread* Thread::getThreadById(ID id){
	Queue::Elem* tek=all_threads.first;
	while (tek !=0){
		if(((Thread*)(tek->info))->getId()==id) return (Thread*)(tek->info);
		tek=tek->next;

	}
	return 0;
}

//////////////////////////////////////////////////////////
void Thread::wrapper(){

	    running->myPCB->status=PCB::RUNNING;
	    running->run();
	    lock
	    running->myPCB->status=PCB::FINISHED;
	    //DEO ZA SIGNALE
	    if(Thread::getThreadById(running->myPCB->ppid))
	    if(running->parent)
	   	    running->parent->signal(1);
	   	    running->signal(2);
	   	    running->processAllSignals();

	   	    /////////////////////////////
	    running->notifyAllEndWaiting();

	    unlock
	    running->dispatch();

		//sta da se radi kad nit zavrsi???


}
void Thread::run(){}
void Thread::start(){
	myPCB->status=PCB::READY;
	lock
	Scheduler::put(myPCB);
    unlock
}

/////////////////////////////////
//DEO GDE SE CEKAJU SVE NITI DA ZAVRSE
void Thread::waitToComplete(){
	if(myPCB==0) return;
	if(myPCB == mainThread->myPCB) return;
	if(running->myPCB==myPCB) return;
	if(myPCB->status==PCB::FINISHED || myPCB->status==PCB::NEW || myPCB->status==PCB::IDLE)return;
	lock
	myPCB->waiting_for_end.add(running->myPCB);
	running->myPCB->status=PCB::BLOCKED;
	unlock
	dispatch();
}

Thread::~Thread(){
	if(myPCB){
	waitToComplete();
	//notifyAllWaiting()???????? //NE TREBA JER NE POZIVA SAMA SEBI DESTRUKTOR
	all_threads.deletee(myPCB);
	delete [] myPCB->stack;
	delete myPCB;
	/////////////////////////////////////////////
	//DEO ZA SIGNALE
	delete [] handlers;
	delete [] signalMask;
	}

	//////////////////////////////////////
}
void Thread::notifyAllEndWaiting(){
	   lock
	  Queue::Elem* tek= myPCB->waiting_for_end.first;
		  while(tek!=nullptr){
			  ((PCB*)(tek->info))->status=PCB::READY;

			  Scheduler::put((PCB*)(tek->info));

			  	 tek=tek->next;//ovde treba lista tek da se unisti
		  }

		 unlock

}


///////////////////////////////////////////////////
///////////////////////////////////////////////
//SIGNALI DEO
static unsigned int masks[numOfSignals]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
unsigned int* Thread::globalSignalMasks=masks;


void Thread::signal(SignalId signal){
	if(running->myPCB->status==PCB::FINISHED)
	{  if(signal < 0 || signal > 15) return; }//ovo je visak
	else
	if(signal ==1 || signal ==2) return;
	signals.add(new SignalId(signal));//brisi sa heapa kad budes obradjivao
}
void Thread::registerHandler(SignalId signal, SignalHandler handler){
	if(signal < 1 || signal > 15) return;
	handlers[signal].add(handler); //ne brises sa heapa
}

void Thread::unregisterAllHandelrs(SignalId signal){
	if(signal < 1 || signal > 15) return;
	handlers[signal].clear();
}

void Thread::swap(SignalId signal,SignalHandler h1, SignalHandler h2){
	if(signal < 1 || signal > 15) return;
	handlers[signal].swap(h1,h2);
}
void Thread::blockSignal(SignalId signal){
	if(signal < 0 || signal > 15) return;
	signalMask[signal]=0;
}
void Thread::unblockSignal(SignalId signal){
	if(signal < 0 || signal > 15) return;
	signalMask[signal]=1;
}

void Thread::blockSignalGlobally(SignalId signal){
	if(signal < 0 || signal > 15) return;
	globalSignalMasks[signal]=0;
}

void Thread::unblockSignalGlobally(SignalId signal){
	if(signal < 0 || signal > 15) return;
	globalSignalMasks[signal]=1;
}
void Thread::processAllSignals(){
	//CAST NULL POINTERA JE STA???
	//zabranjena promena konteksta, dodaj to
	int flag=0;

	for(int j=0;signals.getInfo(j);){
		SignalId* signal=(SignalId*)(signals.getInfo(j));
		if(*signal==0){

			AddToDeathList();flag=1;}
		if(!signalMask[*signal] || !globalSignalMasks[*signal]) {
			j++; //jer deletee u queue smanjuje broj elemenata u redu
			continue;}
		int i=0;
		while(handlers[*signal].getInfo(i)){
				SignalHandler hand=(SignalHandler)(handlers[*signal].getInfo(i));
				(*hand)();
				i++;
		}
		signals.deletee(signal);
		delete signal;

	} if(flag) dispatch();



}


////////////////////////////////////////////////
//SIGNAL 0
extern Queue waitingToDie;
extern Thread* killer;

void Thread::AddToDeathList(){
	//ne treba lock jer se vec izversava u kriticnoj sekciji
	waitingToDie.add(this);
	myPCB->status=PCB::FINISHED;
	if(killer -> myPCB-> status!=PCB::READY){
	killer -> myPCB-> status=PCB::READY;
	Scheduler::put(killer->myPCB);
	}
}

void dispatch(){
	requested_context_switch=1;
	  //context_switch_disabled=0;
	  timer_interrupt();
}

