/*******************************************************************************
* File Name: CountFall.c  
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

#include "CountFall.h"

uint8 CountFall_initVar = 0u;


/*******************************************************************************
* Function Name: CountFall_Init
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
void CountFall_Init(void) 
{
        #if (!CountFall_UsingFixedFunction && !CountFall_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!CountFall_UsingFixedFunction && !CountFall_ControlRegRemoved) */
        
        #if(!CountFall_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 CountFall_interruptState;
        #endif /* (!CountFall_UsingFixedFunction) */
        
        #if (CountFall_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            CountFall_CONTROL &= CountFall_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                CountFall_CONTROL2 &= ~CountFall_CTRL_MODE_MASK;
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                CountFall_CONTROL3 &= ~CountFall_CTRL_MODE_MASK;                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (CountFall_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                CountFall_CONTROL |= CountFall_ONESHOT;
            #endif /* (CountFall_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            CountFall_CONTROL2 |= CountFall_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            CountFall_RT1 &= ~CountFall_RT1_MASK;
            CountFall_RT1 |= CountFall_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            CountFall_RT1 &= ~(CountFall_SYNCDSI_MASK);
            CountFall_RT1 |= CountFall_SYNCDSI_EN;

        #else
            #if(!CountFall_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = CountFall_CONTROL & ~CountFall_CTRL_CMPMODE_MASK;
            CountFall_CONTROL = ctrl | CountFall_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = CountFall_CONTROL & ~CountFall_CTRL_CAPMODE_MASK;
            CountFall_CONTROL = ctrl | CountFall_DEFAULT_CAPTURE_MODE;
            #endif /* (!CountFall_ControlRegRemoved) */
        #endif /* (CountFall_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!CountFall_UsingFixedFunction)
            CountFall_ClearFIFO();
        #endif /* (!CountFall_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        CountFall_WritePeriod(CountFall_INIT_PERIOD_VALUE);
        #if (!(CountFall_UsingFixedFunction && (CY_PSOC5A)))
            CountFall_WriteCounter(CountFall_INIT_COUNTER_VALUE);
        #endif /* (!(CountFall_UsingFixedFunction && (CY_PSOC5A))) */
        CountFall_SetInterruptMode(CountFall_INIT_INTERRUPTS_MASK);
        
        #if (!CountFall_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            CountFall_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            CountFall_WriteCompare(CountFall_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            CountFall_interruptState = CyEnterCriticalSection();
            
            CountFall_STATUS_AUX_CTRL |= CountFall_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(CountFall_interruptState);
            
        #endif /* (!CountFall_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: CountFall_Enable
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
void CountFall_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (CountFall_UsingFixedFunction)
        CountFall_GLOBAL_ENABLE |= CountFall_BLOCK_EN_MASK;
        CountFall_GLOBAL_STBY_ENABLE |= CountFall_BLOCK_STBY_EN_MASK;
    #endif /* (CountFall_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!CountFall_ControlRegRemoved || CountFall_UsingFixedFunction)
        CountFall_CONTROL |= CountFall_CTRL_ENABLE;                
    #endif /* (!CountFall_ControlRegRemoved || CountFall_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: CountFall_Start
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
*  CountFall_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void CountFall_Start(void) 
{
    if(CountFall_initVar == 0u)
    {
        CountFall_Init();
        
        CountFall_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    CountFall_Enable();        
}


/*******************************************************************************
* Function Name: CountFall_Stop
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
void CountFall_Stop(void) 
{
    /* Disable Counter */
    #if(!CountFall_ControlRegRemoved || CountFall_UsingFixedFunction)
        CountFall_CONTROL &= ~CountFall_CTRL_ENABLE;        
    #endif /* (!CountFall_ControlRegRemoved || CountFall_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (CountFall_UsingFixedFunction)
        CountFall_GLOBAL_ENABLE &= ~CountFall_BLOCK_EN_MASK;
        CountFall_GLOBAL_STBY_ENABLE &= ~CountFall_BLOCK_STBY_EN_MASK;
    #endif /* (CountFall_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: CountFall_SetInterruptMode
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
void CountFall_SetInterruptMode(uint8 interruptsMask) 
{
    CountFall_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: CountFall_ReadStatusRegister
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
uint8   CountFall_ReadStatusRegister(void) 
{
    return CountFall_STATUS;
}


#if(!CountFall_ControlRegRemoved)
/*******************************************************************************
* Function Name: CountFall_ReadControlRegister
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
uint8   CountFall_ReadControlRegister(void) 
{
    return CountFall_CONTROL;
}


/*******************************************************************************
* Function Name: CountFall_WriteControlRegister
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
void    CountFall_WriteControlRegister(uint8 control) 
{
    CountFall_CONTROL = control;
}

#endif  /* (!CountFall_ControlRegRemoved) */


#if (!(CountFall_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: CountFall_WriteCounter
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
void CountFall_WriteCounter(uint16 counter) \
                                   
{
    #if(CountFall_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (!(CountFall_GLOBAL_ENABLE & CountFall_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        CountFall_GLOBAL_ENABLE |= CountFall_BLOCK_EN_MASK;
        CY_SET_REG16(CountFall_COUNTER_LSB_PTR, (uint16)counter);
        CountFall_GLOBAL_ENABLE &= ~CountFall_BLOCK_EN_MASK;
    #else
        CY_SET_REG16(CountFall_COUNTER_LSB_PTR, counter);
    #endif /* (CountFall_UsingFixedFunction) */
}
#endif /* (!(CountFall_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: CountFall_ReadCounter
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
uint16 CountFall_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    CY_GET_REG8(CountFall_COUNTER_LSB_PTR);
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    return (CY_GET_REG16(CountFall_STATICCOUNT_LSB_PTR));
}


/*******************************************************************************
* Function Name: CountFall_ReadCapture
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
uint16 CountFall_ReadCapture(void) 
{
   return ( CY_GET_REG16(CountFall_STATICCOUNT_LSB_PTR) );  
}


/*******************************************************************************
* Function Name: CountFall_WritePeriod
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
void CountFall_WritePeriod(uint16 period) 
{
    #if(CountFall_UsingFixedFunction)
        CY_SET_REG16(CountFall_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG16(CountFall_PERIOD_LSB_PTR,period);
    #endif /* (CountFall_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: CountFall_ReadPeriod
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
uint16 CountFall_ReadPeriod(void) 
{
   return ( CY_GET_REG16(CountFall_PERIOD_LSB_PTR));
}


#if (!CountFall_UsingFixedFunction)
/*******************************************************************************
* Function Name: CountFall_WriteCompare
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
void CountFall_WriteCompare(uint16 compare) \
                                   
{
    #if(CountFall_UsingFixedFunction)
        CY_SET_REG16(CountFall_COMPARE_LSB_PTR,(uint16)compare);
    #else
        CY_SET_REG16(CountFall_COMPARE_LSB_PTR,compare);
    #endif /* (CountFall_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: CountFall_ReadCompare
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
uint16 CountFall_ReadCompare(void) 
{
   return ( CY_GET_REG16(CountFall_COMPARE_LSB_PTR));
}


#if (CountFall_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: CountFall_SetCompareMode
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
void CountFall_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    CountFall_CONTROL &= ~CountFall_CTRL_CMPMODE_MASK;
    
    /* Write the new setting */
    CountFall_CONTROL |= (compareMode << CountFall_CTRL_CMPMODE0_SHIFT);
}
#endif  /* (CountFall_COMPARE_MODE_SOFTWARE) */


#if (CountFall_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: CountFall_SetCaptureMode
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
void CountFall_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    CountFall_CONTROL &= ~CountFall_CTRL_CAPMODE_MASK;
    
    /* Write the new setting */
    CountFall_CONTROL |= (captureMode << CountFall_CTRL_CAPMODE0_SHIFT);
}
#endif  /* (CountFall_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: CountFall_ClearFIFO
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
void CountFall_ClearFIFO(void) 
{

    while(CountFall_ReadStatusRegister() & CountFall_STATUS_FIFONEMP)
    {
        CountFall_ReadCapture();
    }

}
#endif  /* (!CountFall_UsingFixedFunction) */


/* [] END OF FILE */

