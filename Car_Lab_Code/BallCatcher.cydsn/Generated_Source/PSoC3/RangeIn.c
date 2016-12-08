/*******************************************************************************
* File Name: RangeIn.c  
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
#include "RangeIn.h"


/*******************************************************************************
* Function Name: RangeIn_Write
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
void RangeIn_Write(uint8 value) 
{
    uint8 staticBits = RangeIn_DR & ~RangeIn_MASK;
    RangeIn_DR = staticBits | ((value << RangeIn_SHIFT) & RangeIn_MASK);
}


/*******************************************************************************
* Function Name: RangeIn_SetDriveMode
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
void RangeIn_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(RangeIn_0, mode);
}


/*******************************************************************************
* Function Name: RangeIn_Read
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
*  Macro RangeIn_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 RangeIn_Read(void) 
{
    return (RangeIn_PS & RangeIn_MASK) >> RangeIn_SHIFT;
}


/*******************************************************************************
* Function Name: RangeIn_ReadDataReg
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
uint8 RangeIn_ReadDataReg(void) 
{
    return (RangeIn_DR & RangeIn_MASK) >> RangeIn_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(RangeIn_INTSTAT) 

    /*******************************************************************************
    * Function Name: RangeIn_ClearInterrupt
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
    uint8 RangeIn_ClearInterrupt(void) 
    {
        return (RangeIn_INTSTAT & RangeIn_MASK) >> RangeIn_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */ 
