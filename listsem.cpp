#include "listsem.h"
#include "kersem.h"
#include "listpcb.h"
#include "pcb.h"

ListSem::ListSem() {
	head = tail = 0;
	size = 0;
}

ListSem::~ListSem() {
	while (head) {
		tail = head;
		head = head->next;
		delete tail;
	}
}

void ListSem::add(KernelSem* sem) {
	size++;

	if (head == 0) {						// if first insertion
		head = tail = new node(sem);
		return;
	}

	tail = tail->next = new node(sem);	//else
}

void ListSem::remove(KernelSem* sem) {
	if (head->sem == sem) {		// if first node
		node *old = head;
		head = head->next;
		delete old;
		return;
	}

	node *curr = head;		// else
	while (curr->next) {
		if (curr->next->sem == sem) {
			node *old = curr->next;
			curr->next = curr->next->next;
			if (old == tail) // refresh tail if last element
				tail = curr;
			delete old;
			return;
		}
		curr = curr->next;
	}
}

int ListSem::isEmpty() {
	return size == 0 ? 1 : 0;;
}

KernelSem* ListSem::removeFirst() {
	if (!head)
		return 0;

	size--;
	node*old = head;
	KernelSem *sem = old->sem;
	head = head->next;
	delete old;
	return sem;
}

void ListSem::tick(){
	node *curr = head;
	while(curr){
		curr->sem->tick();
		curr=curr->next;
	}
}
