/*
 * pcb.h
 *
 *  Created on: Apr 21, 2019
 *      Author: OS1
 */

#ifndef PCBB_H_
#define PCBB_H_
#include "queue.h"


typedef unsigned long StackSize;
 const StackSize defaultStackSize=4096;
typedef unsigned int Time; // time, x 55ms
extern const Time defaultTimeSlice; // default = 2*55ms
typedef int ID;

class PCB {
	friend void interrupt timer_interrupt(...);
	friend class KernelSem;
	friend class Thread;
	friend class KernelEvent;
	friend class Killer;

	static const int NEW;
	static const int READY;
	static const int RUNNING;
	static const int BLOCKED;
	static const int FINISHED;
	static const int IDLE;

	static int PID;
	ID pid;
	ID ppid;
	StackSize stack_size;
	Time time_slice;

	unsigned int * stack; //ili unsigned long ??????
    unsigned SP;
    unsigned SS;
    unsigned BP;
    int status;
    Queue waiting_for_end;
	ID creatorpid;

    PCB(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice,ID ppid);







};



#endif /* PCB_H_ */
