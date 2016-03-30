/* ========================================
 *
 * Copyright Ethan Gordon, Luke Pfleger, 2016
 * Car Lab, Task 2
 * PID Loop Interface
 *
 * ========================================
*/
#ifndef PID_H
#define PID_H

/* PID Object */
typedef struct PID* PID_T;

/* Pull a pointer for a PID Object. */
PID_T PID_Alloc();

/**
  * Initialize PID Object
  **/
void PID_Start(PID_T pObj, float kp, float ki, float lowClamp, float highClamp);

/* Change PID Setpoint, Also Resets Transient Measurement Information */
void PID_Setpoint(PID_T pObj, float setpoint);

/* Update PID Loop with value. If customDt == 0, dt is calculated from millisecond clock. */
void PID_Update(PID_T pObj, float val, int customDt);

/* Get output of PID loop at this time step. */
float PID_Output(PID_T pObj);

/*
 * Populates Array with Transient Information:
 * Rise Time, Overshoot, Undershoot, 
 * and Steady State Estimation
 */
void PID_Transients(PID_T pObj, float* ret);

#endif
/* [] END OF FILE */
