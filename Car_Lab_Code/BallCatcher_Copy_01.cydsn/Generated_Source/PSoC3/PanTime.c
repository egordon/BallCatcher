/*******************************************************************************
* File Name: PanTime.c
* Version 2.30
*
* Description:
*  The Timer component consists of a 8, 16, 24 or 32-bit timer with
*  a selectable period between 2 and 2^Width - 1.  The timer may free run
*  or be used as a capture timer as well.  The capture can be initiated
*  by a positive or negative edge signal as well as via software.
*  A trigger input can be programmed to enable the timer on rising edge
*  falling edge, either edge or continous run.
*  Interrupts may be generated due to a terminal count condition
*  or a capture event.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "PanTime.h"

uint8 PanTime_initVar = 0u;


/*******************************************************************************
* Function Name: PanTime_Init
********************************************************************************
*
* Summary:
*  Initialize to the schematic state
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void PanTime_Init(void) 
{
    #if(!PanTime_UsingFixedFunction)
            /* Interrupt State Backup for Critical Region*/
            uint8 PanTime_interruptState;
    #endif /* Interrupt state back up for Fixed Function only */

    #if (PanTime_UsingFixedFunction)
        /* Clear all bits but the enable bit (if it's already set) for Timer operation */
        PanTime_CONTROL &= PanTime_CTRL_ENABLE;

        /* Clear the mode bits for continuous run mode */
        #if (CY_PSOC5A)
            PanTime_CONTROL2 &= ~PanTime_CTRL_MODE_MASK;
        #endif /* Clear bits in CONTROL2 only in PSOC5A */

        #if (CY_PSOC3 || CY_PSOC5LP)
            PanTime_CONTROL3 &= ~PanTime_CTRL_MODE_MASK;
        #endif /* CONTROL3 register exists only in PSoC3 OR PSoC5LP */

        /* Check if One Shot mode is enabled i.e. RunMode !=0*/
        #if (PanTime_RunModeUsed != 0x0u)
            /* Set 3rd bit of Control register to enable one shot mode */
            PanTime_CONTROL |= 0x04u;
        #endif /* One Shot enabled only when RunModeUsed is not Continuous*/

        #if (PanTime_RunModeUsed == 2)
            #if (CY_PSOC5A)
                /* Set last 2 bits of control2 register if one shot(halt on
                interrupt) is enabled*/
                PanTime_CONTROL2 |= 0x03u;
            #endif /* Set One-Shot Halt on Interrupt bit in CONTROL2 for PSoC5A */

            #if (CY_PSOC3 || CY_PSOC5LP)
                /* Set last 2 bits of control3 register if one shot(halt on
                interrupt) is enabled*/
                PanTime_CONTROL3 |= 0x03u;
            #endif /* Set One-Shot Halt on Interrupt bit in CONTROL3 for PSoC3 or PSoC5LP */

        #endif /* Remove section if One Shot Halt on Interrupt is not enabled */

        #if (PanTime_UsingHWEnable != 0)
            #if (CY_PSOC5A)
                /* Set the default Run Mode of the Timer to Continuous */
                PanTime_CONTROL2 |= PanTime_CTRL_MODE_PULSEWIDTH;
            #endif /* Set Continuous Run Mode in CONTROL2 for PSoC5A */

            #if (CY_PSOC3 || CY_PSOC5LP)
                /* Clear and Set ROD and COD bits of CFG2 register */
                PanTime_CONTROL3 &= ~PanTime_CTRL_RCOD_MASK;
                PanTime_CONTROL3 |= PanTime_CTRL_RCOD;

                /* Clear and Enable the HW enable bit in CFG2 register */
                PanTime_CONTROL3 &= ~PanTime_CTRL_ENBL_MASK;
                PanTime_CONTROL3 |= PanTime_CTRL_ENBL;

                /* Set the default Run Mode of the Timer to Continuous */
                PanTime_CONTROL3 |= PanTime_CTRL_MODE_CONTINUOUS;
            #endif /* Set Continuous Run Mode in CONTROL3 for PSoC3ES3 or PSoC5A */

        #endif /* Configure Run Mode with hardware enable */

        /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        PanTime_RT1 &= ~PanTime_RT1_MASK;
        PanTime_RT1 |= PanTime_SYNC;

        /*Enable DSI Sync all all inputs of the Timer*/
        PanTime_RT1 &= ~(PanTime_SYNCDSI_MASK);
        PanTime_RT1 |= PanTime_SYNCDSI_EN;

        /* Set the IRQ to use the status register interrupts */
        PanTime_CONTROL2 |= PanTime_CTRL2_IRQ_SEL;
    #endif /* Configuring registers of fixed function implementation */

    /* Set Initial values from Configuration */
    PanTime_WritePeriod(PanTime_INIT_PERIOD);
    PanTime_WriteCounter(PanTime_INIT_PERIOD);

    #if (PanTime_UsingHWCaptureCounter)/* Capture counter is enabled */
        PanTime_CAPTURE_COUNT_CTRL |= PanTime_CNTR_ENABLE;
        PanTime_SetCaptureCount(PanTime_INIT_CAPTURE_COUNT);
    #endif /* Configure capture counter value */

    #if (!PanTime_UsingFixedFunction)
        #if (PanTime_SoftwareCaptureMode)
            PanTime_SetCaptureMode(PanTime_INIT_CAPTURE_MODE);
        #endif /* Set Capture Mode for UDB implementation if capture mode is software controlled */

        #if (PanTime_SoftwareTriggerMode)
            if (!(PanTime_CONTROL & PanTime__B_TIMER__TM_SOFTWARE))
            {
                PanTime_SetTriggerMode(PanTime_INIT_TRIGGER_MODE);
            }
        #endif /* Set trigger mode for UDB Implementation if trigger mode is software controlled */

        /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
        /* Enter Critical Region*/
        PanTime_interruptState = CyEnterCriticalSection();

        /* Use the interrupt output of the status register for IRQ output */
        PanTime_STATUS_AUX_CTRL |= PanTime_STATUS_ACTL_INT_EN_MASK;

        /* Exit Critical Region*/
        CyExitCriticalSection(PanTime_interruptState);

        #if (PanTime_EnableTriggerMode)
            PanTime_EnableTrigger();
        #endif /* Set Trigger enable bit for UDB implementation in the control register*/

        #if (PanTime_InterruptOnCaptureCount)
             #if (!PanTime_ControlRegRemoved)
                PanTime_SetInterruptCount(PanTime_INIT_INT_CAPTURE_COUNT);
            #endif /* Set interrupt count in control register if control register is not removed */
        #endif /*Set interrupt count in UDB implementation if interrupt count feature is checked.*/

        PanTime_ClearFIFO();
    #endif /* Configure additional features of UDB implementation */

    PanTime_SetInterruptMode(PanTime_INIT_INTERRUPT_MODE);
}


/*******************************************************************************
* Function Name: PanTime_Enable
********************************************************************************
*
* Summary:
*  Enable the Timer
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void PanTime_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (PanTime_UsingFixedFunction)
        PanTime_GLOBAL_ENABLE |= PanTime_BLOCK_EN_MASK;
        PanTime_GLOBAL_STBY_ENABLE |= PanTime_BLOCK_STBY_EN_MASK;
    #endif /* Set Enable bit for enabling Fixed function timer*/

    /* Remove assignment if control register is removed */
    #if (!PanTime_ControlRegRemoved || PanTime_UsingFixedFunction)
        PanTime_CONTROL |= PanTime_CTRL_ENABLE;
    #endif /* Remove assignment if control register is removed */
}


/*******************************************************************************
* Function Name: PanTime_Start
********************************************************************************
*
* Summary:
*  The start function initializes the timer with the default values, the
*  enables the timerto begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  PanTime_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void PanTime_Start(void) 
{
    if(PanTime_initVar == 0u)
    {
        PanTime_Init();

        PanTime_initVar = 1u;   /* Clear this bit for Initialization */
    }

    /* Enable the Timer */
    PanTime_Enable();
}


/*******************************************************************************
* Function Name: PanTime_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the timer, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the timer.
*
*******************************************************************************/
void PanTime_Stop(void) 
{
    /* Disable Timer */
    #if(!PanTime_ControlRegRemoved || PanTime_UsingFixedFunction)
        PanTime_CONTROL &= ~PanTime_CTRL_ENABLE;
    #endif /* Remove assignment if control register is removed */

    /* Globally disable the Fixed Function Block chosen */
    #if (PanTime_UsingFixedFunction)
        PanTime_GLOBAL_ENABLE &= ~PanTime_BLOCK_EN_MASK;
        PanTime_GLOBAL_STBY_ENABLE &= ~PanTime_BLOCK_STBY_EN_MASK;
    #endif /* Disable global enable for the Timer Fixed function block to stop the Timer*/
}


/*******************************************************************************
* Function Name: PanTime_SetInterruptMode
********************************************************************************
*
* Summary:
*  This function selects which of the interrupt inputs may cause an interrupt.
*  The twosources are caputure and terminal.  One, both or neither may
*  be selected.
*
* Parameters:
*  interruptMode:   This parameter is used to enable interrups on either/or
*                   terminal count or capture.
*
* Return:
*  void
*
*******************************************************************************/
void PanTime_SetInterruptMode(uint8 interruptMode) 
{
    PanTime_STATUS_MASK = interruptMode;
}


/*******************************************************************************
* Function Name: PanTime_SoftwareCapture
********************************************************************************
*
* Summary:
*  This function forces a capture independent of the capture signal.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Side Effects:
*  An existing hardware capture could be overwritten.
*
*******************************************************************************/
void PanTime_SoftwareCapture(void) 
{
    /* Generate a software capture by reading the counter register */
    CY_GET_REG8(PanTime_COUNTER_LSB_PTR);
    /* Capture Data is now in the FIFO */
}


/*******************************************************************************
* Function Name: PanTime_ReadStatusRegister
********************************************************************************
*
* Summary:
*  Reads the status register and returns it's state. This function should use
*  defined types for the bit-field information as the bits in this register may
*  be permuteable.
*
* Parameters:
*  void
*
* Return:
*  The contents of the status register
*
* Side Effects:
*  Status register bits may be clear on read.
*
*******************************************************************************/
uint8   PanTime_ReadStatusRegister(void) 
{
    return PanTime_STATUS;
}


#if (!PanTime_ControlRegRemoved) /* Remove API if control register is removed */


/*******************************************************************************
* Function Name: PanTime_ReadControlRegister
********************************************************************************
*
* Summary:
*  Reads the control register and returns it's value.
*
* Parameters:
*  void
*
* Return:
*  The contents of the control register
*
*******************************************************************************/
uint8 PanTime_ReadControlRegister(void) 
{
    return PanTime_CONTROL;
}


/*******************************************************************************
* Function Name: PanTime_WriteControlRegister
********************************************************************************
*
* Summary:
*  Sets the bit-field of the control register.
*
* Parameters:
*  control: The contents of the control register
*
* Return:
*
*******************************************************************************/
void PanTime_WriteControlRegister(uint8 control) 
{
    PanTime_CONTROL = control;
}
#endif /* Remove API if control register is removed */


/*******************************************************************************
* Function Name: PanTime_ReadPeriod
********************************************************************************
*
* Summary:
*  This function returns the current value of the Period.
*
* Parameters:
*  void
*
* Return:
*  The present value of the counter.
*
*******************************************************************************/
uint16 PanTime_ReadPeriod(void) 
{
   return ( CY_GET_REG16(PanTime_PERIOD_LSB_PTR) );
}


/*******************************************************************************
* Function Name: PanTime_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period: This value may be between 1 and (2^Resolution)-1.  A value of 0 will
*          result in the counter remaining at zero.
*
* Return:
*  void
*
*******************************************************************************/
void PanTime_WritePeriod(uint16 period) 
{
    #if(PanTime_UsingFixedFunction)
        uint16 period_temp = (uint16)period;
        CY_SET_REG16(PanTime_PERIOD_LSB_PTR, period_temp);
    #else
        CY_SET_REG16(PanTime_PERIOD_LSB_PTR, period);
    #endif /*Write Period value with appropriate resolution suffix depending on UDB or fixed function implementation */
}


/*******************************************************************************
* Function Name: PanTime_ReadCapture
********************************************************************************
*
* Summary:
*  This function returns the last value captured.
*
* Parameters:
*  void
*
* Return:
*  Present Capture value.
*
*******************************************************************************/
uint16 PanTime_ReadCapture(void) 
{
   return ( CY_GET_REG16(PanTime_CAPTURE_LSB_PTR) );
}


/*******************************************************************************
* Function Name: PanTime_WriteCounter
********************************************************************************
*
* Summary:
*  This funtion is used to set the counter to a specific value
*
* Parameters:
*  counter:  New counter value.
*
* Return:
*  void
*
*******************************************************************************/
void PanTime_WriteCounter(uint16 counter) \
                                   
{
   #if(PanTime_UsingFixedFunction)
        counter = counter;
        /* This functionality is removed until a FixedFunction HW update to
         * allow this register to be written
         */
        /* uint16 counter_temp = (uint16)counter;
         * CY_SET_REG16(PanTime_COUNTER_LSB_PTR, counter_temp);
         */
    #else
        CY_SET_REG16(PanTime_COUNTER_LSB_PTR, counter);
    #endif /* Set Write Counter only for the UDB implementation (Write Counter not available in fixed function Timer */
}


/*******************************************************************************
* Function Name: PanTime_ReadCounter
********************************************************************************
*
* Summary:
*  This function returns the current counter value.
*
* Parameters:
*  void
*
* Return:
*  Present compare value.
*
*******************************************************************************/
uint16 PanTime_ReadCounter(void) 
{

    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    CY_GET_REG8(PanTime_COUNTER_LSB_PTR);

    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    return (CY_GET_REG16(PanTime_CAPTURE_LSB_PTR));
}


#if(!PanTime_UsingFixedFunction) /* UDB Specific Functions */

/*******************************************************************************
 * The functions below this point are only available using the UDB
 * implementation.  If a feature is selected, then the API is enabled.
 ******************************************************************************/


#if (PanTime_SoftwareCaptureMode)


/*******************************************************************************
* Function Name: PanTime_SetCaptureMode
********************************************************************************
*
* Summary:
*  This function sets the capture mode to either rising or falling edge.
*
* Parameters:
*  captureMode: This parameter sets the capture mode of the UDB capture feature
*  The parameter values are defined using the
*  #define PanTime__B_TIMER__CM_NONE 0
#define PanTime__B_TIMER__CM_RISINGEDGE 1
#define PanTime__B_TIMER__CM_FALLINGEDGE 2
#define PanTime__B_TIMER__CM_EITHEREDGE 3
#define PanTime__B_TIMER__CM_SOFTWARE 4
 identifiers
*  The following are the possible values of the parameter
*  PanTime__B_TIMER__CM_NONE        - Set Capture mode to None
*  PanTime__B_TIMER__CM_RISINGEDGE  - Rising edge of Capture input
*  PanTime__B_TIMER__CM_FALLINGEDGE - Falling edge of Capture input
*  PanTime__B_TIMER__CM_EITHEREDGE  - Either edge of Capture input
*
* Return:
*  void
*
*******************************************************************************/
void PanTime_SetCaptureMode(uint8 captureMode) 
{
    /* This must only set to two bits of the control register associated */
    captureMode = (captureMode << PanTime_CTRL_CAP_MODE_SHIFT);
    captureMode &= (PanTime_CTRL_CAP_MODE_MASK);

    /* Clear the Current Setting */
    PanTime_CONTROL &= ~PanTime_CTRL_CAP_MODE_MASK;

    /* Write The New Setting */
    PanTime_CONTROL |= captureMode;
}
#endif /* Remove API if Capture Mode is not Software Controlled */


#if (PanTime_SoftwareTriggerMode)


/*******************************************************************************
* Function Name: PanTime_SetTriggerMode
********************************************************************************
*
* Summary:
*  This function sets the trigger input mode
*
* Parameters:
*  triggerMode: Pass one of the pre-defined Trigger Modes (except Software)
    #define PanTime__B_TIMER__TM_NONE 0x00u
    #define PanTime__B_TIMER__TM_RISINGEDGE 0x04u
    #define PanTime__B_TIMER__TM_FALLINGEDGE 0x08u
    #define PanTime__B_TIMER__TM_EITHEREDGE 0x0Cu
    #define PanTime__B_TIMER__TM_SOFTWARE 0x10u
*
* Return:
*  void
*
*******************************************************************************/
void PanTime_SetTriggerMode(uint8 triggerMode) 
{
    /* This must only set to two bits of the control register associated */
    triggerMode &= PanTime_CTRL_TRIG_MODE_MASK;

    /* Clear the Current Setting */
    PanTime_CONTROL &= ~PanTime_CTRL_TRIG_MODE_MASK;

    /* Write The New Setting */
    PanTime_CONTROL |= (triggerMode | PanTime__B_TIMER__TM_SOFTWARE);

}
#endif /* Remove API if Trigger Mode is not Software Controlled */

#if (PanTime_EnableTriggerMode)


/*******************************************************************************
* Function Name: PanTime_EnableTrigger
********************************************************************************
*
* Summary:
*  Sets the control bit enabling Hardware Trigger mode
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void PanTime_EnableTrigger(void) 
{
    #if (!PanTime_ControlRegRemoved)   /* Remove assignment if control register is removed */
        PanTime_CONTROL |= PanTime_CTRL_TRIG_EN;
    #endif /* Remove code section if control register is not used */
}


/*******************************************************************************
* Function Name: PanTime_DisableTrigger
********************************************************************************
*
* Summary:
*  Clears the control bit enabling Hardware Trigger mode
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void PanTime_DisableTrigger(void) 
{
    #if (!PanTime_ControlRegRemoved)   /* Remove assignment if control register is removed */
        PanTime_CONTROL &= ~PanTime_CTRL_TRIG_EN;
    #endif /* Remove code section if control register is not used */
}
#endif /* Remove API is Trigger Mode is set to None */


#if(PanTime_InterruptOnCaptureCount)
#if (!PanTime_ControlRegRemoved)   /* Remove API if control register is removed */


/*******************************************************************************
* Function Name: PanTime_SetInterruptCount
********************************************************************************
*
* Summary:
*  This function sets the capture count before an interrupt is triggered.
*
* Parameters:
*  interruptCount:  A value between 0 and 3 is valid.  If the value is 0, then
*                   an interrupt will occur each time a capture occurs.
*                   A value of 1 to 3 will cause the interrupt
*                   to delay by the same number of captures.
*
* Return:
*  void
*
*******************************************************************************/
void PanTime_SetInterruptCount(uint8 interruptCount) 
{
    /* This must only set to two bits of the control register associated */
    interruptCount &= PanTime_CTRL_INTCNT_MASK;

    /* Clear the Current Setting */
    PanTime_CONTROL &= ~PanTime_CTRL_INTCNT_MASK;
    /* Write The New Setting */
    PanTime_CONTROL |= interruptCount;
}
#endif /* Remove API if control register is removed */
#endif /* PanTime_InterruptOnCaptureCount */


#if (PanTime_UsingHWCaptureCounter)


/*******************************************************************************
* Function Name: PanTime_SetCaptureCount
********************************************************************************
*
* Summary:
*  This function sets the capture count
*
* Parameters:
*  captureCount: A value between 2 and 127 inclusive is valid.  A value of 1
*                to 127 will cause the interrupt to delay by the same number of
*                captures.
*
* Return:
*  void
*
*******************************************************************************/
void PanTime_SetCaptureCount(uint8 captureCount) 
{
    PanTime_CAP_COUNT = captureCount;
}


/*******************************************************************************
* Function Name: PanTime_ReadCaptureCount
********************************************************************************
*
* Summary:
*  This function reads the capture count setting
*
* Parameters:
*  void
*
* Return:
*  Returns the Capture Count Setting
*
*******************************************************************************/
uint8 PanTime_ReadCaptureCount(void) 
{
    return PanTime_CAP_COUNT ;
}
#endif /* PanTime_UsingHWCaptureCounter */


/*******************************************************************************
* Function Name: PanTime_ClearFIFO
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
void PanTime_ClearFIFO(void) 
{
    while(PanTime_ReadStatusRegister() & PanTime_STATUS_FIFONEMP)
    {
        PanTime_ReadCapture();
    }
}

#endif /* UDB Specific Functions */


/* [] END OF FILE */
