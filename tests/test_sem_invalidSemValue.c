/*
  Test case for Semaphores. Invalid sempahore value

  should print some message similar to the below one, and ideally should call Halt, thereby preventing operations on an invalid semaphore.

  ---------Starting Test INVALID_SEM_VALUE ----------
  1
  ---------Ending Test INVALID_SEM_VALUE ------------
  0

*/

#include "phase1.h"
#include <stdio.h>

P1_Semaphore sem1;

int child(void *arg) {
    USLOSS_Console(" %d \n", 0);
    P1_V(sem1);
    return 0;
}

int P2_Startup(void *arg){

    USLOSS_Console(" \n---------Starting Test INVALID_SEM_VALUE ----------\n");
    P1_Fork("child", child, NULL, USLOSS_MIN_STACK, 2 , 0);
    int result = P1_SemCreate("semaphore" , -1, &sem1 );
    if ( result != 0 )
    {
        USLOSS_Console("Semaphore was not created, result: %d\n", result );
    }
    P1_P(sem1);
    USLOSS_Console(" %d \n", 1);

    USLOSS_Console(" ---------Ending Test INVALID_SEM_VALUE ----------\n");
    return 0;
}

void test_setup(int argc, char **argv) {
    // Do nothing.
}

void test_cleanup(int argc, char **argv) {
    // Do nothing.
}
