/* ========================================
 *
 * Copyright Ethan Gordon, Luke Pfleger, 2016
 * Car Lab, Task 1
 * State Estimation Interface
 *
 * Goal: Maintain a constant speed of 4 ft/s.
 * ========================================
*/
#ifndef STATE_H
#define STATE_H

/* Initialize State */
void State_Start(float alpha);

/* Update State Estimate Given New Information */
void State_Update(int val);

/* Called Every Millisecond to Alert Watchdog */
void State_Watchdog(void);

/* Return State Estimate */
float State_Avg(void);
/* Return State Covariance */
float State_Var(void);

#endif
/* [] END OF FILE */
