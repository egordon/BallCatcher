/*******************************************************************************
* File Name: CountRise_int.h
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
#if !defined(__CountRise_int_INTC_H__)
#define __CountRise_int_INTC_H__


#include <cytypes.h>
#include <cyfitter.h>

#if(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3)     
    #if(CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2)      
        #include <intrins.h>
        #define CountRise_int_ISR_PATCH() _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
    #endif
#endif


/* Interrupt Controller API. */
void CountRise_int_Start(void);
void CountRise_int_StartEx(cyisraddress address);
void CountRise_int_Stop(void) ;

CY_ISR_PROTO(CountRise_int_Interrupt);

void CountRise_int_SetVector(cyisraddress address) ;
cyisraddress CountRise_int_GetVector(void) ;

void CountRise_int_SetPriority(uint8 priority) ;
uint8 CountRise_int_GetPriority(void) ;

void CountRise_int_Enable(void) ;
uint8 CountRise_int_GetState(void) ;
void CountRise_int_Disable(void) ;

void CountRise_int_SetPending(void) ;
void CountRise_int_ClearPending(void) ;


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the CountRise_int ISR. */
#define CountRise_int_INTC_VECTOR            ((reg16 *) CountRise_int__INTC_VECT)

/* Address of the CountRise_int ISR priority. */
#define CountRise_int_INTC_PRIOR             ((reg8 *) CountRise_int__INTC_PRIOR_REG)

/* Priority of the CountRise_int interrupt. */
#define CountRise_int_INTC_PRIOR_NUMBER      CountRise_int__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable CountRise_int interrupt. */
#define CountRise_int_INTC_SET_EN            ((reg8 *) CountRise_int__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the CountRise_int interrupt. */
#define CountRise_int_INTC_CLR_EN            ((reg8 *) CountRise_int__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the CountRise_int interrupt state to pending. */
#define CountRise_int_INTC_SET_PD            ((reg8 *) CountRise_int__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the CountRise_int interrupt. */
#define CountRise_int_INTC_CLR_PD            ((reg8 *) CountRise_int__INTC_CLR_PD_REG)



/* __CountRise_int_INTC_H__ */
#endif
