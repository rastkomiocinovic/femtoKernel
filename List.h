#ifndef LIST_H_
#define LIST_H_


template <class T>
class ListTemp {
public:
	struct node {
		node *next;
		T data;
		node(T d) { data = d; next = 0; }
	};

private:
	node *head, *tail;
	int size;

public:
	List() {
		head = tail = 0;
		size = 0;
	}

	~List() {
		node *prev = head;
		while(head) {
			head = head->next;
			delete prev;
			prev = head;
		}
	}

	void add(T data) {
		node* newNode = new node(data);
		if(!head) {
			head = tail = newNode;
			return ;
		}

		tail = tail->next = newNode;
	}

	void remove(T data) {
		if(!head) {
			return;
		}

		if(head->data == data){
			node* old = head;
			head=head->next;
			delete old;
			size--;
			return;
		}

		node* curr = head;
		while(curr->next){
			if(curr->next->data == data){
				node* old = curr->next;
				curr->next = old->next;
				delete old;
				size--;
				return;
			}
		}
	}

	T getHead() {
		return head != 0 ? head->data : 0;
	}


	int notEmpty() {
		return getHead() != 0;
	}

	int getSize() {
		return size;
	}

};

#endif
