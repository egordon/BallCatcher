/*******************************************************************************
* File Name: Gyro_ADC_AMux.c
* Version 1.60
*
*  Description:
*    This file contains all functions required for the analog multiplexer
*    AMux User Module.
*
*   Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "Gyro_ADC_AMux.h"

uint8 Gyro_ADC_AMux_lastChannel = Gyro_ADC_AMux_NULL_CHANNEL;


/*******************************************************************************
* Function Name: Gyro_ADC_AMux_Start
********************************************************************************
* Summary:
*  Disconnect all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Gyro_ADC_AMux_Start(void)
{
    uint8 chan;

    for(chan = 0; chan < Gyro_ADC_AMux_CHANNELS ; chan++)
    {
#if(Gyro_ADC_AMux_MUXTYPE == Gyro_ADC_AMux_MUX_SINGLE)
        Gyro_ADC_AMux_Unset(chan);
#else
        Gyro_ADC_AMux_CYAMUXSIDE_A_Unset(chan);
        Gyro_ADC_AMux_CYAMUXSIDE_B_Unset(chan);
#endif
    }

	Gyro_ADC_AMux_lastChannel = Gyro_ADC_AMux_NULL_CHANNEL;
}


#if(!Gyro_ADC_AMux_ATMOSTONE)
/*******************************************************************************
* Function Name: Gyro_ADC_AMux_Select
********************************************************************************
* Summary:
*  This functions first disconnects all channels then connects the given
*  channel.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void Gyro_ADC_AMux_Select(uint8 channel) 
{
    Gyro_ADC_AMux_DisconnectAll();        /* Disconnect all previous connections */
    Gyro_ADC_AMux_Connect(channel);       /* Make the given selection */
    Gyro_ADC_AMux_lastChannel = channel;  /* Update last channel */
}
#endif


/*******************************************************************************
* Function Name: Gyro_ADC_AMux_FastSelect
********************************************************************************
* Summary:
*  This function first disconnects the last connection made with FastSelect or
*  Select, then connects the given channel. The FastSelect function is similar
*  to the Select function, except it is faster since it only disconnects the
*  last channel selected rather than all channels.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void Gyro_ADC_AMux_FastSelect(uint8 channel) 
{
    /* Disconnect the last valid channel */
    if( Gyro_ADC_AMux_lastChannel != Gyro_ADC_AMux_NULL_CHANNEL)
    {
        Gyro_ADC_AMux_Disconnect(Gyro_ADC_AMux_lastChannel);
    }

    /* Make the new channel connection */
#if(Gyro_ADC_AMux_MUXTYPE == Gyro_ADC_AMux_MUX_SINGLE)
    Gyro_ADC_AMux_Set(channel);
#else
    Gyro_ADC_AMux_CYAMUXSIDE_A_Set(channel);
    Gyro_ADC_AMux_CYAMUXSIDE_B_Set(channel);
#endif


	Gyro_ADC_AMux_lastChannel = channel;   /* Update last channel */
}


#if(Gyro_ADC_AMux_MUXTYPE == Gyro_ADC_AMux_MUX_DIFF)
#if(!Gyro_ADC_AMux_ATMOSTONE)
/*******************************************************************************
* Function Name: Gyro_ADC_AMux_Connect
********************************************************************************
* Summary:
*  This function connects the given channel without affecting other connections.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void Gyro_ADC_AMux_Connect(uint8 channel) 
{
    Gyro_ADC_AMux_CYAMUXSIDE_A_Set(channel);
    Gyro_ADC_AMux_CYAMUXSIDE_B_Set(channel);
}
#endif

/*******************************************************************************
* Function Name: Gyro_ADC_AMux_Disconnect
********************************************************************************
* Summary:
*  This function disconnects the given channel from the common or output
*  terminal without affecting other connections.
*
* Parameters:
*  channel:  The channel to disconnect from the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void Gyro_ADC_AMux_Disconnect(uint8 channel) 
{
    Gyro_ADC_AMux_CYAMUXSIDE_A_Unset(channel);
    Gyro_ADC_AMux_CYAMUXSIDE_B_Unset(channel);
}
#endif

#if(Gyro_ADC_AMux_ATMOSTONE)
/*******************************************************************************
* Function Name: Gyro_ADC_AMux_DisconnectAll
********************************************************************************
* Summary:
*  This function disconnects all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Gyro_ADC_AMux_DisconnectAll(void) 
{
    if(Gyro_ADC_AMux_lastChannel != Gyro_ADC_AMux_NULL_CHANNEL) 
    {
        Gyro_ADC_AMux_Disconnect(Gyro_ADC_AMux_lastChannel);
		Gyro_ADC_AMux_lastChannel = Gyro_ADC_AMux_NULL_CHANNEL;
    }
}
#endif

/* [] END OF FILE */
