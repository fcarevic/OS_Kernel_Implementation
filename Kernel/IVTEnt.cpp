#include "IVTEnt.h"
#include <dos.h>

IVTEntry* IVTEntry::allEntries[256]={0};
IVTEntry::IVTEntry(IVTNo n,PIntrr new_routine):num(n)
{  asm cli;
	routine=getvect(num);
	setvect(num,new_routine);
	allEntries[num]=this;
	asm sti;
	}

IVTEntry::~IVTEntry(){
	asm cli;
	setvect(num,routine);
	//asm int num;
	routine();
	allEntries[num]=0;
	asm sti;

}

void IVTEntry::call(){
	routine();

}




