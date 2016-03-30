/* ========================================
 *
 * Copyright Ethan Gordon, Luke Pfleger, 2016
 * Car Lab, Task 1
 * State Estimation Module
 *
 * Goal: Maintain a constant speed of 4 ft/s.
 * ========================================
*/
#include "state.h"

/* Constants */
#define STDDEV_SIZE 10

/* Total Circumference = 20cm = 0.656168 ft */
/* 5 Magnets per Revolution: 0.656168 / 5 = 0.1312336 ft */
#define MAG_DIST 0.1312336f

/* State Variables */
static int i = 0;
static float pastVals[STDDEV_SIZE] = {0};

static float mAlpha = 0.0f;
static float mAvg = 0.0f;

/* Watchdog */
static int errCount = 0;

/* Initialize State */
void State_Start(float alpha) {
    mAlpha = alpha;
    mAvg = 0.0f;
}

/* Update State Estimate Given New Information */
void State_Update(int val) {
    float testSpeed;
    /* Feed Watchdog */
    errCount = 0;
    
    /* Exponential Moviong Average */
    testSpeed = (MAG_DIST * 1000.0f) / (float)(val);
    mAvg = mAvg*(1-mAlpha) + (float)(testSpeed)*mAlpha;
    
    pastVals[i] = testSpeed;
    i = (i + 1) % STDDEV_SIZE;
}

/* Return State Estimate */
float State_Avg(void) {
    return mAvg;
}

/* Return State Covariance */
float State_Var(void) {
    float var = 0.0f;
    int i;
    for(i = 0; i < STDDEV_SIZE; i++) {
        var += (pastVals[i] - mAvg)*(pastVals[i] - mAvg);
    }
    return var;
}

/* Called Every Millisecond */
void State_Watchdog(void) {
    errCount++;
    /* If Idle for 1s, Reset Speed */
    if(errCount >= 1000)
        mAvg = 0;
}
    
/* [] END OF FILE */
