#include "phase1.h"
#include <stdlib.h>
#include <assert.h>
#include "test.h"
#include <limits.h>

int childPid;

int child() {
    USLOSS_Console("No fork should succeed!\n");
    assert(false);
    return 0;
}

int P2_Startup(void *notused)
{
    childPid = P1_Fork("child", child, NULL, 0, 2 , 0);
    assert(childPid == -2);
    childPid = P1_Fork("child", child, NULL, -1, 2 , 0);
    assert(childPid == -2);
    childPid = P1_Fork("child", child, NULL, USLOSS_MIN_STACK - 1, 2 , 0);
    assert(childPid == -2);
    childPid = P1_Fork("child", child, NULL, USLOSS_MIN_STACK / 2, 2 , 0);
    assert(childPid == -2);
    childPid = P1_Fork("child", child, NULL, USLOSS_MIN_STACK - 2, 2 , 0);
    assert(childPid == -2);
    return 0;
}

void test_setup(int argc, char **argv) {
    // Do nothing.
}

void test_cleanup(int argc, char **argv) {
    // Do nothing.
}
