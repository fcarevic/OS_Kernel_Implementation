/*
 * Main.cpp
 *
 *  Created on: May 11, 2019
 *      Author: OS1
 */
#include "thread.h"
#include <dos.h>
#include <iostream.h>
extern Thread* running;
extern Thread* idleThread;
void init();
void restore();
int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]){
	//asm cli
	cout<<"pre\n";
	init();
  cout<<"init\n";
   userMain(argc,argv);
cout<<"usermain\n";

	restore();


	//delete running;
	//delete idleThread;


return 0;
}



