/*******************************************************************************
* File Name: Pixy_UART_PM.c
* Version 2.20
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
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


/***************************************
* Local data allocation
***************************************/

static Pixy_UART_BACKUP_STRUCT  Pixy_UART_backup =
{
    /* enableState - disabled */
    0u,
};        



/*******************************************************************************
* Function Name: Pixy_UART_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  Pixy_UART_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Pixy_UART_SaveConfig(void)
{
    #if (CY_UDB_V0)

        #if(Pixy_UART_CONTROL_REG_REMOVED == 0u)
            Pixy_UART_backup.cr = Pixy_UART_CONTROL_REG;
        #endif /* End Pixy_UART_CONTROL_REG_REMOVED */

        #if( (Pixy_UART_RX_ENABLED) || (Pixy_UART_HD_ENABLED) )
            Pixy_UART_backup.rx_period = Pixy_UART_RXBITCTR_PERIOD_REG;
            Pixy_UART_backup.rx_mask = Pixy_UART_RXSTATUS_MASK_REG;
            #if (Pixy_UART_RXHW_ADDRESS_ENABLED)
                Pixy_UART_backup.rx_addr1 = Pixy_UART_RXADDRESS1_REG;
                Pixy_UART_backup.rx_addr2 = Pixy_UART_RXADDRESS2_REG;
            #endif /* End Pixy_UART_RXHW_ADDRESS_ENABLED */
        #endif /* End Pixy_UART_RX_ENABLED | Pixy_UART_HD_ENABLED*/

        #if(Pixy_UART_TX_ENABLED)
            #if(Pixy_UART_TXCLKGEN_DP)
                Pixy_UART_backup.tx_clk_ctr = Pixy_UART_TXBITCLKGEN_CTR_REG;
                Pixy_UART_backup.tx_clk_compl = Pixy_UART_TXBITCLKTX_COMPLETE_REG;
            #else
                Pixy_UART_backup.tx_period = Pixy_UART_TXBITCTR_PERIOD_REG;
            #endif /*End Pixy_UART_TXCLKGEN_DP */
            Pixy_UART_backup.tx_mask = Pixy_UART_TXSTATUS_MASK_REG;
        #endif /*End Pixy_UART_TX_ENABLED */

    
    #else /* CY_UDB_V1 */

        #if(Pixy_UART_CONTROL_REG_REMOVED == 0u)
            Pixy_UART_backup.cr = Pixy_UART_CONTROL_REG;
        #endif /* End Pixy_UART_CONTROL_REG_REMOVED */

    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: Pixy_UART_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  Pixy_UART_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Pixy_UART_RestoreConfig(void)
{

    #if (CY_UDB_V0)

        #if(Pixy_UART_CONTROL_REG_REMOVED == 0u)
            Pixy_UART_CONTROL_REG = Pixy_UART_backup.cr;
        #endif /* End Pixy_UART_CONTROL_REG_REMOVED */

        #if( (Pixy_UART_RX_ENABLED) || (Pixy_UART_HD_ENABLED) )
            Pixy_UART_RXBITCTR_PERIOD_REG = Pixy_UART_backup.rx_period;
            Pixy_UART_RXSTATUS_MASK_REG = Pixy_UART_backup.rx_mask;
            #if (Pixy_UART_RXHW_ADDRESS_ENABLED)
                Pixy_UART_RXADDRESS1_REG = Pixy_UART_backup.rx_addr1;
                Pixy_UART_RXADDRESS2_REG = Pixy_UART_backup.rx_addr2;
            #endif /* End Pixy_UART_RXHW_ADDRESS_ENABLED */
        #endif  /* End (Pixy_UART_RX_ENABLED) || (Pixy_UART_HD_ENABLED) */

        #if(Pixy_UART_TX_ENABLED)
            #if(Pixy_UART_TXCLKGEN_DP)
                Pixy_UART_TXBITCLKGEN_CTR_REG = Pixy_UART_backup.tx_clk_ctr;
                Pixy_UART_TXBITCLKTX_COMPLETE_REG = Pixy_UART_backup.tx_clk_compl;
            #else
                Pixy_UART_TXBITCTR_PERIOD_REG = Pixy_UART_backup.tx_period;
            #endif /*End Pixy_UART_TXCLKGEN_DP */
            Pixy_UART_TXSTATUS_MASK_REG = Pixy_UART_backup.tx_mask;
        #endif /*End Pixy_UART_TX_ENABLED */

    #else /* CY_UDB_V1 */

        #if(Pixy_UART_CONTROL_REG_REMOVED == 0u)
            Pixy_UART_CONTROL_REG = Pixy_UART_backup.cr;
        #endif /* End Pixy_UART_CONTROL_REG_REMOVED */

    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: Pixy_UART_Sleep
********************************************************************************
*
* Summary:
*  Stops and saves the user configuration. Should be called 
*  just prior to entering sleep.
*  
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  Pixy_UART_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Pixy_UART_Sleep(void)
{

    #if(Pixy_UART_RX_ENABLED || Pixy_UART_HD_ENABLED)
        if((Pixy_UART_RXSTATUS_ACTL_REG  & Pixy_UART_INT_ENABLE) != 0u) 
        {
            Pixy_UART_backup.enableState = 1u;
        }
        else
        {
            Pixy_UART_backup.enableState = 0u;
        }
    #else
        if((Pixy_UART_TXSTATUS_ACTL_REG  & Pixy_UART_INT_ENABLE) !=0u)
        {
            Pixy_UART_backup.enableState = 1u;
        }
        else
        {
            Pixy_UART_backup.enableState = 0u;
        }
    #endif /* End Pixy_UART_RX_ENABLED || Pixy_UART_HD_ENABLED*/

    Pixy_UART_Stop();
    Pixy_UART_SaveConfig();
}


/*******************************************************************************
* Function Name: Pixy_UART_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called
*  just after awaking from sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  Pixy_UART_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Pixy_UART_Wakeup(void)
{
    Pixy_UART_RestoreConfig();
    #if( (Pixy_UART_RX_ENABLED) || (Pixy_UART_HD_ENABLED) )
        Pixy_UART_ClearRxBuffer();
    #endif /* End (Pixy_UART_RX_ENABLED) || (Pixy_UART_HD_ENABLED) */
    #if(Pixy_UART_TX_ENABLED || Pixy_UART_HD_ENABLED)
        Pixy_UART_ClearTxBuffer();
    #endif /* End Pixy_UART_TX_ENABLED || Pixy_UART_HD_ENABLED */

    if(Pixy_UART_backup.enableState != 0u)
    {
        Pixy_UART_Enable();
    } 
}


/* [] END OF FILE */
