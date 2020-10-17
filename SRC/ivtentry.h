#ifndef _ivtentry_h_
#define _ivtentry_h_

#include <IOSTREAM.H>

typedef void interrupt (*pInterrupt)(...);
class KernelEv;
class Kernel;

#define PREPAREENTRY(entryNum,callOldRoutine)\
void interrupt intRoutine##entryNum(...);\
IVTEntry entry##entryNum(entryNum, intRoutine##entryNum);\
void interrupt intRoutine##entryNum(...) { \
	entry##entryNum.signal();\
	if (callOldRoutine == 1) entry##entryNum.callOld();\
}


class IVTEntry {

	int entryNum;
	pInterrupt oldInterruptRoutine;
	KernelEv *kerEv;

	void setKernelEvent(KernelEv *kerEv);

	friend class KernelEv;
	friend class Kernel;

public:
	IVTEntry(int entryNum, pInterrupt interruptRoutine);
	~IVTEntry();

	void callOld();
	void signal();
	void wait();


};

#endif
