/* ========================================
 *
 * Copyright Ethan Gordon, Luke Pfleger, 2016
 * Car Lab, Task 1
 * Speed Controller
 *
 * Goal: Maintain a constant speed of 4 ft/s.
 * ========================================
*/
#include <device.h>
#include <stdio.h>

/* State Estimation */
#include "state.h"

/* Constants */
#define ALPHA 0.2f
#define SETPOINT 4.0f

static int millis = 0;

/* Define Clock Interrupt */
CY_ISR(clk)
{
    millis++;
    State_Watchdog();
}

/* Hall Effect Interrupt */
CY_ISR(spd)
{
    static int lastTime = 0;
    int currentTime = millis;
    
    /* Millisecond Pulse */
    HallOut_Write(1);
    
    /* Update State Estimate */
    if (lastTime != 0) State_Update(currentTime - lastTime);
    lastTime = currentTime;
}

void main()
{
    /*** Variable Declaration ***/
    /* PID Gains */
    float kp, ki, kd;
    
    /* PID Local Vars */
    float error, sum, last;
    
    float p, i, d;
    int final;
    
    float speed;
    
    /* LCD Display */
    char tstr[17] = {0};
    
    /* Debug Output Vars */
    float rt = 0.0f;
    float count = 0.0f;
    float sumAvg = 0.0f;
    float overshoot = 0.0f;
    float undershoot = 4.0f;
    
    /*** END Variable Declaration ***/
    
    CyGlobalIntEnable; /* Uncomment this line to enable global interrupts. */
    /* Clock Interrupt */
    clk_inter_Start();
    clk_inter_SetVector(clk);
    
    /* Start Counter */
    Counter_Start();
    
    /* Speed Interrupt */
    spd_inter_Start();
    spd_inter_SetVector(spd);
    
    /* Start LCD */
    LCD_Start();
    
    /* Start PWM */
    MOSFET_Start();
    
    /* Start State Estimator */
    State_Start(ALPHA);
    
    
    /**** Variable Initialization ****/
    
    /* PID Gains */
    kp = 800.0f;
    ki = 8.0f;
    kd = 0.0f;
    
    /* PID Local Vars */
    error = 0.0f;
    sum = 0.0f;
    last = SETPOINT;
    
    for(;;)
    {
        /* Begin PID Loop */
        
        /* Get Speed */
        speed = State_Avg();
        
        if (rt > 0.001) {
            sumAvg += speed;
            count += 1.0f;
        }
        
        if (rt < 0.001 && speed >= 3.6f)
            rt = (float)(millis) / 1000.0f;
            
        if (rt > 0.001 && speed > overshoot)
            overshoot = speed;
            
        if (overshoot > 4.0f && speed < undershoot)
            undershoot = speed;
        
        /* Debug Speed */
        LCD_Position(0, 0);
        sprintf(tstr, "Speed: %+1.2f", speed);
        LCD_PrintString(tstr);
        
        /* Get Error */
        error = SETPOINT - speed;
        sum += error;
        
        /* Set PID */
        p = (kp * error);
        i = (ki * sum);
        d = (kd * (error - last));
        last = error;
        
        final = (int)(p + i + d);
        
        /* Clamp */
        if (final > 10000) final = 10000;
        if (final < 0) final = 0;
        
        /* Write */
        MOSFET_WriteCompare(final);
        
        /* Sleep 10ms */
        while(millis % 10);
    }
    
    /****************** Testing Data Collection *********************/
    MOSFET_WriteCompare(0);
    
    /* Debug Output */
    LCD_Position(0, 0);
    sprintf(tstr, "R:%1.3f; O:%1.3f", rt, overshoot); 
    LCD_PrintString(tstr);
    
    LCD_Position(1, 0);
    sprintf(tstr, "U:%1.3f; A:%1.3f", undershoot, sumAvg / count); 
    LCD_PrintString(tstr);
}

/* [] END OF FILE */
