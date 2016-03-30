/* ========================================
 *
 * Copyright Ethan Gordon, Luke Pfleger, 2016
 * Car Lab, Task 2
 * Angle Module
 *
 * ========================================
*/
#include "angle.h"
#include <device.h>

/**
  * Initialize Angle Servo
  **/
void Angle_Start()
{
    Servo_Start();
    Angle_Set(0.0f);
}

/* Set Angle (in degrees), Clamps from -30 to 30 */

/* Servo_WriteCompare(1470); Center */
/* Servo_WriteCompare(1847); Right */
/* Servo_WriteCompare(1093); Left */
/* angle = m*x + b; m = 0.079576; b=-116.98 */
/* inverted slope: 1/m = 12.5667 */
/* inverted intercept: -b/m = 1470 */
void Angle_Set(int angle)
{
    int output;
    
    output = angle + 1470;
    
    Servo_WriteCompare(output);
}
    
/* [] END OF FILE */
