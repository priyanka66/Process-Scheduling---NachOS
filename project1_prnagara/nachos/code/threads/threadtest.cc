#include "kernel.h"
#include "main.h"
#include "thread.h"
#include "process.h"
#include <stdio.h>
#include <stdlib.h>

void test(){
 kernel->interrupt->SetLevel(IntOff);
    kernel->interrupt->SetLevel(IntOn);
    cout << " @@ RUNNING THREAD @@ " << kernel->currentProcess->currentThread->getID()<<"\n" ;
} 

void
SimpleThread(int which)
{  
    int i=0;
    cout << " ** RUNNING PROCESS ** "<< kernel->currentProcess->getName()<<"\n";
    for ( i=0;i<which;i++) {
    kernel->currentProcess->createProcessThread( kernel->currentProcess->getName() , (VoidFunctionPtr) test, NULL);
     }
   kernel->interrupt->SetLevel(IntOff);
   kernel->interrupt->SetLevel(IntOn);
   kernel->currentProcess->Yield();
}

void
SimpleThreadChild(int which)
  {
      int i=0;
      cout << " ** RUNNING CHILD PROCESS ** "<< kernel->currentProcess->getName()<<"\n";
      for ( i=0;i<which;i++) {
      kernel->currentProcess->createProcessThread( kernel->currentProcess->getName() , (VoidFunctionPtr) test, NULL);
       }
     kernel->interrupt->SetLevel(IntOff);
     kernel->interrupt->SetLevel(IntOn);
     kernel->currentProcess->Yield();
  }


void
ThreadTest()
{
   int priority = (rand()%5);
    Process *p1 = new Process("Process 1 ", priority);
    p1->Fork((VoidFunctionPtr) SimpleThread, (void *) 15);
    
  //  Process *p1C1 =  p1->createChild("Process 1 -Child",(VoidFunctionPtr) SimpleThreadChild, (void *) 3);
    
    //Process *p1C1C1 = p1C1->createChild("Process 1 - Child 1- Child 1",(VoidFunctionPtr) SimpleThreadChild, (void *) 3);
    priority = (rand()%5);
    Process *p2 = new Process("Process 2 ", priority);
    p2->Fork((VoidFunctionPtr) SimpleThread, (void *) 3);
  
 
   Process *p3 =  p2->createChild("Process 2 - Child ",(VoidFunctionPtr) SimpleThreadChild, (void *) 3);
     
   priority = (rand()%5);
   Process *p4 = new Process("Process  3", priority);
   p4->Fork((VoidFunctionPtr) SimpleThread, (void *) 4);
   Process *p4C1 =  p4->createChild("Process 3 - Child - 1 ",(VoidFunctionPtr) SimpleThreadChild, (void *) 3);
   kernel->currentProcess->Yield();
}
