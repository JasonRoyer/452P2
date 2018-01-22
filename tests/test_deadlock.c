/*
  test case for deadlock detection
  system should halt before output instruction

  O/P should never contain "should not see this"

  like

---------Starting Test 12 ----------
deadlock!

*/

#include "phase1.h"
#include <stdio.h>


int P2_Startup(void *arg){

  USLOSS_Console(" \n---------Starting test DEADLOCK ----------\n");
  P1_Semaphore sem;
  int result = P1_SemCreate("deadlock" , 0, &sem);
  if ( result != 0 )
  {
      USLOSS_Console("Could not create semaphore, result: %d\n", result );
  }
  
  P1_P(sem);
  USLOSS_Console("should not see this\n");
  return 0;
}

void test_setup(int argc, char **argv) {
    // Do nothing.
}

void test_cleanup(int argc, char **argv) {
    // Do nothing.
}
