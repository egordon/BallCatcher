/*******************************************************************************
* File Name: FR_HBridge_PM.c
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
#include "FR_HBridge.h"

static FR_HBridge_backupStruct FR_HBridge_backup;


/*******************************************************************************
* Function Name: FR_HBridge_SaveConfig
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
*  FR_HBridge_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void FR_HBridge_SaveConfig(void) 
{
    
    #if(!FR_HBridge_UsingFixedFunction)
        #if (CY_PSOC5A)
            FR_HBridge_backup.PWMUdb = FR_HBridge_ReadCounter();
            FR_HBridge_backup.PWMPeriod = FR_HBridge_ReadPeriod();
            #if (FR_HBridge_UseStatus)
                FR_HBridge_backup.InterruptMaskValue = FR_HBridge_STATUS_MASK;
            #endif /* (FR_HBridge_UseStatus) */
            
            #if(FR_HBridge_UseOneCompareMode)
                FR_HBridge_backup.PWMCompareValue = FR_HBridge_ReadCompare();
            #else
                FR_HBridge_backup.PWMCompareValue1 = FR_HBridge_ReadCompare1();
                FR_HBridge_backup.PWMCompareValue2 = FR_HBridge_ReadCompare2();
            #endif /* (FR_HBridge_UseOneCompareMode) */
            
           #if(FR_HBridge_DeadBandUsed)
                FR_HBridge_backup.PWMdeadBandValue = FR_HBridge_ReadDeadTime();
            #endif /* (FR_HBridge_DeadBandUsed) */
          
            #if ( FR_HBridge_KillModeMinTime)
                FR_HBridge_backup.PWMKillCounterPeriod = FR_HBridge_ReadKillTime();
            #endif /* ( FR_HBridge_KillModeMinTime) */
        #endif /* (CY_PSOC5A) */
        
        #if (CY_PSOC3 || CY_PSOC5LP)
            #if(!FR_HBridge_PWMModeIsCenterAligned)
                FR_HBridge_backup.PWMPeriod = FR_HBridge_ReadPeriod();
            #endif /* (!FR_HBridge_PWMModeIsCenterAligned) */
            FR_HBridge_backup.PWMUdb = FR_HBridge_ReadCounter();
            #if (FR_HBridge_UseStatus)
                FR_HBridge_backup.InterruptMaskValue = FR_HBridge_STATUS_MASK;
            #endif /* (FR_HBridge_UseStatus) */
            
            #if(FR_HBridge_DeadBandMode == FR_HBridge__B_PWM__DBM_256_CLOCKS || \
                FR_HBridge_DeadBandMode == FR_HBridge__B_PWM__DBM_2_4_CLOCKS)
                FR_HBridge_backup.PWMdeadBandValue = FR_HBridge_ReadDeadTime();
            #endif /*  deadband count is either 2-4 clocks or 256 clocks */
            
            #if(FR_HBridge_KillModeMinTime)
                 FR_HBridge_backup.PWMKillCounterPeriod = FR_HBridge_ReadKillTime();
            #endif /* (FR_HBridge_KillModeMinTime) */
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
        
        #if(FR_HBridge_UseControl)
            FR_HBridge_backup.PWMControlRegister = FR_HBridge_ReadControlRegister();
        #endif /* (FR_HBridge_UseControl) */
    #endif  /* (!FR_HBridge_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: FR_HBridge_RestoreConfig
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
*  FR_HBridge_backup:  Variables of this global structure are used to  
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void FR_HBridge_RestoreConfig(void) 
{
        #if(!FR_HBridge_UsingFixedFunction)
            #if (CY_PSOC5A)
                /* Interrupt State Backup for Critical Region*/
                uint8 FR_HBridge_interruptState;
                /* Enter Critical Region*/
                FR_HBridge_interruptState = CyEnterCriticalSection();
                #if (FR_HBridge_UseStatus)
                    /* Use the interrupt output of the status register for IRQ output */
                    FR_HBridge_STATUS_AUX_CTRL |= FR_HBridge_STATUS_ACTL_INT_EN_MASK;
                    
                    FR_HBridge_STATUS_MASK = FR_HBridge_backup.InterruptMaskValue;
                #endif /* (FR_HBridge_UseStatus) */
                
                #if (FR_HBridge_Resolution == 8)
                    /* Set FIFO 0 to 1 byte register for period*/
                    FR_HBridge_AUX_CONTROLDP0 |= (FR_HBridge_AUX_CTRL_FIFO0_CLR);
                #else /* (FR_HBridge_Resolution == 16)*/
                    /* Set FIFO 0 to 1 byte register for period */
                    FR_HBridge_AUX_CONTROLDP0 |= (FR_HBridge_AUX_CTRL_FIFO0_CLR);
                    FR_HBridge_AUX_CONTROLDP1 |= (FR_HBridge_AUX_CTRL_FIFO0_CLR);
                #endif /* (FR_HBridge_Resolution == 8) */
                /* Exit Critical Region*/
                CyExitCriticalSection(FR_HBridge_interruptState);
                
                FR_HBridge_WriteCounter(FR_HBridge_backup.PWMUdb);
                FR_HBridge_WritePeriod(FR_HBridge_backup.PWMPeriod);
                
                #if(FR_HBridge_UseOneCompareMode)
                    FR_HBridge_WriteCompare(FR_HBridge_backup.PWMCompareValue);
                #else
                    FR_HBridge_WriteCompare1(FR_HBridge_backup.PWMCompareValue1);
                    FR_HBridge_WriteCompare2(FR_HBridge_backup.PWMCompareValue2);
                #endif /* (FR_HBridge_UseOneCompareMode) */
                
               #if(FR_HBridge_DeadBandMode == FR_HBridge__B_PWM__DBM_256_CLOCKS || \
                   FR_HBridge_DeadBandMode == FR_HBridge__B_PWM__DBM_2_4_CLOCKS)
                    FR_HBridge_WriteDeadTime(FR_HBridge_backup.PWMdeadBandValue);
                #endif /* deadband count is either 2-4 clocks or 256 clocks */
            
                #if ( FR_HBridge_KillModeMinTime)
                    FR_HBridge_WriteKillTime(FR_HBridge_backup.PWMKillCounterPeriod);
                #endif /* ( FR_HBridge_KillModeMinTime) */
            #endif /* (CY_PSOC5A) */
            
            #if (CY_PSOC3 || CY_PSOC5LP)
                #if(!FR_HBridge_PWMModeIsCenterAligned)
                    FR_HBridge_WritePeriod(FR_HBridge_backup.PWMPeriod);
                #endif /* (!FR_HBridge_PWMModeIsCenterAligned) */
                FR_HBridge_WriteCounter(FR_HBridge_backup.PWMUdb);
                #if (FR_HBridge_UseStatus)
                    FR_HBridge_STATUS_MASK = FR_HBridge_backup.InterruptMaskValue;
                #endif /* (FR_HBridge_UseStatus) */
                
                #if(FR_HBridge_DeadBandMode == FR_HBridge__B_PWM__DBM_256_CLOCKS || \
                    FR_HBridge_DeadBandMode == FR_HBridge__B_PWM__DBM_2_4_CLOCKS)
                    FR_HBridge_WriteDeadTime(FR_HBridge_backup.PWMdeadBandValue);
                #endif /* deadband count is either 2-4 clocks or 256 clocks */
                
                #if(FR_HBridge_KillModeMinTime)
                    FR_HBridge_WriteKillTime(FR_HBridge_backup.PWMKillCounterPeriod);
                #endif /* (FR_HBridge_KillModeMinTime) */
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            
            #if(FR_HBridge_UseControl)
                FR_HBridge_WriteControlRegister(FR_HBridge_backup.PWMControlRegister); 
            #endif /* (FR_HBridge_UseControl) */
        #endif  /* (!FR_HBridge_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: FR_HBridge_Sleep
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
*  FR_HBridge_backup.PWMEnableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void FR_HBridge_Sleep(void) 
{
    #if(FR_HBridge_UseControl)
        if(FR_HBridge_CTRL_ENABLE == (FR_HBridge_CONTROL & FR_HBridge_CTRL_ENABLE))
        {
            /*Component is enabled */
            FR_HBridge_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            FR_HBridge_backup.PWMEnableState = 0u;
        }
    #endif /* (FR_HBridge_UseControl) */
    /* Stop component */
    FR_HBridge_Stop();
    
    /* Save registers configuration */
    FR_HBridge_SaveConfig();
}


/*******************************************************************************
* Function Name: FR_HBridge_Wakeup
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
*  FR_HBridge_backup.pwmEnable:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void FR_HBridge_Wakeup(void) 
{
     /* Restore registers values */
    FR_HBridge_RestoreConfig();
    
    if(FR_HBridge_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        FR_HBridge_Enable();
    } /* Do nothing if component's block was disabled before */
    
}


/* [] END OF FILE */
