/*******************************************************************************
* File Name: BL_HBridge_PM.c
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
#include "BL_HBridge.h"

static BL_HBridge_backupStruct BL_HBridge_backup;


/*******************************************************************************
* Function Name: BL_HBridge_SaveConfig
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
*  BL_HBridge_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void BL_HBridge_SaveConfig(void) 
{
    
    #if(!BL_HBridge_UsingFixedFunction)
        #if (CY_PSOC5A)
            BL_HBridge_backup.PWMUdb = BL_HBridge_ReadCounter();
            BL_HBridge_backup.PWMPeriod = BL_HBridge_ReadPeriod();
            #if (BL_HBridge_UseStatus)
                BL_HBridge_backup.InterruptMaskValue = BL_HBridge_STATUS_MASK;
            #endif /* (BL_HBridge_UseStatus) */
            
            #if(BL_HBridge_UseOneCompareMode)
                BL_HBridge_backup.PWMCompareValue = BL_HBridge_ReadCompare();
            #else
                BL_HBridge_backup.PWMCompareValue1 = BL_HBridge_ReadCompare1();
                BL_HBridge_backup.PWMCompareValue2 = BL_HBridge_ReadCompare2();
            #endif /* (BL_HBridge_UseOneCompareMode) */
            
           #if(BL_HBridge_DeadBandUsed)
                BL_HBridge_backup.PWMdeadBandValue = BL_HBridge_ReadDeadTime();
            #endif /* (BL_HBridge_DeadBandUsed) */
          
            #if ( BL_HBridge_KillModeMinTime)
                BL_HBridge_backup.PWMKillCounterPeriod = BL_HBridge_ReadKillTime();
            #endif /* ( BL_HBridge_KillModeMinTime) */
        #endif /* (CY_PSOC5A) */
        
        #if (CY_PSOC3 || CY_PSOC5LP)
            #if(!BL_HBridge_PWMModeIsCenterAligned)
                BL_HBridge_backup.PWMPeriod = BL_HBridge_ReadPeriod();
            #endif /* (!BL_HBridge_PWMModeIsCenterAligned) */
            BL_HBridge_backup.PWMUdb = BL_HBridge_ReadCounter();
            #if (BL_HBridge_UseStatus)
                BL_HBridge_backup.InterruptMaskValue = BL_HBridge_STATUS_MASK;
            #endif /* (BL_HBridge_UseStatus) */
            
            #if(BL_HBridge_DeadBandMode == BL_HBridge__B_PWM__DBM_256_CLOCKS || \
                BL_HBridge_DeadBandMode == BL_HBridge__B_PWM__DBM_2_4_CLOCKS)
                BL_HBridge_backup.PWMdeadBandValue = BL_HBridge_ReadDeadTime();
            #endif /*  deadband count is either 2-4 clocks or 256 clocks */
            
            #if(BL_HBridge_KillModeMinTime)
                 BL_HBridge_backup.PWMKillCounterPeriod = BL_HBridge_ReadKillTime();
            #endif /* (BL_HBridge_KillModeMinTime) */
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
        
        #if(BL_HBridge_UseControl)
            BL_HBridge_backup.PWMControlRegister = BL_HBridge_ReadControlRegister();
        #endif /* (BL_HBridge_UseControl) */
    #endif  /* (!BL_HBridge_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BL_HBridge_RestoreConfig
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
*  BL_HBridge_backup:  Variables of this global structure are used to  
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void BL_HBridge_RestoreConfig(void) 
{
        #if(!BL_HBridge_UsingFixedFunction)
            #if (CY_PSOC5A)
                /* Interrupt State Backup for Critical Region*/
                uint8 BL_HBridge_interruptState;
                /* Enter Critical Region*/
                BL_HBridge_interruptState = CyEnterCriticalSection();
                #if (BL_HBridge_UseStatus)
                    /* Use the interrupt output of the status register for IRQ output */
                    BL_HBridge_STATUS_AUX_CTRL |= BL_HBridge_STATUS_ACTL_INT_EN_MASK;
                    
                    BL_HBridge_STATUS_MASK = BL_HBridge_backup.InterruptMaskValue;
                #endif /* (BL_HBridge_UseStatus) */
                
                #if (BL_HBridge_Resolution == 8)
                    /* Set FIFO 0 to 1 byte register for period*/
                    BL_HBridge_AUX_CONTROLDP0 |= (BL_HBridge_AUX_CTRL_FIFO0_CLR);
                #else /* (BL_HBridge_Resolution == 16)*/
                    /* Set FIFO 0 to 1 byte register for period */
                    BL_HBridge_AUX_CONTROLDP0 |= (BL_HBridge_AUX_CTRL_FIFO0_CLR);
                    BL_HBridge_AUX_CONTROLDP1 |= (BL_HBridge_AUX_CTRL_FIFO0_CLR);
                #endif /* (BL_HBridge_Resolution == 8) */
                /* Exit Critical Region*/
                CyExitCriticalSection(BL_HBridge_interruptState);
                
                BL_HBridge_WriteCounter(BL_HBridge_backup.PWMUdb);
                BL_HBridge_WritePeriod(BL_HBridge_backup.PWMPeriod);
                
                #if(BL_HBridge_UseOneCompareMode)
                    BL_HBridge_WriteCompare(BL_HBridge_backup.PWMCompareValue);
                #else
                    BL_HBridge_WriteCompare1(BL_HBridge_backup.PWMCompareValue1);
                    BL_HBridge_WriteCompare2(BL_HBridge_backup.PWMCompareValue2);
                #endif /* (BL_HBridge_UseOneCompareMode) */
                
               #if(BL_HBridge_DeadBandMode == BL_HBridge__B_PWM__DBM_256_CLOCKS || \
                   BL_HBridge_DeadBandMode == BL_HBridge__B_PWM__DBM_2_4_CLOCKS)
                    BL_HBridge_WriteDeadTime(BL_HBridge_backup.PWMdeadBandValue);
                #endif /* deadband count is either 2-4 clocks or 256 clocks */
            
                #if ( BL_HBridge_KillModeMinTime)
                    BL_HBridge_WriteKillTime(BL_HBridge_backup.PWMKillCounterPeriod);
                #endif /* ( BL_HBridge_KillModeMinTime) */
            #endif /* (CY_PSOC5A) */
            
            #if (CY_PSOC3 || CY_PSOC5LP)
                #if(!BL_HBridge_PWMModeIsCenterAligned)
                    BL_HBridge_WritePeriod(BL_HBridge_backup.PWMPeriod);
                #endif /* (!BL_HBridge_PWMModeIsCenterAligned) */
                BL_HBridge_WriteCounter(BL_HBridge_backup.PWMUdb);
                #if (BL_HBridge_UseStatus)
                    BL_HBridge_STATUS_MASK = BL_HBridge_backup.InterruptMaskValue;
                #endif /* (BL_HBridge_UseStatus) */
                
                #if(BL_HBridge_DeadBandMode == BL_HBridge__B_PWM__DBM_256_CLOCKS || \
                    BL_HBridge_DeadBandMode == BL_HBridge__B_PWM__DBM_2_4_CLOCKS)
                    BL_HBridge_WriteDeadTime(BL_HBridge_backup.PWMdeadBandValue);
                #endif /* deadband count is either 2-4 clocks or 256 clocks */
                
                #if(BL_HBridge_KillModeMinTime)
                    BL_HBridge_WriteKillTime(BL_HBridge_backup.PWMKillCounterPeriod);
                #endif /* (BL_HBridge_KillModeMinTime) */
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            
            #if(BL_HBridge_UseControl)
                BL_HBridge_WriteControlRegister(BL_HBridge_backup.PWMControlRegister); 
            #endif /* (BL_HBridge_UseControl) */
        #endif  /* (!BL_HBridge_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: BL_HBridge_Sleep
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
*  BL_HBridge_backup.PWMEnableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void BL_HBridge_Sleep(void) 
{
    #if(BL_HBridge_UseControl)
        if(BL_HBridge_CTRL_ENABLE == (BL_HBridge_CONTROL & BL_HBridge_CTRL_ENABLE))
        {
            /*Component is enabled */
            BL_HBridge_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            BL_HBridge_backup.PWMEnableState = 0u;
        }
    #endif /* (BL_HBridge_UseControl) */
    /* Stop component */
    BL_HBridge_Stop();
    
    /* Save registers configuration */
    BL_HBridge_SaveConfig();
}


/*******************************************************************************
* Function Name: BL_HBridge_Wakeup
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
*  BL_HBridge_backup.pwmEnable:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void BL_HBridge_Wakeup(void) 
{
     /* Restore registers values */
    BL_HBridge_RestoreConfig();
    
    if(BL_HBridge_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        BL_HBridge_Enable();
    } /* Do nothing if component's block was disabled before */
    
}


/* [] END OF FILE */
