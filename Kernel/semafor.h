#ifndef _MOJ_SEMMM
#define _MOJ_SEMMM
#include "KernelS.h"


class Semaphore {
public:
Semaphore(int n){ mySem= new KernelSem(n);}
int wait(int n=0) {return mySem->wait(n);}
int signal(int n=0) {return mySem->signal(n);}
int val(){return mySem->value();}
~Semaphore(){delete mySem;}

private:
 KernelSem* mySem;
};
#endif
