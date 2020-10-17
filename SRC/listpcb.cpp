#include "listpcb.h"
#include "pcb.h"

ListPCB::ListPCB() {
	head = tail = 0;
	size = 0;
}

ListPCB::~ListPCB() {
	while(head){
		tail = head;
		head = head->next;
		delete tail;
	}
}

void ListPCB::add(PCB *p) {
	size++;

	if(head == 0){						// if first insertion
		head = tail = new node(p);
		return;
	}

	tail = tail->next = new node(p);	//else
}

void ListPCB::remove(PCB *p){
	if(head->pcb == p){		// if first node
		node *old = head;
		head = head->next;
		delete old;
		return;
	}

	node *curr = head;		// else
	while(curr->next){
		if(curr->next->pcb == p){
			node *old = curr->next;
			curr->next = curr->next->next;
			if(old == tail) // refresh tail if last element
				tail = curr;
			delete old;
			return;
		}
		curr = curr->next;
	}
}


/*void ListPCB::remove(PCB* pcb) {
	node *temp = head, *prev = 0, *oldNode;
	while (temp) {
		if (temp->pcb != pcb) {
			prev = temp;
			temp = temp->next;
		}
		else {
			oldNode = temp;
			temp = temp->next;
			if (!prev) head = temp;
			else prev->next = temp;
			delete oldNode;
			size--;
		}
	}
}*/


int ListPCB::empty() {
	return size == 0 ? 1 : 0;
}

PCB* ListPCB::removeFirst() {
	if(!head)
		return 0;

	size--;
	node*old = head;
	PCB *p = old->pcb;
	head = head->next;
	delete old;
	return p;
}
/*PCB* ListPCB::removeFirst() {
	if (!head) return 0;
	node *temp = head;
	PCB* temp_pcb = temp->pcb;
	if (!head->next) head = tail = 0;
	else head = head->next;
	size--;
	delete temp;
	return temp_pcb;
}*/

PCB* ListPCB::getById(int id) {
	node *curr = head;
	while(curr){
		if(curr->pcb->getId() == id)
			return curr->pcb;
		curr = curr->next;
	}
	return 0;
}







