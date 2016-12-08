/*******************************************************************************
* File Name: FL_HBridge.c  
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
#include "FL_HBridge.h"

uint8 FL_HBridge_initVar = 0u;


/*******************************************************************************
* Function Name: FL_HBridge_Start
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
*  FL_HBridge_initVar: Is modified when this function is called for the 
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void FL_HBridge_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(FL_HBridge_initVar == 0u)
    {
        FL_HBridge_Init();
        FL_HBridge_initVar = 1u;
    }
    FL_HBridge_Enable();

}


/*******************************************************************************
* Function Name: FL_HBridge_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  FL_HBridge_Start().
*
* Parameters:  
*  void
*
* Return: 
*  void
*
*******************************************************************************/
void FL_HBridge_Init(void) 
{
    #if (FL_HBridge_UsingFixedFunction || FL_HBridge_UseControl)
        uint8 ctrl;
    #endif /* (FL_HBridge_UsingFixedFunction || FL_HBridge_UseControl) */
    
    #if(!FL_HBridge_UsingFixedFunction) 
        #if(FL_HBridge_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 FL_HBridge_interruptState;
        #endif /* (FL_HBridge_UseStatus) */
    #endif /* (!FL_HBridge_UsingFixedFunction) */
    
    #if (FL_HBridge_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        FL_HBridge_CONTROL |= FL_HBridge_CFG0_MODE;
        #if (FL_HBridge_DeadBand2_4)
            FL_HBridge_CONTROL |= FL_HBridge_CFG0_DB;
        #endif /* (FL_HBridge_DeadBand2_4) */
                
        /* Set the default Compare Mode */
        #if(CY_PSOC5A)
                ctrl = FL_HBridge_CONTROL2 & ~FL_HBridge_CTRL_CMPMODE1_MASK;
                FL_HBridge_CONTROL2 = ctrl | FL_HBridge_DEFAULT_COMPARE1_MODE;
        #endif /* (CY_PSOC5A) */
        #if(CY_PSOC3 || CY_PSOC5LP)
                ctrl = FL_HBridge_CONTROL3 & ~FL_HBridge_CTRL_CMPMODE1_MASK;
                FL_HBridge_CONTROL3 = ctrl | FL_HBridge_DEFAULT_COMPARE1_MODE;
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
        
         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        FL_HBridge_RT1 &= ~FL_HBridge_RT1_MASK;
        FL_HBridge_RT1 |= FL_HBridge_SYNC;     
                
        /*Enable DSI Sync all all inputs of the PWM*/
        FL_HBridge_RT1 &= ~(FL_HBridge_SYNCDSI_MASK);
        FL_HBridge_RT1 |= FL_HBridge_SYNCDSI_EN;
       
    #elif (FL_HBridge_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = FL_HBridge_CONTROL & ~FL_HBridge_CTRL_CMPMODE2_MASK & ~FL_HBridge_CTRL_CMPMODE1_MASK;
        FL_HBridge_CONTROL = ctrl | FL_HBridge_DEFAULT_COMPARE2_MODE | 
                                   FL_HBridge_DEFAULT_COMPARE1_MODE;
    #endif /* (FL_HBridge_UsingFixedFunction) */
        
    #if (!FL_HBridge_UsingFixedFunction)
        #if (FL_HBridge_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            FL_HBridge_AUX_CONTROLDP0 |= (FL_HBridge_AUX_CTRL_FIFO0_CLR);
        #else /* (FL_HBridge_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            FL_HBridge_AUX_CONTROLDP0 |= (FL_HBridge_AUX_CTRL_FIFO0_CLR);
            FL_HBridge_AUX_CONTROLDP1 |= (FL_HBridge_AUX_CTRL_FIFO0_CLR);
        #endif /* (FL_HBridge_Resolution == 8) */

        FL_HBridge_WriteCounter(FL_HBridge_INIT_PERIOD_VALUE);
    #endif /* (!FL_HBridge_UsingFixedFunction) */
        
    FL_HBridge_WritePeriod(FL_HBridge_INIT_PERIOD_VALUE);

        #if (FL_HBridge_UseOneCompareMode)
            FL_HBridge_WriteCompare(FL_HBridge_INIT_COMPARE_VALUE1);
        #else
            FL_HBridge_WriteCompare1(FL_HBridge_INIT_COMPARE_VALUE1);
            FL_HBridge_WriteCompare2(FL_HBridge_INIT_COMPARE_VALUE2);
        #endif /* (FL_HBridge_UseOneCompareMode) */
        
        #if (FL_HBridge_KillModeMinTime)
            FL_HBridge_WriteKillTime(FL_HBridge_MinimumKillTime);
        #endif /* (FL_HBridge_KillModeMinTime) */
        
        #if (FL_HBridge_DeadBandUsed)
            FL_HBridge_WriteDeadTime(FL_HBridge_INIT_DEAD_TIME);
        #endif /* (FL_HBridge_DeadBandUsed) */

    #if (FL_HBridge_UseStatus || FL_HBridge_UsingFixedFunction)
        FL_HBridge_SetInterruptMode(FL_HBridge_INIT_INTERRUPTS_MODE);
    #endif /* (FL_HBridge_UseStatus || FL_HBridge_UsingFixedFunction) */
        
    #if (FL_HBridge_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        FL_HBridge_GLOBAL_ENABLE |= FL_HBridge_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        FL_HBridge_CONTROL2 |= FL_HBridge_CTRL2_IRQ_SEL;
    #else
        #if(FL_HBridge_UseStatus)
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            FL_HBridge_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            FL_HBridge_STATUS_AUX_CTRL |= FL_HBridge_STATUS_ACTL_INT_EN_MASK;
            
             /* Exit Critical Region*/
            CyExitCriticalSection(FL_HBridge_interruptState);
            
            /* Clear the FIFO to enable the FL_HBridge_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            FL_HBridge_ClearFIFO();
        #endif /* (FL_HBridge_UseStatus) */
    #endif /* (FL_HBridge_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: FL_HBridge_Enable
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
void FL_HBridge_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (FL_HBridge_UsingFixedFunction)
        FL_HBridge_GLOBAL_ENABLE |= FL_HBridge_BLOCK_EN_MASK;
        FL_HBridge_GLOBAL_STBY_ENABLE |= FL_HBridge_BLOCK_STBY_EN_MASK;
    #endif /* (FL_HBridge_UsingFixedFunction) */
    
    /* Enable the PWM from the control register  */
    #if (FL_HBridge_UseControl || FL_HBridge_UsingFixedFunction)
        FL_HBridge_CONTROL |= FL_HBridge_CTRL_ENABLE;
    #endif /* (FL_HBridge_UseControl || FL_HBridge_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: FL_HBridge_Stop
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
void FL_HBridge_Stop(void) 
{
    #if (FL_HBridge_UseControl || FL_HBridge_UsingFixedFunction)
        FL_HBridge_CONTROL &= ~FL_HBridge_CTRL_ENABLE;
    #endif /* (FL_HBridge_UseControl || FL_HBridge_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (FL_HBridge_UsingFixedFunction)
        FL_HBridge_GLOBAL_ENABLE &= ~FL_HBridge_BLOCK_EN_MASK;
        FL_HBridge_GLOBAL_STBY_ENABLE &= ~FL_HBridge_BLOCK_STBY_EN_MASK;
    #endif /* (FL_HBridge_UsingFixedFunction) */
}


#if (FL_HBridge_UseOneCompareMode)
#if (FL_HBridge_CompareMode1SW)


/*******************************************************************************
* Function Name: FL_HBridge_SetCompareMode
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
void FL_HBridge_SetCompareMode(uint8 comparemode) 
{
    #if(FL_HBridge_UsingFixedFunction)
        #if(CY_PSOC5A)
            uint8 comparemodemasked = (comparemode << FL_HBridge_CTRL_CMPMODE1_SHIFT);
            FL_HBridge_CONTROL2 &= ~FL_HBridge_CTRL_CMPMODE1_MASK; /*Clear Existing Data */
            FL_HBridge_CONTROL2 |= comparemodemasked;  
        #endif /* (CY_PSOC5A) */
                
        #if(CY_PSOC3 || CY_PSOC5LP)
            uint8 comparemodemasked = (comparemode << FL_HBridge_CTRL_CMPMODE1_SHIFT);
            FL_HBridge_CONTROL3 &= ~FL_HBridge_CTRL_CMPMODE1_MASK; /*Clear Existing Data */
            FL_HBridge_CONTROL3 |= comparemodemasked;     
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
                
    #elif (FL_HBridge_UseControl)
        uint8 comparemode1masked = (comparemode << FL_HBridge_CTRL_CMPMODE1_SHIFT) & 
                                    FL_HBridge_CTRL_CMPMODE1_MASK;
        uint8 comparemode2masked = (comparemode << FL_HBridge_CTRL_CMPMODE2_SHIFT) & 
                                   FL_HBridge_CTRL_CMPMODE2_MASK;
        /*Clear existing mode */
        FL_HBridge_CONTROL &= ~(FL_HBridge_CTRL_CMPMODE1_MASK | FL_HBridge_CTRL_CMPMODE2_MASK); 
        FL_HBridge_CONTROL |= (comparemode1masked | comparemode2masked);
        
    #else
        uint8 temp = comparemode;
    #endif /* (FL_HBridge_UsingFixedFunction) */
}
#endif /* FL_HBridge_CompareMode1SW */

#else /* UseOneCompareMode */


#if (FL_HBridge_CompareMode1SW)


/*******************************************************************************
* Function Name: FL_HBridge_SetCompareMode1
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
void FL_HBridge_SetCompareMode1(uint8 comparemode) 
{
    uint8 comparemodemasked = (comparemode << FL_HBridge_CTRL_CMPMODE1_SHIFT) & 
                               FL_HBridge_CTRL_CMPMODE1_MASK;
    #if(FL_HBridge_UsingFixedFunction)
        #if(CY_PSOC5A)
            FL_HBridge_CONTROL2 &= FL_HBridge_CTRL_CMPMODE1_MASK; /*Clear existing mode */
            FL_HBridge_CONTROL2 |= comparemodemasked; 
        #endif /* (CY_PSOC5A) */
                
        #if(CY_PSOC3 || CY_PSOC5LP)
            FL_HBridge_CONTROL3 &= FL_HBridge_CTRL_CMPMODE1_MASK; /*Clear existing mode */
            FL_HBridge_CONTROL3 |= comparemodemasked; 
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
                
    #elif (FL_HBridge_UseControl)
        FL_HBridge_CONTROL &= FL_HBridge_CTRL_CMPMODE1_MASK; /*Clear existing mode */
        FL_HBridge_CONTROL |= comparemodemasked;
    #endif    /* (FL_HBridge_UsingFixedFunction) */
}
#endif /* FL_HBridge_CompareMode1SW */


#if (FL_HBridge_CompareMode2SW)


/*******************************************************************************
* Function Name: FL_HBridge_SetCompareMode2
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
void FL_HBridge_SetCompareMode2(uint8 comparemode) 
{
    #if(FL_HBridge_UsingFixedFunction)
        /* Do Nothing because there is no second Compare Mode Register in FF block */ 
    #elif (FL_HBridge_UseControl)
        uint8 comparemodemasked = (comparemode << FL_HBridge_CTRL_CMPMODE2_SHIFT) & 
                                             FL_HBridge_CTRL_CMPMODE2_MASK;
        FL_HBridge_CONTROL &= FL_HBridge_CTRL_CMPMODE2_MASK; /*Clear existing mode */
        FL_HBridge_CONTROL |= comparemodemasked;
    #endif /* (FL_HBridge_UsingFixedFunction) */
}
#endif /*FL_HBridge_CompareMode2SW */
#endif /* UseOneCompareMode */


#if (!FL_HBridge_UsingFixedFunction)


/*******************************************************************************
* Function Name: FL_HBridge_WriteCounter
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
void FL_HBridge_WriteCounter(uint16 counter) \
                                   
{
    CY_SET_REG16(FL_HBridge_COUNTER_LSB_PTR, counter);
}

/*******************************************************************************
* Function Name: FL_HBridge_ReadCounter
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
uint16 FL_HBridge_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    CY_GET_REG8(FL_HBridge_COUNTERCAP_LSB_PTR);
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    return (CY_GET_REG16(FL_HBridge_CAPTURE_LSB_PTR));
}


#if (FL_HBridge_UseStatus)


/*******************************************************************************
* Function Name: FL_HBridge_ClearFIFO
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
void FL_HBridge_ClearFIFO(void) 
{
    while(FL_HBridge_ReadStatusRegister() & FL_HBridge_STATUS_FIFONEMPTY)
        FL_HBridge_ReadCapture();
}
#endif /* FL_HBridge_UseStatus */
#endif /* !FL_HBridge_UsingFixedFunction */


/*******************************************************************************
* Function Name: FL_HBridge_WritePeriod
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
void FL_HBridge_WritePeriod(uint16 period) 
{
    #if(FL_HBridge_UsingFixedFunction)
        CY_SET_REG16(FL_HBridge_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG16(FL_HBridge_PERIOD_LSB_PTR, period);
    #endif /* (FL_HBridge_UsingFixedFunction) */
}


#if (FL_HBridge_UseOneCompareMode)


/*******************************************************************************
* Function Name: FL_HBridge_WriteCompare
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
void FL_HBridge_WriteCompare(uint16 compare) \
                                   
{
   CY_SET_REG16(FL_HBridge_COMPARE1_LSB_PTR, compare);
   #if (FL_HBridge_PWMMode == FL_HBridge__B_PWM__DITHER)
        CY_SET_REG16(FL_HBridge_COMPARE2_LSB_PTR, compare+1);
   #endif /* (FL_HBridge_PWMMode == FL_HBridge__B_PWM__DITHER) */
}


#else


/*******************************************************************************
* Function Name: FL_HBridge_WriteCompare1
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
void FL_HBridge_WriteCompare1(uint16 compare) \
                                    
{
    #if(FL_HBridge_UsingFixedFunction)
        CY_SET_REG16(FL_HBridge_COMPARE1_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG16(FL_HBridge_COMPARE1_LSB_PTR, compare);
    #endif /* (FL_HBridge_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: FL_HBridge_WriteCompare2
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
void FL_HBridge_WriteCompare2(uint16 compare) \
                                    
{
    #if(FL_HBridge_UsingFixedFunction)
        CY_SET_REG16(FL_HBridge_COMPARE2_LSB_PTR, compare);
    #else
        CY_SET_REG16(FL_HBridge_COMPARE2_LSB_PTR, compare);
    #endif /* (FL_HBridge_UsingFixedFunction) */
}
#endif /* UseOneCompareMode */


#if (FL_HBridge_DeadBandUsed)


/*******************************************************************************
* Function Name: FL_HBridge_WriteDeadTime
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
void FL_HBridge_WriteDeadTime(uint8 deadtime) 
{
    /* If using the Dead Band 1-255 mode then just write the register */
    #if(!FL_HBridge_DeadBand2_4)
        CY_SET_REG8(FL_HBridge_DEADBAND_COUNT_PTR, deadtime);
    #else
        /* Otherwise the data has to be masked and offset */        
        /* Clear existing data */
        FL_HBridge_DEADBAND_COUNT &= ~FL_HBridge_DEADBAND_COUNT_MASK; 
            /* Set new dead time */
        FL_HBridge_DEADBAND_COUNT |= (deadtime << FL_HBridge_DEADBAND_COUNT_SHIFT) & 
                                            FL_HBridge_DEADBAND_COUNT_MASK; 
    #endif /* (!FL_HBridge_DeadBand2_4) */
}


/*******************************************************************************
* Function Name: FL_HBridge_ReadDeadTime
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
uint8 FL_HBridge_ReadDeadTime(void) 
{
    /* If using the Dead Band 1-255 mode then just read the register */
    #if(!FL_HBridge_DeadBand2_4)
        return (CY_GET_REG8(FL_HBridge_DEADBAND_COUNT_PTR));
    #else
        /* Otherwise the data has to be masked and offset */
        return ((FL_HBridge_DEADBAND_COUNT & FL_HBridge_DEADBAND_COUNT_MASK) >> 
                 FL_HBridge_DEADBAND_COUNT_SHIFT);
    #endif /* (!FL_HBridge_DeadBand2_4) */
}
#endif /* DeadBandUsed */


/* [] END OF FILE */
