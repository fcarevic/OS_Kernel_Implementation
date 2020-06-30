/*
 * KernelS.h
 *
 *  Created on: May 9, 2019
 *      Author: OS1
 */

#ifndef KERNELSS_H_
#define KERNELSS_H_
#include "queue.h"
#include "pcb.h"
#include "thread.h"

class KernelSem {
	struct TimePCB{
		PCB* pcb;
		int time_left;
		TimePCB(PCB* p, int n):pcb(p),time_left(n){}
	};
public:
	KernelSem(int);
    int wait(int i=0);
    int signal(int i=0);
    int value();
    ~KernelSem();
    static void signalTimerinterrupt();


private:
	int val;
	Queue blocked;
	static Queue allSems;
};




#endif /* KERNELS_H_ */
