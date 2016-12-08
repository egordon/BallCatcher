/*******************************************************************************
* File Name: AMux_Sel.c  
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
#include "AMux_Sel.h"

uint8 AMux_Sel_initVar = 0u;


/*******************************************************************************
* Function Name: AMux_Sel_Start
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
*  AMux_Sel_initVar: Is modified when this function is called for the 
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void AMux_Sel_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(AMux_Sel_initVar == 0u)
    {
        AMux_Sel_Init();
        AMux_Sel_initVar = 1u;
    }
    AMux_Sel_Enable();

}


/*******************************************************************************
* Function Name: AMux_Sel_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  AMux_Sel_Start().
*
* Parameters:  
*  void
*
* Return: 
*  void
*
*******************************************************************************/
void AMux_Sel_Init(void) 
{
    #if (AMux_Sel_UsingFixedFunction || AMux_Sel_UseControl)
        uint8 ctrl;
    #endif /* (AMux_Sel_UsingFixedFunction || AMux_Sel_UseControl) */
    
    #if(!AMux_Sel_UsingFixedFunction) 
        #if(AMux_Sel_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 AMux_Sel_interruptState;
        #endif /* (AMux_Sel_UseStatus) */
    #endif /* (!AMux_Sel_UsingFixedFunction) */
    
    #if (AMux_Sel_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        AMux_Sel_CONTROL |= AMux_Sel_CFG0_MODE;
        #if (AMux_Sel_DeadBand2_4)
            AMux_Sel_CONTROL |= AMux_Sel_CFG0_DB;
        #endif /* (AMux_Sel_DeadBand2_4) */
                
        /* Set the default Compare Mode */
        #if(CY_PSOC5A)
                ctrl = AMux_Sel_CONTROL2 & ~AMux_Sel_CTRL_CMPMODE1_MASK;
                AMux_Sel_CONTROL2 = ctrl | AMux_Sel_DEFAULT_COMPARE1_MODE;
        #endif /* (CY_PSOC5A) */
        #if(CY_PSOC3 || CY_PSOC5LP)
                ctrl = AMux_Sel_CONTROL3 & ~AMux_Sel_CTRL_CMPMODE1_MASK;
                AMux_Sel_CONTROL3 = ctrl | AMux_Sel_DEFAULT_COMPARE1_MODE;
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
        
         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        AMux_Sel_RT1 &= ~AMux_Sel_RT1_MASK;
        AMux_Sel_RT1 |= AMux_Sel_SYNC;     
                
        /*Enable DSI Sync all all inputs of the PWM*/
        AMux_Sel_RT1 &= ~(AMux_Sel_SYNCDSI_MASK);
        AMux_Sel_RT1 |= AMux_Sel_SYNCDSI_EN;
       
    #elif (AMux_Sel_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = AMux_Sel_CONTROL & ~AMux_Sel_CTRL_CMPMODE2_MASK & ~AMux_Sel_CTRL_CMPMODE1_MASK;
        AMux_Sel_CONTROL = ctrl | AMux_Sel_DEFAULT_COMPARE2_MODE | 
                                   AMux_Sel_DEFAULT_COMPARE1_MODE;
    #endif /* (AMux_Sel_UsingFixedFunction) */
        
    #if (!AMux_Sel_UsingFixedFunction)
        #if (AMux_Sel_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            AMux_Sel_AUX_CONTROLDP0 |= (AMux_Sel_AUX_CTRL_FIFO0_CLR);
        #else /* (AMux_Sel_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            AMux_Sel_AUX_CONTROLDP0 |= (AMux_Sel_AUX_CTRL_FIFO0_CLR);
            AMux_Sel_AUX_CONTROLDP1 |= (AMux_Sel_AUX_CTRL_FIFO0_CLR);
        #endif /* (AMux_Sel_Resolution == 8) */

        AMux_Sel_WriteCounter(AMux_Sel_INIT_PERIOD_VALUE);
    #endif /* (!AMux_Sel_UsingFixedFunction) */
        
    AMux_Sel_WritePeriod(AMux_Sel_INIT_PERIOD_VALUE);

        #if (AMux_Sel_UseOneCompareMode)
            AMux_Sel_WriteCompare(AMux_Sel_INIT_COMPARE_VALUE1);
        #else
            AMux_Sel_WriteCompare1(AMux_Sel_INIT_COMPARE_VALUE1);
            AMux_Sel_WriteCompare2(AMux_Sel_INIT_COMPARE_VALUE2);
        #endif /* (AMux_Sel_UseOneCompareMode) */
        
        #if (AMux_Sel_KillModeMinTime)
            AMux_Sel_WriteKillTime(AMux_Sel_MinimumKillTime);
        #endif /* (AMux_Sel_KillModeMinTime) */
        
        #if (AMux_Sel_DeadBandUsed)
            AMux_Sel_WriteDeadTime(AMux_Sel_INIT_DEAD_TIME);
        #endif /* (AMux_Sel_DeadBandUsed) */

    #if (AMux_Sel_UseStatus || AMux_Sel_UsingFixedFunction)
        AMux_Sel_SetInterruptMode(AMux_Sel_INIT_INTERRUPTS_MODE);
    #endif /* (AMux_Sel_UseStatus || AMux_Sel_UsingFixedFunction) */
        
    #if (AMux_Sel_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        AMux_Sel_GLOBAL_ENABLE |= AMux_Sel_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        AMux_Sel_CONTROL2 |= AMux_Sel_CTRL2_IRQ_SEL;
    #else
        #if(AMux_Sel_UseStatus)
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            AMux_Sel_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            AMux_Sel_STATUS_AUX_CTRL |= AMux_Sel_STATUS_ACTL_INT_EN_MASK;
            
             /* Exit Critical Region*/
            CyExitCriticalSection(AMux_Sel_interruptState);
            
            /* Clear the FIFO to enable the AMux_Sel_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            AMux_Sel_ClearFIFO();
        #endif /* (AMux_Sel_UseStatus) */
    #endif /* (AMux_Sel_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: AMux_Sel_Enable
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
void AMux_Sel_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (AMux_Sel_UsingFixedFunction)
        AMux_Sel_GLOBAL_ENABLE |= AMux_Sel_BLOCK_EN_MASK;
        AMux_Sel_GLOBAL_STBY_ENABLE |= AMux_Sel_BLOCK_STBY_EN_MASK;
    #endif /* (AMux_Sel_UsingFixedFunction) */
    
    /* Enable the PWM from the control register  */
    #if (AMux_Sel_UseControl || AMux_Sel_UsingFixedFunction)
        AMux_Sel_CONTROL |= AMux_Sel_CTRL_ENABLE;
    #endif /* (AMux_Sel_UseControl || AMux_Sel_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: AMux_Sel_Stop
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
void AMux_Sel_Stop(void) 
{
    #if (AMux_Sel_UseControl || AMux_Sel_UsingFixedFunction)
        AMux_Sel_CONTROL &= ~AMux_Sel_CTRL_ENABLE;
    #endif /* (AMux_Sel_UseControl || AMux_Sel_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (AMux_Sel_UsingFixedFunction)
        AMux_Sel_GLOBAL_ENABLE &= ~AMux_Sel_BLOCK_EN_MASK;
        AMux_Sel_GLOBAL_STBY_ENABLE &= ~AMux_Sel_BLOCK_STBY_EN_MASK;
    #endif /* (AMux_Sel_UsingFixedFunction) */
}


#if (AMux_Sel_UseOneCompareMode)
#if (AMux_Sel_CompareMode1SW)


/*******************************************************************************
* Function Name: AMux_Sel_SetCompareMode
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
void AMux_Sel_SetCompareMode(uint8 comparemode) 
{
    #if(AMux_Sel_UsingFixedFunction)
        #if(CY_PSOC5A)
            uint8 comparemodemasked = (comparemode << AMux_Sel_CTRL_CMPMODE1_SHIFT);
            AMux_Sel_CONTROL2 &= ~AMux_Sel_CTRL_CMPMODE1_MASK; /*Clear Existing Data */
            AMux_Sel_CONTROL2 |= comparemodemasked;  
        #endif /* (CY_PSOC5A) */
                
        #if(CY_PSOC3 || CY_PSOC5LP)
            uint8 comparemodemasked = (comparemode << AMux_Sel_CTRL_CMPMODE1_SHIFT);
            AMux_Sel_CONTROL3 &= ~AMux_Sel_CTRL_CMPMODE1_MASK; /*Clear Existing Data */
            AMux_Sel_CONTROL3 |= comparemodemasked;     
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
                
    #elif (AMux_Sel_UseControl)
        uint8 comparemode1masked = (comparemode << AMux_Sel_CTRL_CMPMODE1_SHIFT) & 
                                    AMux_Sel_CTRL_CMPMODE1_MASK;
        uint8 comparemode2masked = (comparemode << AMux_Sel_CTRL_CMPMODE2_SHIFT) & 
                                   AMux_Sel_CTRL_CMPMODE2_MASK;
        /*Clear existing mode */
        AMux_Sel_CONTROL &= ~(AMux_Sel_CTRL_CMPMODE1_MASK | AMux_Sel_CTRL_CMPMODE2_MASK); 
        AMux_Sel_CONTROL |= (comparemode1masked | comparemode2masked);
        
    #else
        uint8 temp = comparemode;
    #endif /* (AMux_Sel_UsingFixedFunction) */
}
#endif /* AMux_Sel_CompareMode1SW */

#else /* UseOneCompareMode */


#if (AMux_Sel_CompareMode1SW)


/*******************************************************************************
* Function Name: AMux_Sel_SetCompareMode1
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
void AMux_Sel_SetCompareMode1(uint8 comparemode) 
{
    uint8 comparemodemasked = (comparemode << AMux_Sel_CTRL_CMPMODE1_SHIFT) & 
                               AMux_Sel_CTRL_CMPMODE1_MASK;
    #if(AMux_Sel_UsingFixedFunction)
        #if(CY_PSOC5A)
            AMux_Sel_CONTROL2 &= AMux_Sel_CTRL_CMPMODE1_MASK; /*Clear existing mode */
            AMux_Sel_CONTROL2 |= comparemodemasked; 
        #endif /* (CY_PSOC5A) */
                
        #if(CY_PSOC3 || CY_PSOC5LP)
            AMux_Sel_CONTROL3 &= AMux_Sel_CTRL_CMPMODE1_MASK; /*Clear existing mode */
            AMux_Sel_CONTROL3 |= comparemodemasked; 
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
                
    #elif (AMux_Sel_UseControl)
        AMux_Sel_CONTROL &= AMux_Sel_CTRL_CMPMODE1_MASK; /*Clear existing mode */
        AMux_Sel_CONTROL |= comparemodemasked;
    #endif    /* (AMux_Sel_UsingFixedFunction) */
}
#endif /* AMux_Sel_CompareMode1SW */


#if (AMux_Sel_CompareMode2SW)


/*******************************************************************************
* Function Name: AMux_Sel_SetCompareMode2
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
void AMux_Sel_SetCompareMode2(uint8 comparemode) 
{
    #if(AMux_Sel_UsingFixedFunction)
        /* Do Nothing because there is no second Compare Mode Register in FF block */ 
    #elif (AMux_Sel_UseControl)
        uint8 comparemodemasked = (comparemode << AMux_Sel_CTRL_CMPMODE2_SHIFT) & 
                                             AMux_Sel_CTRL_CMPMODE2_MASK;
        AMux_Sel_CONTROL &= AMux_Sel_CTRL_CMPMODE2_MASK; /*Clear existing mode */
        AMux_Sel_CONTROL |= comparemodemasked;
    #endif /* (AMux_Sel_UsingFixedFunction) */
}
#endif /*AMux_Sel_CompareMode2SW */
#endif /* UseOneCompareMode */


#if (!AMux_Sel_UsingFixedFunction)


/*******************************************************************************
* Function Name: AMux_Sel_WriteCounter
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
void AMux_Sel_WriteCounter(uint8 counter) \
                                   
{
    CY_SET_REG8(AMux_Sel_COUNTER_LSB_PTR, counter);
}

/*******************************************************************************
* Function Name: AMux_Sel_ReadCounter
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
uint8 AMux_Sel_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    CY_GET_REG8(AMux_Sel_COUNTERCAP_LSB_PTR);
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    return (CY_GET_REG8(AMux_Sel_CAPTURE_LSB_PTR));
}


#if (AMux_Sel_UseStatus)


/*******************************************************************************
* Function Name: AMux_Sel_ClearFIFO
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
void AMux_Sel_ClearFIFO(void) 
{
    while(AMux_Sel_ReadStatusRegister() & AMux_Sel_STATUS_FIFONEMPTY)
        AMux_Sel_ReadCapture();
}
#endif /* AMux_Sel_UseStatus */
#endif /* !AMux_Sel_UsingFixedFunction */


/*******************************************************************************
* Function Name: AMux_Sel_WritePeriod
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
void AMux_Sel_WritePeriod(uint8 period) 
{
    #if(AMux_Sel_UsingFixedFunction)
        CY_SET_REG16(AMux_Sel_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG8(AMux_Sel_PERIOD_LSB_PTR, period);
    #endif /* (AMux_Sel_UsingFixedFunction) */
}


#if (AMux_Sel_UseOneCompareMode)


/*******************************************************************************
* Function Name: AMux_Sel_WriteCompare
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
void AMux_Sel_WriteCompare(uint8 compare) \
                                   
{
   CY_SET_REG8(AMux_Sel_COMPARE1_LSB_PTR, compare);
   #if (AMux_Sel_PWMMode == AMux_Sel__B_PWM__DITHER)
        CY_SET_REG8(AMux_Sel_COMPARE2_LSB_PTR, compare+1);
   #endif /* (AMux_Sel_PWMMode == AMux_Sel__B_PWM__DITHER) */
}


#else


/*******************************************************************************
* Function Name: AMux_Sel_WriteCompare1
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
void AMux_Sel_WriteCompare1(uint8 compare) \
                                    
{
    #if(AMux_Sel_UsingFixedFunction)
        CY_SET_REG16(AMux_Sel_COMPARE1_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG8(AMux_Sel_COMPARE1_LSB_PTR, compare);
    #endif /* (AMux_Sel_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: AMux_Sel_WriteCompare2
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
void AMux_Sel_WriteCompare2(uint8 compare) \
                                    
{
    #if(AMux_Sel_UsingFixedFunction)
        CY_SET_REG16(AMux_Sel_COMPARE2_LSB_PTR, compare);
    #else
        CY_SET_REG8(AMux_Sel_COMPARE2_LSB_PTR, compare);
    #endif /* (AMux_Sel_UsingFixedFunction) */
}
#endif /* UseOneCompareMode */


#if (AMux_Sel_DeadBandUsed)


/*******************************************************************************
* Function Name: AMux_Sel_WriteDeadTime
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
void AMux_Sel_WriteDeadTime(uint8 deadtime) 
{
    /* If using the Dead Band 1-255 mode then just write the register */
    #if(!AMux_Sel_DeadBand2_4)
        CY_SET_REG8(AMux_Sel_DEADBAND_COUNT_PTR, deadtime);
    #else
        /* Otherwise the data has to be masked and offset */        
        /* Clear existing data */
        AMux_Sel_DEADBAND_COUNT &= ~AMux_Sel_DEADBAND_COUNT_MASK; 
            /* Set new dead time */
        AMux_Sel_DEADBAND_COUNT |= (deadtime << AMux_Sel_DEADBAND_COUNT_SHIFT) & 
                                            AMux_Sel_DEADBAND_COUNT_MASK; 
    #endif /* (!AMux_Sel_DeadBand2_4) */
}


/*******************************************************************************
* Function Name: AMux_Sel_ReadDeadTime
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
uint8 AMux_Sel_ReadDeadTime(void) 
{
    /* If using the Dead Band 1-255 mode then just read the register */
    #if(!AMux_Sel_DeadBand2_4)
        return (CY_GET_REG8(AMux_Sel_DEADBAND_COUNT_PTR));
    #else
        /* Otherwise the data has to be masked and offset */
        return ((AMux_Sel_DEADBAND_COUNT & AMux_Sel_DEADBAND_COUNT_MASK) >> 
                 AMux_Sel_DEADBAND_COUNT_SHIFT);
    #endif /* (!AMux_Sel_DeadBand2_4) */
}
#endif /* DeadBandUsed */


/* [] END OF FILE */
