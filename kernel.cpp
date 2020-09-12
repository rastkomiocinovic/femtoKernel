#include "thread.h"
#include "pcb.h"
#include <dos.h>
#include <stdio.h>
#include "kernel.h"
#include "listpcb.h"
#include "listsem.h"
#include "ivtentry.h"
#include <IOSTREAM.H>
#include "schedule.h"


ListPCB *Kernel::pcbs = new ListPCB();
ListSem *Kernel::semaphores = new ListSem();

volatile unsigned Kernel::lockFlag = 0;

volatile int Kernel::counter = 0;
volatile int Kernel::context_on_demand = 0;

//char Kernel::ivtReady = 0;

PCB *Kernel::mainPCB = 0, *Kernel::idlePCB = 0;
volatile PCB* Kernel::runningPCB = 0;

pInterrupt Kernel::oldRoutine = 0;

IVTEntry *Kernel::ivtEntries[256];

Address tsp = 0, tss = 0, tbp = 0;


void Kernel::setup() {
	#ifndef BCC_BLOCK_IGNORE
	asm pushf;
	asm cli;
	Kernel::oldRoutine = getvect(OLD_ENTRY);
	setvect(OLD_ENTRY, timer);
	asm popf;
	#endif


	mainPCB = new PCB(4096, 2, 0);
	mainPCB->status = PCB::READY;
	counter = mainPCB->timeSlice;
	Kernel::runningPCB = mainPCB;

	idlePCB = new PCB(256, 1, 0);
	idlePCB->status = PCB::READY;

	//------------- setup IVTEntry table  -------------//
	/*for(int i = 0; i < 256; i++){
		Kernel::ivtEntries[i] = 0;
	}*/
}

void Kernel::restore() {
	#ifndef BCC_BLOCK_IGNORE
	asm pushf;
	asm cli;
	setvect(OLD_ENTRY, Kernel::oldRoutine);
	asm popf;
	#endif
}


void interrupt timer(...) {

	//------------------------------ If timer isn't on demand -------------------------------//

	if (!Kernel::context_on_demand) {
		Kernel::counter--;
		tick();
		Kernel::semaphores->tick();
		(*Kernel::oldRoutine)();
	}


	//------------------------------ Should the context change? -----------------------------//

	if (!(Kernel::counter <= 0 && Kernel::runningPCB->timeSlice != 0)	// if context should change
			&& !Kernel::context_on_demand)
		return;

	if (Kernel::lockFlag) {												// if locked, change on unlock
		Kernel::context_on_demand = 1;
		return;
	}



	//------------------------------ Context change -----------------------------------------//
	//cout << "number of sems: " << Kernel::sems->num_of_nodes << endl;

	Kernel::context_on_demand = 0;	// Reset request

	// Context saving

	#ifndef BCC_BLOCK_IGNORE
	asm {
		mov tsp, sp
		mov tss, ss
		mov tbp, bp
	}
	#endif

	Kernel::runningPCB->sp = tsp;
	Kernel::runningPCB->ss = tss;
	Kernel::runningPCB->bp = tbp;
	if (Kernel::runningPCB->status == PCB::READY
			&& Kernel::runningPCB != Kernel::idlePCB) {
		//cout << "ubacen" << endl;
		Scheduler::put((PCB*) Kernel::runningPCB);
	}


	//do {
	Kernel::runningPCB = Scheduler::get();		// pick new pcb
	if (!Kernel::runningPCB){
		Kernel::runningPCB = Kernel::idlePCB;
		//cout << "idle";
	}
	//} while(!Kernel::runningPCB->stack);

	//cout << Kernel::runningPCB->sp;

	// Context restoring

	Kernel::counter = Kernel::runningPCB->timeSlice;

	tsp = Kernel::runningPCB->sp;
	tss = Kernel::runningPCB->ss;
	tbp = Kernel::runningPCB->bp;

	#ifndef BCC_BLOCK_IGNORE
	asm {
		mov sp, tsp
		mov ss, tss
		mov bp, tbp
	}
	#endif


}//*/

/*void interrupt timer(...) {

	if (!Kernel::context_on_demand) {
		Kernel::counter--;
		tick();
		Kernel::semaphores->tick();
		(*Kernel::oldRoutine)();
	}
	if (Kernel::counter <= 0 || Kernel::context_on_demand) {
		if (Kernel::lockFlag) { 	// If it's unlocked
			Kernel::context_on_demand = 0;	// Reset request
#ifndef BCC_BLOCK_IGNORE
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
	#endif
			Kernel::runningPCB->sp = tsp;
			Kernel::runningPCB->ss = tss;
			Kernel::runningPCB->bp = tbp;
			if (Kernel::runningPCB->status == PCB::READY && Kernel::runningPCB != Kernel::idlePCB) {
				Scheduler::put((PCB*) Kernel::runningPCB);
			}

			do {
				Kernel::runningPCB = Scheduler::get();
				if (!(Kernel::runningPCB))
					Kernel::runningPCB= Kernel::idlePCB;
			} while(!Kernel::runningPCB->stack);

			Kernel::counter = Kernel::runningPCB->timeSlice;

			tsp = Kernel::runningPCB->sp;
			tss = Kernel::runningPCB->ss;
			tbp = Kernel::runningPCB->bp;


#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
#endif
		}
		else {
			Kernel::context_on_demand = 1;
		}
	}
}//*/

void dispatch() {
	asm pushf;
	asm cli;
	Kernel::context_on_demand = 1;
	timer();
	asm popf;
}


void Kernel::lock() {
	Kernel::lockFlag = 1;
}
void Kernel::unlock() {
	Kernel::lockFlag = 0;
	if (Kernel::context_on_demand)
		dispatch();
}
