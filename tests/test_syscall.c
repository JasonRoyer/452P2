/*
test case for system call handling
output should be something like:

---------Starting Test 13 ----------
System call handlers has not been inplemented yet! (the content of this line depends on the implementation of dummy handler)
Not in kernel mode!
SIMULATOR TRAP: privileged instruction (USLOSS_Halt)
Aborted

*/
#include "phase1.h"
#include <stdio.h>

P1_Semaphore sem;



int P2_Startup(void *arg){

    USLOSS_Console(" \n---------Starting Test SYSCALL ----------\n");
    int psrResult = USLOSS_PsrSet(USLOSS_PsrGet()&0xfe);    //switch to user mode
    if ( USLOSS_DEV_OK != psrResult ) {
        USLOSS_Console("Problem with PsrSet: %d\n", psrResult);
    }
    USLOSS_Syscall(NULL);
    return 0;
}


void test_setup(int argc, char **argv) {
    // Do nothing.
}

void test_cleanup(int argc, char **argv) {
    // Do nothing.
}
