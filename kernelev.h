#ifndef _kernelev_h_
#define _kernelev_h_

#include "defines.h"

class Event;
class PCB;
class IVTEntry;

class KernelEv {
	Event *myEvent;
	PCB *creator, *blocked;
	IVTEntry *ivtEntry;

	int val;
public:
	KernelEv(IVTNo ivtNo, Event *myEvent);
	~KernelEv();

	void wait   ();
	void signal ();
};

#endif
