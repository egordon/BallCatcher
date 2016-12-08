/*******************************************************************************
* File Name: Gyro_ADC_AMux.h
* Version 1.60
*
*  Description:
*    This file contains the constants and function prototypes for the Analog
*    Multiplexer User Module AMux.
*
*   Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_AMUX_Gyro_ADC_AMux_H)
#define CY_AMUX_Gyro_ADC_AMux_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cyfitter_cfg.h"


/***************************************
*        Function Prototypes
***************************************/

void Gyro_ADC_AMux_Start(void);
# define Gyro_ADC_AMux_Init() Gyro_ADC_AMux_Start()
void Gyro_ADC_AMux_FastSelect(uint8 channel) ;
/* The Stop, Select, Connect, Disconnect and DisconnectAll functions are declared elsewhere */
/* void Gyro_ADC_AMux_Stop(void); */
/* void Gyro_ADC_AMux_Select(uint8 channel); */
/* void Gyro_ADC_AMux_Connect(uint8 channel); */
/* void Gyro_ADC_AMux_Disconnect(uint8 channel); */
/* void Gyro_ADC_AMux_DisconnectAll(void) */


/***************************************
*     Initial Parameter Constants
***************************************/

#define Gyro_ADC_AMux_CHANNELS  2
#define Gyro_ADC_AMux_MUXTYPE   1
#define Gyro_ADC_AMux_ATMOSTONE 0

/***************************************
*             API Constants
***************************************/

#define Gyro_ADC_AMux_NULL_CHANNEL  0xFFu
#define Gyro_ADC_AMux_MUX_SINGLE   1
#define Gyro_ADC_AMux_MUX_DIFF     2


/***************************************
*        Conditional Functions
***************************************/

#if Gyro_ADC_AMux_MUXTYPE == Gyro_ADC_AMux_MUX_SINGLE
#if !Gyro_ADC_AMux_ATMOSTONE
# define Gyro_ADC_AMux_Connect(channel) Gyro_ADC_AMux_Set(channel)
#endif
# define Gyro_ADC_AMux_Disconnect(channel) Gyro_ADC_AMux_Unset(channel)
#else
#if !Gyro_ADC_AMux_ATMOSTONE
void Gyro_ADC_AMux_Connect(uint8 channel) ;
#endif
void Gyro_ADC_AMux_Disconnect(uint8 channel) ;
#endif

#if Gyro_ADC_AMux_ATMOSTONE
# define Gyro_ADC_AMux_Stop() Gyro_ADC_AMux_DisconnectAll()
# define Gyro_ADC_AMux_Select(channel) Gyro_ADC_AMux_FastSelect(channel)
void Gyro_ADC_AMux_DisconnectAll(void) ;
#else
# define Gyro_ADC_AMux_Stop() Gyro_ADC_AMux_Start()
void Gyro_ADC_AMux_Select(uint8 channel) ;
# define Gyro_ADC_AMux_DisconnectAll() Gyro_ADC_AMux_Start()
#endif

#endif /* CY_AMUX_Gyro_ADC_AMux_H */


/* [] END OF FILE */
