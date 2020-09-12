#include "pcb.h"
#include <stdio.h>
#include <dos.h>
#include "listpcb.h"
#include "kernel.h"
#include "schedule.h"
#include <IOSTREAM.H>


ID PCB::staticID = 0;

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* myThread) {

	//------------- setup values ---------------//

	this->stackSize = stackSize;
	this->timeSlice = timeSlice;
	this->myThread = myThread;
	status = START;
	maxTimeToWait = 0;
	waitedTime = 0;
	Id = ++staticID;

	Kernel::pcbs->add(this);

	//------------- setup stack ---------------//

	stackSize /= sizeof(unsigned);
	stack = new Address[stackSize];

	stack[stackSize - 3] = 0x200;

	#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 4] = FP_SEG(PCB::wrapper);
	stack[stackSize - 5] = FP_OFF(PCB::wrapper);

	ss = FP_SEG(stack + stackSize - 14);
	sp = FP_OFF(stack + stackSize - 14);
	#endif
	bp = sp;

	stack[stackSize - 14] = bp;


}

/*PCB::PCB(StackSize stackSize, Time timeSlice) {
	this->stackSize = stackSize;
	this->timeSlice = timeSlice;
	this->myThread = 0;				// not used for idle and main pcb
	state = START;
	maxTimeToWait = 0;
	this->waitTime = 0;

	stackSize /= sizeof(unsigned);
	stack = new Address[stackSize];

	stack[stackSize - 3] = 0x200;

	#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 4] = FP_SEG(PCB::wrapper);
	stack[stackSize - 5] = FP_OFF(PCB::wrapper);

	ss = FP_SEG(stack + stackSize - 14);
	sp = FP_OFF(stack + stackSize - 14);
	#endif
	bp = sp;

	stack[stackSize - 14] = bp;
}//*/


PCB::~PCB() {
	Kernel::lock();
	Kernel::pcbs->remove(this);
	unblockBlocked();
	delete stack;
	Kernel::unlock();
}

void PCB::wrapper() {
	if (Kernel::runningPCB == Kernel::idlePCB){
		while(1) { }		// if idle thread, loop indefinitely
	}


	Kernel::runningPCB->myThread->run();

	Kernel::runningPCB->unblockBlocked(); // unblock all pcbs blocked by this one ( resolves mutual waiting )

	Kernel::runningPCB->status = FINISHED;
	dispatch();
}

void PCB::start() {
	if (status == READY)	// don't restart
		return;

	status = READY;
	Scheduler::put(this);

}

void PCB::waitToComplete() {

	if (status == FINISHED || status == START || this == Kernel::idlePCB || this == Kernel::mainPCB || this == (PCB*) Kernel::runningPCB){
		return;
	}



	Kernel::lock();
	Kernel::runningPCB->status = PCB::BLOCKED;
	blockedBy.add((PCB*) Kernel::runningPCB);	//set status to blocked and add to blockedBy list in order to unblock on finish
	dispatch();
	Kernel::unlock();
}

void PCB::unblockBlocked() {
	while(!blockedBy.empty()){
		PCB *p = blockedBy.removeFirst();
		p->status = READY;
		Scheduler::put(p);
	}
}

PCB* PCB::getPCBById(ID id){
	return Kernel::pcbs->getById(id);
}

ID PCB::getId() const {
	return Id;
}
