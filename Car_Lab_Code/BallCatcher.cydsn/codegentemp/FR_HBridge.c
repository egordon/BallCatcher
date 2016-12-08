/*******************************************************************************
* File Name: FR_HBridge.c  
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
#include "FR_HBridge.h"

uint8 FR_HBridge_initVar = 0u;


/*******************************************************************************
* Function Name: FR_HBridge_Start
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
*  FR_HBridge_initVar: Is modified when this function is called for the 
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void FR_HBridge_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(FR_HBridge_initVar == 0u)
    {
        FR_HBridge_Init();
        FR_HBridge_initVar = 1u;
    }
    FR_HBridge_Enable();

}


/*******************************************************************************
* Function Name: FR_HBridge_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  FR_HBridge_Start().
*
* Parameters:  
*  void
*
* Return: 
*  void
*
*******************************************************************************/
void FR_HBridge_Init(void) 
{
    #if (FR_HBridge_UsingFixedFunction || FR_HBridge_UseControl)
        uint8 ctrl;
    #endif /* (FR_HBridge_UsingFixedFunction || FR_HBridge_UseControl) */
    
    #if(!FR_HBridge_UsingFixedFunction) 
        #if(FR_HBridge_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 FR_HBridge_interruptState;
        #endif /* (FR_HBridge_UseStatus) */
    #endif /* (!FR_HBridge_UsingFixedFunction) */
    
    #if (FR_HBridge_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        FR_HBridge_CONTROL |= FR_HBridge_CFG0_MODE;
        #if (FR_HBridge_DeadBand2_4)
            FR_HBridge_CONTROL |= FR_HBridge_CFG0_DB;
        #endif /* (FR_HBridge_DeadBand2_4) */
                
        /* Set the default Compare Mode */
        #if(CY_PSOC5A)
                ctrl = FR_HBridge_CONTROL2 & ~FR_HBridge_CTRL_CMPMODE1_MASK;
                FR_HBridge_CONTROL2 = ctrl | FR_HBridge_DEFAULT_COMPARE1_MODE;
        #endif /* (CY_PSOC5A) */
        #if(CY_PSOC3 || CY_PSOC5LP)
                ctrl = FR_HBridge_CONTROL3 & ~FR_HBridge_CTRL_CMPMODE1_MASK;
                FR_HBridge_CONTROL3 = ctrl | FR_HBridge_DEFAULT_COMPARE1_MODE;
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
        
         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        FR_HBridge_RT1 &= ~FR_HBridge_RT1_MASK;
        FR_HBridge_RT1 |= FR_HBridge_SYNC;     
                
        /*Enable DSI Sync all all inputs of the PWM*/
        FR_HBridge_RT1 &= ~(FR_HBridge_SYNCDSI_MASK);
        FR_HBridge_RT1 |= FR_HBridge_SYNCDSI_EN;
       
    #elif (FR_HBridge_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = FR_HBridge_CONTROL & ~FR_HBridge_CTRL_CMPMODE2_MASK & ~FR_HBridge_CTRL_CMPMODE1_MASK;
        FR_HBridge_CONTROL = ctrl | FR_HBridge_DEFAULT_COMPARE2_MODE | 
                                   FR_HBridge_DEFAULT_COMPARE1_MODE;
    #endif /* (FR_HBridge_UsingFixedFunction) */
        
    #if (!FR_HBridge_UsingFixedFunction)
        #if (FR_HBridge_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            FR_HBridge_AUX_CONTROLDP0 |= (FR_HBridge_AUX_CTRL_FIFO0_CLR);
        #else /* (FR_HBridge_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            FR_HBridge_AUX_CONTROLDP0 |= (FR_HBridge_AUX_CTRL_FIFO0_CLR);
            FR_HBridge_AUX_CONTROLDP1 |= (FR_HBridge_AUX_CTRL_FIFO0_CLR);
        #endif /* (FR_HBridge_Resolution == 8) */

        FR_HBridge_WriteCounter(FR_HBridge_INIT_PERIOD_VALUE);
    #endif /* (!FR_HBridge_UsingFixedFunction) */
        
    FR_HBridge_WritePeriod(FR_HBridge_INIT_PERIOD_VALUE);

        #if (FR_HBridge_UseOneCompareMode)
            FR_HBridge_WriteCompare(FR_HBridge_INIT_COMPARE_VALUE1);
        #else
            FR_HBridge_WriteCompare1(FR_HBridge_INIT_COMPARE_VALUE1);
            FR_HBridge_WriteCompare2(FR_HBridge_INIT_COMPARE_VALUE2);
        #endif /* (FR_HBridge_UseOneCompareMode) */
        
        #if (FR_HBridge_KillModeMinTime)
            FR_HBridge_WriteKillTime(FR_HBridge_MinimumKillTime);
        #endif /* (FR_HBridge_KillModeMinTime) */
        
        #if (FR_HBridge_DeadBandUsed)
            FR_HBridge_WriteDeadTime(FR_HBridge_INIT_DEAD_TIME);
        #endif /* (FR_HBridge_DeadBandUsed) */

    #if (FR_HBridge_UseStatus || FR_HBridge_UsingFixedFunction)
        FR_HBridge_SetInterruptMode(FR_HBridge_INIT_INTERRUPTS_MODE);
    #endif /* (FR_HBridge_UseStatus || FR_HBridge_UsingFixedFunction) */
        
    #if (FR_HBridge_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        FR_HBridge_GLOBAL_ENABLE |= FR_HBridge_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        FR_HBridge_CONTROL2 |= FR_HBridge_CTRL2_IRQ_SEL;
    #else
        #if(FR_HBridge_UseStatus)
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            FR_HBridge_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            FR_HBridge_STATUS_AUX_CTRL |= FR_HBridge_STATUS_ACTL_INT_EN_MASK;
            
             /* Exit Critical Region*/
            CyExitCriticalSection(FR_HBridge_interruptState);
            
            /* Clear the FIFO to enable the FR_HBridge_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            FR_HBridge_ClearFIFO();
        #endif /* (FR_HBridge_UseStatus) */
    #endif /* (FR_HBridge_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: FR_HBridge_Enable
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
void FR_HBridge_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (FR_HBridge_UsingFixedFunction)
        FR_HBridge_GLOBAL_ENABLE |= FR_HBridge_BLOCK_EN_MASK;
        FR_HBridge_GLOBAL_STBY_ENABLE |= FR_HBridge_BLOCK_STBY_EN_MASK;
    #endif /* (FR_HBridge_UsingFixedFunction) */
    
    /* Enable the PWM from the control register  */
    #if (FR_HBridge_UseControl || FR_HBridge_UsingFixedFunction)
        FR_HBridge_CONTROL |= FR_HBridge_CTRL_ENABLE;
    #endif /* (FR_HBridge_UseControl || FR_HBridge_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: FR_HBridge_Stop
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
void FR_HBridge_Stop(void) 
{
    #if (FR_HBridge_UseControl || FR_HBridge_UsingFixedFunction)
        FR_HBridge_CONTROL &= ~FR_HBridge_CTRL_ENABLE;
    #endif /* (FR_HBridge_UseControl || FR_HBridge_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (FR_HBridge_UsingFixedFunction)
        FR_HBridge_GLOBAL_ENABLE &= ~FR_HBridge_BLOCK_EN_MASK;
        FR_HBridge_GLOBAL_STBY_ENABLE &= ~FR_HBridge_BLOCK_STBY_EN_MASK;
    #endif /* (FR_HBridge_UsingFixedFunction) */
}


#if (FR_HBridge_UseOneCompareMode)
#if (FR_HBridge_CompareMode1SW)


/*******************************************************************************
* Function Name: FR_HBridge_SetCompareMode
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
void FR_HBridge_SetCompareMode(uint8 comparemode) 
{
    #if(FR_HBridge_UsingFixedFunction)
        #if(CY_PSOC5A)
            uint8 comparemodemasked = (comparemode << FR_HBridge_CTRL_CMPMODE1_SHIFT);
            FR_HBridge_CONTROL2 &= ~FR_HBridge_CTRL_CMPMODE1_MASK; /*Clear Existing Data */
            FR_HBridge_CONTROL2 |= comparemodemasked;  
        #endif /* (CY_PSOC5A) */
                
        #if(CY_PSOC3 || CY_PSOC5LP)
            uint8 comparemodemasked = (comparemode << FR_HBridge_CTRL_CMPMODE1_SHIFT);
            FR_HBridge_CONTROL3 &= ~FR_HBridge_CTRL_CMPMODE1_MASK; /*Clear Existing Data */
            FR_HBridge_CONTROL3 |= comparemodemasked;     
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
                
    #elif (FR_HBridge_UseControl)
        uint8 comparemode1masked = (comparemode << FR_HBridge_CTRL_CMPMODE1_SHIFT) & 
                                    FR_HBridge_CTRL_CMPMODE1_MASK;
        uint8 comparemode2masked = (comparemode << FR_HBridge_CTRL_CMPMODE2_SHIFT) & 
                                   FR_HBridge_CTRL_CMPMODE2_MASK;
        /*Clear existing mode */
        FR_HBridge_CONTROL &= ~(FR_HBridge_CTRL_CMPMODE1_MASK | FR_HBridge_CTRL_CMPMODE2_MASK); 
        FR_HBridge_CONTROL |= (comparemode1masked | comparemode2masked);
        
    #else
        uint8 temp = comparemode;
    #endif /* (FR_HBridge_UsingFixedFunction) */
}
#endif /* FR_HBridge_CompareMode1SW */

#else /* UseOneCompareMode */


#if (FR_HBridge_CompareMode1SW)


/*******************************************************************************
* Function Name: FR_HBridge_SetCompareMode1
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
void FR_HBridge_SetCompareMode1(uint8 comparemode) 
{
    uint8 comparemodemasked = (comparemode << FR_HBridge_CTRL_CMPMODE1_SHIFT) & 
                               FR_HBridge_CTRL_CMPMODE1_MASK;
    #if(FR_HBridge_UsingFixedFunction)
        #if(CY_PSOC5A)
            FR_HBridge_CONTROL2 &= FR_HBridge_CTRL_CMPMODE1_MASK; /*Clear existing mode */
            FR_HBridge_CONTROL2 |= comparemodemasked; 
        #endif /* (CY_PSOC5A) */
                
        #if(CY_PSOC3 || CY_PSOC5LP)
            FR_HBridge_CONTROL3 &= FR_HBridge_CTRL_CMPMODE1_MASK; /*Clear existing mode */
            FR_HBridge_CONTROL3 |= comparemodemasked; 
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
                
    #elif (FR_HBridge_UseControl)
        FR_HBridge_CONTROL &= FR_HBridge_CTRL_CMPMODE1_MASK; /*Clear existing mode */
        FR_HBridge_CONTROL |= comparemodemasked;
    #endif    /* (FR_HBridge_UsingFixedFunction) */
}
#endif /* FR_HBridge_CompareMode1SW */


#if (FR_HBridge_CompareMode2SW)


/*******************************************************************************
* Function Name: FR_HBridge_SetCompareMode2
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
void FR_HBridge_SetCompareMode2(uint8 comparemode) 
{
    #if(FR_HBridge_UsingFixedFunction)
        /* Do Nothing because there is no second Compare Mode Register in FF block */ 
    #elif (FR_HBridge_UseControl)
        uint8 comparemodemasked = (comparemode << FR_HBridge_CTRL_CMPMODE2_SHIFT) & 
                                             FR_HBridge_CTRL_CMPMODE2_MASK;
        FR_HBridge_CONTROL &= FR_HBridge_CTRL_CMPMODE2_MASK; /*Clear existing mode */
        FR_HBridge_CONTROL |= comparemodemasked;
    #endif /* (FR_HBridge_UsingFixedFunction) */
}
#endif /*FR_HBridge_CompareMode2SW */
#endif /* UseOneCompareMode */


#if (!FR_HBridge_UsingFixedFunction)


/*******************************************************************************
* Function Name: FR_HBridge_WriteCounter
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
void FR_HBridge_WriteCounter(uint16 counter) \
                                   
{
    CY_SET_REG16(FR_HBridge_COUNTER_LSB_PTR, counter);
}

/*******************************************************************************
* Function Name: FR_HBridge_ReadCounter
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
uint16 FR_HBridge_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    CY_GET_REG8(FR_HBridge_COUNTERCAP_LSB_PTR);
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    return (CY_GET_REG16(FR_HBridge_CAPTURE_LSB_PTR));
}


#if (FR_HBridge_UseStatus)


/*******************************************************************************
* Function Name: FR_HBridge_ClearFIFO
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
void FR_HBridge_ClearFIFO(void) 
{
    while(FR_HBridge_ReadStatusRegister() & FR_HBridge_STATUS_FIFONEMPTY)
        FR_HBridge_ReadCapture();
}
#endif /* FR_HBridge_UseStatus */
#endif /* !FR_HBridge_UsingFixedFunction */


/*******************************************************************************
* Function Name: FR_HBridge_WritePeriod
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
void FR_HBridge_WritePeriod(uint16 period) 
{
    #if(FR_HBridge_UsingFixedFunction)
        CY_SET_REG16(FR_HBridge_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG16(FR_HBridge_PERIOD_LSB_PTR, period);
    #endif /* (FR_HBridge_UsingFixedFunction) */
}


#if (FR_HBridge_UseOneCompareMode)


/*******************************************************************************
* Function Name: FR_HBridge_WriteCompare
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
void FR_HBridge_WriteCompare(uint16 compare) \
                                   
{
   CY_SET_REG16(FR_HBridge_COMPARE1_LSB_PTR, compare);
   #if (FR_HBridge_PWMMode == FR_HBridge__B_PWM__DITHER)
        CY_SET_REG16(FR_HBridge_COMPARE2_LSB_PTR, compare+1);
   #endif /* (FR_HBridge_PWMMode == FR_HBridge__B_PWM__DITHER) */
}


#else


/*******************************************************************************
* Function Name: FR_HBridge_WriteCompare1
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
void FR_HBridge_WriteCompare1(uint16 compare) \
                                    
{
    #if(FR_HBridge_UsingFixedFunction)
        CY_SET_REG16(FR_HBridge_COMPARE1_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG16(FR_HBridge_COMPARE1_LSB_PTR, compare);
    #endif /* (FR_HBridge_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: FR_HBridge_WriteCompare2
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
void FR_HBridge_WriteCompare2(uint16 compare) \
                                    
{
    #if(FR_HBridge_UsingFixedFunction)
        CY_SET_REG16(FR_HBridge_COMPARE2_LSB_PTR, compare);
    #else
        CY_SET_REG16(FR_HBridge_COMPARE2_LSB_PTR, compare);
    #endif /* (FR_HBridge_UsingFixedFunction) */
}
#endif /* UseOneCompareMode */


#if (FR_HBridge_DeadBandUsed)


/*******************************************************************************
* Function Name: FR_HBridge_WriteDeadTime
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
void FR_HBridge_WriteDeadTime(uint8 deadtime) 
{
    /* If using the Dead Band 1-255 mode then just write the register */
    #if(!FR_HBridge_DeadBand2_4)
        CY_SET_REG8(FR_HBridge_DEADBAND_COUNT_PTR, deadtime);
    #else
        /* Otherwise the data has to be masked and offset */        
        /* Clear existing data */
        FR_HBridge_DEADBAND_COUNT &= ~FR_HBridge_DEADBAND_COUNT_MASK; 
            /* Set new dead time */
        FR_HBridge_DEADBAND_COUNT |= (deadtime << FR_HBridge_DEADBAND_COUNT_SHIFT) & 
                                            FR_HBridge_DEADBAND_COUNT_MASK; 
    #endif /* (!FR_HBridge_DeadBand2_4) */
}


/*******************************************************************************
* Function Name: FR_HBridge_ReadDeadTime
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
uint8 FR_HBridge_ReadDeadTime(void) 
{
    /* If using the Dead Band 1-255 mode then just read the register */
    #if(!FR_HBridge_DeadBand2_4)
        return (CY_GET_REG8(FR_HBridge_DEADBAND_COUNT_PTR));
    #else
        /* Otherwise the data has to be masked and offset */
        return ((FR_HBridge_DEADBAND_COUNT & FR_HBridge_DEADBAND_COUNT_MASK) >> 
                 FR_HBridge_DEADBAND_COUNT_SHIFT);
    #endif /* (!FR_HBridge_DeadBand2_4) */
}
#endif /* DeadBandUsed */


/* [] END OF FILE */
