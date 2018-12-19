#include "copyright.h"
#include "process.h"

#include "switch.h"
#include "synch.h"
#include "sysdep.h"

int id = 1;
Process::Process(char *processName, int priority){
    this->PID = id++;
    this->name = processName;
    this->PPID = 0;
    this->priority = priority;
    this->status = JUST_CREATED;
    Thread *t = new Thread(name);
    
    this->currentThread = t;
    this->scheduler = new ProcessScheduler();
    threadList = new List<Thread*>;
    childList = new List<Process *>;
}
Process::~Process()
{
    DEBUG(dbgThread, "Deleting thread: " << name);

    ASSERT(this != kernel->currentProcess);
    delete this->currentThread;
//    if (stack != NULL)
//	DeallocBoundedArray((char *) stack, StackSize * sizeof(int));
}
void
Process::Begin ()
{
    ASSERT(this == kernel->currentProcess);
    DEBUG(dbgThread, "Beginning thread: " << name);
    
    kernel->scheduler->CheckProcessToBeDestroyed();
    kernel->interrupt->Enable();
}

char* Process::getName() { return (name); }

int Process::getPriority() { 
    return priority;
}

void Process::setPriority(int priority) {
	this->priority = priority;
}

void Process::setStatus(ThreadStatus st) {
        this->status = st;
}

Thread* Process::getCurrentThread() {
    return this->currentThread;
}

int Process::getPid()
{
  return this->PID;
}

int Process::getPpid()
{
  return this->PPID;
}

void Process::createProcessThread(char *name,VoidFunctionPtr func, void *arg){
   Thread *t = new Thread(name);
   t->Fork(this, func, arg);

}

Process* Process::createChild(char *name, VoidFunctionPtr func, void *arg){
   Process *child = new Process(name, this->getPriority());
   child->Fork(func, arg);
   this->setChildList(child);
   child->PPID = this->getPid();
   return child;
}

void Process::setThreadList(Thread* t){
   threadList->Append(t);
}

void Process::setChildList(Process* child){
  childList->Append(child);
}

bool Process::isEmpty() {
  return threadList->IsEmpty();
}

Thread* Process::getProcessThread() {
    return threadList->RemoveFront();
}

void Process::Fork(VoidFunctionPtr func, void *arg){
    Interrupt *interrupt = kernel->interrupt;

    IntStatus oldLevel;
    
    DEBUG(dbgThread, "Forking thread: " << name << " f(a): " << (int) func << " " << arg);
    
    Thread *t = new Thread(this->getName()); 
    oldLevel = interrupt->SetLevel(IntOff);
    t->Fork(this,func, arg);
    
    cout <<" ## FORKING  PROCESS " << this->getName() << " with PRIORITY " << this->getPriority() <<" =>" ;
    this->currentThread = this->scheduler->FindNextThreadToRun();
    
    this->setStatus(READY);
    kernel->scheduler->ProcessReadyToRun(this, this->getPriority());	// ReadyToRun assumes that interrupts 
					// are disabled!
    (void) interrupt->SetLevel(oldLevel);

}

void
Process::Yield ()
{
    Process *nextProcess;
    IntStatus oldLevel = kernel->interrupt->SetLevel(IntOff);
    ASSERT(this == kernel->currentProcess);
    cout << " ** CURRENT PROCESS ** " << kernel->currentProcess->currentThread->getName()<<"\n";   
    nextProcess = kernel->scheduler->FindNextProcessToRun();
    if (nextProcess != NULL) {
	kernel->scheduler->ProcessReadyToRun(this,this->getPriority());
	kernel->scheduler->RunProcess(nextProcess, FALSE);
    }
    (void) kernel->interrupt->SetLevel(oldLevel);
}

void
Process::Finish ()
{
    (void) kernel->interrupt->SetLevel(IntOff);		
    ASSERT(this == kernel->currentProcess);
    
  // DEBUG(dbgThread, "Finishing thread: " << name);
    
    Sleep(TRUE);				// invokes SWITCH
    // not reached
}

void
Process::Sleep (bool finishing)
{
    Process *nextProcess;
   // Thread *nextThread;
    
    ASSERT(this == kernel->currentProcess);
    ASSERT(kernel->interrupt->getLevel() == IntOff);
    
    DEBUG(dbgThread, "Sleeping thread: " << name);

    status = BLOCKED;
    while ((nextProcess = kernel->scheduler->FindNextProcessToRun()) == NULL)
	kernel->interrupt->Idle();	// no one to run, wait for an interrupt
    
    // returns when it's time for us to run
    this->Begin();
    (void) kernel->interrupt->SetLevel(IntOff);
    kernel->scheduler->RunProcess(nextProcess, finishing); 
}
