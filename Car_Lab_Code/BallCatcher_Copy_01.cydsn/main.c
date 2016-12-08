/* ========================================
 *
 * Car Lab 2016: Independent Project
 * Ethan Gordon, Luke Pfleger
 *
 * Goal: Catch a ball.
 *
 * ========================================
*/
#include <device.h>
#include <stdio.h>
#include <math.h>
#include "drivetrain.h"

/* Pixy Params */
static short xScreen = 0;
static short yScreen = 0;
static short width = 0;
static short height = 0;

/* UART States */
#define STATE_SYNC 0
#define STATE_READ 1

/* Offsets */
#define X_OFFSET 160
#define Y_OFFSET 100

#define GYRO_SET 24425

#define GYRO_DEADBAND 30

/* Constants */
#define PI 3.14159f
#define GRAVITY 386.1f /* in/s^2 */
#define ALPHA 0.05f
#define ALPHAZ 0.2f


/* Trig Functions */
#define COS(x) (1 - (x)*(x)/2.0f)
#define SIN(x) ((x) - (x)*(x)*(x)/6.0f )
#define DEG(x) (x)*(180.0f / 3.14159f)
#define RAD(x) (x)*(3.14159f / 180.0f)

static int watchdog = 0;

/* Read data from the Pixy Cam */
static int dataRead = 0;
CY_ISR(uart)
{
    static int state = STATE_SYNC;
    static short word = 0;
    static int readPos = 0;
    
    unsigned char byte = Pixy_UART_GetChar();
    
    if (state == STATE_SYNC) {
        switch (readPos) {
        case 0:
            readPos = (byte == 0x55) ? 1 : 0;
            break;
        case 1:
            readPos = (byte == 0xaa) ? 2 : 0;
            break;
        case 2:
            if (byte == 0x55) {
                readPos = 3;
            } else {
                word = byte;
                readPos = 1;
                state = STATE_READ;
            }
            break;
        case 3:
            readPos = 0;
            state = (byte == 0xaa) ? STATE_READ : STATE_SYNC;
        break;
        default:
            readPos = 0;
        }
    } else { 
        if (readPos%2) {
            word += (((short)byte) << 8);
            switch(readPos){
            case 5:
                xScreen = word;
            break;
            case 7:
                yScreen = word;
            break;
            case 9:
                width = word;
            break;
            case 11:
                height = word;
                state = STATE_SYNC;
                readPos = -1;
                dataRead = 1;
                watchdog = 0;
            break;
            }
        } else {
            word = byte;
        }
        readPos++;
    }
    
}

/* Read Gyro Data */
static int gyro = GYRO_SET;
CY_ISR(analog_isr)
{
    gyro = ADC_GetResult16();
}

/* Read the pan and tilt data from Pixy cam */
static float pan = 0.0f;

CY_ISR(pan_isr)
{
    int panRead = (int)PanTime_ReadCapture();
    pan = (float)(panRead) * 0.0967049f - 799.878f; /* Linear Interpolation from Wolfram */
    Pan_Reset_Write(1);
}

static float tilt = 0;

CY_ISR(tilt_isr)
{
    int tiltRead = (int)TiltTime_ReadCapture();
    tilt = 2.0f * ((float)(tiltRead) * 0.0367647f - 289.154f); /* Linear Interpolation from Wolfram */
    if(tilt > 40.0f) tilt = 40.0f;
    Tilt_Reset_Write(1);
}

/* Spherical */
static float r = 0.0f;
static float phi = 0.0f;
static float theta = 0.0f;

static float dr = 0.0f;
static float dphi = 0.0f;
static float dtheta = 0.0f;

CY_ISR(clk_isr)
{
    char tstr[17] = {0};
    static float rPrev = 0.0f;
    static float phiPrev = 0.0f;
    static float thetaPrev = 0.0f;
    
    watchdog++;
    
    if(dataRead) {
        float tanTheta, camTheta, heightPredicted;
        dataRead = 0;
    
        /* Calculate Spherical Radius */
        r = (320.0f * 7.0f) / (2.0f * (float)(width) * 0.767f); /* r = pix * diameter / (pixWidth * 2*tan(75/2)) */
        dr = 50.0f * (r - rPrev);
        rPrev = r;
        
        /* Calculate Theta, include Camera Tilt and Y position */
        heightPredicted = (1481.14075f) / r;
        tanTheta = (7.0f * (yScreen - Y_OFFSET)) / (r * heightPredicted);
        camTheta = DEG(tanTheta*tanTheta*tanTheta / 3.0f - tanTheta);
        if ((camTheta < 2.0f) && (camTheta > -2.0f)) camTheta = 0.0f;
        if (tilt < 0.0f)
            theta = 0.0f;
        else theta = tilt + camTheta;
        dtheta = 50.0f * (theta - thetaPrev);
        thetaPrev = theta;
        
        /* Calculate Phi, ignore X location, only use Camera Pan */
        phi = pan;
        dphi = (phi - phiPrev) * 50.0f;
        phiPrev = phi;
        
    }
}

void main()
{
    int finished = 0;
    char tstr[17] = {0};
    
    /* Start STUFFZ */
    DriveTrain_Start();
    LCD_Start();
    ADC_Start();
    ADC_StartConvert();
    
    /* Start Counters */
    PanTime_Start();
    TiltTime_Start();
    
    /* Interrupts */
    Pixy_int_Start();
    Pixy_int_SetVector(uart);
    Pan_int_Start();
    Pan_int_SetVector(pan_isr);
    Tilt_int_Start();
    Tilt_int_SetVector(tilt_isr);
    Clk_int_Start();
    Clk_int_SetVector(clk_isr);
    Analog_int_Start();
    Analog_int_SetVector(analog_isr);
    
    /* Start UART */
    Pixy_UART_Start();

    CyGlobalIntEnable;
    while(!finished)
    {
        float z, dz, x, dx, y, dy, dxMeas, dyMeas, dzMeas, radical, tol, kpx;
        static float xTarget = 0.0f;
        static float yTarget = 0.0f;
        
        float gyro_err, gyro_p, gyro_i;
        static float gyro_sum = 0.0f;
    
        z = r * SIN(RAD(theta));
        dzMeas = dr * SIN(RAD(theta)) + r*COS(RAD(theta))*RAD(dtheta);
        
        x = r * COS(RAD(theta)) * COS(RAD(phi)) - 9.0f; /* Center on center of basket */
        dxMeas = dr * COS(RAD(theta)) * COS(RAD(phi)) - r * SIN(RAD(theta)) * COS(RAD(phi))*RAD(dtheta) - r * COS(RAD(theta)) * SIN(RAD(phi))*RAD(dphi);
    
        y = -r * COS(RAD(theta)) * SIN(RAD(phi));
        dyMeas = -dr * COS(RAD(theta)) * SIN(RAD(phi)) + r * SIN(RAD(theta)) * SIN(RAD(phi))*RAD(dtheta) - r * COS(RAD(theta)) * COS(RAD(phi))*RAD(dphi);
        
        /* Average Velocities */
        if (dyMeas > 60.0f) dyMeas = 0.0f;
        if (dyMeas < -60.0f) dyMeas = 0.0f;
        if (dxMeas > 60.0f) dxMeas = 0.0f;
        if (dxMeas < -60.0f) dxMeas = 0.0f;
        if(z < 3.0f) {
            dx = dxMeas;
            dy = dyMeas;
        } else {
            dx = ALPHA*dxMeas + (1.0f-ALPHA)*dx;
            dy = ALPHA*dyMeas + (1.0f-ALPHA)*dy;
        }
        dz = ALPHAZ*dzMeas + (1.0f-ALPHAZ)*dz;
        
        radical = dz * dz + 2.0f*GRAVITY*z;
        if(radical < 0)
            tol = 0.0f;
        else
            tol = (dz + (float)sqrt((double)radical)) / GRAVITY;
            
        xTarget = x + tol*dx;
        yTarget = y + tol*dy;
            
        sprintf(tstr, "x: %+2.2f  ", xTarget);
        
        LCD_Position(0, 0);
        LCD_PrintString(tstr);
        
        
        sprintf(tstr, "y: %+2.2f  ", yTarget);
        
        LCD_Position(1, 0);
        LCD_PrintString(tstr);
        
        /* Gyro PID Loop */
        gyro_err = gyro - GYRO_SET;
        if ((gyro_err < GYRO_DEADBAND) && (gyro_err > -GYRO_DEADBAND))
            gyro_err = 0;
        gyro_p = 0.5f * gyro_err;
        
        gyro_sum += gyro_err;
        /* Clamp Sum */
        if (gyro_sum > 10000) gyro_sum = 10000;
        else if (gyro_sum < -10000) gyro_sum = -10000;
        gyro_i = 0.01f * gyro_sum;
        
        if (xTarget < 0)
            kpx = 100.0f;
        else kpx = 35.0f;
        
        if ((tilt > 10.0f) && (watchdog < 10)) {
            DriveTrain_Set(-45.0f * yTarget, kpx * xTarget, (int)(gyro_p + gyro_i));
        } else DriveTrain_Stop();
        
    }
    
    /* Stop Drive Train */
    DriveTrain_Stop();
    
    for(;;){}
}

/* [] END OF FILE */

/* Saved Junk
static float xLast = 0.0f;
    static float yLast = 0.0f;
    static float zLast = 0.0f;
    if (width != 0 && height != 0)
    {
        float tanPhi, tanTheta, xDotTest, yDotTest, zDotTest, r, phi, theta;
        float tof;
        r = (320.0f * 7.0f) / (2.0f * (float)(width) * 0.767f);
        tanPhi = (7.0f * (x - X_OFFSET)) / (r * width);
        tanTheta = (7.0f * (y - Y_OFFSET)) / (r * height);
        phi = pan + (tanPhi - tanPhi*tanPhi*tanPhi / 3) * (180.0f / PI);
        theta = tilt - (tanTheta - tanTheta*tanTheta*tanTheta / 3) * (180.0f / PI);

        
        xBall = r * cos(theta * PI / 180.0f) * cos(phi * PI / 180.0f);
        yBall = r * cos(theta * PI / 180.0f) * sin(phi * PI / 180.0f);
        zBall = r * sin(theta * PI / 180.0f);
        
        if ((xBall - xLast > 100) || (xBall - xLast < -100)) xBall = xLast;
        if ((yBall - yLast > 100) || (yBall - yLast < -100)) yBall = yLast;
        if ((zBall - zLast > 50) || (zBall - zLast < -50)) zBall = zLast;
        

        xDotTest = (xBall - xLast) * 50.0f;
        yDotTest = (yBall - yLast) * 50.0f;
        zDotTest = (zBall - zLast) * 50.0f;
        
        xDot = ALPHA*xDotTest + (1 - ALPHA)*xDot;
        yDot = ALPHA*yDotTest + (1 - ALPHA)*yDot;
        zDot = ALPHA*zDotTest + (1 - ALPHA)*zDot;
        
        xLast = xBall;
        yLast = yBall;
        zLast = zBall;
        

        tof = (zDot + sqrt(zDot*zDot + 2*GRAVITY*zBall)) / GRAVITY;
        xTarget = xBall + tof*xDot;
        yTarget = yBall + tof*yDot;
    }
    */