/*******************************************************************************
* File Name: CountFall_PM.c  
* Version 2.20
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
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

#include "CountFall.h"

static CountFall_backupStruct CountFall_backup;


/*******************************************************************************
* Function Name: CountFall_SaveConfig
********************************************************************************
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
*  CountFall_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void CountFall_SaveConfig(void) 
{
    #if (!CountFall_UsingFixedFunction)
        /* Backup the UDB non-rentention registers for PSoC5A */
        #if (CY_PSOC5A)
            CountFall_backup.CounterUdb = CountFall_ReadCounter();
            CountFall_backup.CounterPeriod = CountFall_ReadPeriod();
            CountFall_backup.CompareValue = CountFall_ReadCompare();
            CountFall_backup.InterruptMaskValue = CountFall_STATUS_MASK;
        #endif /* (CY_PSOC5A) */
        
        #if (CY_PSOC3 || CY_PSOC5LP)
            CountFall_backup.CounterUdb = CountFall_ReadCounter();
            CountFall_backup.InterruptMaskValue = CountFall_STATUS_MASK;
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
        
        #if(!CountFall_ControlRegRemoved)
            CountFall_backup.CounterControlRegister = CountFall_ReadControlRegister();
        #endif /* (!CountFall_ControlRegRemoved) */
    #endif /* (!CountFall_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: CountFall_RestoreConfig
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
*  CountFall_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void CountFall_RestoreConfig(void) 
{      
    #if (!CountFall_UsingFixedFunction)     
        /* Restore the UDB non-rentention registers for PSoC5A */
        #if (CY_PSOC5A)
            /* Interrupt State Backup for Critical Region*/
            uint8 CountFall_interruptState;
        
            CountFall_WriteCounter(CountFall_backup.CounterUdb);
            CountFall_WritePeriod(CountFall_backup.CounterPeriod);
            CountFall_WriteCompare(CountFall_backup.CompareValue);
            /* Enter Critical Region*/
            CountFall_interruptState = CyEnterCriticalSection();
        
            CountFall_STATUS_AUX_CTRL |= CountFall_STATUS_ACTL_INT_EN_MASK;
            /* Exit Critical Region*/
            CyExitCriticalSection(CountFall_interruptState);
            CountFall_STATUS_MASK = CountFall_backup.InterruptMaskValue;
        #endif  /* (CY_PSOC5A) */
        
        #if (CY_PSOC3 || CY_PSOC5LP)
            CountFall_WriteCounter(CountFall_backup.CounterUdb);
            CountFall_STATUS_MASK = CountFall_backup.InterruptMaskValue;
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
        
        #if(!CountFall_ControlRegRemoved)
            CountFall_WriteControlRegister(CountFall_backup.CounterControlRegister);
        #endif /* (!CountFall_ControlRegRemoved) */
    #endif /* (!CountFall_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: CountFall_Sleep
********************************************************************************
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
*  CountFall_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void CountFall_Sleep(void) 
{
    #if(!CountFall_ControlRegRemoved)
        /* Save Counter's enable state */
        if(CountFall_CTRL_ENABLE == (CountFall_CONTROL & CountFall_CTRL_ENABLE))
        {
            /* Counter is enabled */
            CountFall_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            CountFall_backup.CounterEnableState = 0u;
        }
    #endif /* (!CountFall_ControlRegRemoved) */
    CountFall_Stop();
    CountFall_SaveConfig();
}


/*******************************************************************************
* Function Name: CountFall_Wakeup
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
*  CountFall_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void CountFall_Wakeup(void) 
{
    CountFall_RestoreConfig();
    #if(!CountFall_ControlRegRemoved)
        if(CountFall_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            CountFall_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!CountFall_ControlRegRemoved) */
    
}


/* [] END OF FILE */
