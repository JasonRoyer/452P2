#include "usloss.h"
int totalPoints = 0;
int score = 0;

typedef enum bool {
    false,
    true
}bool;

void judge(int stat, int pt) {
    if (stat) {
        score+=pt;
        USLOSS_Console("###TEST###<+%d> PASSED\n", pt);
    } else {
        USLOSS_Console("###TEST###FAILED\n");
    }
    totalPoints+=pt;
}

void report() {
    USLOSS_Console("###TEST TOTAL###%d\n", totalPoints);
    USLOSS_Console("###TEST SCORE###%d\n", score);
}



