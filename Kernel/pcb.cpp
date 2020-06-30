/*
 * pcb.cpp
 *
 *  Created on: Apr 21, 2019
 *      Author: OS1
 */
#include "pcb.h"

int PCB::PID;
const int PCB::NEW=0;
const int PCB::READY=1;
const int PCB::RUNNING=2;
const int PCB::BLOCKED=3;
const int PCB::FINISHED=4;
const int PCB::IDLE=5;
PCB::PCB(StackSize stackSize, Time timeSlice, ID pp):
    	stack_size(stackSize/sizeof(unsigned int)),time_slice(timeSlice){
	    pid=PID++;
	    ppid=pp;


    	}




