/* ========================================
 *
 * Copyright Ethan Gordon, Luke Pfleger, 2016
 * Car Lab, Task 2
 * PID Loop Module
 *
 * ========================================
*/
#include "pid.h"
#include <stdlib.h>

extern int millis;

/* Constants */
#define MAX_PID 1
#define EMA_ALPHA 0.1f
#define DEADBAND 0.001f
#define THRESHOLD 0.1f

/* Macros */
#define ABS(x)  ( ( (x) < 0) ? -(x) : (x) )

/* PID Structure */
struct PID {
    float kp, ki; /* Gains */
    float rt, ov, un, av; /* Transients */
    float set, out; /* Setpoint, Output */
    float sum; /* Past Error Information */
    float cLow, cHigh; /* Clamps */
    int lastTime, firstTime;
};

/* Static State Array */
struct PID mPIDArr[MAX_PID];

/* Pull a pointer for a PID Object. */
PID_T PID_Alloc()
{
    static int sAllocated = 0;
    if (sAllocated >= MAX_PID)
        return NULL;
    return &mPIDArr[sAllocated++];
}

/**
  * Initialize PID Object
  **/
void PID_Start(PID_T pObj, float kp, float ki, float lowClamp, float highClamp)
{
    pObj->kp    = kp;
    pObj->ki    = ki;
    pObj->set   = 0.0f;
    pObj->sum   = 0.0f;
    pObj->out   = 0.0f;
    pObj->cLow  = lowClamp;
    pObj->cHigh = highClamp;
    pObj->rt = pObj->ov = pObj->un = pObj->av = 0.0f;
    
    pObj->lastTime = 0;
    pObj->firstTime = 0;
}

/* Change PID Setpoint, Also Resets Transient Measurement Information */
void PID_Setpoint(PID_T pObj, float setpoint)
{
    pObj->set = setpoint;
    pObj->rt = pObj->av = 0.0f;
    pObj->un = pObj->ov = setpoint;
    pObj->lastTime = 0;
    pObj->firstTime = 0;
    pObj->sum   = 0.0f;
}

/* Update PID Loop with value. If customDt == 0, dt is calculated from millisecond clock. */
void PID_Update(PID_T pObj, float val, int customDt)
{
    float p, i, error;
    
    /* Custom Timestep in Millis */
    int dt = customDt ? customDt : millis - pObj->lastTime;
    pObj->lastTime = millis;
    
    /* Calculate Steady State Average */
    if (pObj->rt > DEADBAND) {
        pObj->av = EMA_ALPHA * val + (1.0f-EMA_ALPHA)*pObj->av;
    }
    
    /* Calculate Rise Time when value is within 10% of Setpoint */
    if (pObj->rt < DEADBAND && ABS(pObj->set - val) < (pObj->set * THRESHOLD))
        pObj->rt = (float)(millis - pObj->firstTime) / 1000.0f;
    
    /* Calculate Overshoot: Maximum value reached. */
    if (pObj->rt > DEADBAND && val > pObj->ov)
        pObj->ov = val;
    
    /* Calculate undershoot: Lowest value reached correcting overshoot. */
    if (pObj->ov > pObj->set && val < pObj->un)
        pObj->un = val;
        
    /*** PID Loop ***/
    /* Get Error */
    error = pObj->set - val;
    pObj->sum += (error * (float)(dt));
        
    /* Set PID */
    p = (pObj->kp * error);
    i = (pObj->ki * pObj->sum);
       
    pObj->out = (int)(p + i);
        
    /* Clamp */
    if (pObj->out > pObj->cHigh) pObj->out = pObj->cHigh;
    if (pObj->out < pObj->cLow)  pObj->out = pObj->cLow;
}

/* Get output of PID loop at this time step. */
float PID_Output(PID_T pObj)
{
    return pObj->out;
}

/*
 * Populates Array with Transient Information:
 * Rise Time, Overshoot, Undershoot, 
 * and Steady State Estimation
 */
void PID_Transients(PID_T pObj, float* ret)
{
    ret[0] = pObj->rt;
    ret[1] = pObj->ov;
    ret[2] = pObj->un;
    ret[3] = pObj->av;
}

    
/* [] END OF FILE */
