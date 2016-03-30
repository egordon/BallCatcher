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

/* Angle Actuation */
#include "angle.h"

/* PID Loop */
#include "pid.h"

/* Constants */
#define ALPHA 0.2f
/* Total Circumference = 20cm = 0.656168 ft */
/* 5 Magnets per Revolution: 0.656168 / 5 = 0.1312336 ft */
#define MAG_DIST 0.1312336f
#define SECOND 1000
#define TICKS_PER_LAB 750
#define LAPS 2

/* Direction Detection */
#define DIR_ANG 20
#define DIR_CAP 60
#define DIR_THRESH 5


/** Changeable Constants **/
#define SPD_KP  600.0f
#define SPD_KI  0.8f
#define SPD_SETPOINT 4.0f

#define AVG_KP  0.38f
#define AVG_SETPOINT 720

#define ANG_KP  0.0f
#define ANG_SETPOINT 0

/** END Changeable Constants **/

/* Macros */
#define ABS(x)  ( ( (x) < 0) ? -(x) : (x) )


/* State Estimators */
State_T spdState = NULL;

/* PID Loops */
PID_T spdPID = NULL;

/* Clock and Clock Interrupt */
int millis = 0;
CY_ISR(clk)
{
    millis++;
    
    /* Watchdog Functions */
    State_Watchdog(spdState);
}

/* Line Interrupts */
static int topLineX = AVG_SETPOINT / 2;
static int linePos = AVG_SETPOINT / 2;
static int isLineLost = 0;

CY_ISR(timer)
{
    static int flag = 15;
    static int lastX = 0;
    isLineLost = 0;
    if(flag) {
        flag--;
        lastX = topLineX;
    }
    topLineX = Line_Timer_ReadCapture();
    lastX = topLineX;
    if (topLineX > 67)
        linePos = topLineX;
    else isLineLost = 1;
}

static int botLineX = AVG_SETPOINT;
static int linePosBot = AVG_SETPOINT;
CY_ISR(timer_bot)
{
    static int flag = 15;
    static int lastX = 0;
    if(flag) {
        flag--;
        lastX = botLineX;
    }
    botLineX = Line_Timer_bot_ReadCapture();
    lastX = botLineX;
    if (botLineX > 67)
        linePosBot = botLineX;
    else isLineLost = 1;
}

/* Hall Effect Interrupt */
static int ticks = 0;
CY_ISR(spd)
{
    static int lastTime = 0;
    int currentTime = millis;
    
    /* Update State Estimate */
    if (lastTime != 0) {
        State_Update(spdState, (MAG_DIST * 1000.0f)/((float)(currentTime - lastTime)));
    }
    lastTime = currentTime;
    ticks++;
}

void main()
{
    
    /* LCD Display */
    char tstr[17] = {0};
    
    CyGlobalIntEnable;
    
    /***** Firmware Declarations *****/
    
    /* Start Reference */
    VRef_Start();
    
    /* Start Comparator */
    Thresholder_Start();
    
    /* Start Counters */
    Counter_Start();
    LineCounter_Start();
    Line_Timer_Start();
    LineCounter_bot_Start();
    Line_Timer_bot_Start();
    
    /* Start LCD */
    LCD_Start();
    
    /* Start PWM */
    MOSFET_Start();
    Angle_Start();
    
    /***** End Firmware Declarations *****/
    
    /***** Object Declarations *****/
    
    /* Start Speed State Estimator */
    spdState = State_Alloc();
    State_Start(spdState, ALPHA, 0.0f, SECOND);
    
    /* Start Speed PID Loop */
    spdPID = PID_Alloc();
    PID_Start(spdPID, SPD_KP, SPD_KI, 0.0f, (float)(MOSFET_ReadPeriod() + 1));
    
    /***** End Object Declarations *****/
    
    /***** Interrupt Declarations *****/
    
    /* Clock Interrupt */
    clk_inter_Start();
    clk_inter_SetVector(clk);
    
    /* Line Timer Interrupt */
    timer_inter_Start();
    timer_inter_SetVector(timer);
    timer_inter_bot_Start();
    timer_inter_bot_SetVector(timer_bot);
    
    
    /* Speed Interrupt */
    spd_inter_Start();
    spd_inter_SetVector(spd);
    /***** End Interrupt Declarations *****/
    
    /*** Set PID Setpoints ***/
    PID_Setpoint(spdPID, SPD_SETPOINT);
    
    while(ticks < (TICKS_PER_LAB * LAPS))
    {
        float speed, out;
        int err, avg, ang;
        static int dir = 0;
        
        /* Update PID Loop */
        PID_Update(spdPID, State_Avg(spdState), 1);
        avg = linePos + linePosBot;
        ang = linePos - linePosBot;
        
        /**** Direction Detection ****/
        if (!isLineLost) {
            if(ang > DIR_ANG) {
                dir += 2;
            } else if (ang < -DIR_ANG) {
                dir -= 2;
            } else {
                if(dir > 0) dir--;
                else dir++;
            }
        }
        
        /* Clamp Direction Variable */
        if (dir < -DIR_CAP) dir = -DIR_CAP;
        if (dir > DIR_CAP) dir = DIR_CAP;
        
        if (dir > DIR_THRESH) { /* LEFT: Use left-most point. */
            PID_Setpoint(spdPID, SPD_SETPOINT);
            if (linePos > linePosBot)
                avg = linePos + linePos;
            else avg = linePosBot + linePosBot;
        } else if (dir < -DIR_THRESH) { /* RIGHT: Use right-most point. */
            PID_Setpoint(spdPID, SPD_SETPOINT);
            if (linePos < linePosBot)
                avg = linePos + linePos;
            else avg = linePosBot + linePosBot;
        } else { /* STRAIGHT: Use average of points */
            PID_Setpoint(spdPID, SPD_SETPOINT + 1.5);
        }
        /*** END Direction Detection ***/
        
        /* Debug Output */
        LCD_Position(0, 0);
        sprintf(tstr, "Avg: %5d", avg);
        LCD_PrintString(tstr);
        LCD_Position(1, 0);
        if(dir > DIR_THRESH) {
            LCD_PrintString("Left     ");
        } else if (dir < -DIR_THRESH) {
            LCD_PrintString("Right    ");
        } else {
            LCD_PrintString("Straight ");
        }
        
        /* Calculate Error */
        err = AVG_SETPOINT - avg;
        
        /* Write */
        MOSFET_WriteCompare(PID_Output(spdPID));
        Angle_Set((int)(AVG_KP * (float)err));
    }
    
    /* Stop */
    MOSFET_WriteCompare(0);
    Angle_Set(0.0f);
    
}

/* [] END OF FILE */
