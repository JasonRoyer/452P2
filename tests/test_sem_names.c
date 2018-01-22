/*
  test case for semaphores with the same name

*/

#include "phase1.h"
#include <stdio.h>
#include <assert.h>

int childpid1, child_status1, exit_childpid1;
int childpid2, child_status2, exit_childpid2;
P1_Semaphore sem;
P1_Semaphore sem2;
char *same_name = "semaphore";


int low(void *arg){
    USLOSS_Console("Testing of create semaphore with identical name\n" );
    int result = P1_SemCreate( same_name , 0, &sem2 );
    assert( result == -1 );
    USLOSS_Console("Semaphore not created\n" );
    return -2;
}

int P2_Startup(void *arg){

    int result = P1_SemCreate( same_name , 0, &sem );
    if ( result != 0 )
    {
        USLOSS_Console("Could not create semaphore, result: %d\n", result );
    }
    childpid1 = P1_Fork("low", low, NULL, USLOSS_MIN_STACK, 3 , 0);
    return 0;
}

void test_setup(int argc, char **argv) {
    // Do nothing.
}

void test_cleanup(int argc, char **argv) {
    // Do nothing.
}
