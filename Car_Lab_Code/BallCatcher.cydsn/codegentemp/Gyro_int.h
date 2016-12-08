/*******************************************************************************
* File Name: Gyro_int.h
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
#if !defined(__Gyro_int_INTC_H__)
#define __Gyro_int_INTC_H__


#include <cytypes.h>
#include <cyfitter.h>

#if(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3)     
    #if(CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2)      
        #include <intrins.h>
        #define Gyro_int_ISR_PATCH() _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
    #endif
#endif


/* Interrupt Controller API. */
void Gyro_int_Start(void);
void Gyro_int_StartEx(cyisraddress address);
void Gyro_int_Stop(void) ;

CY_ISR_PROTO(Gyro_int_Interrupt);

void Gyro_int_SetVector(cyisraddress address) ;
cyisraddress Gyro_int_GetVector(void) ;

void Gyro_int_SetPriority(uint8 priority) ;
uint8 Gyro_int_GetPriority(void) ;

void Gyro_int_Enable(void) ;
uint8 Gyro_int_GetState(void) ;
void Gyro_int_Disable(void) ;

void Gyro_int_SetPending(void) ;
void Gyro_int_ClearPending(void) ;


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the Gyro_int ISR. */
#define Gyro_int_INTC_VECTOR            ((reg16 *) Gyro_int__INTC_VECT)

/* Address of the Gyro_int ISR priority. */
#define Gyro_int_INTC_PRIOR             ((reg8 *) Gyro_int__INTC_PRIOR_REG)

/* Priority of the Gyro_int interrupt. */
#define Gyro_int_INTC_PRIOR_NUMBER      Gyro_int__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable Gyro_int interrupt. */
#define Gyro_int_INTC_SET_EN            ((reg8 *) Gyro_int__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the Gyro_int interrupt. */
#define Gyro_int_INTC_CLR_EN            ((reg8 *) Gyro_int__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the Gyro_int interrupt state to pending. */
#define Gyro_int_INTC_SET_PD            ((reg8 *) Gyro_int__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the Gyro_int interrupt. */
#define Gyro_int_INTC_CLR_PD            ((reg8 *) Gyro_int__INTC_CLR_PD_REG)



/* __Gyro_int_INTC_H__ */
#endif
