#ifndef PROCESS_H
#define PROCESS_H
#include "list.h"
#include"processScheduler.h"

//enum ProcessStatus {JUST_CREATED, RUNNING, READY, BLOCKED};
class Thread;
class Process {

    public:
    
    Process(char *name, int priority);
    ~Process();

    void Fork(VoidFunctionPtr func, void *arg); 
   
    void Yield();  		// Relinquish the CPU if any 
				// other thread is runnable
    void Sleep(bool finishing); // Put the thread to sleep and 
				// relinquish the processor
    void Begin();		// Startup code for the thread	
    void Finish(); 
    char* getName(); 


    void setStatus(ThreadStatus st);
    
    int getPriority();
    void setPriority(int priority);
		
    Thread* getCurrentThread();
    
    List<Thread*> getThreadList();
    void setThreadList(Thread* t);
    void setChildList(Process* child);
    bool isEmpty();
    
    bool isChildEmpty();
    void removeChild(Process *child);
    
   
    Thread* getProcessThread();
    Thread *currentThread;
    ProcessScheduler *scheduler;
    
    void createProcessThread(char *name,VoidFunctionPtr func, void *arg);
    Process* createChild(char *name, VoidFunctionPtr func, void *arg);
   
    int getPid();
    int getPpid();
    
    private:
    int priority;
    int PID;
    int PPID;
    ThreadStatus status;
    char *name;
    List<Thread*> *threadList;
    List<Process*> *childList;
    Process *parent;
  
   // Thread *currentThread; 
             
};

#endif // PROCESS_H

