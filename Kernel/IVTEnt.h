#ifndef _IVTEntry_
#define _IVTEntry_

#include "KernEv.h"

typedef void interrupt (*PIntrr)(...);


class IVTEntry{

public:
	IVTEntry(IVTNo,PIntrr);
	~IVTEntry();
	void call();


static IVTEntry* allEntries[256];
private:
IVTNo num;
PIntrr routine;

};




#endif
