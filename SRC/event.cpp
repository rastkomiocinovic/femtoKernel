#include "event.h"
#include "kernelev.h"
#include <IOSTREAM.H>

Event::Event(IVTNo ivtNo) {
	myImpl = new KernelEv(ivtNo, this);
}

Event::~Event(){
	delete myImpl;
}

void Event::wait() {
	myImpl->wait();
}

void Event::signal() {
	myImpl->signal();
}
