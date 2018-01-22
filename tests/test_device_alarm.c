/*
  test case for alarm

  O/P should resemble

  ---------Starting Test 10 ----------
wait period: 60005  (should be close to 60000)
 ---------Ending Test 10 ----------
*/

#include "phase1.h"
#include <stdio.h>

extern int USLOSSClock();

int P2_Startup(void *arg){

    USLOSS_Console(" \n---------Starting Test ALARM ----------\n");
    int status;
    int temp;
    temp = USLOSSClock();
    int deviceResult = USLOSS_DeviceOutput(USLOSS_ALARM_DEV, 0, (void*)3);
    if ( deviceResult != USLOSS_DEV_OK ) {
        USLOSS_Console("Result from DeviceOutput not ok: %d\n", deviceResult );
    }
    P1_WaitDevice(USLOSS_ALARM_DEV, 0, &status);
    temp = USLOSSClock() - temp;
    USLOSS_Console("wait period: %d  (should be close to 60000)\n", temp);

    USLOSS_Console(" ---------Ending Test ALARM ----------\n");
    return 0;
}

void test_setup(int argc, char **argv) {
    // Do nothing.
}

void test_cleanup(int argc, char **argv) {
    // Do nothing.
}
