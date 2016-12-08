/*******************************************************************************
* File Name: RangeTime_PM.c
* Version 2.30
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "RangeTime.h"
static RangeTime_backupStruct RangeTime_backup;


/*******************************************************************************
* Function Name: RangeTime_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  RangeTime_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void RangeTime_SaveConfig(void) 
{
    #if (!RangeTime_UsingFixedFunction)
        /* Backup the UDB non-rentention registers for PSoC5A*/
        #if (CY_PSOC5A)
            RangeTime_backup.TimerUdb = RangeTime_ReadCounter();
            RangeTime_backup.TimerPeriod = RangeTime_ReadPeriod();
            RangeTime_backup.InterruptMaskValue = RangeTime_STATUS_MASK;
            #if (RangeTime_UsingHWCaptureCounter)
                RangeTime_backup.TimerCaptureCounter = RangeTime_ReadCaptureCount();
            #endif /* Backup the UDB non-rentention register capture counter for PSoC5A */
        #endif /* Backup the UDB non-rentention registers for PSoC5A */

        #if (CY_PSOC3 || CY_PSOC5LP)
            RangeTime_backup.TimerUdb = RangeTime_ReadCounter();
            RangeTime_backup.InterruptMaskValue = RangeTime_STATUS_MASK;
            #if (RangeTime_UsingHWCaptureCounter)
                RangeTime_backup.TimerCaptureCounter = RangeTime_ReadCaptureCount();
            #endif /* Back Up capture counter register  */
        #endif /* Backup non retention registers, interrupt mask and capture counter for PSoC3 or PSoC5LP */

        #if(!RangeTime_ControlRegRemoved)
            RangeTime_backup.TimerControlRegister = RangeTime_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: RangeTime_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  RangeTime_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void RangeTime_RestoreConfig(void) 
{   
    #if (!RangeTime_UsingFixedFunction)
        /* Restore the UDB non-rentention registers for PSoC5A */
        #if (CY_PSOC5A)
            /* Interrupt State Backup for Critical Region*/
            uint8 RangeTime_interruptState;

            RangeTime_WriteCounter(RangeTime_backup.TimerUdb);
            RangeTime_WritePeriod(RangeTime_backup.TimerPeriod);
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            RangeTime_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            RangeTime_STATUS_AUX_CTRL |= RangeTime_STATUS_ACTL_INT_EN_MASK;
            /* Exit Critical Region*/
            CyExitCriticalSection(RangeTime_interruptState);
            RangeTime_STATUS_MASK =RangeTime_backup.InterruptMaskValue;
            #if (RangeTime_UsingHWCaptureCounter)
                RangeTime_SetCaptureCount(RangeTime_backup.TimerCaptureCounter);
            #endif /* Restore the UDB non-rentention register capture counter for PSoC5A */
        #endif /* Restore the UDB non-rentention registers for PSoC5A */

        #if (CY_PSOC3 || CY_PSOC5LP)
            RangeTime_WriteCounter(RangeTime_backup.TimerUdb);
            RangeTime_STATUS_MASK =RangeTime_backup.InterruptMaskValue;
            #if (RangeTime_UsingHWCaptureCounter)
                RangeTime_SetCaptureCount(RangeTime_backup.TimerCaptureCounter);
            #endif /* Restore Capture counter register*/
        #endif /* Restore up non retention registers, interrupt mask and capture counter for PSoC3 or PSoC5LP */

        #if(!RangeTime_ControlRegRemoved)
            RangeTime_WriteControlRegister(RangeTime_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: RangeTime_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  RangeTime_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void RangeTime_Sleep(void) 
{
    #if(!RangeTime_ControlRegRemoved)
        /* Save Counter's enable state */
        if(RangeTime_CTRL_ENABLE == (RangeTime_CONTROL & RangeTime_CTRL_ENABLE))
        {
            /* Timer is enabled */
            RangeTime_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            RangeTime_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    RangeTime_Stop();
    RangeTime_SaveConfig();
}


/*******************************************************************************
* Function Name: RangeTime_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  RangeTime_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void RangeTime_Wakeup(void) 
{
    RangeTime_RestoreConfig();
    #if(!RangeTime_ControlRegRemoved)
        if(RangeTime_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                RangeTime_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
