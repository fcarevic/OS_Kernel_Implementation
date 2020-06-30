/*
 * queue.cpp
 *
 *  Created on: Apr 21, 2019
 *      Author: OS1
 */
#include "queue.h"
extern int requested_context_switch;
extern int context_switch_disabled;
void interrupt timer_interrupt(...);
#define lock context_switch_disabled++;
#define unlock if(context_switch_disabled){context_switch_disabled--; if(!context_switch_disabled) if(requested_context_switch)timer_interrupt();};



 void Queue::add(void* i){
	lock
	  if(first==nullptr)
		  first=last=new Elem(i);
	  else {last->next=new Elem(i);
                last=last->next;                   }
	 unlock
}


 /////////////////////////
 void* Queue::remove(){

	 if(!first) return nullptr;
	 lock
	 void* info=first->info;
	 Elem* disp=first;
	 first=first->next;
	 if(first==nullptr) last=first;
	 delete disp;
	 unlock
	 return info;

	 }
 void Queue::deletee(void* n){

	 if(!first)return;
	 lock
	 Elem* disp;
	 if(n==first->info){disp=first; first=first->next;}
	 else{
		 Elem* prev=first;
		 for(;prev && prev->next->info!=n;prev=prev->next);
		 if(!prev) {unlock return;}
		 if(n==last->info){disp=last; last=prev; last->next=nullptr;}
		 else {disp=prev->next; prev->next=disp->next;}


	 }
	 delete disp;
	 unlock
}
 void* Queue::getInfo(int n){
	 if(n<0)return nullptr;
	 lock
	 Elem* tek=first;
	 while(tek){
		 if(!(n--)){ unlock;return tek->info;}
		 tek=tek->next;
	 }
	unlock
	 return nullptr;


 }
void Queue::copy(Queue& q){

	Elem* tek=q.first;
	while(tek){
		add(tek->info);
		tek=tek->next;
	}
}
 void Queue::swap(void* arg1, void* arg2){
   lock
   Elem* tek= first;
   Elem* temp1=0;
   Elem* temp2=0;
   while(tek){
	   if(tek->info==arg1) temp1=tek;
	   else if(tek->info==arg2) temp2=tek;
	   tek=tek->next;
   }

   if(!temp1 || !temp2) { unlock; return;}
   temp1->info=arg2;
   temp2->info=arg1;
 unlock
 }






 void Queue::clear(){
	 lock
	 while(first!=nullptr){last=first;
		 first=first->next;
		 delete last;
		 }
		 last=nullptr;
	unlock
}


 Queue::~Queue(){
     clear();
}

 //void* first_(){return first->info;}
