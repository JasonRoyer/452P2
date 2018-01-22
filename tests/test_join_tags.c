#include "phase1.h"
#include <assert.h>
#include <stdio.h>

int ChildTag1(void *arg) {
    return 1;
}

int ChildTag0(void *arg ){
    return 2;
}

int P2_Startup(void *notused)
{
    USLOSS_Console( "Testing P1_Join with differing tags\n" );
    int status = 0;
    P1_Fork("ChildTag0", ChildTag0, NULL, USLOSS_MIN_STACK, 3 , 0);
    P1_Fork("ChildTag1", ChildTag1, NULL, USLOSS_MIN_STACK, 3 , 1);
    P1_Join( 1, &status );
    assert( status == 1 );
    P1_Join( 0 , &status );
    assert( status == 2 );
    return 0;
}

void test_setup(int argc, char **argv) {
    // Do nothing.
}

void test_cleanup(int argc, char **argv) {
    // Do nothing.
}
