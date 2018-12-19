#ifndef PROCESS_SCHEDULER_H
#define PROCESS_SCHEDULER_H

#include "copyright.h"
#include "list.h"
#include "thread.h"

//class Thread;
class ProcessScheduler {
  public:
    ProcessScheduler();		// Initialize list of ready threads 
    ~ProcessScheduler();		// De-allocate ready list

    void ReadyToRun(Thread* thread);	
    void addBackMainThread (Thread *thread);   				// Thread can be dispatched.

    Thread* FindNextThreadToRun();	// Dequeue first thread on the ready 
				// list, if any, and return thread.
    void Run(Thread* nextThread, bool finishing);
    				// Cause nextThread to start running
    void CheckToBeDestroyed();// Check if thread that had been
    				// running needs to be deleted
    void Print();		// Print contents of ready list
    
    // SelfTest for scheduler is implemented in class Thread
    
  private:
    List<Thread *> *readyList;  // queue of threads that are ready to run,
				// but not running
    Thread *toBeDestroyed;	// finishing thread to be destroyed
    				// by the next thread that runs
};

#endif // PROCESS_SCHEDULER_H

