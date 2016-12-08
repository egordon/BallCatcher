/*******************************************************************************
* File Name: BR_HBridge_PM.c
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
#include "BR_HBridge.h"

static BR_HBridge_backupStruct BR_HBridge_backup;


/*******************************************************************************
* Function Name: BR_HBridge_SaveConfig
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
*  BR_HBridge_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void BR_HBridge_SaveConfig(void) 
{
    
    #if(!BR_HBridge_UsingFixedFunction)
        #if (CY_PSOC5A)
            BR_HBridge_backup.PWMUdb = BR_HBridge_ReadCounter();
            BR_HBridge_backup.PWMPeriod = BR_HBridge_ReadPeriod();
            #if (BR_HBridge_UseStatus)
                BR_HBridge_backup.InterruptMaskValue = BR_HBridge_STATUS_MASK;
            #endif /* (BR_HBridge_UseStatus) */
            
            #if(BR_HBridge_UseOneCompareMode)
                BR_HBridge_backup.PWMCompareValue = BR_HBridge_ReadCompare();
            #else
                BR_HBridge_backup.PWMCompareValue1 = BR_HBridge_ReadCompare1();
                BR_HBridge_backup.PWMCompareValue2 = BR_HBridge_ReadCompare2();
            #endif /* (BR_HBridge_UseOneCompareMode) */
            
           #if(BR_HBridge_DeadBandUsed)
                BR_HBridge_backup.PWMdeadBandValue = BR_HBridge_ReadDeadTime();
            #endif /* (BR_HBridge_DeadBandUsed) */
          
            #if ( BR_HBridge_KillModeMinTime)
                BR_HBridge_backup.PWMKillCounterPeriod = BR_HBridge_ReadKillTime();
            #endif /* ( BR_HBridge_KillModeMinTime) */
        #endif /* (CY_PSOC5A) */
        
        #if (CY_PSOC3 || CY_PSOC5LP)
            #if(!BR_HBridge_PWMModeIsCenterAligned)
                BR_HBridge_backup.PWMPeriod = BR_HBridge_ReadPeriod();
            #endif /* (!BR_HBridge_PWMModeIsCenterAligned) */
            BR_HBridge_backup.PWMUdb = BR_HBridge_ReadCounter();
            #if (BR_HBridge_UseStatus)
                BR_HBridge_backup.InterruptMaskValue = BR_HBridge_STATUS_MASK;
            #endif /* (BR_HBridge_UseStatus) */
            
            #if(BR_HBridge_DeadBandMode == BR_HBridge__B_PWM__DBM_256_CLOCKS || \
                BR_HBridge_DeadBandMode == BR_HBridge__B_PWM__DBM_2_4_CLOCKS)
                BR_HBridge_backup.PWMdeadBandValue = BR_HBridge_ReadDeadTime();
            #endif /*  deadband count is either 2-4 clocks or 256 clocks */
            
            #if(BR_HBridge_KillModeMinTime)
                 BR_HBridge_backup.PWMKillCounterPeriod = BR_HBridge_ReadKillTime();
            #endif /* (BR_HBridge_KillModeMinTime) */
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
        
        #if(BR_HBridge_UseControl)
            BR_HBridge_backup.PWMControlRegister = BR_HBridge_ReadControlRegister();
        #endif /* (BR_HBridge_UseControl) */
    #endif  /* (!BR_HBridge_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BR_HBridge_RestoreConfig
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
*  BR_HBridge_backup:  Variables of this global structure are used to  
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void BR_HBridge_RestoreConfig(void) 
{
        #if(!BR_HBridge_UsingFixedFunction)
            #if (CY_PSOC5A)
                /* Interrupt State Backup for Critical Region*/
                uint8 BR_HBridge_interruptState;
                /* Enter Critical Region*/
                BR_HBridge_interruptState = CyEnterCriticalSection();
                #if (BR_HBridge_UseStatus)
                    /* Use the interrupt output of the status register for IRQ output */
                    BR_HBridge_STATUS_AUX_CTRL |= BR_HBridge_STATUS_ACTL_INT_EN_MASK;
                    
                    BR_HBridge_STATUS_MASK = BR_HBridge_backup.InterruptMaskValue;
                #endif /* (BR_HBridge_UseStatus) */
                
                #if (BR_HBridge_Resolution == 8)
                    /* Set FIFO 0 to 1 byte register for period*/
                    BR_HBridge_AUX_CONTROLDP0 |= (BR_HBridge_AUX_CTRL_FIFO0_CLR);
                #else /* (BR_HBridge_Resolution == 16)*/
                    /* Set FIFO 0 to 1 byte register for period */
                    BR_HBridge_AUX_CONTROLDP0 |= (BR_HBridge_AUX_CTRL_FIFO0_CLR);
                    BR_HBridge_AUX_CONTROLDP1 |= (BR_HBridge_AUX_CTRL_FIFO0_CLR);
                #endif /* (BR_HBridge_Resolution == 8) */
                /* Exit Critical Region*/
                CyExitCriticalSection(BR_HBridge_interruptState);
                
                BR_HBridge_WriteCounter(BR_HBridge_backup.PWMUdb);
                BR_HBridge_WritePeriod(BR_HBridge_backup.PWMPeriod);
                
                #if(BR_HBridge_UseOneCompareMode)
                    BR_HBridge_WriteCompare(BR_HBridge_backup.PWMCompareValue);
                #else
                    BR_HBridge_WriteCompare1(BR_HBridge_backup.PWMCompareValue1);
                    BR_HBridge_WriteCompare2(BR_HBridge_backup.PWMCompareValue2);
                #endif /* (BR_HBridge_UseOneCompareMode) */
                
               #if(BR_HBridge_DeadBandMode == BR_HBridge__B_PWM__DBM_256_CLOCKS || \
                   BR_HBridge_DeadBandMode == BR_HBridge__B_PWM__DBM_2_4_CLOCKS)
                    BR_HBridge_WriteDeadTime(BR_HBridge_backup.PWMdeadBandValue);
                #endif /* deadband count is either 2-4 clocks or 256 clocks */
            
                #if ( BR_HBridge_KillModeMinTime)
                    BR_HBridge_WriteKillTime(BR_HBridge_backup.PWMKillCounterPeriod);
                #endif /* ( BR_HBridge_KillModeMinTime) */
            #endif /* (CY_PSOC5A) */
            
            #if (CY_PSOC3 || CY_PSOC5LP)
                #if(!BR_HBridge_PWMModeIsCenterAligned)
                    BR_HBridge_WritePeriod(BR_HBridge_backup.PWMPeriod);
                #endif /* (!BR_HBridge_PWMModeIsCenterAligned) */
                BR_HBridge_WriteCounter(BR_HBridge_backup.PWMUdb);
                #if (BR_HBridge_UseStatus)
                    BR_HBridge_STATUS_MASK = BR_HBridge_backup.InterruptMaskValue;
                #endif /* (BR_HBridge_UseStatus) */
                
                #if(BR_HBridge_DeadBandMode == BR_HBridge__B_PWM__DBM_256_CLOCKS || \
                    BR_HBridge_DeadBandMode == BR_HBridge__B_PWM__DBM_2_4_CLOCKS)
                    BR_HBridge_WriteDeadTime(BR_HBridge_backup.PWMdeadBandValue);
                #endif /* deadband count is either 2-4 clocks or 256 clocks */
                
                #if(BR_HBridge_KillModeMinTime)
                    BR_HBridge_WriteKillTime(BR_HBridge_backup.PWMKillCounterPeriod);
                #endif /* (BR_HBridge_KillModeMinTime) */
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            
            #if(BR_HBridge_UseControl)
                BR_HBridge_WriteControlRegister(BR_HBridge_backup.PWMControlRegister); 
            #endif /* (BR_HBridge_UseControl) */
        #endif  /* (!BR_HBridge_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: BR_HBridge_Sleep
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
*  BR_HBridge_backup.PWMEnableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void BR_HBridge_Sleep(void) 
{
    #if(BR_HBridge_UseControl)
        if(BR_HBridge_CTRL_ENABLE == (BR_HBridge_CONTROL & BR_HBridge_CTRL_ENABLE))
        {
            /*Component is enabled */
            BR_HBridge_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            BR_HBridge_backup.PWMEnableState = 0u;
        }
    #endif /* (BR_HBridge_UseControl) */
    /* Stop component */
    BR_HBridge_Stop();
    
    /* Save registers configuration */
    BR_HBridge_SaveConfig();
}


/*******************************************************************************
* Function Name: BR_HBridge_Wakeup
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
*  BR_HBridge_backup.pwmEnable:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void BR_HBridge_Wakeup(void) 
{
     /* Restore registers values */
    BR_HBridge_RestoreConfig();
    
    if(BR_HBridge_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        BR_HBridge_Enable();
    } /* Do nothing if component's block was disabled before */
    
}


/* [] END OF FILE */
