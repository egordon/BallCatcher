/*******************************************************************************
* File Name: clk_int.h
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
#if !defined(__clk_int_INTC_H__)
#define __clk_int_INTC_H__


#include <cytypes.h>
#include <cyfitter.h>

#if(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3)     
    #if(CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2)      
        #include <intrins.h>
        #define clk_int_ISR_PATCH() _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
    #endif
#endif


/* Interrupt Controller API. */
void clk_int_Start(void);
void clk_int_StartEx(cyisraddress address);
void clk_int_Stop(void) ;

CY_ISR_PROTO(clk_int_Interrupt);

void clk_int_SetVector(cyisraddress address) ;
cyisraddress clk_int_GetVector(void) ;

void clk_int_SetPriority(uint8 priority) ;
uint8 clk_int_GetPriority(void) ;

void clk_int_Enable(void) ;
uint8 clk_int_GetState(void) ;
void clk_int_Disable(void) ;

void clk_int_SetPending(void) ;
void clk_int_ClearPending(void) ;


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the clk_int ISR. */
#define clk_int_INTC_VECTOR            ((reg16 *) clk_int__INTC_VECT)

/* Address of the clk_int ISR priority. */
#define clk_int_INTC_PRIOR             ((reg8 *) clk_int__INTC_PRIOR_REG)

/* Priority of the clk_int interrupt. */
#define clk_int_INTC_PRIOR_NUMBER      clk_int__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable clk_int interrupt. */
#define clk_int_INTC_SET_EN            ((reg8 *) clk_int__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the clk_int interrupt. */
#define clk_int_INTC_CLR_EN            ((reg8 *) clk_int__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the clk_int interrupt state to pending. */
#define clk_int_INTC_SET_PD            ((reg8 *) clk_int__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the clk_int interrupt. */
#define clk_int_INTC_CLR_PD            ((reg8 *) clk_int__INTC_CLR_PD_REG)



/* __clk_int_INTC_H__ */
#endif
