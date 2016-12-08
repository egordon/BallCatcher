/* ========================================
 *
 * Copyright Ethan Gordon, Luke Pfleger, 2016
 * Car Lab, Task 2
 * Line Follower
 *
 * Goal: Follow a line for 2 laps.
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
#define TICKS_PER_LAB 730
#define LAPS 2

/* Direction Detection */
#define DIR_ANG 20
#define DIR_CAP 60
#define DIR_THRESH 5


/** Changeable Constants **/
#define SPD_KP  600.0f
#define SPD_KI  0.8f
#define SPD_SETPOINT 4.0f

#define AVG_KP_DEF 0.58f
static float AVG_KP = AVG_KP_DEF;
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

/* UART Debug Interrupt */
static int16 dataArr[3500] = {0};
int restart = 1;
CY_ISR(uart)
{
    int i, j;
    char c = UART_GetChar();
    if(c == 'd') {
        /* Output Error Data from Last Run */
        for(i = 0; i < 3500; i++) {
            char tstr[9] = {0};
            if(i < 3499) sprintf(tstr, "%+4d, ", dataArr[i]);
            else sprintf(tstr, "%+4d", dataArr[i]);
            for(j = 0; j < 8; j++)
                UART_PutChar(tstr[j]);
        }
    } else if (c == '+') {
        /* Increase Kp */
        char tstr[11] = {0};
        AVG_KP += 0.05f;
        sprintf(tstr, "\nKp=%1.2f, ", AVG_KP);
        for(j = 0; j < 10; j++)
            UART_PutChar(tstr[j]);
    } else if (c == '-') {
        /* Decrease Kp */
        char tstr[11] = {0};
        AVG_KP -= 0.01f;
        sprintf(tstr, "\nKp=%1.2f, ", AVG_KP);
        for(j = 0; j < 10; j++)
            UART_PutChar(tstr[j]);
    } else if (c == 'k') {
        /* Reset Kp to Default */
        char tstr[11] = {0};
        AVG_KP = AVG_KP_DEF;
        sprintf(tstr, "\nKp=%1.2f, ", AVG_KP);
        for(j = 0; j < 10; j++)
            UART_PutChar(tstr[j]);
    } else if (c == 'r') {
        /* Start New Run */
        restart = 1;
    }

}

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
    isLineLost = 0;
    topLineX = Line_Timer_ReadCapture();
    if (topLineX > 67)
        linePos = topLineX;
    else isLineLost = 1;
}

static int botLineX = AVG_SETPOINT;
static int linePosBot = AVG_SETPOINT;
CY_ISR(timer_bot)
{
    botLineX = Line_Timer_bot_ReadCapture();
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

    /* Start UART */
    UART_Start();

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

    /* UART Interrupt */
    uart_rx_Start();
    uart_rx_SetVector(uart);


    /* Speed Interrupt */
    spd_inter_Start();
    spd_inter_SetVector(spd);
    /***** End Interrupt Declarations *****/

    /*** Set PID Setpoints ***/
    for(;;) {
        int i = 0;
        int dir = 0;
        /* Wait for Serial Restart */
        while(!restart) {
            LCD_Position(1, 0);
            LCD_PrintString("Wait on Restart");
        }
        /* 4 Second Delay */
        {
            int targetmillis = millis + 4000;
            LCD_Position(1, 0);
            LCD_PrintString("Starting........");
            while(millis < targetmillis);
        }
        ticks = 0;
        restart = 0;
        PID_Setpoint(spdPID, SPD_SETPOINT);

        while(ticks < (TICKS_PER_LAB * LAPS))
        {
            int16 err, avg, ang;

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
                if (linePos > linePosBot)
                    avg = linePos + linePos;
                else avg = linePosBot + linePosBot;
            } else if (dir < -DIR_THRESH) { /* RIGHT: Use right-most point. */
                if (linePos < linePosBot)
                    avg = linePos + linePos;
                else avg = linePosBot + linePosBot;
            } /* Else STRAIGHT: Use average of points. */

            /*** END Direction Detection ***/

            /* Debug Output */
            LCD_Position(0, 0);
            sprintf(tstr, "Avg: %5d", avg);
            LCD_PrintString(tstr);
            LCD_Position(1, 0);/*
            if(dir > DIR_THRESH) {
                LCD_PrintString("Left     ");
            } else if (dir < -DIR_THRESH) {
                LCD_PrintString("Right    ");
            } else {
                LCD_PrintString("Straight ");
            }*/
            sprintf(tstr, "Spd: %1.1f       ", State_Avg(spdState));
            LCD_PrintString(tstr);

            /* Calculate Error */
            err = AVG_SETPOINT - avg;
            if(i < 7000) dataArr[i/2] = err;

            /* Write */
            MOSFET_WriteCompare(PID_Output(spdPID));
            Angle_Set((int)(AVG_KP * (float)err));
            i++;
        }

        /* Stop */
        MOSFET_WriteCompare(0);
        Angle_Set(0.0f);
    }
}

/* [] END OF FILE */
