#include "kernel.h"
#include "kersem.h"
#include "listpcb.h"
#include "listsem.h"
#include "pcb.h"
#include "schedule.h"

KernelSem::KernelSem(int init) {
	value = init;
	Kernel::semaphores->add(this);
}

int KernelSem::signal(int n){

	//--------------    if negative, echo    ------------------//
	if(n < 0)
		return n;

	//--------------- standard signal (+1)  -------------------//
	if(n == 0) {
		Kernel::lock();
		if(value++ < 0)
			unblock();
		Kernel::unlock();
		return 0;
	}

	//----------------   modified signal (+n) -----------------//
	value += n;
	int cnt = 0;
	while (cnt < n && (!blockedTime.empty() || !blockedInfinite.empty())){
		cnt++;
		unblock();
	}
	return cnt;
}

int KernelSem::wait(Time maxTimeToWait){
	if(maxTimeToWait < 0)
			return 2; 			// error handling

	//------------- standard wait (block handles time) --------//
	Kernel::lock();
	if(--value < 0)
		block(maxTimeToWait);
	Kernel::unlock();

	PCB *p = (PCB*)Kernel::runningPCB;

	int ret = p->maxTimeToWait != 0 ? (p->maxTimeToWait <= p->waitedTime ? 0 : 1) : 1; // calculate return

	p->maxTimeToWait = 0;	// restore pcb state
	p->waitedTime = 0;

	return ret;
}

void KernelSem::block(int maxTimeToWait){

	PCB *p = (PCB*)Kernel::runningPCB;
	p->status = PCB::BLOCKED;				// will not be added to scheduler on context change

	//------------- standard block -------------//
	if(maxTimeToWait == 0) {
		blockedInfinite.add(p);
		dispatch();
		return;
	}

	//------------ block with maxTime ---------//
	p->maxTimeToWait = maxTimeToWait;
	p->waitedTime = 0;
	blockedTime.add(p);
	dispatch();
	return;

}

void KernelSem::unblock(){
	PCB *p;
	if(!blockedTime.empty())				// Time restricted PCBs have priority
		p = blockedTime.removeFirst();
	else if (!blockedInfinite.empty())
		p = blockedInfinite.removeFirst();
	else
		return;

	p->status = PCB::READY;
	Scheduler::put(p);
}

void KernelSem::tick(){
	ListPCB::node *curr = blockedTime.head;
	while(curr){
		if(++curr->pcb->waitedTime >= curr->pcb->maxTimeToWait){		// If exceeded time, unblock pcb
			PCB *p = curr->pcb;
			curr = curr->next;
			blockedTime.remove(p);
			p->status = PCB::READY;
			Scheduler::put(p);
			continue; // as to not skip a pcb
		}
		curr = curr->next;
	}
}

int KernelSem::getVal() const {
	return value;
}
