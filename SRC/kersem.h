#ifndef _kersem_h_
#define _kersem_h_

#include "listpcb.h"
#include "defines.h"



class KernelSem {
	int value;

	ListPCB blockedInfinite;
	ListPCB blockedTime;

	void block(int maxTimeToWait);
	void unblock();

public:
	KernelSem(int init);

	int signal(int n);
	int wait(Time maxTimeToWait);

	int getVal() const;

	void tick();
};

#endif
