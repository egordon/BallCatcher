/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <device.h>
#include <stdio.h>
#include "drivetrain.h"

#define PAN_OFFSET 8320
#define TILT_OFFSET 8180
#define GYRO_OFFSET 0

#define RANGE_CUTOFF 1900
#define GYRO_DEADBAND 30
#define RANGE_ALPHA 0.2f

#define ALPHA 0.9f

#define GYRO_SEL 0
#define RANGE_SEL 1

/* PID Params */
#define PAN_SET 0
#define RANGE_SET 53000
#define GYRO_SET 24420

static short pan = 0;

CY_ISR(pan_isr)
{
    pan = (int)(ALPHA * (float)((int)PanTime_ReadCapture() - (int)PAN_OFFSET) + (1.0f - ALPHA)*(float)(pan));
    Pan_Reset_Write(1);
}

static short tilt = 0;

CY_ISR(tilt_isr)
{
    tilt = (int)(ALPHA * (float)((int)TiltTime_ReadCapture() - (int)TILT_OFFSET) + (1.0f - ALPHA)*(float)(tilt));
    Tilt_Reset_Write(1);
}

static int gyro = GYRO_SET;
static unsigned int range = 45000;
static int flag = GYRO_SEL;
CY_ISR(analog_isr)
{
    if (flag == RANGE_SEL) {
        if (Detection_Status_Read()) {
            range = ADC_GetResult16();
        } else range = 0;
    } else {
        int gyro_tst = ADC_GetResult16();
        gyro = (gyro_tst > 23000) ? ((gyro_tst < 26000) ? gyro_tst : gyro) : gyro;
    }
}

static int startDelay = 600;
CY_ISR(gyro_isr)
{
    if (startDelay > 0) startDelay--;
    flag = GYRO_SEL;
}

CY_ISR(range_isr)
{
    flag = RANGE_SEL;
}

void main()
{
    int finished = 0;
    char tstr[17] = {0};
    unsigned int range_end = 56000;

    /* Loop Vars */
    float gyro_sum = 0.0f;
    float pan_prev = 0.0f;
    float range_prev = 0.0f;
    
    /* Start LCD Screen */
    LCD_Start();
    
    /* Start PWM Signals */
    DriveTrain_Start();
    
    /* Start Counters */
    PanTime_Start();
    TiltTime_Start();
    AMux_Sel_Start();
    ADC_Start();
    ADC_StartConvert();
    
    /* Start Interrupts */
    Pan_int_Start();
    Pan_int_SetVector(pan_isr);
    Tilt_int_Start();
    Tilt_int_SetVector(tilt_isr);
    Gyro_int_Start();
    Gyro_int_SetVector(gyro_isr);
    Range_int_Start();
    Range_int_SetVector(range_isr);
    Analog_int_Start();
    Analog_int_SetVector(analog_isr);

    CyGlobalIntEnable;
    while(finished < 2)
    {
        float pan_err, range_err, gyro_err;
        float gyro_p, gyro_i;
        float pan_p, pan_d;
        float range_p, range_d;
        
        sprintf(tstr, "Tilt: %5d", tilt);
        
        LCD_Position(0, 0);
        LCD_PrintString(tstr);
        
        pan_err = pan - PAN_SET;
        range_err = range - RANGE_SET;
        gyro_err = gyro - GYRO_SET;
        if ((gyro_err < GYRO_DEADBAND) && (gyro_err > -GYRO_DEADBAND))
            gyro_err = 0;
        
        /* Gyro PID Loop */
        gyro_p = 0.3 * gyro_err;
        
        gyro_sum += gyro_err;
        /* Clamp Sum */
        if (gyro_sum > 10000) gyro_sum = 20000;
        else if (gyro_sum < -10000) gyro_sum = -20000;
        gyro_i = 0.00 * gyro_sum;
        
        /* Pan PID Loop */
        pan_p = 11.0f * pan_err;
        pan_d = 3.0f *(pan_err - pan_prev);
        pan_prev = pan_err;
        
        /* Tilt PD Loop */
        range_p = 3.0f * (range_err / 30.0f);
        range_d = 2.5f *(range_err - range_prev);
        range_prev = range_err;
        
        /* Don't do dangerous things before start delay */
        if (startDelay <= 0) {
            if(Detection_Status_Read()) {
                    if (tilt < -150) {
                        DriveTrain_Stop();
                        LCD_Position(1, 0); 
                        LCD_PrintString("Stopped.....");
                    } else {
                        DriveTrain_Set((int)(pan_p + pan_d), -(int)(range_p + range_d), (int)(gyro_p + gyro_i));
                        LCD_Position(1, 0); 
                        LCD_PrintString("Running.....");
                    }
            } else {
                DriveTrain_Stop();
                LCD_Position(1, 0); 
                LCD_PrintString("Stopped.....");
            }
        }
    }
    
    DriveTrain_Stop();
    
    for(;;){}
}

/* [] END OF FILE */
