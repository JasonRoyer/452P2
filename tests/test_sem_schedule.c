/*
  test case for preemptive scheduling (test if p and v opts are priority aware)
  should output:

---------Starting Test SCHEDULER_WITH_SEM ----------
high start
low start
high finish
low finish
 ---------Ending Test SCHEDULER_WITH_SEM ----------

*/

#include "phase1.h"
#include <stdio.h>

    int childpid1, child_status1, exit_childpid1;
    int childpid2, child_status2, exit_childpid2;
    P1_Semaphore sem;

int high(void *arg){
    USLOSS_Console("high start\n");
    P1_P(sem);  //will block
    USLOSS_Console("high finish\n");
    return -1;
}

int low(void *arg){
    USLOSS_Console("low start\n");
    P1_V(sem);  //will be switched off
    USLOSS_Console("low finish\n");
    return -2;
}

int P2_Startup(void *arg){

    USLOSS_Console(" \n---------Starting Test SCHEDULER_WITH_SEM ----------\n");

    int result = P1_SemCreate("semaphore" , 0, &sem );
    if ( result != 0 )
    {
        USLOSS_Console("Could not create semaphore, result: %d\n", result );
    }
    childpid1 = P1_Fork("low", low, NULL, USLOSS_MIN_STACK, 3 , 0);
    childpid2 = P1_Fork("high", high, NULL, USLOSS_MIN_STACK, 2 , 0);
    exit_childpid1 = P1_Join(0, &child_status1);
    exit_childpid2 = P1_Join(0, &child_status2);

    USLOSS_Console(" ---------Ending Test SCHEDULER_WITH_SEM ----------\n");
    return 0;
}

void test_setup(int argc, char **argv) {
    // Do nothing.
}

void test_cleanup(int argc, char **argv) {
    // Do nothing.
}
