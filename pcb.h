#ifndef PCB_H_
#define PCB_H_

#include "thread.h"
#include "listpcb.h"
#include "defines.h"


class Semaphore;
class Thread;

class PCB {
	void unblockBlocked(); // unblocks all pcbs waiting on this one

	static ID staticID;
	ID Id;

public:
	enum StatusFlag { START, READY, FINISHED, BLOCKED };

	Time maxTimeToWait;
	Time waitedTime;
	Semaphore* semWaitingOn;
	StackSize stackSize;
	Time timeSlice;
	Address *stack, ss, sp, bp;
	StatusFlag status;

	Thread* myThread;
	ListPCB blockedBy;

	PCB(StackSize stackSize, Time timeSlice, Thread* myThread);
	//PCB(StackSize stackSize = 4096, Time timeSlice = 2);
	virtual ~PCB();

	static void wrapper();
	void start();
	void waitToComplete();

	int getId() const;

	static PCB* getPCBById(ID id);


};

#endif /* PCB_H_ */
