/* ------------------------------------------------------------------------
   phase2.c
   Version: 2.0

   Jason Royer
   
   Skeleton file for Phase 2. These routines are very incomplete and are
   intended to give you a starting point. Feel free to use this or not.


   ------------------------------------------------------------------------ */

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <usloss.h>
#include <phase1.h>
#include <phase2.h>
#include "libuser.h"


static int ClockDriver(void *arg);
int P2_Spawn(char *name, int(*func)(void *), void *arg, int stackSize, int priority);
int P2_Wait(int *status);
static P1_Semaphore running;
typedef struct wrapperField
{
  int(*func)(void *);
  void *arg;
} wrapperField;

typedef struct mailBox
{
  int numSlots;
  int msgSize;
  int numMsg;
  int currentCount;
  int numWaitingSends;
  int numWaitingRecvs;
  int isRunning;
  P1_Semaphore*	canSend;
  P1_Semaphore*	canRead;
  P1_Semaphore*	readMutex;
  char** box;
  
} mailBox;
mailBox mailBoxTable[P2_MAX_MBOX];


/* the process table */
wrapperField wrapperTable[P1_MAXPROC];



void syscallHandler(int dev, void * arg)
{
	USLOSS_Sysargs* sa = (USLOSS_Sysargs*) arg;
	int sysNum = sa->number;
	int outPut;
	int success = 0;
	int fail = -1;
	switch(sysNum){
		case 1:
		
			break;
		case 2:
		
			break;
		case 3:
			// Sys_Spawn (syscall 3)
			outPut = P2_Spawn((char*)sa->arg5, sa->arg1, sa->arg2, (int)sa->arg3,(int) sa->arg4);
			if(outPut >-2){
				
				sa->arg1 = (void*)outPut;
				sa->arg4 = (void*)success;
			}else {
				sa->arg4 = (void*)fail;
			}
			break;
		case 4:
			// Sys_Wait (syscall 4)
			outPut = P2_Wait((int*)sa->arg2);
			if(outPut >0){
				sa->arg4 = (void*)success;
			}else {
				sa->arg4 = (void*)fail;
			}
			break;
		case 5:
			// Sys_Terminate (syscall 5)
			P2_Terminate((int)sa->arg1);
			break;
		case 6:
			//Sys_MboxCreate (syscall 6)
			outPut = P2_MboxCreate((int)sa->arg1,(int)sa->arg2);
			if(outPut != -2){
				 sa->arg1 = (void*)outPut;
				sa->arg4 = (void*)success;
			}else {
				sa->arg4 = (void*)fail;
			}
			break;
		case 7:
		
			break;
		case 8:
			// Sys_MboxSend (syscall 8)
			outPut = P2_MboxSend((int) sa->arg1, sa->arg2, sa->arg3);
			sa->arg4 = (void*)outPut;
			break;
		case 9:
			//  Sys_MboxReceive (syscall 9)
			//USLOSS_Console("outside recieve\n");
			outPut = P2_MboxReceive((int) sa->arg1, sa->arg2, sa->arg3);
			//USLOSS_Console("P2_MboxReceive output %d\n", outPut);
			if(outPut >0){
				sa->arg2 = (void*)outPut;
				sa->arg4 = (void*)success;
			}else {
				sa->arg4 = (void*)outPut;
			}
			break;
		case 10:
		
			break;
		
		case 11:
		
			break;
			
		case 12:
			// Sys_Sleep (syscall 12)
			sa->arg4 = (void*)P2_Sleep((int)sa->arg1);
			
			break;
		case 13:
		
			break;
		case 14:
		
			break;
		case 15:
		
			break;
			
		case 16:
			//Sys_SemCreate (syscall 16)
			outPut = P1_SemCreate(sa->arg2, (int)sa->arg1, sa->arg1);
			sa->arg4 = (void*)outPut;
			break;
			
		case 17:
			// Sys_SemP (syscall 17)
			sa->arg4 = (void*)P1_P(sa->arg1);
			break;
			
		case 18:
			//Sys_SemV (syscall 18)
			sa->arg4 = (void*)P1_V(sa->arg1);
			break;
			
		case 19:
			// Sys_SemFree (syscall 19)
			sa->arg4 =(void*)P1_SemFree(sa->arg1);
			break;
			
		case 20:
			//  Sys_GetTimeOfDay (syscall 20)
			break;
			
		case 21:
			//Sys_CPUTime (syscall 21)
			sa->arg1 = (void*)P1_ReadTime();
			break;
			
		case 22:
			//Sys_GetPID (syscall 22)
			sa->arg1 = (void*)P1_GetPID();
			break;
			
		case 23:
			//  Sys_DumpProcesses (syscall 23)
			P1_DumpProcesses();
			break;
		
	}
	
	
}

void setUserMode(){
	int currentPSR = USLOSS_PsrGet();	
	int newPSR = (currentPSR & 0xe) | 0x4;
	int i = USLOSS_PsrSet(newPSR);
	if(i != 0){
		
	}
}

void setKernelMode(){
	int currentPSR = USLOSS_PsrGet();	
	int newPSR = (currentPSR & 0xb) | 0x1;
	int i = USLOSS_PsrSet(newPSR);
	if(i != 0){
		
	}
}



int
P2_Startup(void *arg)
{
    int               status;
    int               result = 1; // default is there was a problem
    int               rc;
	USLOSS_IntVec[USLOSS_SYSCALL_INT] = syscallHandler;
    /*
     * Check kernel mode
     */
    // ...

    /*
     * Create clock device drivers.
     */
    rc = P1_SemCreate("running", 0, &running);
    if (rc != 0) {
        USLOSS_Console("P1_SemCreate of running failed: %d\n", rc);
        goto done;
    }
    for (int i = 0; i < USLOSS_CLOCK_UNITS; i++) {
        rc = P1_Fork("Clock driver", ClockDriver, (void *) i, USLOSS_MIN_STACK, 2, 0);
        if (rc < 0) {
            USLOSS_Console("Can't create clock driver\n");
            goto done;
        }
        /*
         * Wait for the clock driver to start.
         */
        rc = P1_P(running);
		//USLOSS_Console("Made it past running\n");
        if (rc != 0) {
            USLOSS_Console("P1_P(running) failed: %d\n", rc);
            goto done;
        }
    }
    /*
     * Create the other device drivers.
     */
    // ...

    /* 
     * Create initial user-level process. You'll need to check error codes, etc. P2_Spawn
     * and P2_Wait are assumed to be the kernel-level functions that implement the Spawn and 
     * Wait system calls, respectively (you can't invoke a system call from the kernel).
     */
    rc = P2_Spawn("P3_Startup", P3_Startup, NULL,  4 * USLOSS_MIN_STACK, 3);
    if (rc < 0) {
        USLOSS_Console("Spawn of P3_Startup failed: %d\n", rc);
    } else {
        rc = P2_Wait(&status);
        if (rc < 0) {
            USLOSS_Console("Wait for P3_Startup failed: %d\n", rc);
            goto done;
        }
    }
	

    /*
     * Make the device drivers abort using P1_WakeupDevice.
     */

    for (int i = 0; i < USLOSS_CLOCK_UNITS; i++) {
        rc = P1_WakeupDevice(USLOSS_CLOCK_DEV, i, 1);
        if (rc < 0) {
            USLOSS_Console("Wakeup of clock device driver failed : %d\n", rc);
            goto done;
        }
    }

    // ...

    /*
     * Join with the device drivers.
     */
    // ...

    result = 0;
done:
    return result;
}




static int
ClockDriver(void *arg)
{
    int unit = (int) arg;
    int result = 1; // default is there was a problem
    int dummy;
    int rc; // return codes from functions we call

    /*
     * Let the parent know we are running and enable interrupts.
     */
    rc = P1_V(running);
    if (rc != 0) {
        goto done;
    }
    rc = USLOSS_PsrSet(USLOSS_PsrGet() | USLOSS_PSR_CURRENT_INT);
    if (rc != 0) {
        USLOSS_Console("ClockDriver: USLOSS_PsrSet returned %d\n", rc);
        goto done;
    }
    while(1) {
        /*
         * Read new sleep requests from the clock mailbox and update the bookkeeping appropriately.
         */
        rc = P1_WaitDevice(USLOSS_CLOCK_DEV, unit, &dummy);
        if (rc == -3) { // aborted
            break;
        } else if (rc != 0) {
            USLOSS_Console("ClockDriver: P1_WaitDevice returned %d\n", rc);
            goto done;
        }
        /*
         * Compute the current time and wake up any processes
         * that are done sleeping by sending them a response.
         */
    }
    result = 0; // if we get here then everything is ok
done:
    return rc; 
}

int wrapperFunc(){
	int pid = P1_GetPID();
	setUserMode();
	int status = wrapperTable[pid].func(wrapperTable[pid].arg);
	 
	//setKernelMode();
	//P1_DumpProcesses();
	//Sys_DumpProcesses();
	Sys_Terminate(status);
	return 0;
}

int     
P2_Spawn(char *name, int (*func)(void *arg), void *arg, int stackSize,  int priority) 
{
	// fork should handle all the error checking if we return the return from fork
	// may need wrapper around func to call terminate
	
	int rc = P1_Fork(name, wrapperFunc, NULL, stackSize, priority, 1);
	if(rc >=0){
		wrapperTable[rc].func = func;
		wrapperTable[rc].arg = arg;
	}
    return rc;
}

void P2_Terminate(int status){
	// should just be able to call p1_quit
	//USLOSS_Console("terminating a proccess %d\n", P1_GetPID());
	P1_Quit(status);
//	USLOSS_Console("quit returned\n");	
}

int     
P2_Wait(int *status) 
{
	// should be able to call p1_Join with tag 0
	//USLOSS_Console("waiting for proccess %d to have child complete\n", P1_GetPID());
	int rc = P1_Join(1,status);
	//USLOSS_Console("waking from wait on  proccess %d\n", P1_GetPID());
    return rc;
}


// SYS CALLS
int P2_MboxCreate(int boxSize, int msgSize){
	int id = -1;
	for(int i =0;  i < P2_MAX_MBOX; i++){
		if(mailBoxTable[i].isRunning == 0){
			mailBoxTable[i].isRunning = 1;
			id = i;			
			break;
		}
		
	}
	//USLOSS_Console("Found usable id for mailbox %d\n", id);
	if(id >= 0){
			
		
		mailBoxTable[id].numSlots = boxSize;
		//USLOSS_Console("num slots now  %d\n", mailBoxTable[id].numSlots);
		mailBoxTable[id].msgSize = msgSize;
		mailBoxTable[id].currentCount = -1;
		char semName[7];
		
		sprintf(semName, "%ds",id);
		
		P1_SemCreate(semName,1,(P1_Semaphore *)&mailBoxTable[id].canSend);
		//USLOSS_Console("got to break %s\n", semName);
		char semName2[7];
		sprintf(semName2, "%dr",id);
		P1_SemCreate(semName2,0,(P1_Semaphore *)&mailBoxTable[id].canRead);
		
		char semName3[7];
		sprintf(semName3, "%dm",id);
		P1_SemCreate(semName3,1,(P1_Semaphore *)&mailBoxTable[id].readMutex);
		
		mailBoxTable[id].box = malloc(sizeof(char*)*boxSize);
	} else {
		// no mailboxes left
		return -2;
	}
	return id;
}

int P2_MboxRelease(int id){
	if(id <0 || id > P2_MAX_MBOX){
		return -2;
	}else if(mailBoxTable[id].isRunning == 0){
		// trying to release mailbox that is not created
		return -1;
	}else {
		// release mailbox
		for(int i = 0; i < mailBoxTable[id].numSlots; i++){
			free(mailBoxTable[id].box[i]);
		}
		for(int i=0; i < mailBoxTable[id].numWaitingSends; i++){
			// free all the waiting sends
			//they should just return -2;
			P1_V(mailBoxTable[id].canSend);
		}
		
		for(int i=0; i < mailBoxTable[id].numWaitingRecvs; i++){
			// free all the waiting recieves
			//they should just return -2;
			P1_V(mailBoxTable[id].canRead);
		}
		
		P1_SemFree(mailBoxTable[id].canSend);
		P1_SemFree(mailBoxTable[id].canRead);
		mailBoxTable[id].msgSize = 0;
		mailBoxTable[id].numMsg = 0;
		mailBoxTable[id].numSlots = 0;
		mailBoxTable[id].isRunning = 0;
		return 0;
	}
}

int P2_MboxSend(int id, void* msg, int *size){
	
	if(mailBoxTable[id].numSlots <0){
		return -1;
	}
	//USLOSS_Console("send called %d \n", id);
	//int mySize = (int) *size;
	if((int)size > mailBoxTable[id].msgSize){
		return -1;
	}
	
	mailBoxTable[id].numWaitingSends++;
	while(1){
		P1_P(mailBoxTable[id].canSend);
		mailBoxTable[id].numWaitingSends--;
		if(mailBoxTable[id].isRunning){
			if(mailBoxTable[id].currentCount +1 < mailBoxTable[id].numSlots){
				mailBoxTable[id].currentCount++;
				mailBoxTable[id].box[mailBoxTable[id].currentCount] = (char*)malloc(mailBoxTable[id].msgSize);
				memcpy(mailBoxTable[id].box[mailBoxTable[id].currentCount], msg, (int)size);
				
				P1_V(mailBoxTable[id].canRead);
				P1_V(mailBoxTable[id].canSend);
				return 0;
			}else {
				// mailbox full and we need to block again
				mailBoxTable[id].numWaitingSends++;
			}
		}else {
			// send called after mailbox released
			return -2;
		}
	}
}

int P2_MboxReceive(int id, void* buff, int *sizeBuff){
	
	if(mailBoxTable[id].numSlots <0){
		return -1;
	}
	mailBoxTable[id].numWaitingRecvs++;
	while(1){
		//USLOSS_Console("waiting on canRead %d\n", id);
		P1_P(mailBoxTable[id].canRead);
		//USLOSS_Console("got past on canRead\n");
		P1_P(mailBoxTable[id].readMutex);
		//USLOSS_Console("RECEVING\n");
		mailBoxTable[id].numWaitingRecvs--;
		if(mailBoxTable[id].isRunning){
			if(mailBoxTable[id].currentCount >= 0 ){
				//USLOSS_Console("before copy in recieve\n");
				memcpy(buff, mailBoxTable[id].box[mailBoxTable[id].currentCount], (int)sizeBuff);
				//USLOSS_Console("after copy in recieve\n");
				P1_V(mailBoxTable[id].readMutex);
				return (int)sizeBuff;
			}else {
				// should never have this problem where we don't have anything to read.
				mailBoxTable[id].numWaitingRecvs++;
			}
		}else {
			// recev called after mailbox released
			return -2;
		}
	}

}

int P2_Sleep(int seconds){
	//USLOSS_Console("P2_sleep was called\n");
//	int status;
	//P1_WaitDevice(USLOSS_CLOCK_DEV, 0, &status);
	return 2;
}
