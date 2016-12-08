/*******************************************************************************
* File Name: Range_int.h
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
#if !defined(__Range_int_INTC_H__)
#define __Range_int_INTC_H__


#include <cytypes.h>
#include <cyfitter.h>

#if(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3)     
    #if(CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2)      
        #include <intrins.h>
        #define Range_int_ISR_PATCH() _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
    #endif
#endif


/* Interrupt Controller API. */
void Range_int_Start(void);
void Range_int_StartEx(cyisraddress address);
void Range_int_Stop(void) ;

CY_ISR_PROTO(Range_int_Interrupt);

void Range_int_SetVector(cyisraddress address) ;
cyisraddress Range_int_GetVector(void) ;

void Range_int_SetPriority(uint8 priority) ;
uint8 Range_int_GetPriority(void) ;

void Range_int_Enable(void) ;
uint8 Range_int_GetState(void) ;
void Range_int_Disable(void) ;

void Range_int_SetPending(void) ;
void Range_int_ClearPending(void) ;


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the Range_int ISR. */
#define Range_int_INTC_VECTOR            ((reg16 *) Range_int__INTC_VECT)

/* Address of the Range_int ISR priority. */
#define Range_int_INTC_PRIOR             ((reg8 *) Range_int__INTC_PRIOR_REG)

/* Priority of the Range_int interrupt. */
#define Range_int_INTC_PRIOR_NUMBER      Range_int__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable Range_int interrupt. */
#define Range_int_INTC_SET_EN            ((reg8 *) Range_int__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the Range_int interrupt. */
#define Range_int_INTC_CLR_EN            ((reg8 *) Range_int__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the Range_int interrupt state to pending. */
#define Range_int_INTC_SET_PD            ((reg8 *) Range_int__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the Range_int interrupt. */
#define Range_int_INTC_CLR_PD            ((reg8 *) Range_int__INTC_CLR_PD_REG)



/* __Range_int_INTC_H__ */
#endif
