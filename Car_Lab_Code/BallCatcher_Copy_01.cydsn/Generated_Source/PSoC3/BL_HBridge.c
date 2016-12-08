/*******************************************************************************
* File Name: BL_HBridge.c  
* Version 2.20
*
* Description:
*  The PWM User Module consist of an 8 or 16-bit counter with two 8 or 16-bit
*  comparitors. Each instance of this user module is capable of generating
*  two PWM outputs with the same period. The pulse width is selectable between
*  1 and 255/65535. The period is selectable between 2 and 255/65536 clocks. 
*  The compare value output may be configured to be active when the present 
*  counter is less than or less than/equal to the compare value.
*  A terminal count output is also provided. It generates a pulse one clock
*  width wide when the counter is equal to zero.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cytypes.h"
#include "BL_HBridge.h"

uint8 BL_HBridge_initVar = 0u;


/*******************************************************************************
* Function Name: BL_HBridge_Start
********************************************************************************
*
* Summary:
*  The start function initializes the pwm with the default values, the 
*  enables the counter to begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Global variables:
*  BL_HBridge_initVar: Is modified when this function is called for the 
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void BL_HBridge_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(BL_HBridge_initVar == 0u)
    {
        BL_HBridge_Init();
        BL_HBridge_initVar = 1u;
    }
    BL_HBridge_Enable();

}


/*******************************************************************************
* Function Name: BL_HBridge_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  BL_HBridge_Start().
*
* Parameters:  
*  void
*
* Return: 
*  void
*
*******************************************************************************/
void BL_HBridge_Init(void) 
{
    #if (BL_HBridge_UsingFixedFunction || BL_HBridge_UseControl)
        uint8 ctrl;
    #endif /* (BL_HBridge_UsingFixedFunction || BL_HBridge_UseControl) */
    
    #if(!BL_HBridge_UsingFixedFunction) 
        #if(BL_HBridge_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 BL_HBridge_interruptState;
        #endif /* (BL_HBridge_UseStatus) */
    #endif /* (!BL_HBridge_UsingFixedFunction) */
    
    #if (BL_HBridge_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        BL_HBridge_CONTROL |= BL_HBridge_CFG0_MODE;
        #if (BL_HBridge_DeadBand2_4)
            BL_HBridge_CONTROL |= BL_HBridge_CFG0_DB;
        #endif /* (BL_HBridge_DeadBand2_4) */
                
        /* Set the default Compare Mode */
        #if(CY_PSOC5A)
                ctrl = BL_HBridge_CONTROL2 & ~BL_HBridge_CTRL_CMPMODE1_MASK;
                BL_HBridge_CONTROL2 = ctrl | BL_HBridge_DEFAULT_COMPARE1_MODE;
        #endif /* (CY_PSOC5A) */
        #if(CY_PSOC3 || CY_PSOC5LP)
                ctrl = BL_HBridge_CONTROL3 & ~BL_HBridge_CTRL_CMPMODE1_MASK;
                BL_HBridge_CONTROL3 = ctrl | BL_HBridge_DEFAULT_COMPARE1_MODE;
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
        
         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        BL_HBridge_RT1 &= ~BL_HBridge_RT1_MASK;
        BL_HBridge_RT1 |= BL_HBridge_SYNC;     
                
        /*Enable DSI Sync all all inputs of the PWM*/
        BL_HBridge_RT1 &= ~(BL_HBridge_SYNCDSI_MASK);
        BL_HBridge_RT1 |= BL_HBridge_SYNCDSI_EN;
       
    #elif (BL_HBridge_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = BL_HBridge_CONTROL & ~BL_HBridge_CTRL_CMPMODE2_MASK & ~BL_HBridge_CTRL_CMPMODE1_MASK;
        BL_HBridge_CONTROL = ctrl | BL_HBridge_DEFAULT_COMPARE2_MODE | 
                                   BL_HBridge_DEFAULT_COMPARE1_MODE;
    #endif /* (BL_HBridge_UsingFixedFunction) */
        
    #if (!BL_HBridge_UsingFixedFunction)
        #if (BL_HBridge_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            BL_HBridge_AUX_CONTROLDP0 |= (BL_HBridge_AUX_CTRL_FIFO0_CLR);
        #else /* (BL_HBridge_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            BL_HBridge_AUX_CONTROLDP0 |= (BL_HBridge_AUX_CTRL_FIFO0_CLR);
            BL_HBridge_AUX_CONTROLDP1 |= (BL_HBridge_AUX_CTRL_FIFO0_CLR);
        #endif /* (BL_HBridge_Resolution == 8) */

        BL_HBridge_WriteCounter(BL_HBridge_INIT_PERIOD_VALUE);
    #endif /* (!BL_HBridge_UsingFixedFunction) */
        
    BL_HBridge_WritePeriod(BL_HBridge_INIT_PERIOD_VALUE);

        #if (BL_HBridge_UseOneCompareMode)
            BL_HBridge_WriteCompare(BL_HBridge_INIT_COMPARE_VALUE1);
        #else
            BL_HBridge_WriteCompare1(BL_HBridge_INIT_COMPARE_VALUE1);
            BL_HBridge_WriteCompare2(BL_HBridge_INIT_COMPARE_VALUE2);
        #endif /* (BL_HBridge_UseOneCompareMode) */
        
        #if (BL_HBridge_KillModeMinTime)
            BL_HBridge_WriteKillTime(BL_HBridge_MinimumKillTime);
        #endif /* (BL_HBridge_KillModeMinTime) */
        
        #if (BL_HBridge_DeadBandUsed)
            BL_HBridge_WriteDeadTime(BL_HBridge_INIT_DEAD_TIME);
        #endif /* (BL_HBridge_DeadBandUsed) */

    #if (BL_HBridge_UseStatus || BL_HBridge_UsingFixedFunction)
        BL_HBridge_SetInterruptMode(BL_HBridge_INIT_INTERRUPTS_MODE);
    #endif /* (BL_HBridge_UseStatus || BL_HBridge_UsingFixedFunction) */
        
    #if (BL_HBridge_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        BL_HBridge_GLOBAL_ENABLE |= BL_HBridge_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        BL_HBridge_CONTROL2 |= BL_HBridge_CTRL2_IRQ_SEL;
    #else
        #if(BL_HBridge_UseStatus)
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            BL_HBridge_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            BL_HBridge_STATUS_AUX_CTRL |= BL_HBridge_STATUS_ACTL_INT_EN_MASK;
            
             /* Exit Critical Region*/
            CyExitCriticalSection(BL_HBridge_interruptState);
            
            /* Clear the FIFO to enable the BL_HBridge_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            BL_HBridge_ClearFIFO();
        #endif /* (BL_HBridge_UseStatus) */
    #endif /* (BL_HBridge_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BL_HBridge_Enable
********************************************************************************
*
* Summary: 
*  Enables the PWM block operation
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Side Effects: 
*  This works only if software enable mode is chosen
*
*******************************************************************************/
void BL_HBridge_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (BL_HBridge_UsingFixedFunction)
        BL_HBridge_GLOBAL_ENABLE |= BL_HBridge_BLOCK_EN_MASK;
        BL_HBridge_GLOBAL_STBY_ENABLE |= BL_HBridge_BLOCK_STBY_EN_MASK;
    #endif /* (BL_HBridge_UsingFixedFunction) */
    
    /* Enable the PWM from the control register  */
    #if (BL_HBridge_UseControl || BL_HBridge_UsingFixedFunction)
        BL_HBridge_CONTROL |= BL_HBridge_CTRL_ENABLE;
    #endif /* (BL_HBridge_UseControl || BL_HBridge_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BL_HBridge_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the PWM, but does not change any modes or disable
*  interrupts.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects:
*  If the Enable mode is set to Hardware only then this function
*  has no effect on the operation of the PWM
*
*******************************************************************************/
void BL_HBridge_Stop(void) 
{
    #if (BL_HBridge_UseControl || BL_HBridge_UsingFixedFunction)
        BL_HBridge_CONTROL &= ~BL_HBridge_CTRL_ENABLE;
    #endif /* (BL_HBridge_UseControl || BL_HBridge_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (BL_HBridge_UsingFixedFunction)
        BL_HBridge_GLOBAL_ENABLE &= ~BL_HBridge_BLOCK_EN_MASK;
        BL_HBridge_GLOBAL_STBY_ENABLE &= ~BL_HBridge_BLOCK_STBY_EN_MASK;
    #endif /* (BL_HBridge_UsingFixedFunction) */
}


#if (BL_HBridge_UseOneCompareMode)
#if (BL_HBridge_CompareMode1SW)


/*******************************************************************************
* Function Name: BL_HBridge_SetCompareMode
********************************************************************************
* 
* Summary:
*  This function writes the Compare Mode for the pwm output when in Dither mode,
*  Center Align Mode or One Output Mode.
*
* Parameters:
*  comparemode:  The new compare mode for the PWM output. Use the compare types
*                defined in the H file as input arguments.
*
* Return:
*  void
*
*******************************************************************************/
void BL_HBridge_SetCompareMode(uint8 comparemode) 
{
    #if(BL_HBridge_UsingFixedFunction)
        #if(CY_PSOC5A)
            uint8 comparemodemasked = (comparemode << BL_HBridge_CTRL_CMPMODE1_SHIFT);
            BL_HBridge_CONTROL2 &= ~BL_HBridge_CTRL_CMPMODE1_MASK; /*Clear Existing Data */
            BL_HBridge_CONTROL2 |= comparemodemasked;  
        #endif /* (CY_PSOC5A) */
                
        #if(CY_PSOC3 || CY_PSOC5LP)
            uint8 comparemodemasked = (comparemode << BL_HBridge_CTRL_CMPMODE1_SHIFT);
            BL_HBridge_CONTROL3 &= ~BL_HBridge_CTRL_CMPMODE1_MASK; /*Clear Existing Data */
            BL_HBridge_CONTROL3 |= comparemodemasked;     
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
                
    #elif (BL_HBridge_UseControl)
        uint8 comparemode1masked = (comparemode << BL_HBridge_CTRL_CMPMODE1_SHIFT) & 
                                    BL_HBridge_CTRL_CMPMODE1_MASK;
        uint8 comparemode2masked = (comparemode << BL_HBridge_CTRL_CMPMODE2_SHIFT) & 
                                   BL_HBridge_CTRL_CMPMODE2_MASK;
        /*Clear existing mode */
        BL_HBridge_CONTROL &= ~(BL_HBridge_CTRL_CMPMODE1_MASK | BL_HBridge_CTRL_CMPMODE2_MASK); 
        BL_HBridge_CONTROL |= (comparemode1masked | comparemode2masked);
        
    #else
        uint8 temp = comparemode;
    #endif /* (BL_HBridge_UsingFixedFunction) */
}
#endif /* BL_HBridge_CompareMode1SW */

#else /* UseOneCompareMode */


#if (BL_HBridge_CompareMode1SW)


/*******************************************************************************
* Function Name: BL_HBridge_SetCompareMode1
********************************************************************************
* 
* Summary:
*  This function writes the Compare Mode for the pwm or pwm1 output
*
* Parameters:  
*  comparemode:  The new compare mode for the PWM output. Use the compare types
*                defined in the H file as input arguments.
*
* Return: 
*  void
*
*******************************************************************************/
void BL_HBridge_SetCompareMode1(uint8 comparemode) 
{
    uint8 comparemodemasked = (comparemode << BL_HBridge_CTRL_CMPMODE1_SHIFT) & 
                               BL_HBridge_CTRL_CMPMODE1_MASK;
    #if(BL_HBridge_UsingFixedFunction)
        #if(CY_PSOC5A)
            BL_HBridge_CONTROL2 &= BL_HBridge_CTRL_CMPMODE1_MASK; /*Clear existing mode */
            BL_HBridge_CONTROL2 |= comparemodemasked; 
        #endif /* (CY_PSOC5A) */
                
        #if(CY_PSOC3 || CY_PSOC5LP)
            BL_HBridge_CONTROL3 &= BL_HBridge_CTRL_CMPMODE1_MASK; /*Clear existing mode */
            BL_HBridge_CONTROL3 |= comparemodemasked; 
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
                
    #elif (BL_HBridge_UseControl)
        BL_HBridge_CONTROL &= BL_HBridge_CTRL_CMPMODE1_MASK; /*Clear existing mode */
        BL_HBridge_CONTROL |= comparemodemasked;
    #endif    /* (BL_HBridge_UsingFixedFunction) */
}
#endif /* BL_HBridge_CompareMode1SW */


#if (BL_HBridge_CompareMode2SW)


/*******************************************************************************
* Function Name: BL_HBridge_SetCompareMode2
********************************************************************************
* 
* Summary:
*  This function writes the Compare Mode for the pwm or pwm2 output
*
* Parameters:  
*  comparemode:  The new compare mode for the PWM output. Use the compare types
*                defined in the H file as input arguments.
*
* Return: 
*  void
*
*******************************************************************************/
void BL_HBridge_SetCompareMode2(uint8 comparemode) 
{
    #if(BL_HBridge_UsingFixedFunction)
        /* Do Nothing because there is no second Compare Mode Register in FF block */ 
    #elif (BL_HBridge_UseControl)
        uint8 comparemodemasked = (comparemode << BL_HBridge_CTRL_CMPMODE2_SHIFT) & 
                                             BL_HBridge_CTRL_CMPMODE2_MASK;
        BL_HBridge_CONTROL &= BL_HBridge_CTRL_CMPMODE2_MASK; /*Clear existing mode */
        BL_HBridge_CONTROL |= comparemodemasked;
    #endif /* (BL_HBridge_UsingFixedFunction) */
}
#endif /*BL_HBridge_CompareMode2SW */
#endif /* UseOneCompareMode */


#if (!BL_HBridge_UsingFixedFunction)


/*******************************************************************************
* Function Name: BL_HBridge_WriteCounter
********************************************************************************
* 
* Summary:
*  This function is used to change the counter value.
*
* Parameters:  
*  counter:  This value may be between 1 and (2^Resolution)-1.   
*
* Return: 
*  void
*
*******************************************************************************/
void BL_HBridge_WriteCounter(uint16 counter) \
                                   
{
    CY_SET_REG16(BL_HBridge_COUNTER_LSB_PTR, counter);
}

/*******************************************************************************
* Function Name: BL_HBridge_ReadCounter
********************************************************************************
* 
* Summary:
*  This function returns the current value of the counter.  It doesn't matter
*  if the counter is enabled or running.
*
* Parameters:  
*  void  
*
* Return: 
*  The current value of the counter.
*
*******************************************************************************/
uint16 BL_HBridge_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    CY_GET_REG8(BL_HBridge_COUNTERCAP_LSB_PTR);
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    return (CY_GET_REG16(BL_HBridge_CAPTURE_LSB_PTR));
}


#if (BL_HBridge_UseStatus)


/*******************************************************************************
* Function Name: BL_HBridge_ClearFIFO
********************************************************************************
* 
* Summary:
*  This function clears all capture data from the capture FIFO
*
* Parameters:  
*  void
*
* Return: 
*  void
*
*******************************************************************************/
void BL_HBridge_ClearFIFO(void) 
{
    while(BL_HBridge_ReadStatusRegister() & BL_HBridge_STATUS_FIFONEMPTY)
        BL_HBridge_ReadCapture();
}
#endif /* BL_HBridge_UseStatus */
#endif /* !BL_HBridge_UsingFixedFunction */


/*******************************************************************************
* Function Name: BL_HBridge_WritePeriod
********************************************************************************
* 
* Summary:
*  This function is used to change the period of the counter.  The new period 
*  will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period:  Period value. May be between 1 and (2^Resolution)-1.  A value of 0 
*           will result in the counter remaining at zero.
*
* Return: 
*  void
*
*******************************************************************************/
void BL_HBridge_WritePeriod(uint16 period) 
{
    #if(BL_HBridge_UsingFixedFunction)
        CY_SET_REG16(BL_HBridge_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG16(BL_HBridge_PERIOD_LSB_PTR, period);
    #endif /* (BL_HBridge_UsingFixedFunction) */
}


#if (BL_HBridge_UseOneCompareMode)


/*******************************************************************************
* Function Name: BL_HBridge_WriteCompare
********************************************************************************
* 
* Summary:
*  This funtion is used to change the compare1 value when the PWM is in Dither
*  mode. The compare output will reflect the new value on the next UDB clock. 
*  The compare output will be driven high when the present counter value is 
*  compared to the compare value based on the compare mode defined in 
*  Dither Mode.
*
* Parameters:  
*  compare:  New compare value.  
*
* Return: 
*  void
*
* Side Effects:
*  This function is only available if the PWM mode parameter is set to
*  Dither Mode, Center Aligned Mode or One Output Mode
*
*******************************************************************************/
void BL_HBridge_WriteCompare(uint16 compare) \
                                   
{
   CY_SET_REG16(BL_HBridge_COMPARE1_LSB_PTR, compare);
   #if (BL_HBridge_PWMMode == BL_HBridge__B_PWM__DITHER)
        CY_SET_REG16(BL_HBridge_COMPARE2_LSB_PTR, compare+1);
   #endif /* (BL_HBridge_PWMMode == BL_HBridge__B_PWM__DITHER) */
}


#else


/*******************************************************************************
* Function Name: BL_HBridge_WriteCompare1
********************************************************************************
* 
* Summary:
*  This funtion is used to change the compare1 value.  The compare output will 
*  reflect the new value on the next UDB clock.  The compare output will be 
*  driven high when the present counter value is less than or less than or 
*  equal to the compare register, depending on the mode.
*
* Parameters:  
*  compare:  New compare value.  
*
* Return: 
*  void
*
*******************************************************************************/
void BL_HBridge_WriteCompare1(uint16 compare) \
                                    
{
    #if(BL_HBridge_UsingFixedFunction)
        CY_SET_REG16(BL_HBridge_COMPARE1_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG16(BL_HBridge_COMPARE1_LSB_PTR, compare);
    #endif /* (BL_HBridge_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BL_HBridge_WriteCompare2
********************************************************************************
* 
* Summary:
*  This funtion is used to change the compare value, for compare1 output.  
*  The compare output will reflect the new value on the next UDB clock.  
*  The compare output will be driven high when the present counter value is 
*  less than or less than or equal to the compare register, depending on the 
*  mode.
*
* Parameters:  
*  compare:  New compare value.  
*
* Return: 
*  void
*
*******************************************************************************/
void BL_HBridge_WriteCompare2(uint16 compare) \
                                    
{
    #if(BL_HBridge_UsingFixedFunction)
        CY_SET_REG16(BL_HBridge_COMPARE2_LSB_PTR, compare);
    #else
        CY_SET_REG16(BL_HBridge_COMPARE2_LSB_PTR, compare);
    #endif /* (BL_HBridge_UsingFixedFunction) */
}
#endif /* UseOneCompareMode */


#if (BL_HBridge_DeadBandUsed)


/*******************************************************************************
* Function Name: BL_HBridge_WriteDeadTime
********************************************************************************
* 
* Summary:
*  This function writes the dead-band counts to the corresponding register
*
* Parameters:  
*  deadtime:  Number of counts for dead time 
*
* Return: 
*  void
*
*******************************************************************************/
void BL_HBridge_WriteDeadTime(uint8 deadtime) 
{
    /* If using the Dead Band 1-255 mode then just write the register */
    #if(!BL_HBridge_DeadBand2_4)
        CY_SET_REG8(BL_HBridge_DEADBAND_COUNT_PTR, deadtime);
    #else
        /* Otherwise the data has to be masked and offset */        
        /* Clear existing data */
        BL_HBridge_DEADBAND_COUNT &= ~BL_HBridge_DEADBAND_COUNT_MASK; 
            /* Set new dead time */
        BL_HBridge_DEADBAND_COUNT |= (deadtime << BL_HBridge_DEADBAND_COUNT_SHIFT) & 
                                            BL_HBridge_DEADBAND_COUNT_MASK; 
    #endif /* (!BL_HBridge_DeadBand2_4) */
}


/*******************************************************************************
* Function Name: BL_HBridge_ReadDeadTime
********************************************************************************
* 
* Summary:
*  This function reads the dead-band counts from the corresponding register
*
* Parameters:  
*  void
*
* Return: 
*  Dead Band Counts
*
*******************************************************************************/
uint8 BL_HBridge_ReadDeadTime(void) 
{
    /* If using the Dead Band 1-255 mode then just read the register */
    #if(!BL_HBridge_DeadBand2_4)
        return (CY_GET_REG8(BL_HBridge_DEADBAND_COUNT_PTR));
    #else
        /* Otherwise the data has to be masked and offset */
        return ((BL_HBridge_DEADBAND_COUNT & BL_HBridge_DEADBAND_COUNT_MASK) >> 
                 BL_HBridge_DEADBAND_COUNT_SHIFT);
    #endif /* (!BL_HBridge_DeadBand2_4) */
}
#endif /* DeadBandUsed */


/* [] END OF FILE */
