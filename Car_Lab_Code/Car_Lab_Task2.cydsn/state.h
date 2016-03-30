/* ========================================
 *
 * Copyright Ethan Gordon, Luke Pfleger, 2016
 * Car Lab, Task 2
 * State Estimation Interface
 *
 * ========================================
*/
#ifndef STATE_H
#define STATE_H

/* State Estimation Object */
typedef struct State* State_T;

/* Pull a pointer for a state estimator. */
State_T State_Alloc();

/**
  * Initialize State
  * obj: Object to initialize. Is modified.
  * alpha: Parameter for algorithm
  * initVal: Starting Average
  * watchdog: Watchdog number at which to reset to initVal, 0 to disable.
  **/
void State_Start(State_T obj, float alpha, float initVal, int watchdog);

/* Update State Estimate Given New Information */
void State_Update(State_T obj, float val);

/* Increase Watchdog Counter: Usually called every millisecond. */
void State_Watchdog(State_T obj);

/* Return State Estimate */
float State_Avg(State_T obj);
/* Return State Covariance */
float State_Var(State_T obj);

#endif
/* [] END OF FILE */
