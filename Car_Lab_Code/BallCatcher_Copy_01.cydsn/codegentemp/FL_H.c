/*******************************************************************************
* File Name: FL_H.c  
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
#include "FL_H.h"


/*******************************************************************************
* Function Name: FL_H_Write
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
void FL_H_Write(uint8 value) 
{
    uint8 staticBits = FL_H_DR & ~FL_H_MASK;
    FL_H_DR = staticBits | ((value << FL_H_SHIFT) & FL_H_MASK);
}


/*******************************************************************************
* Function Name: FL_H_SetDriveMode
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
void FL_H_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(FL_H_0, mode);
}


/*******************************************************************************
* Function Name: FL_H_Read
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
*  Macro FL_H_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 FL_H_Read(void) 
{
    return (FL_H_PS & FL_H_MASK) >> FL_H_SHIFT;
}


/*******************************************************************************
* Function Name: FL_H_ReadDataReg
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
uint8 FL_H_ReadDataReg(void) 
{
    return (FL_H_DR & FL_H_MASK) >> FL_H_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(FL_H_INTSTAT) 

    /*******************************************************************************
    * Function Name: FL_H_ClearInterrupt
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
    uint8 FL_H_ClearInterrupt(void) 
    {
        return (FL_H_INTSTAT & FL_H_MASK) >> FL_H_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */ 