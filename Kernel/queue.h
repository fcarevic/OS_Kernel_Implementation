/*
 * queue.h
 *
 *  Created on: Apr 21, 2019
 *      Author: OS1
 */

#ifndef QQUEUE_H_
#define QQUEUE_H_
#define nullptr 0

class Queue{
	friend class Thread;
	friend class PCB;
	struct Elem{

		Elem* next;
		void* info;
	  Elem(void* i, Elem* n=nullptr):info(i), next(n){}
	};
  Elem* first, *last;

public:
  Queue(){ first=last=nullptr;}
  void copy(Queue& q);
  void add(void* i);
  void* remove();
  void deletee(void*);
  void* getInfo(int n);
  void clear();
  void swap(void*, void*);
  ~Queue();





};



#endif /* QUEUE_H_ */
