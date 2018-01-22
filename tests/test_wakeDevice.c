#include <stdio.h>
#include "phase1.h"
#include <stdlib.h>
#include <assert.h>

/* 
This tests whether P1_WakeDevice works
*/

int wake_parent( void *arg )
{
    USLOSS_Console("child: Testing P1_WakeupDevice with valid parameters\n");
    P1_WakeupDevice( USLOSS_TERM_DEV, 0, 1 );
    USLOSS_Console("child: After testing P1_WakeupDevice with valid parameters\n");
    return 0;
}

int P2_Startup(void *arg) {
    int status = 0;
    USLOSS_Console("parent: Starting child\n");
    P1_Fork("child", wake_parent, NULL, USLOSS_MIN_STACK, 3 , 0);
    USLOSS_Console("parent: Testing P1_WaitDevice with valid parameters, should now be blocked\n");
    int return_code = P1_WaitDevice( USLOSS_TERM_DEV, 0, &status); // Wait for the interrupt to occur
    USLOSS_Console("parent: After Testing P1_WaitDevice with valid parameters, no longer blocked\n");
    assert ( return_code == 0 );
    assert( status == -3 );
    return 0;
}

void test_setup(int argc, char **argv) {
    // Do nothing.
}

void test_cleanup(int argc, char **argv) {
    // Do nothing.
}
