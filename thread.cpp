#include "thread.h"
#include "pcb.h"
#include <stdio.h>

#include "kernel.h"
#include "listpcb.h"



Thread::Thread(StackSize stackSize, Time timeSlice) {
	myPCB = new PCB(stackSize, timeSlice, this);
}

Thread::~Thread() {
	delete myPCB;
}

ID Thread::getId() {
	return myPCB->getId();
}

ID Thread::getRunningId() {
	return Kernel::runningPCB->getId();
}

void Thread::start() {
	myPCB->start();
}

void Thread::waitToComplete() {
	myPCB->waitToComplete();
}

Thread * Thread::getThreadById(ID id) {
	return PCB::getPCBById(id)->myThread;
}

