#ifndef LISTSEM_H_
#define LISTSEM_H_

#include "defines.h"

class Semaphore;
class KernelSem;

class ListSem {

public:

	class node {
	public:
		KernelSem *sem;
		node *next;
		node(KernelSem *sem, node *next = 0) {
			this->sem = sem;
			this->next = next;
		}
	};

	node *head, *tail;
	int size;

	ListSem();
	~ListSem();

	void add(KernelSem* sem);
	void remove(KernelSem* sem);
	int isEmpty();
	KernelSem* removeFirst();
	void tick();

};

#endif /* LISTSEM_H_ */
