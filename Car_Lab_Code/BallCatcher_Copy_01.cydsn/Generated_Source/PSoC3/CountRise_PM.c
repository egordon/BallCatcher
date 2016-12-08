/*******************************************************************************
* File Name: CountRise_PM.c  
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

#include "CountRise.h"

static CountRise_backupStruct CountRise_backup;


/*******************************************************************************
* Function Name: CountRise_SaveConfig
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
*  CountRise_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void CountRise_SaveConfig(void) 
{
    #if (!CountRise_UsingFixedFunction)
        /* Backup the UDB non-rentention registers for PSoC5A */
        #if (CY_PSOC5A)
            CountRise_backup.CounterUdb = CountRise_ReadCounter();
            CountRise_backup.CounterPeriod = CountRise_ReadPeriod();
            CountRise_backup.CompareValue = CountRise_ReadCompare();
            CountRise_backup.InterruptMaskValue = CountRise_STATUS_MASK;
        #endif /* (CY_PSOC5A) */
        
        #if (CY_PSOC3 || CY_PSOC5LP)
            CountRise_backup.CounterUdb = CountRise_ReadCounter();
            CountRise_backup.InterruptMaskValue = CountRise_STATUS_MASK;
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
        
        #if(!CountRise_ControlRegRemoved)
            CountRise_backup.CounterControlRegister = CountRise_ReadControlRegister();
        #endif /* (!CountRise_ControlRegRemoved) */
    #endif /* (!CountRise_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: CountRise_RestoreConfig
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
*  CountRise_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void CountRise_RestoreConfig(void) 
{      
    #if (!CountRise_UsingFixedFunction)     
        /* Restore the UDB non-rentention registers for PSoC5A */
        #if (CY_PSOC5A)
            /* Interrupt State Backup for Critical Region*/
            uint8 CountRise_interruptState;
        
            CountRise_WriteCounter(CountRise_backup.CounterUdb);
            CountRise_WritePeriod(CountRise_backup.CounterPeriod);
            CountRise_WriteCompare(CountRise_backup.CompareValue);
            /* Enter Critical Region*/
            CountRise_interruptState = CyEnterCriticalSection();
        
            CountRise_STATUS_AUX_CTRL |= CountRise_STATUS_ACTL_INT_EN_MASK;
            /* Exit Critical Region*/
            CyExitCriticalSection(CountRise_interruptState);
            CountRise_STATUS_MASK = CountRise_backup.InterruptMaskValue;
        #endif  /* (CY_PSOC5A) */
        
        #if (CY_PSOC3 || CY_PSOC5LP)
            CountRise_WriteCounter(CountRise_backup.CounterUdb);
            CountRise_STATUS_MASK = CountRise_backup.InterruptMaskValue;
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
        
        #if(!CountRise_ControlRegRemoved)
            CountRise_WriteControlRegister(CountRise_backup.CounterControlRegister);
        #endif /* (!CountRise_ControlRegRemoved) */
    #endif /* (!CountRise_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: CountRise_Sleep
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
*  CountRise_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void CountRise_Sleep(void) 
{
    #if(!CountRise_ControlRegRemoved)
        /* Save Counter's enable state */
        if(CountRise_CTRL_ENABLE == (CountRise_CONTROL & CountRise_CTRL_ENABLE))
        {
            /* Counter is enabled */
            CountRise_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            CountRise_backup.CounterEnableState = 0u;
        }
    #endif /* (!CountRise_ControlRegRemoved) */
    CountRise_Stop();
    CountRise_SaveConfig();
}


/*******************************************************************************
* Function Name: CountRise_Wakeup
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
*  CountRise_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void CountRise_Wakeup(void) 
{
    CountRise_RestoreConfig();
    #if(!CountRise_ControlRegRemoved)
        if(CountRise_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            CountRise_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!CountRise_ControlRegRemoved) */
    
}


/* [] END OF FILE */
