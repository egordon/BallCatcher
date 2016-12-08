/*******************************************************************************
* File Name: CountRise.c  
* Version 2.20
*
*  Description:
*     The Counter component consists of a 8, 16, 24 or 32-bit counter with
*     a selectable period between 2 and 2^Width - 1.  
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

uint8 CountRise_initVar = 0u;


/*******************************************************************************
* Function Name: CountRise_Init
********************************************************************************
* Summary:
*     Initialize to the schematic state
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void CountRise_Init(void) 
{
        #if (!CountRise_UsingFixedFunction && !CountRise_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!CountRise_UsingFixedFunction && !CountRise_ControlRegRemoved) */
        
        #if(!CountRise_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 CountRise_interruptState;
        #endif /* (!CountRise_UsingFixedFunction) */
        
        #if (CountRise_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            CountRise_CONTROL &= CountRise_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                CountRise_CONTROL2 &= ~CountRise_CTRL_MODE_MASK;
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                CountRise_CONTROL3 &= ~CountRise_CTRL_MODE_MASK;                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (CountRise_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                CountRise_CONTROL |= CountRise_ONESHOT;
            #endif /* (CountRise_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            CountRise_CONTROL2 |= CountRise_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            CountRise_RT1 &= ~CountRise_RT1_MASK;
            CountRise_RT1 |= CountRise_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            CountRise_RT1 &= ~(CountRise_SYNCDSI_MASK);
            CountRise_RT1 |= CountRise_SYNCDSI_EN;

        #else
            #if(!CountRise_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = CountRise_CONTROL & ~CountRise_CTRL_CMPMODE_MASK;
            CountRise_CONTROL = ctrl | CountRise_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = CountRise_CONTROL & ~CountRise_CTRL_CAPMODE_MASK;
            CountRise_CONTROL = ctrl | CountRise_DEFAULT_CAPTURE_MODE;
            #endif /* (!CountRise_ControlRegRemoved) */
        #endif /* (CountRise_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!CountRise_UsingFixedFunction)
            CountRise_ClearFIFO();
        #endif /* (!CountRise_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        CountRise_WritePeriod(CountRise_INIT_PERIOD_VALUE);
        #if (!(CountRise_UsingFixedFunction && (CY_PSOC5A)))
            CountRise_WriteCounter(CountRise_INIT_COUNTER_VALUE);
        #endif /* (!(CountRise_UsingFixedFunction && (CY_PSOC5A))) */
        CountRise_SetInterruptMode(CountRise_INIT_INTERRUPTS_MASK);
        
        #if (!CountRise_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            CountRise_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            CountRise_WriteCompare(CountRise_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            CountRise_interruptState = CyEnterCriticalSection();
            
            CountRise_STATUS_AUX_CTRL |= CountRise_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(CountRise_interruptState);
            
        #endif /* (!CountRise_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: CountRise_Enable
********************************************************************************
* Summary:
*     Enable the Counter
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: 
*   If the Enable mode is set to Hardware only then this function has no effect 
*   on the operation of the counter.
*
*******************************************************************************/
void CountRise_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (CountRise_UsingFixedFunction)
        CountRise_GLOBAL_ENABLE |= CountRise_BLOCK_EN_MASK;
        CountRise_GLOBAL_STBY_ENABLE |= CountRise_BLOCK_STBY_EN_MASK;
    #endif /* (CountRise_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!CountRise_ControlRegRemoved || CountRise_UsingFixedFunction)
        CountRise_CONTROL |= CountRise_CTRL_ENABLE;                
    #endif /* (!CountRise_ControlRegRemoved || CountRise_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: CountRise_Start
********************************************************************************
* Summary:
*  Enables the counter for operation 
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Global variables:
*  CountRise_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void CountRise_Start(void) 
{
    if(CountRise_initVar == 0u)
    {
        CountRise_Init();
        
        CountRise_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    CountRise_Enable();        
}


/*******************************************************************************
* Function Name: CountRise_Stop
********************************************************************************
* Summary:
* Halts the counter, but does not change any modes or disable interrupts.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the counter.
*
*******************************************************************************/
void CountRise_Stop(void) 
{
    /* Disable Counter */
    #if(!CountRise_ControlRegRemoved || CountRise_UsingFixedFunction)
        CountRise_CONTROL &= ~CountRise_CTRL_ENABLE;        
    #endif /* (!CountRise_ControlRegRemoved || CountRise_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (CountRise_UsingFixedFunction)
        CountRise_GLOBAL_ENABLE &= ~CountRise_BLOCK_EN_MASK;
        CountRise_GLOBAL_STBY_ENABLE &= ~CountRise_BLOCK_STBY_EN_MASK;
    #endif /* (CountRise_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: CountRise_SetInterruptMode
********************************************************************************
* Summary:
* Configures which interrupt sources are enabled to generate the final interrupt
*
* Parameters:  
*  InterruptsMask: This parameter is an or'd collection of the status bits
*                   which will be allowed to generate the counters interrupt.   
*
* Return: 
*  void
*
*******************************************************************************/
void CountRise_SetInterruptMode(uint8 interruptsMask) 
{
    CountRise_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: CountRise_ReadStatusRegister
********************************************************************************
* Summary:
*   Reads the status register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the status register
*
* Side Effects:
*   Status register bits may be clear on read. 
*
*******************************************************************************/
uint8   CountRise_ReadStatusRegister(void) 
{
    return CountRise_STATUS;
}


#if(!CountRise_ControlRegRemoved)
/*******************************************************************************
* Function Name: CountRise_ReadControlRegister
********************************************************************************
* Summary:
*   Reads the control register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
uint8   CountRise_ReadControlRegister(void) 
{
    return CountRise_CONTROL;
}


/*******************************************************************************
* Function Name: CountRise_WriteControlRegister
********************************************************************************
* Summary:
*   Sets the bit-field of the control register.  This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
void    CountRise_WriteControlRegister(uint8 control) 
{
    CountRise_CONTROL = control;
}

#endif  /* (!CountRise_ControlRegRemoved) */


#if (!(CountRise_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: CountRise_WriteCounter
********************************************************************************
* Summary:
*   This funtion is used to set the counter to a specific value
*
* Parameters:  
*  counter:  New counter value. 
*
* Return: 
*  void 
*
*******************************************************************************/
void CountRise_WriteCounter(uint16 counter) \
                                   
{
    #if(CountRise_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (!(CountRise_GLOBAL_ENABLE & CountRise_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        CountRise_GLOBAL_ENABLE |= CountRise_BLOCK_EN_MASK;
        CY_SET_REG16(CountRise_COUNTER_LSB_PTR, (uint16)counter);
        CountRise_GLOBAL_ENABLE &= ~CountRise_BLOCK_EN_MASK;
    #else
        CY_SET_REG16(CountRise_COUNTER_LSB_PTR, counter);
    #endif /* (CountRise_UsingFixedFunction) */
}
#endif /* (!(CountRise_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: CountRise_ReadCounter
********************************************************************************
* Summary:
* Returns the current value of the counter.  It doesn't matter
* if the counter is enabled or running.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint16) The present value of the counter.
*
*******************************************************************************/
uint16 CountRise_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    CY_GET_REG8(CountRise_COUNTER_LSB_PTR);
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    return (CY_GET_REG16(CountRise_STATICCOUNT_LSB_PTR));
}


/*******************************************************************************
* Function Name: CountRise_ReadCapture
********************************************************************************
* Summary:
*   This function returns the last value captured.
*
* Parameters:  
*  void
*
* Return: 
*  (uint16) Present Capture value.
*
*******************************************************************************/
uint16 CountRise_ReadCapture(void) 
{
   return ( CY_GET_REG16(CountRise_STATICCOUNT_LSB_PTR) );  
}


/*******************************************************************************
* Function Name: CountRise_WritePeriod
********************************************************************************
* Summary:
* Changes the period of the counter.  The new period 
* will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period: (uint16) A value of 0 will result in
*         the counter remaining at zero.  
*
* Return: 
*  void
*
*******************************************************************************/
void CountRise_WritePeriod(uint16 period) 
{
    #if(CountRise_UsingFixedFunction)
        CY_SET_REG16(CountRise_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG16(CountRise_PERIOD_LSB_PTR,period);
    #endif /* (CountRise_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: CountRise_ReadPeriod
********************************************************************************
* Summary:
* Reads the current period value without affecting counter operation.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint16) Present period value.
*
*******************************************************************************/
uint16 CountRise_ReadPeriod(void) 
{
   return ( CY_GET_REG16(CountRise_PERIOD_LSB_PTR));
}


#if (!CountRise_UsingFixedFunction)
/*******************************************************************************
* Function Name: CountRise_WriteCompare
********************************************************************************
* Summary:
* Changes the compare value.  The compare output will 
* reflect the new value on the next UDB clock.  The compare output will be 
* driven high when the present counter value compares true based on the 
* configured compare mode setting. 
*
* Parameters:  
*  Compare:  New compare value. 
*
* Return: 
*  void
*
*******************************************************************************/
void CountRise_WriteCompare(uint16 compare) \
                                   
{
    #if(CountRise_UsingFixedFunction)
        CY_SET_REG16(CountRise_COMPARE_LSB_PTR,(uint16)compare);
    #else
        CY_SET_REG16(CountRise_COMPARE_LSB_PTR,compare);
    #endif /* (CountRise_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: CountRise_ReadCompare
********************************************************************************
* Summary:
* Returns the compare value.
*
* Parameters:  
*  void:
*
* Return: 
*  (uint16) Present compare value.
*
*******************************************************************************/
uint16 CountRise_ReadCompare(void) 
{
   return ( CY_GET_REG16(CountRise_COMPARE_LSB_PTR));
}


#if (CountRise_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: CountRise_SetCompareMode
********************************************************************************
* Summary:
*  Sets the software controlled Compare Mode.
*
* Parameters:
*  compareMode:  Compare Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void CountRise_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    CountRise_CONTROL &= ~CountRise_CTRL_CMPMODE_MASK;
    
    /* Write the new setting */
    CountRise_CONTROL |= (compareMode << CountRise_CTRL_CMPMODE0_SHIFT);
}
#endif  /* (CountRise_COMPARE_MODE_SOFTWARE) */


#if (CountRise_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: CountRise_SetCaptureMode
********************************************************************************
* Summary:
*  Sets the software controlled Capture Mode.
*
* Parameters:
*  captureMode:  Capture Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void CountRise_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    CountRise_CONTROL &= ~CountRise_CTRL_CAPMODE_MASK;
    
    /* Write the new setting */
    CountRise_CONTROL |= (captureMode << CountRise_CTRL_CAPMODE0_SHIFT);
}
#endif  /* (CountRise_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: CountRise_ClearFIFO
********************************************************************************
* Summary:
*   This function clears all capture data from the capture FIFO
*
* Parameters:  
*  void:
*
* Return: 
*  None
*
*******************************************************************************/
void CountRise_ClearFIFO(void) 
{

    while(CountRise_ReadStatusRegister() & CountRise_STATUS_FIFONEMP)
    {
        CountRise_ReadCapture();
    }

}
#endif  /* (!CountRise_UsingFixedFunction) */


/* [] END OF FILE */

