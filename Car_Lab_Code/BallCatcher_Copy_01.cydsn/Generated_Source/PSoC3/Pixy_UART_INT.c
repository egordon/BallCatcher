/*******************************************************************************
* File Name: Pixy_UART_INT.c
* Version 2.20
*
* Description:
*  This file provides all Interrupt Service functionality of the UART component
*
* Note:
*  Any unusual or non-standard behavior should be noted here. Other-
*  wise, this section should remain blank.
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
* Custom Declratations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if( (Pixy_UART_RX_ENABLED || Pixy_UART_HD_ENABLED) && \
     (Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH))

    extern volatile uint8 Pixy_UART_rxBuffer[];
    extern volatile uint8 Pixy_UART_rxBufferRead;
    extern volatile uint8 Pixy_UART_rxBufferWrite;
    extern volatile uint8 Pixy_UART_rxBufferLoopDetect;
    extern volatile uint8 Pixy_UART_rxBufferOverflow;
    #if (Pixy_UART_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 Pixy_UART_rxAddressMode;
        extern volatile uint8 Pixy_UART_rxAddressDetected;
    #endif /* End EnableHWAddress */    

    /*******************************************************************************
    * Function Name: Pixy_UART_RXISR
    ********************************************************************************
    *
    * Summary:
    *  Interrupt Service Routine for RX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  Pixy_UART_rxBuffer - RAM buffer pointer for save received data.
    *  Pixy_UART_rxBufferWrite - cyclic index for write to rxBuffer, 
    *     increments after each byte saved to buffer.
    *  Pixy_UART_rxBufferRead - cyclic index for read from rxBuffer, 
    *     checked to detect overflow condition.
    *  Pixy_UART_rxBufferOverflow - software overflow flag. Set to one
    *     when Pixy_UART_rxBufferWrite index overtakes 
    *     Pixy_UART_rxBufferRead index.
    *  Pixy_UART_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when Pixy_UART_rxBufferWrite is equal to 
    *    Pixy_UART_rxBufferRead
    *  Pixy_UART_rxAddressMode - this variable contains the Address mode, 
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  Pixy_UART_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(Pixy_UART_RXISR)
    {
        uint8 readData;
        uint8 increment_pointer = 0u;

        /* User code required at start of ISR */
        /* `#START Pixy_UART_RXISR_START` */

        /* `#END` */

        readData = Pixy_UART_RXSTATUS_REG;

        if((readData & (Pixy_UART_RX_STS_BREAK | Pixy_UART_RX_STS_PAR_ERROR |
                        Pixy_UART_RX_STS_STOP_ERROR | Pixy_UART_RX_STS_OVERRUN)) != 0u)
        {
            /* ERROR handling. */
            /* `#START Pixy_UART_RXISR_ERROR` */

            /* `#END` */
        }

        while(readData & Pixy_UART_RX_STS_FIFO_NOTEMPTY)
        {
            
            #if (Pixy_UART_RXHW_ADDRESS_ENABLED)
                if(Pixy_UART_rxAddressMode == Pixy_UART__B_UART__AM_SW_DETECT_TO_BUFFER) 
                {
                    if((readData & Pixy_UART_RX_STS_MRKSPC) != 0u)
                    {  
                        if ((readData & Pixy_UART_RX_STS_ADDR_MATCH) != 0u)
                        {
                            Pixy_UART_rxAddressDetected = 1u;
                        }
                        else
                        {
                            Pixy_UART_rxAddressDetected = 0u;
                        }
                    }

                    readData = Pixy_UART_RXDATA_REG;
                    if(Pixy_UART_rxAddressDetected != 0u)
                    {   /* store only addressed data */
                        Pixy_UART_rxBuffer[Pixy_UART_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* without software addressing */
                {
                    Pixy_UART_rxBuffer[Pixy_UART_rxBufferWrite] = Pixy_UART_RXDATA_REG;
                    increment_pointer = 1u;
                }
            #else  /* without addressing */
                Pixy_UART_rxBuffer[Pixy_UART_rxBufferWrite] = Pixy_UART_RXDATA_REG;
                increment_pointer = 1u;
            #endif /* End SW_DETECT_TO_BUFFER */
            
            /* do not increment buffer pointer when skip not adderessed data */
            if( increment_pointer != 0u )
            {
                if(Pixy_UART_rxBufferLoopDetect)
                {   /* Set Software Buffer status Overflow */
                    Pixy_UART_rxBufferOverflow = 1u;
                }
                /* Set next pointer. */
                Pixy_UART_rxBufferWrite++;

                /* Check pointer for a loop condition */
                if(Pixy_UART_rxBufferWrite >= Pixy_UART_RXBUFFERSIZE)
                {
                    Pixy_UART_rxBufferWrite = 0u;
                }
                /* Detect pre-overload condition and set flag */
                if(Pixy_UART_rxBufferWrite == Pixy_UART_rxBufferRead)
                {
                    Pixy_UART_rxBufferLoopDetect = 1u;
                    /* When Hardware Flow Control selected */
                    #if(Pixy_UART_FLOW_CONTROL != 0u)
                    /* Disable RX interrupt mask, it will be enabled when user read data from the buffer using APIs */
                        Pixy_UART_RXSTATUS_MASK_REG  &= ~Pixy_UART_RX_STS_FIFO_NOTEMPTY;
                        CyIntClearPending(Pixy_UART_RX_VECT_NUM); 
                        break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                    #endif /* End Pixy_UART_FLOW_CONTROL != 0 */    
                }
            }

            /* Check again if there is data. */
            readData = Pixy_UART_RXSTATUS_REG;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START Pixy_UART_RXISR_END` */

        /* `#END` */

    }

#endif /* End Pixy_UART_RX_ENABLED && (Pixy_UART_RXBUFFERSIZE > Pixy_UART_FIFO_LENGTH) */


#if(Pixy_UART_TX_ENABLED && (Pixy_UART_TXBUFFERSIZE > Pixy_UART_FIFO_LENGTH))

    extern volatile uint8 Pixy_UART_txBuffer[];
    extern volatile uint8 Pixy_UART_txBufferRead;
    extern uint8 Pixy_UART_txBufferWrite;


    /*******************************************************************************
    * Function Name: Pixy_UART_TXISR
    ********************************************************************************
    *
    * Summary:
    * Interrupt Service Routine for the TX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  Pixy_UART_txBuffer - RAM buffer pointer for transmit data from.
    *  Pixy_UART_txBufferRead - cyclic index for read and transmit data 
    *     from txBuffer, increments after each transmited byte.
    *  Pixy_UART_rxBufferWrite - cyclic index for write to txBuffer, 
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(Pixy_UART_TXISR)
    {

        /* User code required at start of ISR */
        /* `#START Pixy_UART_TXISR_START` */

        /* `#END` */

        while((Pixy_UART_txBufferRead != Pixy_UART_txBufferWrite) && \
             !(Pixy_UART_TXSTATUS_REG & Pixy_UART_TX_STS_FIFO_FULL))
        {
            /* Check pointer. */
            if(Pixy_UART_txBufferRead >= Pixy_UART_TXBUFFERSIZE)
            {
                Pixy_UART_txBufferRead = 0u;
            }

            Pixy_UART_TXDATA_REG = Pixy_UART_txBuffer[Pixy_UART_txBufferRead];

            /* Set next pointer. */
            Pixy_UART_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START Pixy_UART_TXISR_END` */

        /* `#END` */
        
    }

#endif /* End Pixy_UART_TX_ENABLED && (Pixy_UART_TXBUFFERSIZE > Pixy_UART_FIFO_LENGTH) */


/* [] END OF FILE */
