#include "phase1.h"
#include <stdlib.h>
#include <assert.h>
#include "test.h"
#include <limits.h>

int childPid;

int child() {
    assert(true);
    return 0;
}

int bad_child() {
    assert(false);
    return 0;
}

int P2_Startup(void *notused)
{
    int i;
    for (i = 0; i < 48; i++) {
        P1_Fork("child", child, NULL, USLOSS_MIN_STACK, 3 , 0);
    }
    childPid = P1_Fork("bad child", bad_child, NULL, USLOSS_MIN_STACK, 3 , 0);
    assert(childPid == -1);
    return 0;
}

void test_setup(int argc, char **argv) {
    // Do nothing.
}

void test_cleanup(int argc, char **argv) {
    // Do nothing.
}
