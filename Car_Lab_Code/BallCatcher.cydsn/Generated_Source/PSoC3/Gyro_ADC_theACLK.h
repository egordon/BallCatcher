/*******************************************************************************
* File Name: Gyro_ADC_theACLK.h
* Version 1.70
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_CLOCK_Gyro_ADC_theACLK_H)
#define CY_CLOCK_Gyro_ADC_theACLK_H

#include <cytypes.h>
#include <cyfitter.h>

/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v1_70 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

/***************************************
*        Function Prototypes
***************************************/

void Gyro_ADC_theACLK_Start(void) ;
void Gyro_ADC_theACLK_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void Gyro_ADC_theACLK_StopBlock(void) ;
#endif

void Gyro_ADC_theACLK_StandbyPower(uint8 state) ;
void Gyro_ADC_theACLK_SetDividerRegister(uint16 clkDivider, uint8 reset) ;
uint16 Gyro_ADC_theACLK_GetDividerRegister(void) ;
void Gyro_ADC_theACLK_SetModeRegister(uint8 modeBitMask) ;
void Gyro_ADC_theACLK_ClearModeRegister(uint8 modeBitMask) ;
uint8 Gyro_ADC_theACLK_GetModeRegister(void) ;
void Gyro_ADC_theACLK_SetSourceRegister(uint8 clkSource) ;
uint8 Gyro_ADC_theACLK_GetSourceRegister(void) ;
#if defined(Gyro_ADC_theACLK__CFG3)
void Gyro_ADC_theACLK_SetPhaseRegister(uint8 clkPhase) ;
uint8 Gyro_ADC_theACLK_GetPhaseRegister(void) ;
#endif

#define Gyro_ADC_theACLK_Enable()                       Gyro_ADC_theACLK_Start()
#define Gyro_ADC_theACLK_Disable()                      Gyro_ADC_theACLK_Stop()
#define Gyro_ADC_theACLK_SetDivider(clkDivider)         Gyro_ADC_theACLK_SetDividerRegister(clkDivider, 1)
#define Gyro_ADC_theACLK_SetDividerValue(clkDivider)    Gyro_ADC_theACLK_SetDividerRegister((clkDivider) - 1, 1)
#define Gyro_ADC_theACLK_SetMode(clkMode)               Gyro_ADC_theACLK_SetModeRegister(clkMode)
#define Gyro_ADC_theACLK_SetSource(clkSource)           Gyro_ADC_theACLK_SetSourceRegister(clkSource)
#if defined(Gyro_ADC_theACLK__CFG3)
#define Gyro_ADC_theACLK_SetPhase(clkPhase)             Gyro_ADC_theACLK_SetPhaseRegister(clkPhase)
#define Gyro_ADC_theACLK_SetPhaseValue(clkPhase)        Gyro_ADC_theACLK_SetPhaseRegister((clkPhase) + 1)
#endif


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define Gyro_ADC_theACLK_CLKEN              (* (reg8 *) Gyro_ADC_theACLK__PM_ACT_CFG)
#define Gyro_ADC_theACLK_CLKEN_PTR          ((reg8 *) Gyro_ADC_theACLK__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define Gyro_ADC_theACLK_CLKSTBY            (* (reg8 *) Gyro_ADC_theACLK__PM_STBY_CFG)
#define Gyro_ADC_theACLK_CLKSTBY_PTR        ((reg8 *) Gyro_ADC_theACLK__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define Gyro_ADC_theACLK_DIV_LSB            (* (reg8 *) Gyro_ADC_theACLK__CFG0)
#define Gyro_ADC_theACLK_DIV_LSB_PTR        ((reg8 *) Gyro_ADC_theACLK__CFG0)
#define Gyro_ADC_theACLK_DIV_PTR            ((reg16 *) Gyro_ADC_theACLK__CFG0)

/* Clock MSB divider configuration register. */
#define Gyro_ADC_theACLK_DIV_MSB            (* (reg8 *) Gyro_ADC_theACLK__CFG1)
#define Gyro_ADC_theACLK_DIV_MSB_PTR        ((reg8 *) Gyro_ADC_theACLK__CFG1)

/* Mode and source configuration register */
#define Gyro_ADC_theACLK_MOD_SRC            (* (reg8 *) Gyro_ADC_theACLK__CFG2)
#define Gyro_ADC_theACLK_MOD_SRC_PTR        ((reg8 *) Gyro_ADC_theACLK__CFG2)

#if defined(Gyro_ADC_theACLK__CFG3)
/* Analog clock phase configuration register */
#define Gyro_ADC_theACLK_PHASE              (* (reg8 *) Gyro_ADC_theACLK__CFG3)
#define Gyro_ADC_theACLK_PHASE_PTR          ((reg8 *) Gyro_ADC_theACLK__CFG3)
#endif


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define Gyro_ADC_theACLK_CLKEN_MASK         Gyro_ADC_theACLK__PM_ACT_MSK
#define Gyro_ADC_theACLK_CLKSTBY_MASK       Gyro_ADC_theACLK__PM_STBY_MSK

/* CFG2 field masks */
#define Gyro_ADC_theACLK_SRC_SEL_MSK        Gyro_ADC_theACLK__CFG2_SRC_SEL_MASK
#define Gyro_ADC_theACLK_MODE_MASK          (~(Gyro_ADC_theACLK_SRC_SEL_MSK))

#if defined(Gyro_ADC_theACLK__CFG3)
/* CFG3 phase mask */
#define Gyro_ADC_theACLK_PHASE_MASK         Gyro_ADC_theACLK__CFG3_PHASE_DLY_MASK
#endif

#endif /* CY_CLOCK_Gyro_ADC_theACLK_H */


/* [] END OF FILE */
