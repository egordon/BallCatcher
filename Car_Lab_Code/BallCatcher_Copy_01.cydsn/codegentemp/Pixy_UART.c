/*******************************************************************************
* File Name: Pixy_UART.c
* Version 2.20
*
* Description:
*  This file provides all API functionality of the UART component
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Pixy_UART.h"
#include "CyLib.h"


/***************************************
* Global data allocation
***************************************/

#if( Pixy_UART_TX_ENABLED && (Pixy_UART_TXBUFFERSIZE > Pixy_UART_FIFO_LENGTH))
    volatile uint8 Pixy_UART_txBuffer[Pixy_UART_TXBUFFERSIZE];
    volatile uint8 Pixy_UART_txBufferRead = 0u;
    uint8 Pixy_UART_txBufferWrite = 0u;
#endif /* End Pixy_UART_TX_ENABLED */
#if( ( Pixy_UART_RX_ENABLED || Pixy_UART_HD_ENABLED ) && \
     (Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH) )
    volatile uint8 Pixy_UART_rxBuffer[Pixy_UART_RXBUFFERSIZE];
    volatile uint8 Pixy_UART_rxBufferRead = 0u;
    volatile uint8 Pixy_UART_rxBufferWrite = 0u;
    volatile uint8 Pixy_UART_rxBufferLoopDetect = 0u;
    volatile uint8 Pixy_UART_rxBufferOverflow = 0u;
    #if (Pixy_UART_RXHW_ADDRESS_ENABLED)
        volatile uint8 Pixy_UART_rxAddressMode = Pixy_UART_RXADDRESSMODE;
        volatile uint8 Pixy_UART_rxAddressDetected = 0u;
    #endif /* End EnableHWAddress */    
#endif /* End Pixy_UART_RX_ENABLED */


/***************************************
* Local data allocation
***************************************/

uint8 Pixy_UART_initVar = 0u;


/*******************************************************************************
* Function Name: Pixy_UART_Start
********************************************************************************
*
* Summary:
*  Initialize and Enable the UART component.
*  Enable the clock input to enable operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The Pixy_UART_intiVar variable is used to indicate initial 
*  configuration of this component. The variable is initialized to zero (0u) 
*  and set to one (1u) the first time UART_Start() is called. This allows for 
*  component initialization without re-initialization in all subsequent calls 
*  to the Pixy_UART_Start() routine. 
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Pixy_UART_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(Pixy_UART_initVar == 0u)
    {
        Pixy_UART_Init();
        Pixy_UART_initVar = 1u;
    }
    Pixy_UART_Enable();
}


/*******************************************************************************
* Function Name: Pixy_UART_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  Pixy_UART_Start().
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Pixy_UART_Init(void) 
{
    #if(Pixy_UART_RX_ENABLED || Pixy_UART_HD_ENABLED)

        #if(Pixy_UART_RX_INTERRUPT_ENABLED && (Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH))
            /* Set the RX Interrupt. */
            CyIntSetVector(Pixy_UART_RX_VECT_NUM,   Pixy_UART_RXISR);
            CyIntSetPriority(Pixy_UART_RX_VECT_NUM, Pixy_UART_RX_PRIOR_NUM);
        #endif /* End Pixy_UART_RX_INTERRUPT_ENABLED */

        #if (Pixy_UART_RXHW_ADDRESS_ENABLED)
            Pixy_UART_SetRxAddressMode(Pixy_UART_RXAddressMode);
            Pixy_UART_SetRxAddress1(Pixy_UART_RXHWADDRESS1);
            Pixy_UART_SetRxAddress2(Pixy_UART_RXHWADDRESS2);
        #endif /* End Pixy_UART_RXHW_ADDRESS_ENABLED */

        /* Configure the Initial RX interrupt mask */
        Pixy_UART_RXSTATUS_MASK_REG  = Pixy_UART_INIT_RX_INTERRUPTS_MASK;
    #endif /* End Pixy_UART_RX_ENABLED || Pixy_UART_HD_ENABLED*/

    #if(Pixy_UART_TX_ENABLED)
        #if(Pixy_UART_TX_INTERRUPT_ENABLED && (Pixy_UART_TXBUFFERSIZE > Pixy_UART_FIFO_LENGTH))
            /* Set the TX Interrupt. */
            CyIntSetVector(Pixy_UART_TX_VECT_NUM,   Pixy_UART_TXISR);
            CyIntSetPriority(Pixy_UART_TX_VECT_NUM, Pixy_UART_TX_PRIOR_NUM);
        #endif /* End Pixy_UART_TX_INTERRUPT_ENABLED */

        /* Write Counter Value for TX Bit Clk Generator*/
        #if(Pixy_UART_TXCLKGEN_DP)
            Pixy_UART_TXBITCLKGEN_CTR_REG = Pixy_UART_BIT_CENTER;
            Pixy_UART_TXBITCLKTX_COMPLETE_REG = (Pixy_UART_NUMBER_OF_DATA_BITS + \
                                                    Pixy_UART_NUMBER_OF_START_BIT) * \
                                                    Pixy_UART_OVER_SAMPLE_COUNT;
        #else
            Pixy_UART_TXBITCTR_COUNTER_REG = (Pixy_UART_NUMBER_OF_DATA_BITS + \
                                                    Pixy_UART_NUMBER_OF_START_BIT) * \
                                                    Pixy_UART_OVER_SAMPLE_8;
        #endif /* End Pixy_UART_TXCLKGEN_DP */

        /* Configure the Initial TX interrupt mask */
        #if(Pixy_UART_TX_INTERRUPT_ENABLED && (Pixy_UART_TXBUFFERSIZE > Pixy_UART_FIFO_LENGTH))
            Pixy_UART_TXSTATUS_MASK_REG = Pixy_UART_TX_STS_FIFO_EMPTY;
        #else
            Pixy_UART_TXSTATUS_MASK_REG = Pixy_UART_INIT_TX_INTERRUPTS_MASK;
        #endif /*End Pixy_UART_TX_INTERRUPT_ENABLED*/
        
    #endif /* End Pixy_UART_TX_ENABLED */

    #if(Pixy_UART_PARITY_TYPE_SW)  /* Write Parity to Control Register */
        Pixy_UART_WriteControlRegister( \
            (Pixy_UART_ReadControlRegister() & ~Pixy_UART_CTRL_PARITY_TYPE_MASK) | \
            (Pixy_UART_PARITY_TYPE << Pixy_UART_CTRL_PARITY_TYPE0_SHIFT) );
    #endif /* End Pixy_UART_PARITY_TYPE_SW */
}


/*******************************************************************************
* Function Name: Pixy_UART_Enable
********************************************************************************
*
* Summary:
*  Enables the UART block operation
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  Pixy_UART_rxAddressDetected - set to initial state (0).
*
*******************************************************************************/
void Pixy_UART_Enable(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();
    
    #if(Pixy_UART_RX_ENABLED || Pixy_UART_HD_ENABLED)
        /*RX Counter (Count7) Enable */
        Pixy_UART_RXBITCTR_CONTROL_REG |= Pixy_UART_CNTR_ENABLE;
        /* Enable the RX Interrupt. */
        Pixy_UART_RXSTATUS_ACTL_REG  |= Pixy_UART_INT_ENABLE;
        #if(Pixy_UART_RX_INTERRUPT_ENABLED && (Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH))
            CyIntEnable(Pixy_UART_RX_VECT_NUM);
            #if (Pixy_UART_RXHW_ADDRESS_ENABLED)
                Pixy_UART_rxAddressDetected = 0u;
            #endif /* End Pixy_UART_RXHW_ADDRESS_ENABLED */
        #endif /* End Pixy_UART_RX_INTERRUPT_ENABLED */
    #endif /* End Pixy_UART_RX_ENABLED || Pixy_UART_HD_ENABLED*/

    #if(Pixy_UART_TX_ENABLED)
        /*TX Counter (DP/Count7) Enable */
        #if(!Pixy_UART_TXCLKGEN_DP)
            Pixy_UART_TXBITCTR_CONTROL_REG |= Pixy_UART_CNTR_ENABLE;
        #endif /* End Pixy_UART_TXCLKGEN_DP */
        /* Enable the TX Interrupt. */
        Pixy_UART_TXSTATUS_ACTL_REG |= Pixy_UART_INT_ENABLE;
        #if(Pixy_UART_TX_INTERRUPT_ENABLED && (Pixy_UART_TXBUFFERSIZE > Pixy_UART_FIFO_LENGTH))
            CyIntEnable(Pixy_UART_TX_VECT_NUM);
        #endif /* End Pixy_UART_TX_INTERRUPT_ENABLED*/
     #endif /* End Pixy_UART_TX_ENABLED */

    #if(Pixy_UART_INTERNAL_CLOCK_USED)
        /* Set the bit to enable the clock. */
        Pixy_UART_INTCLOCK_CLKEN_REG |= Pixy_UART_INTCLOCK_CLKEN_MASK;
    #endif /* End Pixy_UART_INTERNAL_CLOCK_USED */
    
    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Pixy_UART_Stop
********************************************************************************
*
* Summary:
*  Disable the UART component
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Pixy_UART_Stop(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /*Write Bit Counter Disable */
    #if(Pixy_UART_RX_ENABLED || Pixy_UART_HD_ENABLED)
        Pixy_UART_RXBITCTR_CONTROL_REG &= ~Pixy_UART_CNTR_ENABLE;
    #endif /* End Pixy_UART_RX_ENABLED */

    #if(Pixy_UART_TX_ENABLED)
        #if(!Pixy_UART_TXCLKGEN_DP)
            Pixy_UART_TXBITCTR_CONTROL_REG &= ~Pixy_UART_CNTR_ENABLE;
        #endif /* End Pixy_UART_TXCLKGEN_DP */
    #endif /* Pixy_UART_TX_ENABLED */

    #if(Pixy_UART_INTERNAL_CLOCK_USED)
        /* Clear the bit to enable the clock. */
        Pixy_UART_INTCLOCK_CLKEN_REG &= ~Pixy_UART_INTCLOCK_CLKEN_MASK;
    #endif /* End Pixy_UART_INTERNAL_CLOCK_USED */
    
    /*Disable internal interrupt component*/
    #if(Pixy_UART_RX_ENABLED || Pixy_UART_HD_ENABLED)
        Pixy_UART_RXSTATUS_ACTL_REG  &= ~Pixy_UART_INT_ENABLE;
        #if(Pixy_UART_RX_INTERRUPT_ENABLED && (Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH))
            Pixy_UART_DisableRxInt();
        #endif /* End Pixy_UART_RX_INTERRUPT_ENABLED */
    #endif /* End Pixy_UART_RX_ENABLED */
    
    #if(Pixy_UART_TX_ENABLED)
        Pixy_UART_TXSTATUS_ACTL_REG &= ~Pixy_UART_INT_ENABLE;
        #if(Pixy_UART_TX_INTERRUPT_ENABLED && (Pixy_UART_TXBUFFERSIZE > Pixy_UART_FIFO_LENGTH))
            Pixy_UART_DisableTxInt();
        #endif /* End Pixy_UART_TX_INTERRUPT_ENABLED */
    #endif /* End Pixy_UART_TX_ENABLED */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Pixy_UART_ReadControlRegister
********************************************************************************
*
* Summary:
*  Read the current state of the control register
*
* Parameters:
*  None.
*
* Return:
*  Current state of the control register.
*
*******************************************************************************/
uint8 Pixy_UART_ReadControlRegister(void) 
{
    #if( Pixy_UART_CONTROL_REG_REMOVED )
        return(0u);
    #else
        return(Pixy_UART_CONTROL_REG);
    #endif /* End Pixy_UART_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: Pixy_UART_WriteControlRegister
********************************************************************************
*
* Summary:
*  Writes an 8-bit value into the control register
*
* Parameters:
*  control:  control register value
*
* Return:
*  None.
*
*******************************************************************************/
void  Pixy_UART_WriteControlRegister(uint8 control) 
{
    #if( Pixy_UART_CONTROL_REG_REMOVED )
        control = control;      /* Reassigning to release compiler warning */ 
    #else
       Pixy_UART_CONTROL_REG = control;
    #endif /* End Pixy_UART_CONTROL_REG_REMOVED */
}


#if(Pixy_UART_RX_ENABLED || Pixy_UART_HD_ENABLED)

    #if(Pixy_UART_RX_INTERRUPT_ENABLED)

        /*******************************************************************************
        * Function Name: Pixy_UART_EnableRxInt
        ********************************************************************************
        *
        * Summary:
        *  Enable RX interrupt generation
        *
        * Parameters:
        *  None.
        *
        * Return:
        *  None.
        *
        * Theory:
        *  Enable the interrupt output -or- the interrupt component itself
        *
        *******************************************************************************/
        void Pixy_UART_EnableRxInt(void) 
        {
            CyIntEnable(Pixy_UART_RX_VECT_NUM);
        }


        /*******************************************************************************
        * Function Name: Pixy_UART_DisableRxInt
        ********************************************************************************
        *
        * Summary:
        *  Disable RX interrupt generation
        *
        * Parameters:
        *  None.
        *
        * Return:
        *  None.
        *
        * Theory:
        *  Disable the interrupt output -or- the interrupt component itself
        *
        *******************************************************************************/
        void Pixy_UART_DisableRxInt(void) 
        {
            CyIntDisable(Pixy_UART_RX_VECT_NUM);
        }

    #endif /* Pixy_UART_RX_INTERRUPT_ENABLED */


    /*******************************************************************************
    * Function Name: Pixy_UART_SetRxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configure which status bits trigger an interrupt event
    *
    * Parameters:
    *  IntSrc:  An or'd combination of the desired status bit masks (defined in
    *           the header file)
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void Pixy_UART_SetRxInterruptMode(uint8 intSrc) 
    {
        Pixy_UART_RXSTATUS_MASK_REG  = intSrc;
    }


    /*******************************************************************************
    * Function Name: Pixy_UART_ReadRxData
    ********************************************************************************
    *
    * Summary:
    *  Returns data in RX Data register without checking status register to 
    *  determine if data is valid
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Received data from RX register
    *
    * Global Variables:
    *  Pixy_UART_rxBuffer - RAM buffer pointer for save received data.
    *  Pixy_UART_rxBufferWrite - cyclic index for write to rxBuffer, 
    *     checked to identify new data. 
    *  Pixy_UART_rxBufferRead - cyclic index for read from rxBuffer, 
    *     incremented after each byte has been read from buffer.
    *  Pixy_UART_rxBufferLoopDetect - creared if loop condition was detected
    *     in RX ISR. 
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 Pixy_UART_ReadRxData(void) 
    {
        uint8 rxData;

        #if(Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH)

            /* Disable Rx interrupt. */
            /* Protect variables that could change on interrupt. */
            #if(Pixy_UART_RX_INTERRUPT_ENABLED)
                Pixy_UART_DisableRxInt();
            #endif /* End Pixy_UART_RX_INTERRUPT_ENABLED */

            if( (Pixy_UART_rxBufferRead != Pixy_UART_rxBufferWrite) ||
                (Pixy_UART_rxBufferLoopDetect > 0u) )
            {

                rxData = Pixy_UART_rxBuffer[Pixy_UART_rxBufferRead];

                Pixy_UART_rxBufferRead++;

                if(Pixy_UART_rxBufferRead >= Pixy_UART_RXBUFFERSIZE)
                {
                    Pixy_UART_rxBufferRead = 0u;
                }

                if(Pixy_UART_rxBufferLoopDetect > 0u )
                {
                    Pixy_UART_rxBufferLoopDetect = 0u;
                    #if( (Pixy_UART_RX_INTERRUPT_ENABLED) && (Pixy_UART_FLOW_CONTROL != 0u) && \
                         (Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH) )
                        /* When Hardware Flow Control selected - return RX mask */
                        #if( Pixy_UART_HD_ENABLED )
                            if((Pixy_UART_CONTROL_REG & Pixy_UART_CTRL_HD_SEND) == 0)
                            {   /* In Half duplex mode return RX mask only in RX 
                                *  configuration set, otherwise 
                                *  mask will be returned in LoadRxConfig() API. 
                                */
                                Pixy_UART_RXSTATUS_MASK_REG  |= Pixy_UART_RX_STS_FIFO_NOTEMPTY;
                            }
                        #else
                            Pixy_UART_RXSTATUS_MASK_REG  |= Pixy_UART_RX_STS_FIFO_NOTEMPTY;
                        #endif /* end Pixy_UART_HD_ENABLED */
                    #endif /* Pixy_UART_RX_INTERRUPT_ENABLED and Hardware flow control*/
                }
            }
            else
            {   /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit*/
                rxData = Pixy_UART_RXDATA_REG;
            }

            /* Enable Rx interrupt. */
            #if(Pixy_UART_RX_INTERRUPT_ENABLED)
                Pixy_UART_EnableRxInt();
            #endif /* End Pixy_UART_RX_INTERRUPT_ENABLED */

        #else /* Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH */

            /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit*/
            rxData = Pixy_UART_RXDATA_REG;

        #endif /* Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: Pixy_UART_ReadRxStatus
    ********************************************************************************
    *
    * Summary:
    *  Read the current state of the status register
    *  And detect software buffer overflow.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Current state of the status register.
    *
    * Global Variables:
    *  Pixy_UART_rxBufferOverflow - used to indicate overload condition. 
    *   It set to one in RX interrupt when there isn?t free space in 
    *   Pixy_UART_rxBufferRead to write new data. This condition returned 
    *   and cleared to zero by this API as an 
    *   Pixy_UART_RX_STS_SOFT_BUFF_OVER bit along with RX Status register 
    *   bits.
    *
    *******************************************************************************/
    uint8 Pixy_UART_ReadRxStatus(void) 
    {
        uint8 status;

        status = Pixy_UART_RXSTATUS_REG;
        status &= Pixy_UART_RX_HW_MASK;

        #if(Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH)
            if( Pixy_UART_rxBufferOverflow )
            {
                status |= Pixy_UART_RX_STS_SOFT_BUFF_OVER;
                Pixy_UART_rxBufferOverflow = 0u;
            }
        #endif /* Pixy_UART_RXBUFFERSIZE */

        return(status);
    }


    /*******************************************************************************
    * Function Name: Pixy_UART_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Reads UART RX buffer immediately, if data is not available or an error 
    *  condition exists, zero is returned; otherwise, character is read and 
    *  returned.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Character read from UART RX buffer. ASCII characters from 1 to 255 are valid.
    *  A returned zero signifies an error condition or no data available.
    *
    * Global Variables:
    *  Pixy_UART_rxBuffer - RAM buffer pointer for save received data.
    *  Pixy_UART_rxBufferWrite - cyclic index for write to rxBuffer, 
    *     checked to identify new data. 
    *  Pixy_UART_rxBufferRead - cyclic index for read from rxBuffer, 
    *     incremented after each byte has been read from buffer.
    *  Pixy_UART_rxBufferLoopDetect - creared if loop condition was detected
    *     in RX ISR. 
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 Pixy_UART_GetChar(void) 
    {
        uint8 rxData = 0u;
        uint8 rxStatus;

        #if(Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH)

            /* Disable Rx interrupt. */
            /* Protect variables that could change on interrupt. */
            #if(Pixy_UART_RX_INTERRUPT_ENABLED)
                Pixy_UART_DisableRxInt();
            #endif /* Pixy_UART_RX_INTERRUPT_ENABLED */

            if( (Pixy_UART_rxBufferRead != Pixy_UART_rxBufferWrite) ||
                (Pixy_UART_rxBufferLoopDetect > 0u) )
            {
                rxData = Pixy_UART_rxBuffer[Pixy_UART_rxBufferRead];

                Pixy_UART_rxBufferRead++;

                if(Pixy_UART_rxBufferRead >= Pixy_UART_RXBUFFERSIZE)
                {
                    Pixy_UART_rxBufferRead = 0u;
                }

                if(Pixy_UART_rxBufferLoopDetect > 0u ) 
                {
                    Pixy_UART_rxBufferLoopDetect = 0u;
                    #if( (Pixy_UART_RX_INTERRUPT_ENABLED) && (Pixy_UART_FLOW_CONTROL != 0u) && \
                         (Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH) )
                        /* When Hardware Flow Control selected - return RX mask */
                        #if( Pixy_UART_HD_ENABLED )
                            if((Pixy_UART_CONTROL_REG & Pixy_UART_CTRL_HD_SEND) == 0)
                            {   /* In Half duplex mode return RX mask only if 
                                *  RX configuration set, otherwise 
                                *  mask will be returned in LoadRxConfig() API. 
                                */
                                Pixy_UART_RXSTATUS_MASK_REG  |= Pixy_UART_RX_STS_FIFO_NOTEMPTY;
                            }
                        #else
                            Pixy_UART_RXSTATUS_MASK_REG  |= Pixy_UART_RX_STS_FIFO_NOTEMPTY;
                        #endif /* end Pixy_UART_HD_ENABLED */
                    #endif /* Pixy_UART_RX_INTERRUPT_ENABLED and Hardware flow control*/
                }

            }
            else
            {   rxStatus =Pixy_UART_RXSTATUS_REG;
                if(rxStatus & Pixy_UART_RX_STS_FIFO_NOTEMPTY)
                {   /* Read received data from FIFO*/
                    rxData = Pixy_UART_RXDATA_REG;
                    /*Check status on error*/
                    if(rxStatus & (Pixy_UART_RX_STS_BREAK | Pixy_UART_RX_STS_PAR_ERROR |
                                   Pixy_UART_RX_STS_STOP_ERROR | Pixy_UART_RX_STS_OVERRUN))
                    {
                        rxData = 0u;
                    }    
                }
            }

            /* Enable Rx interrupt. */
            #if(Pixy_UART_RX_INTERRUPT_ENABLED)
                Pixy_UART_EnableRxInt();
            #endif /* Pixy_UART_RX_INTERRUPT_ENABLED */

        #else /* Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH */

            rxStatus =Pixy_UART_RXSTATUS_REG;
            if(rxStatus & Pixy_UART_RX_STS_FIFO_NOTEMPTY)
            {   /* Read received data from FIFO*/
                rxData = Pixy_UART_RXDATA_REG;
                /*Check status on error*/
                if(rxStatus & (Pixy_UART_RX_STS_BREAK | Pixy_UART_RX_STS_PAR_ERROR |
                               Pixy_UART_RX_STS_STOP_ERROR | Pixy_UART_RX_STS_OVERRUN))
                {
                    rxData = 0u;
                }
            }
        #endif /* Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: Pixy_UART_GetByte
    ********************************************************************************
    *
    * Summary:
    *  Grab the next available byte of data from the recieve FIFO
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  MSB contains Status Register and LSB contains UART RX data
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint16 Pixy_UART_GetByte(void) 
    {
        return ( ((uint16)Pixy_UART_ReadRxStatus() << 8u) | Pixy_UART_ReadRxData() );
    }


    /*******************************************************************************
    * Function Name: Pixy_UART_GetRxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Determine the amount of bytes left in the RX buffer and return the count in
    *  bytes
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  uint8: Integer count of the number of bytes left 
    *  in the RX buffer
    *
    * Global Variables:
    *  Pixy_UART_rxBufferWrite - used to calculate left bytes. 
    *  Pixy_UART_rxBufferRead - used to calculate left bytes.
    *  Pixy_UART_rxBufferLoopDetect - checked to decide left bytes amount. 
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the RX Buffer is.
    *
    *******************************************************************************/
    uint8 Pixy_UART_GetRxBufferSize(void) 
                                                            
    {
        uint8 size;

        #if(Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH)

            /* Disable Rx interrupt. */
            /* Protect variables that could change on interrupt. */
            #if(Pixy_UART_RX_INTERRUPT_ENABLED)
                Pixy_UART_DisableRxInt();
            #endif /* Pixy_UART_RX_INTERRUPT_ENABLED */

            if(Pixy_UART_rxBufferRead == Pixy_UART_rxBufferWrite)
            {
                if(Pixy_UART_rxBufferLoopDetect > 0u)
                {
                    size = Pixy_UART_RXBUFFERSIZE;
                }
                else
                {
                    size = 0u;
                }
            }
            else if(Pixy_UART_rxBufferRead < Pixy_UART_rxBufferWrite)
            {
                size = (Pixy_UART_rxBufferWrite - Pixy_UART_rxBufferRead);
            }
            else
            {
                size = (Pixy_UART_RXBUFFERSIZE - Pixy_UART_rxBufferRead) + Pixy_UART_rxBufferWrite;
            }

            /* Enable Rx interrupt. */
            #if(Pixy_UART_RX_INTERRUPT_ENABLED)
                Pixy_UART_EnableRxInt();
            #endif /* End Pixy_UART_RX_INTERRUPT_ENABLED */

        #else /* Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH */

            /* We can only know if there is data in the fifo. */
            size = (Pixy_UART_RXSTATUS_REG & Pixy_UART_RX_STS_FIFO_NOTEMPTY) ? 1u : 0u;

        #endif /* End Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH */

        return(size);
    }


    /*******************************************************************************
    * Function Name: Pixy_UART_ClearRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the RX RAM buffer by setting the read and write pointers both to zero.
    *  Clears hardware RX FIFO.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  Pixy_UART_rxBufferWrite - cleared to zero. 
    *  Pixy_UART_rxBufferRead - cleared to zero.
    *  Pixy_UART_rxBufferLoopDetect - cleared to zero. 
    *  Pixy_UART_rxBufferOverflow - cleared to zero. 
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to 
    *  read and writing will resume at address 0 overwriting any data that may 
    *  have remained in the RAM.
    *
    * Side Effects:
    *  Any received data not read from the RAM or FIFO buffer will be lost.
    *******************************************************************************/
    void Pixy_UART_ClearRxBuffer(void) 
    {
        uint8 enableInterrupts;
        
        /* clear the HW FIFO */
        /* Enter critical section */
        enableInterrupts = CyEnterCriticalSection();        
        Pixy_UART_RXDATA_AUX_CTL_REG |=  Pixy_UART_RX_FIFO_CLR;
        Pixy_UART_RXDATA_AUX_CTL_REG &= ~Pixy_UART_RX_FIFO_CLR;
        /* Exit critical section */
        CyExitCriticalSection(enableInterrupts);
        
        #if(Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH)
            /* Disable Rx interrupt. */
            /* Protect variables that could change on interrupt. */
            #if(Pixy_UART_RX_INTERRUPT_ENABLED)
                Pixy_UART_DisableRxInt();
            #endif /* End Pixy_UART_RX_INTERRUPT_ENABLED */

            Pixy_UART_rxBufferRead = 0u;
            Pixy_UART_rxBufferWrite = 0u;
            Pixy_UART_rxBufferLoopDetect = 0u;
            Pixy_UART_rxBufferOverflow = 0u;

            /* Enable Rx interrupt. */
            #if(Pixy_UART_RX_INTERRUPT_ENABLED)
                Pixy_UART_EnableRxInt();
            #endif /* End Pixy_UART_RX_INTERRUPT_ENABLED */
        #endif /* End Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH */
        
    }


    /*******************************************************************************
    * Function Name: Pixy_UART_SetRxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Set the receive addressing mode
    *
    * Parameters:
    *  addressMode: Enumerated value indicating the mode of RX addressing
    *  Pixy_UART__B_UART__AM_SW_BYTE_BYTE -  Software Byte-by-Byte address 
    *                                               detection
    *  Pixy_UART__B_UART__AM_SW_DETECT_TO_BUFFER - Software Detect to Buffer 
    *                                               address detection
    *  Pixy_UART__B_UART__AM_HW_BYTE_BY_BYTE - Hardware Byte-by-Byte address 
    *                                               detection
    *  Pixy_UART__B_UART__AM_HW_DETECT_TO_BUFFER - Hardware Detect to Buffer 
    *                                               address detection
    *  Pixy_UART__B_UART__AM_NONE - No address detection
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  Pixy_UART_rxAddressMode - the parameter stored in this variable for 
    *   the farther usage in RX ISR.
    *  Pixy_UART_rxAddressDetected - set to initial state (0).
    *
    *******************************************************************************/
    void Pixy_UART_SetRxAddressMode(uint8 addressMode)  
                                                        
    {
        #if(Pixy_UART_RXHW_ADDRESS_ENABLED)
            #if(Pixy_UART_CONTROL_REG_REMOVED)
                addressMode = addressMode;
            #else /* Pixy_UART_CONTROL_REG_REMOVED */
                uint8 tmpCtrl = 0u;
                tmpCtrl = Pixy_UART_CONTROL_REG & ~Pixy_UART_CTRL_RXADDR_MODE_MASK;
                tmpCtrl |= ((addressMode << Pixy_UART_CTRL_RXADDR_MODE0_SHIFT) & 
                           Pixy_UART_CTRL_RXADDR_MODE_MASK);
                Pixy_UART_CONTROL_REG = tmpCtrl;
                #if(Pixy_UART_RX_INTERRUPT_ENABLED && \
                   (Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH) )
                    Pixy_UART_rxAddressMode = addressMode;
                    Pixy_UART_rxAddressDetected = 0u;
                #endif /* End Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH*/   
            #endif /* End Pixy_UART_CONTROL_REG_REMOVED */
        #else /* Pixy_UART_RXHW_ADDRESS_ENABLED */
            addressMode = addressMode;
        #endif /* End Pixy_UART_RXHW_ADDRESS_ENABLED */
    }


    /*******************************************************************************
    * Function Name: Pixy_UART_SetRxAddress1
    ********************************************************************************
    *
    * Summary:
    *  Set the first hardware address compare value
    *
    * Parameters:
    *  address
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void Pixy_UART_SetRxAddress1(uint8 address) 

    {
        Pixy_UART_RXADDRESS1_REG = address;
    }


    /*******************************************************************************
    * Function Name: Pixy_UART_SetRxAddress2
    ********************************************************************************
    *
    * Summary:
    *  Set the second hardware address compare value
    *
    * Parameters:
    *  address
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void Pixy_UART_SetRxAddress2(uint8 address) 
    {
        Pixy_UART_RXADDRESS2_REG = address;
    }
        
#endif  /* Pixy_UART_RX_ENABLED || Pixy_UART_HD_ENABLED*/


#if( (Pixy_UART_TX_ENABLED) || (Pixy_UART_HD_ENABLED) )

    #if(Pixy_UART_TX_INTERRUPT_ENABLED)

        /*******************************************************************************
        * Function Name: Pixy_UART_EnableTxInt
        ********************************************************************************
        *
        * Summary:
        *  Enable TX interrupt generation
        *
        * Parameters:
        *  None.
        *
        * Return:
        *  None.
        *
        * Theory:
        *  Enable the interrupt output -or- the interrupt component itself
        *
        *******************************************************************************/
        void Pixy_UART_EnableTxInt(void) 
        {
            CyIntEnable(Pixy_UART_TX_VECT_NUM);
        }


        /*******************************************************************************
        * Function Name: Pixy_UART_DisableTxInt
        ********************************************************************************
        *
        * Summary:
        *  Disable TX interrupt generation
        *
        * Parameters:
        *  None.
        *
        * Return:
        *  None.
        *
        * Theory:
        *  Disable the interrupt output -or- the interrupt component itself
        *
        *******************************************************************************/
        void Pixy_UART_DisableTxInt(void) 
        {
            CyIntDisable(Pixy_UART_TX_VECT_NUM);
        }

    #endif /* Pixy_UART_TX_INTERRUPT_ENABLED */


    /*******************************************************************************
    * Function Name: Pixy_UART_SetTxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configure which status bits trigger an interrupt event
    *
    * Parameters:
    *  intSrc: An or'd combination of the desired status bit masks (defined in
    *          the header file)
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void Pixy_UART_SetTxInterruptMode(uint8 intSrc) 
    {
        Pixy_UART_TXSTATUS_MASK_REG = intSrc;
    }


    /*******************************************************************************
    * Function Name: Pixy_UART_WriteTxData
    ********************************************************************************
    *
    * Summary:
    *  Write a byte of data to the Transmit FIFO or TX buffer to be sent when the 
    *  bus is available. WriteTxData sends a byte without checking for buffer room 
    *  or status. It is up to the user to separately check status.    
    *
    * Parameters:
    *  TXDataByte: byte of data to place in the transmit FIFO
    *
    * Return:
    * void
    *
    * Global Variables:
    *  Pixy_UART_txBuffer - RAM buffer pointer for save data for transmission
    *  Pixy_UART_txBufferWrite - cyclic index for write to txBuffer, 
    *    incremented after each byte saved to buffer.
    *  Pixy_UART_txBufferRead - cyclic index for read from txBuffer, 
    *    checked to identify the condition to write to FIFO directly or to TX buffer
    *  Pixy_UART_initVar - checked to identify that the component has been  
    *    initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void Pixy_UART_WriteTxData(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function*/
        if(Pixy_UART_initVar != 0u)
        {
            #if(Pixy_UART_TXBUFFERSIZE > Pixy_UART_FIFO_LENGTH)

                /* Disable Tx interrupt. */
                /* Protect variables that could change on interrupt. */
                #if(Pixy_UART_TX_INTERRUPT_ENABLED)
                    Pixy_UART_DisableTxInt();
                #endif /* End Pixy_UART_TX_INTERRUPT_ENABLED */

                if( (Pixy_UART_txBufferRead == Pixy_UART_txBufferWrite) &&
                   !(Pixy_UART_TXSTATUS_REG & Pixy_UART_TX_STS_FIFO_FULL) )
                {
                    /* Add directly to the FIFO. */
                    Pixy_UART_TXDATA_REG = txDataByte;
                }
                else
                {
                    if(Pixy_UART_txBufferWrite >= Pixy_UART_TXBUFFERSIZE)
                    {
                        Pixy_UART_txBufferWrite = 0;
                    }

                    Pixy_UART_txBuffer[Pixy_UART_txBufferWrite] = txDataByte;

                    /* Add to the software buffer. */
                    Pixy_UART_txBufferWrite++;

                }

                /* Enable Tx interrupt. */
                #if(Pixy_UART_TX_INTERRUPT_ENABLED)
                    Pixy_UART_EnableTxInt();
                #endif /* End Pixy_UART_TX_INTERRUPT_ENABLED */

            #else /* Pixy_UART_TXBUFFERSIZE > Pixy_UART_FIFO_LENGTH */

                /* Add directly to the FIFO. */
                Pixy_UART_TXDATA_REG = txDataByte;

            #endif /* End Pixy_UART_TXBUFFERSIZE > Pixy_UART_FIFO_LENGTH */
        }
    }


    /*******************************************************************************
    * Function Name: Pixy_UART_ReadTxStatus
    ********************************************************************************
    *
    * Summary:
    *  Read the status register for the component
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Contents of the status register
    *
    * Theory:
    *  This function reads the status register which is clear on read. It is up to 
    *  the user to handle all bits in this return value accordingly, even if the bit 
    *  was not enabled as an interrupt source the event happened and must be handled
    *  accordingly.    
    *
    *******************************************************************************/
    uint8 Pixy_UART_ReadTxStatus(void) 
    {
        return(Pixy_UART_TXSTATUS_REG);
    }


    /*******************************************************************************
    * Function Name: Pixy_UART_PutChar
    ********************************************************************************
    *
    * Summary:
    *  Wait to send byte until TX register or buffer has room.
    *
    * Parameters:
    *  txDataByte: The 8-bit data value to send across the UART.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  Pixy_UART_txBuffer - RAM buffer pointer for save data for transmission
    *  Pixy_UART_txBufferWrite - cyclic index for write to txBuffer, 
    *     checked to identify free space in txBuffer and incremented after each byte 
    *     saved to buffer.
    *  Pixy_UART_txBufferRead - cyclic index for read from txBuffer, 
    *     checked to identify free space in txBuffer.
    *  Pixy_UART_initVar - checked to identify that the component has been  
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to transmit any byte of data in a single transfer
    *
    *******************************************************************************/
    void Pixy_UART_PutChar(uint8 txDataByte) 
    {
            #if(Pixy_UART_TXBUFFERSIZE > Pixy_UART_FIFO_LENGTH)

                /* Block if buffer is full, so we dont overwrite. */
                while( Pixy_UART_txBufferWrite == (Pixy_UART_txBufferRead - 1u) ||
                    (uint8)(Pixy_UART_txBufferWrite - Pixy_UART_txBufferRead) ==
                    (uint8)(Pixy_UART_TXBUFFERSIZE - 1u) )
                {
                    /* Software buffer is full. */
                }
                /* Disable Tx interrupt. */
                /* Protect variables that could change on interrupt. */
                #if(Pixy_UART_TX_INTERRUPT_ENABLED)
                    Pixy_UART_DisableTxInt();
                #endif /* End Pixy_UART_TX_INTERRUPT_ENABLED */

                if( (Pixy_UART_txBufferRead == Pixy_UART_txBufferWrite) &&
                   !(Pixy_UART_TXSTATUS_REG & Pixy_UART_TX_STS_FIFO_FULL) )
                {
                    /* Add directly to the FIFO. */
                    Pixy_UART_TXDATA_REG = txDataByte;
                }
                else
                {
                    if(Pixy_UART_txBufferWrite >= Pixy_UART_TXBUFFERSIZE)
                    {
                        Pixy_UART_txBufferWrite = 0;
                    }

                    Pixy_UART_txBuffer[Pixy_UART_txBufferWrite] = txDataByte;

                    /* Add to the software buffer. */
                    Pixy_UART_txBufferWrite++;

                }

                /* Enable Rx interrupt. */
                #if(Pixy_UART_TX_INTERRUPT_ENABLED)
                    Pixy_UART_EnableTxInt();
                #endif /* End Pixy_UART_TX_INTERRUPT_ENABLED */

            #else /* Pixy_UART_TXBUFFERSIZE > Pixy_UART_FIFO_LENGTH */

                /* Block if there isnt room. */
                while(Pixy_UART_TXSTATUS_REG & Pixy_UART_TX_STS_FIFO_FULL);

                /* Add directly to the FIFO. */
                Pixy_UART_TXDATA_REG = txDataByte;

            #endif /* End Pixy_UART_TXBUFFERSIZE > Pixy_UART_FIFO_LENGTH */
    }


    /*******************************************************************************
    * Function Name: Pixy_UART_PutString
    ********************************************************************************
    *
    * Summary:
    *  Write a Sequence of bytes on the Transmit line. Data comes from RAM or ROM.
    *
    * Parameters:
    *  string: char pointer to character string of Data to Send.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  Pixy_UART_initVar - checked to identify that the component has been  
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  This function will block if there is not enough memory to place the whole 
    *  string, it will block until the entire string has been written to the 
    *  transmit buffer.
    *
    *******************************************************************************/
    void Pixy_UART_PutString(char* string) 
    {
        /* If not Initialized then skip this function*/
        if(Pixy_UART_initVar != 0u)
        {
            /* This is a blocking function, it will not exit until all data is sent*/
            while(*string != 0u)
            {
                Pixy_UART_PutChar(*string++);
            }
        }
    }


    /*******************************************************************************
    * Function Name: Pixy_UART_PutArray
    ********************************************************************************
    *
    * Summary:
    *  Write a Sequence of bytes on the Transmit line. Data comes from RAM or ROM.
    *
    * Parameters:
    *  string: Address of the memory array residing in RAM or ROM.
    *  byteCount: Number of Bytes to be transmitted.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  Pixy_UART_initVar - checked to identify that the component has been  
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void Pixy_UART_PutArray(uint8* string, uint8 byteCount) 
                                                                    
    {
        /* If not Initialized then skip this function*/
        if(Pixy_UART_initVar != 0u)
        {
            while(byteCount > 0u)
            {
                Pixy_UART_PutChar(*string++);
                byteCount--;
            }
        }
    }


    /*******************************************************************************
    * Function Name: Pixy_UART_PutCRLF
    ********************************************************************************
    *
    * Summary:
    *  Write a character and then carriage return and line feed.
    *
    * Parameters:
    *  txDataByte: uint8 Character to send.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  Pixy_UART_initVar - checked to identify that the component has been  
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void Pixy_UART_PutCRLF(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function*/
        if(Pixy_UART_initVar != 0u)
        {
            Pixy_UART_PutChar(txDataByte);
            Pixy_UART_PutChar(0x0Du);
            Pixy_UART_PutChar(0x0Au);
        }
    }


    /*******************************************************************************
    * Function Name: Pixy_UART_GetTxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Determine the amount of space left in the TX buffer and return the count in
    *  bytes
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Integer count of the number of bytes left in the TX buffer
    *
    * Global Variables:
    *  Pixy_UART_txBufferWrite - used to calculate left space. 
    *  Pixy_UART_txBufferRead - used to calculate left space.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the TX Buffer is.
    *
    *******************************************************************************/
    uint8 Pixy_UART_GetTxBufferSize(void) 
                                                            
    {
        uint8 size;

        #if(Pixy_UART_TXBUFFERSIZE > Pixy_UART_FIFO_LENGTH)

            /* Disable Tx interrupt. */
            /* Protect variables that could change on interrupt. */
            #if(Pixy_UART_TX_INTERRUPT_ENABLED)
                Pixy_UART_DisableTxInt();
            #endif /* End Pixy_UART_TX_INTERRUPT_ENABLED */

            if(Pixy_UART_txBufferRead == Pixy_UART_txBufferWrite)
            {
                size = 0u;
            }
            else if(Pixy_UART_txBufferRead < Pixy_UART_txBufferWrite)
            {
                size = (Pixy_UART_txBufferWrite - Pixy_UART_txBufferRead);
            }
            else
            {
                size = (Pixy_UART_TXBUFFERSIZE - Pixy_UART_txBufferRead) + Pixy_UART_txBufferWrite;
            }

            /* Enable Tx interrupt. */
            #if(Pixy_UART_TX_INTERRUPT_ENABLED)
                Pixy_UART_EnableTxInt();
            #endif /* End Pixy_UART_TX_INTERRUPT_ENABLED */

        #else /* Pixy_UART_TXBUFFERSIZE > Pixy_UART_FIFO_LENGTH */

            size = Pixy_UART_TXSTATUS_REG;

            /* Is the fifo is full. */
            if(size & Pixy_UART_TX_STS_FIFO_FULL)
            {
                size = Pixy_UART_FIFO_LENGTH;
            }
            else if(size & Pixy_UART_TX_STS_FIFO_EMPTY)
            {
                size = 0u;
            }
            else
            {
                /* We only know there is data in the fifo. */
                size = 1u;
            }

        #endif /* End Pixy_UART_TXBUFFERSIZE > Pixy_UART_FIFO_LENGTH */

        return(size);
    }


    /*******************************************************************************
    * Function Name: Pixy_UART_ClearTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the TX RAM buffer by setting the read and write pointers both to zero.
    *  Clears the hardware TX FIFO.  Any data present in the FIFO will not be sent.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  Pixy_UART_txBufferWrite - cleared to zero. 
    *  Pixy_UART_txBufferRead - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to 
    *  read and writing will resume at address 0 overwriting any data that may have
    *  remained in the RAM.
    *
    * Side Effects:
    *  Any received data not read from the RAM buffer will be lost when overwritten.
    *
    *******************************************************************************/
    void Pixy_UART_ClearTxBuffer(void) 
    {
        uint8 enableInterrupts;
        
        /* Enter critical section */
        enableInterrupts = CyEnterCriticalSection();        
        /* clear the HW FIFO */
        Pixy_UART_TXDATA_AUX_CTL_REG |=  Pixy_UART_TX_FIFO_CLR;
        Pixy_UART_TXDATA_AUX_CTL_REG &= ~Pixy_UART_TX_FIFO_CLR;
        /* Exit critical section */
        CyExitCriticalSection(enableInterrupts);

        #if(Pixy_UART_TXBUFFERSIZE > Pixy_UART_FIFO_LENGTH)

            /* Disable Tx interrupt. */
            /* Protect variables that could change on interrupt. */
            #if(Pixy_UART_TX_INTERRUPT_ENABLED)
                Pixy_UART_DisableTxInt();
            #endif /* End Pixy_UART_TX_INTERRUPT_ENABLED */

            Pixy_UART_txBufferRead = 0u;
            Pixy_UART_txBufferWrite = 0u;

            /* Enable Tx interrupt. */
            #if(Pixy_UART_TX_INTERRUPT_ENABLED)
                Pixy_UART_EnableTxInt();
            #endif /* End Pixy_UART_TX_INTERRUPT_ENABLED */

        #endif /* End Pixy_UART_TXBUFFERSIZE > Pixy_UART_FIFO_LENGTH */
    }


    /*******************************************************************************
    * Function Name: Pixy_UART_SendBreak
    ********************************************************************************
    *
    * Summary:
    *  Write a Break command to the UART
    *
    * Parameters:
    *  uint8 retMode:  Wait mode,
    *   0 - Initialize registers for Break, sends the Break signal and return 
    *       imediately.
    *   1 - Wait until Break sending is complete, reinitialize registers to normal
    *       transmission mode then return.
    *   2 - Reinitialize registers to normal transmission mode then return.
    *   3 - both steps: 0 and 1
    *       init registers for Break, send Break signal
    *       wait until Break sending is complete, reinit registers to normal
    *       transmission mode then return.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  Pixy_UART_initVar - checked to identify that the component has been  
    *     initialized.
    *  tx_period - static variable, used for keeping TX period configuration.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  SendBreak function initializes registers to send 13-bit break signal. It is
    *  important to return the registers configuration to normal for continue 8-bit
    *  operation.
    *  Trere are 3 variants for this API usage:
    *  1) SendBreak(3) - function will send the Break signal and take care on the
    *     configuration returning. Funcition will block CPU untill transmition 
    *     complete.
    *  2) User may want to use bloking time if UART configured to the low speed 
    *     operation
    *     Emample for this case:
    *     SendBreak(0);     - init Break signal transmition
    *         Add your code here to use CPU time
    *     SendBreak(1);     - complete Break operation
    *  3) Same to 2) but user may want to init and use the interrupt for complete 
    *     break operation.
    *     Example for this case:
    *     Init TX interrupt whith "TX - On TX Complete" parameter
    *     SendBreak(0);     - init Break signal transmition
    *         Add your code here to use CPU time
    *     When interrupt appear with UART_TX_STS_COMPLETE status:
    *     SendBreak(2);     - complete Break operation
    *
    * Side Effects:
    *   Uses static variable to keep registers configuration.
    *
    *******************************************************************************/
    void Pixy_UART_SendBreak(uint8 retMode) 
    {

        /* If not Initialized then skip this function*/
        if(Pixy_UART_initVar != 0u)
        {
            /*Set the Counter to 13-bits and transmit a 00 byte*/
            /*When that is done then reset the counter value back*/
            uint8 tmpStat;

            #if(Pixy_UART_HD_ENABLED) /* Half Duplex mode*/

                if( (retMode == Pixy_UART_SEND_BREAK) ||
                    (retMode == Pixy_UART_SEND_WAIT_REINIT ) )
                {
                    /* CTRL_HD_SEND_BREAK - sends break bits in HD mode*/
                    Pixy_UART_WriteControlRegister(Pixy_UART_ReadControlRegister() |
                                                          Pixy_UART_CTRL_HD_SEND_BREAK);
                    /* Send zeros*/
                    Pixy_UART_TXDATA_REG = 0u;

                    do /*wait until transmit starts*/
                    {
                        tmpStat = Pixy_UART_TXSTATUS_REG;
                    }while(tmpStat & Pixy_UART_TX_STS_FIFO_EMPTY);
                }

                if( (retMode == Pixy_UART_WAIT_FOR_COMPLETE_REINIT) ||
                    (retMode == Pixy_UART_SEND_WAIT_REINIT) )
                {
                    do /*wait until transmit complete*/
                    {
                        tmpStat = Pixy_UART_TXSTATUS_REG;
                    }while(~tmpStat & Pixy_UART_TX_STS_COMPLETE);
                }

                if( (retMode == Pixy_UART_WAIT_FOR_COMPLETE_REINIT) ||
                    (retMode == Pixy_UART_REINIT) ||
                    (retMode == Pixy_UART_SEND_WAIT_REINIT) )
                {
                    Pixy_UART_WriteControlRegister(Pixy_UART_ReadControlRegister() &
                                                         ~Pixy_UART_CTRL_HD_SEND_BREAK);
                }

            #else /* Pixy_UART_HD_ENABLED Full Duplex mode */

                static uint8 tx_period; 
                
                if( (retMode == Pixy_UART_SEND_BREAK) ||
                    (retMode == Pixy_UART_SEND_WAIT_REINIT) )
                {
                    /* CTRL_HD_SEND_BREAK - skip to send parity bit @ Break signal in Full Duplex mode*/
                    if( (Pixy_UART_PARITY_TYPE != Pixy_UART__B_UART__NONE_REVB) ||
                         Pixy_UART_PARITY_TYPE_SW )
                    {
                        Pixy_UART_WriteControlRegister(Pixy_UART_ReadControlRegister() |
                                                              Pixy_UART_CTRL_HD_SEND_BREAK);
                    }                                                          

                    #if(Pixy_UART_TXCLKGEN_DP)
                        tx_period = Pixy_UART_TXBITCLKTX_COMPLETE_REG;
                        Pixy_UART_TXBITCLKTX_COMPLETE_REG = Pixy_UART_TXBITCTR_BREAKBITS;
                    #else
                        tx_period = Pixy_UART_TXBITCTR_PERIOD_REG;
                        Pixy_UART_TXBITCTR_PERIOD_REG = Pixy_UART_TXBITCTR_BREAKBITS8X;
                    #endif /* End Pixy_UART_TXCLKGEN_DP */

                    /* Send zeros*/
                    Pixy_UART_TXDATA_REG = 0u;

                    do /*wait until transmit starts*/
                    {
                        tmpStat = Pixy_UART_TXSTATUS_REG;
                    }while(tmpStat & Pixy_UART_TX_STS_FIFO_EMPTY);
                }

                if( (retMode == Pixy_UART_WAIT_FOR_COMPLETE_REINIT) ||
                    (retMode == Pixy_UART_SEND_WAIT_REINIT) )
                {
                    do /*wait until transmit complete*/
                    {
                        tmpStat = Pixy_UART_TXSTATUS_REG;
                    }while(~tmpStat & Pixy_UART_TX_STS_COMPLETE);
                }

                if( (retMode == Pixy_UART_WAIT_FOR_COMPLETE_REINIT) ||
                    (retMode == Pixy_UART_REINIT) ||
                    (retMode == Pixy_UART_SEND_WAIT_REINIT) )
                {

                    #if(Pixy_UART_TXCLKGEN_DP)
                        Pixy_UART_TXBITCLKTX_COMPLETE_REG = tx_period;
                    #else
                        Pixy_UART_TXBITCTR_PERIOD_REG = tx_period;
                    #endif /* End Pixy_UART_TXCLKGEN_DP */

                    if( (Pixy_UART_PARITY_TYPE != Pixy_UART__B_UART__NONE_REVB) || 
                         Pixy_UART_PARITY_TYPE_SW )
                    {
                        Pixy_UART_WriteControlRegister(Pixy_UART_ReadControlRegister() &
                                                             ~Pixy_UART_CTRL_HD_SEND_BREAK);
                    }                                     
                }
            #endif    /* End Pixy_UART_HD_ENABLED */
        }
    }


    /*******************************************************************************
    * Function Name: Pixy_UART_SetTxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Set the transmit addressing mode
    *
    * Parameters:
    *  addressMode: 0 -> Space
    *               1 -> Mark
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void Pixy_UART_SetTxAddressMode(uint8 addressMode) 
    {
        /* Mark/Space sending enable*/
        if(addressMode != 0)
        {
            Pixy_UART_WriteControlRegister(Pixy_UART_ReadControlRegister() | Pixy_UART_CTRL_MARK);
        }
        else
        {
            Pixy_UART_WriteControlRegister(Pixy_UART_ReadControlRegister() & ~Pixy_UART_CTRL_MARK);
        }
    }

#endif  /* EndPixy_UART_TX_ENABLED */

#if(Pixy_UART_HD_ENABLED)


    /*******************************************************************************
    * Function Name: Pixy_UART_LoadTxConfig
    ********************************************************************************
    *
    * Summary:
    *  Unloads the Rx configuration if required and loads the
    *  Tx configuration. It is the users responsibility to ensure that any
    *  transaction is complete and it is safe to unload the Tx
    *  configuration.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Valid only for half duplex UART. 
    *
    * Side Effects:
    *  Disable RX interrupt mask, when software buffer has been used.
    *
    *******************************************************************************/
    void Pixy_UART_LoadTxConfig(void) 
    {
        #if((Pixy_UART_RX_INTERRUPT_ENABLED) && (Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH))
            /* Disable RX interrupts before set TX configuration */
            Pixy_UART_SetRxInterruptMode(0);
        #endif /* Pixy_UART_RX_INTERRUPT_ENABLED */

        Pixy_UART_WriteControlRegister(Pixy_UART_ReadControlRegister() | Pixy_UART_CTRL_HD_SEND);
        Pixy_UART_RXBITCTR_PERIOD_REG = Pixy_UART_HD_TXBITCTR_INIT;
        #if(CY_UDB_V0) /* Manually clear status register when mode has been changed */
            /* Clear status register */
            CY_GET_REG8(Pixy_UART_RXSTATUS_PTR);
        #endif /* CY_UDB_V0 */
    }


    /*******************************************************************************
    * Function Name: Pixy_UART_LoadRxConfig
    ********************************************************************************
    *
    * Summary:
    *  Unloads the Tx configuration if required and loads the
    *  Rx configuration. It is the users responsibility to ensure that any
    *  transaction is complete and it is safe to unload the Rx
    *  configuration.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Valid only for half duplex UART
    *
    * Side Effects:
    *  Set RX interrupt mask based on customizer settings, when software buffer 
    *  has been used.
    *
    *******************************************************************************/
    void Pixy_UART_LoadRxConfig(void) 
    {
        Pixy_UART_WriteControlRegister(Pixy_UART_ReadControlRegister() & ~Pixy_UART_CTRL_HD_SEND);
        Pixy_UART_RXBITCTR_PERIOD_REG = Pixy_UART_HD_RXBITCTR_INIT;
        #if(CY_UDB_V0) /* Manually clear status register when mode has been changed */
            /* Clear status register */
            CY_GET_REG8(Pixy_UART_RXSTATUS_PTR);
        #endif /* CY_UDB_V0 */

        #if((Pixy_UART_RX_INTERRUPT_ENABLED) && (Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH))
            /* Enable RX interrupt after set RX configuration */
            Pixy_UART_SetRxInterruptMode(Pixy_UART_INIT_RX_INTERRUPTS_MASK);    
        #endif /* Pixy_UART_RX_INTERRUPT_ENABLED */
    }

#endif  /* Pixy_UART_HD_ENABLED */


/* [] END OF FILE */
