/*******************************************************************************
* File Name: Gyro_Pin.c  
* Version 1.70
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cytypes.h"
#include "Gyro_Pin.h"


/*******************************************************************************
* Function Name: Gyro_Pin_Write
********************************************************************************
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  void 
*  
*******************************************************************************/
void Gyro_Pin_Write(uint8 value) 
{
    uint8 staticBits = Gyro_Pin_DR & ~Gyro_Pin_MASK;
    Gyro_Pin_DR = staticBits | ((value << Gyro_Pin_SHIFT) & Gyro_Pin_MASK);
}


/*******************************************************************************
* Function Name: Gyro_Pin_SetDriveMode
********************************************************************************
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to this drive mode.
*
* Return: 
*  void
*
*******************************************************************************/
void Gyro_Pin_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Gyro_Pin_0, mode);
}


/*******************************************************************************
* Function Name: Gyro_Pin_Read
********************************************************************************
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  void 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro Gyro_Pin_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Gyro_Pin_Read(void) 
{
    return (Gyro_Pin_PS & Gyro_Pin_MASK) >> Gyro_Pin_SHIFT;
}


/*******************************************************************************
* Function Name: Gyro_Pin_ReadDataReg
********************************************************************************
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  void 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 Gyro_Pin_ReadDataReg(void) 
{
    return (Gyro_Pin_DR & Gyro_Pin_MASK) >> Gyro_Pin_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Gyro_Pin_INTSTAT) 

    /*******************************************************************************
    * Function Name: Gyro_Pin_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  void 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 Gyro_Pin_ClearInterrupt(void) 
    {
        return (Gyro_Pin_INTSTAT & Gyro_Pin_MASK) >> Gyro_Pin_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */ 
