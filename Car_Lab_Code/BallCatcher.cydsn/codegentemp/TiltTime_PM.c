/*******************************************************************************
* File Name: TiltTime_PM.c
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

#include "TiltTime.h"
static TiltTime_backupStruct TiltTime_backup;


/*******************************************************************************
* Function Name: TiltTime_SaveConfig
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
*  TiltTime_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void TiltTime_SaveConfig(void) 
{
    #if (!TiltTime_UsingFixedFunction)
        /* Backup the UDB non-rentention registers for PSoC5A*/
        #if (CY_PSOC5A)
            TiltTime_backup.TimerUdb = TiltTime_ReadCounter();
            TiltTime_backup.TimerPeriod = TiltTime_ReadPeriod();
            TiltTime_backup.InterruptMaskValue = TiltTime_STATUS_MASK;
            #if (TiltTime_UsingHWCaptureCounter)
                TiltTime_backup.TimerCaptureCounter = TiltTime_ReadCaptureCount();
            #endif /* Backup the UDB non-rentention register capture counter for PSoC5A */
        #endif /* Backup the UDB non-rentention registers for PSoC5A */

        #if (CY_PSOC3 || CY_PSOC5LP)
            TiltTime_backup.TimerUdb = TiltTime_ReadCounter();
            TiltTime_backup.InterruptMaskValue = TiltTime_STATUS_MASK;
            #if (TiltTime_UsingHWCaptureCounter)
                TiltTime_backup.TimerCaptureCounter = TiltTime_ReadCaptureCount();
            #endif /* Back Up capture counter register  */
        #endif /* Backup non retention registers, interrupt mask and capture counter for PSoC3 or PSoC5LP */

        #if(!TiltTime_ControlRegRemoved)
            TiltTime_backup.TimerControlRegister = TiltTime_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: TiltTime_RestoreConfig
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
*  TiltTime_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void TiltTime_RestoreConfig(void) 
{   
    #if (!TiltTime_UsingFixedFunction)
        /* Restore the UDB non-rentention registers for PSoC5A */
        #if (CY_PSOC5A)
            /* Interrupt State Backup for Critical Region*/
            uint8 TiltTime_interruptState;

            TiltTime_WriteCounter(TiltTime_backup.TimerUdb);
            TiltTime_WritePeriod(TiltTime_backup.TimerPeriod);
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            TiltTime_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            TiltTime_STATUS_AUX_CTRL |= TiltTime_STATUS_ACTL_INT_EN_MASK;
            /* Exit Critical Region*/
            CyExitCriticalSection(TiltTime_interruptState);
            TiltTime_STATUS_MASK =TiltTime_backup.InterruptMaskValue;
            #if (TiltTime_UsingHWCaptureCounter)
                TiltTime_SetCaptureCount(TiltTime_backup.TimerCaptureCounter);
            #endif /* Restore the UDB non-rentention register capture counter for PSoC5A */
        #endif /* Restore the UDB non-rentention registers for PSoC5A */

        #if (CY_PSOC3 || CY_PSOC5LP)
            TiltTime_WriteCounter(TiltTime_backup.TimerUdb);
            TiltTime_STATUS_MASK =TiltTime_backup.InterruptMaskValue;
            #if (TiltTime_UsingHWCaptureCounter)
                TiltTime_SetCaptureCount(TiltTime_backup.TimerCaptureCounter);
            #endif /* Restore Capture counter register*/
        #endif /* Restore up non retention registers, interrupt mask and capture counter for PSoC3 or PSoC5LP */

        #if(!TiltTime_ControlRegRemoved)
            TiltTime_WriteControlRegister(TiltTime_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: TiltTime_Sleep
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
*  TiltTime_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void TiltTime_Sleep(void) 
{
    #if(!TiltTime_ControlRegRemoved)
        /* Save Counter's enable state */
        if(TiltTime_CTRL_ENABLE == (TiltTime_CONTROL & TiltTime_CTRL_ENABLE))
        {
            /* Timer is enabled */
            TiltTime_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            TiltTime_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    TiltTime_Stop();
    TiltTime_SaveConfig();
}


/*******************************************************************************
* Function Name: TiltTime_Wakeup
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
*  TiltTime_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void TiltTime_Wakeup(void) 
{
    TiltTime_RestoreConfig();
    #if(!TiltTime_ControlRegRemoved)
        if(TiltTime_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                TiltTime_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
