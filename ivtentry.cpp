#include "ivtentry.h"
#include "dos.h"
#include "kernelev.h"
#include "kernel.h"
#include <IOSTREAM.H>



IVTEntry::IVTEntry(int entryNum, pInterrupt interruptRoutine){

	Kernel::ivtEntries[entryNum] = this;	// add entry to global table
	kerEv = 0;

	//------------- setup ivt entry -------------//

	#ifndef BCC_BLOCK_IGNORE
	asm pushf;
	asm cli;
	oldInterruptRoutine = getvect(entryNum);
	setvect(entryNum, interruptRoutine);
	asm popf;
	#endif

}

IVTEntry::~IVTEntry(){

	Kernel::ivtEntries[entryNum] = 0;	// restore global table entry

	//------------ restore ivt entry ------------//

	#ifndef BCC_BLOCK_IGNORE
	asm pushf;
	asm cli;
	setvect(entryNum, oldInterruptRoutine);
	asm popf;
	#endif

}

void IVTEntry::callOld(){
	(*oldInterruptRoutine)();
}

void IVTEntry::signal(){
	if(kerEv != 0)		// error checking
		kerEv->signal();
}

void IVTEntry::wait(){
	if(kerEv != 0)		// error checking
		kerEv->wait();
}

void IVTEntry::setKernelEvent(KernelEv *kerEv){
	this->kerEv = kerEv;
}
