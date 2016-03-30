/* ========================================
 *
 * Copyright Ethan Gordon, Luke Pfleger, 2016
 * Car Lab, Task 2
 * State Estimation Module
 *
 * ========================================
*/
#include "state.h"
#include <stdlib.h>

/* Constants */
#define STDDEV_SIZE 10
#define MAX_STATE 1

/* State Structure */
struct State {
    float mInitVal, mAvg, mAlpha;
    float mPastVals[STDDEV_SIZE];
    int mPastSelect;
    int mWatchdog;
    int mWatchcount;
};

/* Static State Array */
struct State mStateArr[MAX_STATE];

State_T State_Alloc()
{
    static int sAllocated = 0;
    if (sAllocated >= MAX_STATE)
        return NULL;
    return &(mStateArr[sAllocated++]);
}


/**
  * Initialize State
  * obj: Object to initialize. Is modified.
  * alpha: Parameter for algorithm
  * initVal: Starting Average
  * watchdog: Watchdog number at which to reset to initVal.
  **/
void State_Start(State_T obj, float alpha, float initVal, int watchdog)
{
    int i;
    
    obj->mInitVal       = initVal;
    obj->mAvg           = initVal;
    obj->mAlpha         = alpha;
    obj->mWatchdog      = watchdog;
    obj->mWatchcount    = 0;
    obj->mPastSelect    = 0;
    
    for(i = 0; i < STDDEV_SIZE; i++)
        obj->mPastVals[i] = initVal;
}

/* Update State Estimate Given New Information */
void State_Update(State_T obj, float val)
{  
    /* Feed Watchdog */
    obj->mWatchcount = 0;
    
    /* Exponential Moviong Average */
    obj->mAvg = obj->mAvg*(1-(obj->mAlpha)) + val*(obj->mAlpha);
    
    /* Update Covariance History */
    obj->mPastVals[obj->mPastSelect] = val;
    obj->mPastSelect = (obj->mPastSelect + 1) % STDDEV_SIZE;
}

/* Return State Estimate */
float State_Avg(State_T obj)
{
    return obj->mAvg;
}

/* Return State Covariance */
float State_Var(State_T obj)
{
    float var = 0.0f;
    int i;
    for(i = 0; i < STDDEV_SIZE; i++) {
        var += (obj->mPastVals[i] - obj->mAvg)*(obj->mPastVals[i] - obj->mAvg);
    }
    return var;
}

/* Increase Watchdog Counter: Usually called every millisecond. */
void State_Watchdog(State_T obj)
{
    /* Check if Watchdog Enabled */
    if(!obj->mWatchdog)
        return;
    
    obj->mWatchcount++;
    
    /* If Watchdog Alerted: Reset */
    if(obj->mWatchcount >= obj->mWatchdog)
        obj->mAvg = obj->mInitVal;
}
    
/* [] END OF FILE */
