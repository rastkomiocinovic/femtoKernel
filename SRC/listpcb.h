#ifndef _list_h_
#define _list_h_

class Thread;
class PCB;

class ListPCB {
	int size;

public:

	struct node {
		PCB *pcb;
		node *next;
		node(PCB *pcb, node *next = 0) {
			this->pcb = pcb; this->next = next;
		}
	};

	node *head, *tail;

	ListPCB();
	~ListPCB();

	void add(PCB* pcb);
	void remove(PCB* pcb);
	int empty();
	PCB* removeFirst();
	PCB* getById(int ID);
	int onTick();

};

#endif
