/*
  Test case for Semaphores. Ensures that a semaphore initialized to 3, blocks - only after allowing 3 P1_P calls without blocking.

  should print :

  ---------Starting Test SEM_INIT ----------
 0
 1
 ---------Ending Test SEM_INIT ----------

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

    USLOSS_Console(" \n---------Starting Test SEM_INIT ----------\n");
    P1_Fork("child", child, NULL, USLOSS_MIN_STACK, 2 , 0);
    int result = P1_SemCreate("semaphore" , 3, &sem1 );
    if ( result != 0 )
    {
        USLOSS_Console("Could not create semaphore, result: %d\n", result );
    }
    P1_P(sem1);
    P1_P(sem1);
    P1_P(sem1);
    P1_P(sem1);
    USLOSS_Console(" %d \n", 1);

    USLOSS_Console(" ---------Ending Test SEM_INIT ----------\n");
    return 0;
}

void test_setup(int argc, char **argv) {
    // Do nothing.
}

void test_cleanup(int argc, char **argv) {
    // Do nothing.
}
