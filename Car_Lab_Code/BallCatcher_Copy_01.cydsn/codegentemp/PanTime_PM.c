/*******************************************************************************
* File Name: PanTime_PM.c
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

#include "PanTime.h"
static PanTime_backupStruct PanTime_backup;


/*******************************************************************************
* Function Name: PanTime_SaveConfig
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
*  PanTime_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PanTime_SaveConfig(void) 
{
    #if (!PanTime_UsingFixedFunction)
        /* Backup the UDB non-rentention registers for PSoC5A*/
        #if (CY_PSOC5A)
            PanTime_backup.TimerUdb = PanTime_ReadCounter();
            PanTime_backup.TimerPeriod = PanTime_ReadPeriod();
            PanTime_backup.InterruptMaskValue = PanTime_STATUS_MASK;
            #if (PanTime_UsingHWCaptureCounter)
                PanTime_backup.TimerCaptureCounter = PanTime_ReadCaptureCount();
            #endif /* Backup the UDB non-rentention register capture counter for PSoC5A */
        #endif /* Backup the UDB non-rentention registers for PSoC5A */

        #if (CY_PSOC3 || CY_PSOC5LP)
            PanTime_backup.TimerUdb = PanTime_ReadCounter();
            PanTime_backup.InterruptMaskValue = PanTime_STATUS_MASK;
            #if (PanTime_UsingHWCaptureCounter)
                PanTime_backup.TimerCaptureCounter = PanTime_ReadCaptureCount();
            #endif /* Back Up capture counter register  */
        #endif /* Backup non retention registers, interrupt mask and capture counter for PSoC3 or PSoC5LP */

        #if(!PanTime_ControlRegRemoved)
            PanTime_backup.TimerControlRegister = PanTime_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: PanTime_RestoreConfig
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
*  PanTime_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PanTime_RestoreConfig(void) 
{   
    #if (!PanTime_UsingFixedFunction)
        /* Restore the UDB non-rentention registers for PSoC5A */
        #if (CY_PSOC5A)
            /* Interrupt State Backup for Critical Region*/
            uint8 PanTime_interruptState;

            PanTime_WriteCounter(PanTime_backup.TimerUdb);
            PanTime_WritePeriod(PanTime_backup.TimerPeriod);
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            PanTime_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            PanTime_STATUS_AUX_CTRL |= PanTime_STATUS_ACTL_INT_EN_MASK;
            /* Exit Critical Region*/
            CyExitCriticalSection(PanTime_interruptState);
            PanTime_STATUS_MASK =PanTime_backup.InterruptMaskValue;
            #if (PanTime_UsingHWCaptureCounter)
                PanTime_SetCaptureCount(PanTime_backup.TimerCaptureCounter);
            #endif /* Restore the UDB non-rentention register capture counter for PSoC5A */
        #endif /* Restore the UDB non-rentention registers for PSoC5A */

        #if (CY_PSOC3 || CY_PSOC5LP)
            PanTime_WriteCounter(PanTime_backup.TimerUdb);
            PanTime_STATUS_MASK =PanTime_backup.InterruptMaskValue;
            #if (PanTime_UsingHWCaptureCounter)
                PanTime_SetCaptureCount(PanTime_backup.TimerCaptureCounter);
            #endif /* Restore Capture counter register*/
        #endif /* Restore up non retention registers, interrupt mask and capture counter for PSoC3 or PSoC5LP */

        #if(!PanTime_ControlRegRemoved)
            PanTime_WriteControlRegister(PanTime_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: PanTime_Sleep
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
*  PanTime_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void PanTime_Sleep(void) 
{
    #if(!PanTime_ControlRegRemoved)
        /* Save Counter's enable state */
        if(PanTime_CTRL_ENABLE == (PanTime_CONTROL & PanTime_CTRL_ENABLE))
        {
            /* Timer is enabled */
            PanTime_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            PanTime_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    PanTime_Stop();
    PanTime_SaveConfig();
}


/*******************************************************************************
* Function Name: PanTime_Wakeup
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
*  PanTime_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PanTime_Wakeup(void) 
{
    PanTime_RestoreConfig();
    #if(!PanTime_ControlRegRemoved)
        if(PanTime_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                PanTime_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
