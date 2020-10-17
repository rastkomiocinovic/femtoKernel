#include "kernelev.h"
#include "kernel.h"
#include "schedule.h"
#include "pcb.h"
#include "ivtentry.h"
#include <IOSTREAM.H>

KernelEv::KernelEv(IVTNo ivtNo, Event *myEvent) {

	blocked = 0;

	//--------  ivt entry setup ----------//
	ivtEntry = Kernel::ivtEntries[ivtNo];
	ivtEntry->setKernelEvent(this);

	//---------- setup mutex -------------//
	this->val = 0;
	this->myEvent = myEvent;
	creator = (PCB*)Kernel::runningPCB;
}

KernelEv::~KernelEv(){
	signal();
	ivtEntry->setKernelEvent(0);
}

void KernelEv::signal() {
	if(blocked == 0) { // if creator isn't blocked, don't unblock
		val = 1;
		return;
	}

	//----------  unblock blocked  -------------//
	blocked->status = PCB::READY;
	Scheduler::put(blocked);
	blocked = 0;
}

void KernelEv::wait() {


	if(Kernel::runningPCB != creator)	// only creator can be blocked
		return;

	if(val == 1) {	// shouldn't block
			val = 0;
	}

	//-------------- block cretor pcb -----------//
	blocked = creator;
	blocked->status = PCB::BLOCKED;
	dispatch();

}


