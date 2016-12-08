/*******************************************************************************
* File Name: AMux_Sel_PM.c
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
#include "AMux_Sel.h"

static AMux_Sel_backupStruct AMux_Sel_backup;


/*******************************************************************************
* Function Name: AMux_Sel_SaveConfig
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
*  AMux_Sel_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void AMux_Sel_SaveConfig(void) 
{
    
    #if(!AMux_Sel_UsingFixedFunction)
        #if (CY_PSOC5A)
            AMux_Sel_backup.PWMUdb = AMux_Sel_ReadCounter();
            AMux_Sel_backup.PWMPeriod = AMux_Sel_ReadPeriod();
            #if (AMux_Sel_UseStatus)
                AMux_Sel_backup.InterruptMaskValue = AMux_Sel_STATUS_MASK;
            #endif /* (AMux_Sel_UseStatus) */
            
            #if(AMux_Sel_UseOneCompareMode)
                AMux_Sel_backup.PWMCompareValue = AMux_Sel_ReadCompare();
            #else
                AMux_Sel_backup.PWMCompareValue1 = AMux_Sel_ReadCompare1();
                AMux_Sel_backup.PWMCompareValue2 = AMux_Sel_ReadCompare2();
            #endif /* (AMux_Sel_UseOneCompareMode) */
            
           #if(AMux_Sel_DeadBandUsed)
                AMux_Sel_backup.PWMdeadBandValue = AMux_Sel_ReadDeadTime();
            #endif /* (AMux_Sel_DeadBandUsed) */
          
            #if ( AMux_Sel_KillModeMinTime)
                AMux_Sel_backup.PWMKillCounterPeriod = AMux_Sel_ReadKillTime();
            #endif /* ( AMux_Sel_KillModeMinTime) */
        #endif /* (CY_PSOC5A) */
        
        #if (CY_PSOC3 || CY_PSOC5LP)
            #if(!AMux_Sel_PWMModeIsCenterAligned)
                AMux_Sel_backup.PWMPeriod = AMux_Sel_ReadPeriod();
            #endif /* (!AMux_Sel_PWMModeIsCenterAligned) */
            AMux_Sel_backup.PWMUdb = AMux_Sel_ReadCounter();
            #if (AMux_Sel_UseStatus)
                AMux_Sel_backup.InterruptMaskValue = AMux_Sel_STATUS_MASK;
            #endif /* (AMux_Sel_UseStatus) */
            
            #if(AMux_Sel_DeadBandMode == AMux_Sel__B_PWM__DBM_256_CLOCKS || \
                AMux_Sel_DeadBandMode == AMux_Sel__B_PWM__DBM_2_4_CLOCKS)
                AMux_Sel_backup.PWMdeadBandValue = AMux_Sel_ReadDeadTime();
            #endif /*  deadband count is either 2-4 clocks or 256 clocks */
            
            #if(AMux_Sel_KillModeMinTime)
                 AMux_Sel_backup.PWMKillCounterPeriod = AMux_Sel_ReadKillTime();
            #endif /* (AMux_Sel_KillModeMinTime) */
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
        
        #if(AMux_Sel_UseControl)
            AMux_Sel_backup.PWMControlRegister = AMux_Sel_ReadControlRegister();
        #endif /* (AMux_Sel_UseControl) */
    #endif  /* (!AMux_Sel_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: AMux_Sel_RestoreConfig
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
*  AMux_Sel_backup:  Variables of this global structure are used to  
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void AMux_Sel_RestoreConfig(void) 
{
        #if(!AMux_Sel_UsingFixedFunction)
            #if (CY_PSOC5A)
                /* Interrupt State Backup for Critical Region*/
                uint8 AMux_Sel_interruptState;
                /* Enter Critical Region*/
                AMux_Sel_interruptState = CyEnterCriticalSection();
                #if (AMux_Sel_UseStatus)
                    /* Use the interrupt output of the status register for IRQ output */
                    AMux_Sel_STATUS_AUX_CTRL |= AMux_Sel_STATUS_ACTL_INT_EN_MASK;
                    
                    AMux_Sel_STATUS_MASK = AMux_Sel_backup.InterruptMaskValue;
                #endif /* (AMux_Sel_UseStatus) */
                
                #if (AMux_Sel_Resolution == 8)
                    /* Set FIFO 0 to 1 byte register for period*/
                    AMux_Sel_AUX_CONTROLDP0 |= (AMux_Sel_AUX_CTRL_FIFO0_CLR);
                #else /* (AMux_Sel_Resolution == 16)*/
                    /* Set FIFO 0 to 1 byte register for period */
                    AMux_Sel_AUX_CONTROLDP0 |= (AMux_Sel_AUX_CTRL_FIFO0_CLR);
                    AMux_Sel_AUX_CONTROLDP1 |= (AMux_Sel_AUX_CTRL_FIFO0_CLR);
                #endif /* (AMux_Sel_Resolution == 8) */
                /* Exit Critical Region*/
                CyExitCriticalSection(AMux_Sel_interruptState);
                
                AMux_Sel_WriteCounter(AMux_Sel_backup.PWMUdb);
                AMux_Sel_WritePeriod(AMux_Sel_backup.PWMPeriod);
                
                #if(AMux_Sel_UseOneCompareMode)
                    AMux_Sel_WriteCompare(AMux_Sel_backup.PWMCompareValue);
                #else
                    AMux_Sel_WriteCompare1(AMux_Sel_backup.PWMCompareValue1);
                    AMux_Sel_WriteCompare2(AMux_Sel_backup.PWMCompareValue2);
                #endif /* (AMux_Sel_UseOneCompareMode) */
                
               #if(AMux_Sel_DeadBandMode == AMux_Sel__B_PWM__DBM_256_CLOCKS || \
                   AMux_Sel_DeadBandMode == AMux_Sel__B_PWM__DBM_2_4_CLOCKS)
                    AMux_Sel_WriteDeadTime(AMux_Sel_backup.PWMdeadBandValue);
                #endif /* deadband count is either 2-4 clocks or 256 clocks */
            
                #if ( AMux_Sel_KillModeMinTime)
                    AMux_Sel_WriteKillTime(AMux_Sel_backup.PWMKillCounterPeriod);
                #endif /* ( AMux_Sel_KillModeMinTime) */
            #endif /* (CY_PSOC5A) */
            
            #if (CY_PSOC3 || CY_PSOC5LP)
                #if(!AMux_Sel_PWMModeIsCenterAligned)
                    AMux_Sel_WritePeriod(AMux_Sel_backup.PWMPeriod);
                #endif /* (!AMux_Sel_PWMModeIsCenterAligned) */
                AMux_Sel_WriteCounter(AMux_Sel_backup.PWMUdb);
                #if (AMux_Sel_UseStatus)
                    AMux_Sel_STATUS_MASK = AMux_Sel_backup.InterruptMaskValue;
                #endif /* (AMux_Sel_UseStatus) */
                
                #if(AMux_Sel_DeadBandMode == AMux_Sel__B_PWM__DBM_256_CLOCKS || \
                    AMux_Sel_DeadBandMode == AMux_Sel__B_PWM__DBM_2_4_CLOCKS)
                    AMux_Sel_WriteDeadTime(AMux_Sel_backup.PWMdeadBandValue);
                #endif /* deadband count is either 2-4 clocks or 256 clocks */
                
                #if(AMux_Sel_KillModeMinTime)
                    AMux_Sel_WriteKillTime(AMux_Sel_backup.PWMKillCounterPeriod);
                #endif /* (AMux_Sel_KillModeMinTime) */
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            
            #if(AMux_Sel_UseControl)
                AMux_Sel_WriteControlRegister(AMux_Sel_backup.PWMControlRegister); 
            #endif /* (AMux_Sel_UseControl) */
        #endif  /* (!AMux_Sel_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: AMux_Sel_Sleep
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
*  AMux_Sel_backup.PWMEnableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void AMux_Sel_Sleep(void) 
{
    #if(AMux_Sel_UseControl)
        if(AMux_Sel_CTRL_ENABLE == (AMux_Sel_CONTROL & AMux_Sel_CTRL_ENABLE))
        {
            /*Component is enabled */
            AMux_Sel_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            AMux_Sel_backup.PWMEnableState = 0u;
        }
    #endif /* (AMux_Sel_UseControl) */
    /* Stop component */
    AMux_Sel_Stop();
    
    /* Save registers configuration */
    AMux_Sel_SaveConfig();
}


/*******************************************************************************
* Function Name: AMux_Sel_Wakeup
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
*  AMux_Sel_backup.pwmEnable:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void AMux_Sel_Wakeup(void) 
{
     /* Restore registers values */
    AMux_Sel_RestoreConfig();
    
    if(AMux_Sel_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        AMux_Sel_Enable();
    } /* Do nothing if component's block was disabled before */
    
}


/* [] END OF FILE */
