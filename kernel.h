#ifndef KERNEL_H_
#define KERNEL_H_

#include "listpcb.h"
#include "listsem.h"
#include <stdio.h>
#include <stdarg.h>
#include <dos.h>
#include "defines.h"




class PCB;
class Thread;
class IVTEntry;

void interrupt timer(...);

extern void tick();

class Kernel {

public:

	static volatile unsigned lockFlag;
	static void lock();
	static void unlock();

	static ListPCB *pcbs;
	static ListSem *semaphores;

	static PCB *mainPCB, *idlePCB;
	static volatile PCB *runningPCB;

	static IVTEntry *ivtEntries[256];

	static pInterrupt oldRoutine;

	static volatile int counter;
	static volatile int context_on_demand;

	static void setup();
	static void restore();
	



};

void dispatch();

#endif /* KERNEL_H_ */
