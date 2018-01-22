#include "phase1.h"
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

int childPid = 0;

int child_2() {
    USLOSS_Console(" \n---------Starting Test DUMP ----------\n");
    P1_DumpProcesses();
    USLOSS_Console("P1_ReadTime(): %d\n", P1_ReadTime());
    USLOSS_Console(" \n---------End Test DUMP ----------\n");
    return 0;
}

int child_4() {
    return 0;
}

int child_3() {
    P1_Fork("child 4", child_4, NULL, USLOSS_MIN_STACK, 4, 0);
    childPid = P1_Fork("child 2", child_2, NULL, USLOSS_MIN_STACK, 2, 0);
    return 0;
}

int P2_Startup(void *notused)
{
    P1_Fork("child 3", child_3, NULL, USLOSS_MIN_STACK, 3, 0);
    return 0;
}

void test_setup(int argc, char **argv) {
    // Do nothing.
}

void test_cleanup(int argc, char **argv) {
    // Do nothing.
}
