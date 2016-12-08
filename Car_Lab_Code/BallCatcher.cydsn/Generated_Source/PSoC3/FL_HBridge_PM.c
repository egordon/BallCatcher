/*******************************************************************************
* File Name: FL_HBridge_PM.c
* Version 2.20
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#include "cytypes.h"
#include "FL_HBridge.h"

static FL_HBridge_backupStruct FL_HBridge_backup;


/*******************************************************************************
* Function Name: FL_HBridge_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  FL_HBridge_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void FL_HBridge_SaveConfig(void) 
{
    
    #if(!FL_HBridge_UsingFixedFunction)
        #if (CY_PSOC5A)
            FL_HBridge_backup.PWMUdb = FL_HBridge_ReadCounter();
            FL_HBridge_backup.PWMPeriod = FL_HBridge_ReadPeriod();
            #if (FL_HBridge_UseStatus)
                FL_HBridge_backup.InterruptMaskValue = FL_HBridge_STATUS_MASK;
            #endif /* (FL_HBridge_UseStatus) */
            
            #if(FL_HBridge_UseOneCompareMode)
                FL_HBridge_backup.PWMCompareValue = FL_HBridge_ReadCompare();
            #else
                FL_HBridge_backup.PWMCompareValue1 = FL_HBridge_ReadCompare1();
                FL_HBridge_backup.PWMCompareValue2 = FL_HBridge_ReadCompare2();
            #endif /* (FL_HBridge_UseOneCompareMode) */
            
           #if(FL_HBridge_DeadBandUsed)
                FL_HBridge_backup.PWMdeadBandValue = FL_HBridge_ReadDeadTime();
            #endif /* (FL_HBridge_DeadBandUsed) */
          
            #if ( FL_HBridge_KillModeMinTime)
                FL_HBridge_backup.PWMKillCounterPeriod = FL_HBridge_ReadKillTime();
            #endif /* ( FL_HBridge_KillModeMinTime) */
        #endif /* (CY_PSOC5A) */
        
        #if (CY_PSOC3 || CY_PSOC5LP)
            #if(!FL_HBridge_PWMModeIsCenterAligned)
                FL_HBridge_backup.PWMPeriod = FL_HBridge_ReadPeriod();
            #endif /* (!FL_HBridge_PWMModeIsCenterAligned) */
            FL_HBridge_backup.PWMUdb = FL_HBridge_ReadCounter();
            #if (FL_HBridge_UseStatus)
                FL_HBridge_backup.InterruptMaskValue = FL_HBridge_STATUS_MASK;
            #endif /* (FL_HBridge_UseStatus) */
            
            #if(FL_HBridge_DeadBandMode == FL_HBridge__B_PWM__DBM_256_CLOCKS || \
                FL_HBridge_DeadBandMode == FL_HBridge__B_PWM__DBM_2_4_CLOCKS)
                FL_HBridge_backup.PWMdeadBandValue = FL_HBridge_ReadDeadTime();
            #endif /*  deadband count is either 2-4 clocks or 256 clocks */
            
            #if(FL_HBridge_KillModeMinTime)
                 FL_HBridge_backup.PWMKillCounterPeriod = FL_HBridge_ReadKillTime();
            #endif /* (FL_HBridge_KillModeMinTime) */
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
        
        #if(FL_HBridge_UseControl)
            FL_HBridge_backup.PWMControlRegister = FL_HBridge_ReadControlRegister();
        #endif /* (FL_HBridge_UseControl) */
    #endif  /* (!FL_HBridge_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: FL_HBridge_RestoreConfig
********************************************************************************
* 
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  FL_HBridge_backup:  Variables of this global structure are used to  
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void FL_HBridge_RestoreConfig(void) 
{
        #if(!FL_HBridge_UsingFixedFunction)
            #if (CY_PSOC5A)
                /* Interrupt State Backup for Critical Region*/
                uint8 FL_HBridge_interruptState;
                /* Enter Critical Region*/
                FL_HBridge_interruptState = CyEnterCriticalSection();
                #if (FL_HBridge_UseStatus)
                    /* Use the interrupt output of the status register for IRQ output */
                    FL_HBridge_STATUS_AUX_CTRL |= FL_HBridge_STATUS_ACTL_INT_EN_MASK;
                    
                    FL_HBridge_STATUS_MASK = FL_HBridge_backup.InterruptMaskValue;
                #endif /* (FL_HBridge_UseStatus) */
                
                #if (FL_HBridge_Resolution == 8)
                    /* Set FIFO 0 to 1 byte register for period*/
                    FL_HBridge_AUX_CONTROLDP0 |= (FL_HBridge_AUX_CTRL_FIFO0_CLR);
                #else /* (FL_HBridge_Resolution == 16)*/
                    /* Set FIFO 0 to 1 byte register for period */
                    FL_HBridge_AUX_CONTROLDP0 |= (FL_HBridge_AUX_CTRL_FIFO0_CLR);
                    FL_HBridge_AUX_CONTROLDP1 |= (FL_HBridge_AUX_CTRL_FIFO0_CLR);
                #endif /* (FL_HBridge_Resolution == 8) */
                /* Exit Critical Region*/
                CyExitCriticalSection(FL_HBridge_interruptState);
                
                FL_HBridge_WriteCounter(FL_HBridge_backup.PWMUdb);
                FL_HBridge_WritePeriod(FL_HBridge_backup.PWMPeriod);
                
                #if(FL_HBridge_UseOneCompareMode)
                    FL_HBridge_WriteCompare(FL_HBridge_backup.PWMCompareValue);
                #else
                    FL_HBridge_WriteCompare1(FL_HBridge_backup.PWMCompareValue1);
                    FL_HBridge_WriteCompare2(FL_HBridge_backup.PWMCompareValue2);
                #endif /* (FL_HBridge_UseOneCompareMode) */
                
               #if(FL_HBridge_DeadBandMode == FL_HBridge__B_PWM__DBM_256_CLOCKS || \
                   FL_HBridge_DeadBandMode == FL_HBridge__B_PWM__DBM_2_4_CLOCKS)
                    FL_HBridge_WriteDeadTime(FL_HBridge_backup.PWMdeadBandValue);
                #endif /* deadband count is either 2-4 clocks or 256 clocks */
            
                #if ( FL_HBridge_KillModeMinTime)
                    FL_HBridge_WriteKillTime(FL_HBridge_backup.PWMKillCounterPeriod);
                #endif /* ( FL_HBridge_KillModeMinTime) */
            #endif /* (CY_PSOC5A) */
            
            #if (CY_PSOC3 || CY_PSOC5LP)
                #if(!FL_HBridge_PWMModeIsCenterAligned)
                    FL_HBridge_WritePeriod(FL_HBridge_backup.PWMPeriod);
                #endif /* (!FL_HBridge_PWMModeIsCenterAligned) */
                FL_HBridge_WriteCounter(FL_HBridge_backup.PWMUdb);
                #if (FL_HBridge_UseStatus)
                    FL_HBridge_STATUS_MASK = FL_HBridge_backup.InterruptMaskValue;
                #endif /* (FL_HBridge_UseStatus) */
                
                #if(FL_HBridge_DeadBandMode == FL_HBridge__B_PWM__DBM_256_CLOCKS || \
                    FL_HBridge_DeadBandMode == FL_HBridge__B_PWM__DBM_2_4_CLOCKS)
                    FL_HBridge_WriteDeadTime(FL_HBridge_backup.PWMdeadBandValue);
                #endif /* deadband count is either 2-4 clocks or 256 clocks */
                
                #if(FL_HBridge_KillModeMinTime)
                    FL_HBridge_WriteKillTime(FL_HBridge_backup.PWMKillCounterPeriod);
                #endif /* (FL_HBridge_KillModeMinTime) */
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            
            #if(FL_HBridge_UseControl)
                FL_HBridge_WriteControlRegister(FL_HBridge_backup.PWMControlRegister); 
            #endif /* (FL_HBridge_UseControl) */
        #endif  /* (!FL_HBridge_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: FL_HBridge_Sleep
********************************************************************************
* 
* Summary:
*  Disables block's operation and saves the user configuration. Should be called 
*  just prior to entering sleep.
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  FL_HBridge_backup.PWMEnableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void FL_HBridge_Sleep(void) 
{
    #if(FL_HBridge_UseControl)
        if(FL_HBridge_CTRL_ENABLE == (FL_HBridge_CONTROL & FL_HBridge_CTRL_ENABLE))
        {
            /*Component is enabled */
            FL_HBridge_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            FL_HBridge_backup.PWMEnableState = 0u;
        }
    #endif /* (FL_HBridge_UseControl) */
    /* Stop component */
    FL_HBridge_Stop();
    
    /* Save registers configuration */
    FL_HBridge_SaveConfig();
}


/*******************************************************************************
* Function Name: FL_HBridge_Wakeup
********************************************************************************
* 
* Summary:
*  Restores and enables the user configuration. Should be called just after 
*  awaking from sleep.
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  FL_HBridge_backup.pwmEnable:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void FL_HBridge_Wakeup(void) 
{
     /* Restore registers values */
    FL_HBridge_RestoreConfig();
    
    if(FL_HBridge_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        FL_HBridge_Enable();
    } /* Do nothing if component's block was disabled before */
    
}


/* [] END OF FILE */
