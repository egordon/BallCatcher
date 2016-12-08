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

#include "drivetrain.h"
#include <device.h>

/*constants*/
#define FL_OFFSET 0
#define FR_OFFSET 0
#define BL_OFFSET 0
#define BR_OFFSET 0

#define DEADBAND 10

void DriveTrain_Start(void)
{
    /* Start PWM Signals */
    FL_HBridge_Start();
    FR_HBridge_Start();
    BL_HBridge_Start();
    BR_HBridge_Start();   
    
    DriveTrain_Stop();
}

void DriveTrain_Set(int x, int y, int a)
{
    /*initialize motor write values*/
    int writeValues[4] = {0};
    int max = 0;
    int i;
    
    /*project (x, y, a) vector onto 4-motor 4-vector space*/
    writeValues[0] = x + y - a;
    writeValues[1] = -(-x + y + a);
    writeValues[2] = -x + y - a;
    writeValues[3] = -(x + y + a);
    
    /*if any value larger than 5000 (our maximum), scale to 5000*/
    for (i = 0; i < 4; i++)
    {
        if (writeValues[i] > max)
            max = writeValues[i];
        else if (-writeValues[i] > max)
            max = -writeValues[i];
            
        /* Deadband */
        if ((writeValues[i] < DEADBAND) && (writeValues[i] > -DEADBAND))
            writeValues[i] = 0;
    }
    if (max > 500)
    {
        float div = (float)max / 500.0f;
        
        for (i = 0; i < 4; i++) {
            writeValues[i] = (int)((float)(writeValues[i])/div); 
            if (writeValues[i] < -500)
                writeValues[i] = -500;
        }
    }
    
    /*Convert them from -500~500 to 0~1000*/
    writeValues[0] += (500 + FL_OFFSET);
    writeValues[1] += (500 - FR_OFFSET);
    writeValues[2] += (500 + BL_OFFSET);
    writeValues[3] += (500 - BR_OFFSET);
    
    FL_HBridge_WriteCompare(writeValues[0]);
    FR_HBridge_WriteCompare(writeValues[1]);
    BL_HBridge_WriteCompare(writeValues[2]);
    BR_HBridge_WriteCompare(writeValues[3]);
}

void DriveTrain_Stop(void)
{
    FL_HBridge_WriteCompare(500 + FL_OFFSET);
    FR_HBridge_WriteCompare(500 - FR_OFFSET);
    BL_HBridge_WriteCompare(500 + BL_OFFSET);
    BR_HBridge_WriteCompare(500 - BR_OFFSET);
    
}

/* [] END OF FILE */
