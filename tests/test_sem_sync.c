/*
  Test case for Semaphores. Tests synchronization combinations using semaphores

 Should print:

---------Starting Test SEM_SYNC ----------
start phase2_start
join kernel_start (1)
start XXp1
exit status1 = -3
join kernel_start (2)
start XXp2
join XXp2
start XXp3
exit status3 = 0
exit status2 = 5
P(sem2)
start XXp4
V(sem2)
V(sem1)
join kernel_start (3)
P(sem1)
exit status4 = 7
 ---------Ending Test SEM_SYNC ----------

*/



#include "phase1.h"
#include <stdio.h>


int XXp1(void *), XXp2(void *), XXp3(void *), XXp4(void *);
char buf[256];

P1_Semaphore sem1, sem2;

int P2_Startup(void *arg) {

  USLOSS_Console(" \n---------Starting Test SEM_SYNC ----------\n");
  int status, pid1, pid2;

  USLOSS_Console("start phase2_start\n");
  int result;
  result = P1_SemCreate("sem1" , 0, &sem1 );
  if ( result != 0 )
  {
    USLOSS_Console("Could not create semaphore 1, result: %d\n", result );
  }
  result = P1_SemCreate("sem2" , 0, &sem2 );
  if ( result != 0 )
  {
    USLOSS_Console("Could not create semaphore 2, result: %d\n", result );
  }
  //while(i<5){
    //i++;
    pid1 = P1_Fork("XXp1",XXp1, "XXp1", USLOSS_MIN_STACK,1 , 0);
    /*print_proc(2);*/
  //}
  pid2 = P1_Fork("XXp2",XXp2, "XXp2", USLOSS_MIN_STACK,2 , 0);
  USLOSS_Console("join kernel_start (1)\n");
  //while(i+3){
    //i--;
    P1_Join(0, &status);
  //}
  sprintf(buf,"exit status1 = %d\n", status); USLOSS_Console(buf);
  USLOSS_Console("join kernel_start (2)\n");
  P1_Join(0, &status);
  sprintf(buf,"exit status2 = %d\n", status); USLOSS_Console(buf);
  pid1 = P1_Fork("XXp4",XXp4, "XXp4", USLOSS_MIN_STACK, 3 , 0);
  USLOSS_Console("P(sem2)\n");
  P1_P(sem2);
  USLOSS_Console("V(sem1)\n");
  P1_V(sem1);
  USLOSS_Console("join kernel_start (3)\n");
  P1_Join(0, &status);
  sprintf(buf,"exit status4 = %d\n", status); USLOSS_Console(buf);

  USLOSS_Console(" ---------Ending Test SEM_SYNC ----------\n");
  return 0;
}

int XXp1(void *arg)
{
  int i;

  USLOSS_Console("start %s\n", (char *) arg);
  for(i=0; i<100; i++) ;
  return -3;
}

int XXp2(void *arg)
{
  int status, pid;

  USLOSS_Console("start %s\n", (char *) arg);
  pid = P1_Fork( "XXp3", XXp3, "XXp3", USLOSS_MIN_STACK,4 , 0);
  USLOSS_Console("join XXp2\n");
  P1_Join(0, &status);
  sprintf(buf,"exit status3 = %d\n", status); USLOSS_Console(buf);
  //P1_Join(&status);
  //sprintf(buf,"exit status3 = %d\n", status); USLOSS_Console(buf);
  return 5;
}

int XXp3(void *arg)
{
  int i;
  USLOSS_Console("start %s\n", (char *) arg);
  for(i=0; i<5000000; i++) ;
  return 0;
}

int XXp4(void *arg)
{
  USLOSS_Console("start %s\n", (char *) arg);
  USLOSS_Console("V(sem2)\n");
  P1_V(sem2);
  USLOSS_Console("P(sem1)\n");
  P1_P(sem1);
  return 7;
}

void test_setup(int argc, char **argv) {
    // Do nothing.
}

void test_cleanup(int argc, char **argv) {
    // Do nothing.
}
