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


/** Changeable Constants **/
#define SPD_KP  600.0f
#define SPD_KI  0.8f
#define SPD_SETPOINT 5.0f

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
static int tripped = 0;
CY_ISR(timer)
{
    static int flag = 15;
    static int lastX = 0;
    tripped = 0;
    if(flag) {
        flag--;
        lastX = topLineX;
    }
    topLineX = Line_Timer_ReadCapture();
    lastX = topLineX;
    if (topLineX > 67)
        linePos = topLineX;
    else tripped = 1;
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
    else tripped = 1;
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
    
    /* Start State Estimator */
    spdState = State_Alloc();
    State_Start(spdState, ALPHA, 0.0f, SECOND);
    
    /* Start PID Loop */
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
    
    while(ticks < 1530)
    {
        float speed, out;
        int err, avg, ang;
        static int dir = 0;
        
        /* Update PID Loop */
        PID_Update(spdPID, State_Avg(spdState), 1);
        avg = linePos + linePosBot;
        ang = linePos - linePosBot;
        
        if (!tripped) {
            if(ang > 30) {
                dir += 2;
                if (dir > -5 && dir < 5) dir = 5;
            } else if (ang < -30) {
                dir -= 2;
                if (dir > -5 && dir < 5) dir = -5;
            } else {
                if(dir > 0) dir--;
                else dir++;
            }
        }
        if (dir < -60) dir = -60;
        if (dir > 60) dir = 60;
        
        if (dir > 5) {
            PID_Setpoint(spdPID, SPD_SETPOINT);
            if (linePos > linePosBot)
                avg = linePos + linePos;
            else avg = linePosBot + linePosBot;
        } else if (dir < -5) {
            PID_Setpoint(spdPID, SPD_SETPOINT);
            if (linePos < linePosBot)
                avg = linePos + linePos;
            else avg = linePosBot + linePosBot;
        } else PID_Setpoint(spdPID, SPD_SETPOINT + 1.5);
        
        err = AVG_SETPOINT - avg;
       
        if (dir < 15 && dir > -15)
            err  = (err * 3) / 4;
        
        speed = State_Avg(spdState);
        
        /* Debug Speed */
        LCD_Position(0, 0);
        sprintf(tstr, "Avg: %5d", avg);
        LCD_PrintString(tstr);
        LCD_Position(1, 0);
        if(dir > 15) {
            LCD_PrintString("Left     ");
        } else if (dir < -15) {
            LCD_PrintString("Right    ");
        } else {
            LCD_PrintString("Straight ");
        }
        /*sprintf(tstr, "Ang: %5d", ang);
        LCD_PrintString(tstr);*/
        
        /* Write */
        if (err < 0) {
            out = AVG_KP * (float)err * (1.0f + (float)(-err)/350.0f);
        } else {
            out = AVG_KP * (float)err * (1.0f + (float)(err)/350.0f);
        }
        MOSFET_WriteCompare(PID_Output(spdPID));
        Angle_Set((int)(out));
    }
    
    /* Stop */
    MOSFET_WriteCompare(0);
    Angle_Set(0.0f);
    
}

/* [] END OF FILE */
