/*******************************************************************************
* File Name: Clk_int.h
* Version 1.60
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/
#if !defined(__Clk_int_INTC_H__)
#define __Clk_int_INTC_H__


#include <cytypes.h>
#include <cyfitter.h>

#if(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3)     
    #if(CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2)      
        #include <intrins.h>
        #define Clk_int_ISR_PATCH() _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
    #endif
#endif


/* Interrupt Controller API. */
void Clk_int_Start(void);
void Clk_int_StartEx(cyisraddress address);
void Clk_int_Stop(void) ;

CY_ISR_PROTO(Clk_int_Interrupt);

void Clk_int_SetVector(cyisraddress address) ;
cyisraddress Clk_int_GetVector(void) ;

void Clk_int_SetPriority(uint8 priority) ;
uint8 Clk_int_GetPriority(void) ;

void Clk_int_Enable(void) ;
uint8 Clk_int_GetState(void) ;
void Clk_int_Disable(void) ;

void Clk_int_SetPending(void) ;
void Clk_int_ClearPending(void) ;


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the Clk_int ISR. */
#define Clk_int_INTC_VECTOR            ((reg16 *) Clk_int__INTC_VECT)

/* Address of the Clk_int ISR priority. */
#define Clk_int_INTC_PRIOR             ((reg8 *) Clk_int__INTC_PRIOR_REG)

/* Priority of the Clk_int interrupt. */
#define Clk_int_INTC_PRIOR_NUMBER      Clk_int__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable Clk_int interrupt. */
#define Clk_int_INTC_SET_EN            ((reg8 *) Clk_int__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the Clk_int interrupt. */
#define Clk_int_INTC_CLR_EN            ((reg8 *) Clk_int__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the Clk_int interrupt state to pending. */
#define Clk_int_INTC_SET_PD            ((reg8 *) Clk_int__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the Clk_int interrupt. */
#define Clk_int_INTC_CLR_PD            ((reg8 *) Clk_int__INTC_CLR_PD_REG)



/* __Clk_int_INTC_H__ */
#endif
