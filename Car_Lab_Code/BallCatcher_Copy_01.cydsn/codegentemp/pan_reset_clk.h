/*******************************************************************************
* File Name: pan_reset_clk.h
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

#if !defined(CY_CLOCK_pan_reset_clk_H)
#define CY_CLOCK_pan_reset_clk_H

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

void pan_reset_clk_Start(void) ;
void pan_reset_clk_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void pan_reset_clk_StopBlock(void) ;
#endif

void pan_reset_clk_StandbyPower(uint8 state) ;
void pan_reset_clk_SetDividerRegister(uint16 clkDivider, uint8 reset) ;
uint16 pan_reset_clk_GetDividerRegister(void) ;
void pan_reset_clk_SetModeRegister(uint8 modeBitMask) ;
void pan_reset_clk_ClearModeRegister(uint8 modeBitMask) ;
uint8 pan_reset_clk_GetModeRegister(void) ;
void pan_reset_clk_SetSourceRegister(uint8 clkSource) ;
uint8 pan_reset_clk_GetSourceRegister(void) ;
#if defined(pan_reset_clk__CFG3)
void pan_reset_clk_SetPhaseRegister(uint8 clkPhase) ;
uint8 pan_reset_clk_GetPhaseRegister(void) ;
#endif

#define pan_reset_clk_Enable()                       pan_reset_clk_Start()
#define pan_reset_clk_Disable()                      pan_reset_clk_Stop()
#define pan_reset_clk_SetDivider(clkDivider)         pan_reset_clk_SetDividerRegister(clkDivider, 1)
#define pan_reset_clk_SetDividerValue(clkDivider)    pan_reset_clk_SetDividerRegister((clkDivider) - 1, 1)
#define pan_reset_clk_SetMode(clkMode)               pan_reset_clk_SetModeRegister(clkMode)
#define pan_reset_clk_SetSource(clkSource)           pan_reset_clk_SetSourceRegister(clkSource)
#if defined(pan_reset_clk__CFG3)
#define pan_reset_clk_SetPhase(clkPhase)             pan_reset_clk_SetPhaseRegister(clkPhase)
#define pan_reset_clk_SetPhaseValue(clkPhase)        pan_reset_clk_SetPhaseRegister((clkPhase) + 1)
#endif


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define pan_reset_clk_CLKEN              (* (reg8 *) pan_reset_clk__PM_ACT_CFG)
#define pan_reset_clk_CLKEN_PTR          ((reg8 *) pan_reset_clk__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define pan_reset_clk_CLKSTBY            (* (reg8 *) pan_reset_clk__PM_STBY_CFG)
#define pan_reset_clk_CLKSTBY_PTR        ((reg8 *) pan_reset_clk__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define pan_reset_clk_DIV_LSB            (* (reg8 *) pan_reset_clk__CFG0)
#define pan_reset_clk_DIV_LSB_PTR        ((reg8 *) pan_reset_clk__CFG0)
#define pan_reset_clk_DIV_PTR            ((reg16 *) pan_reset_clk__CFG0)

/* Clock MSB divider configuration register. */
#define pan_reset_clk_DIV_MSB            (* (reg8 *) pan_reset_clk__CFG1)
#define pan_reset_clk_DIV_MSB_PTR        ((reg8 *) pan_reset_clk__CFG1)

/* Mode and source configuration register */
#define pan_reset_clk_MOD_SRC            (* (reg8 *) pan_reset_clk__CFG2)
#define pan_reset_clk_MOD_SRC_PTR        ((reg8 *) pan_reset_clk__CFG2)

#if defined(pan_reset_clk__CFG3)
/* Analog clock phase configuration register */
#define pan_reset_clk_PHASE              (* (reg8 *) pan_reset_clk__CFG3)
#define pan_reset_clk_PHASE_PTR          ((reg8 *) pan_reset_clk__CFG3)
#endif


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define pan_reset_clk_CLKEN_MASK         pan_reset_clk__PM_ACT_MSK
#define pan_reset_clk_CLKSTBY_MASK       pan_reset_clk__PM_STBY_MSK

/* CFG2 field masks */
#define pan_reset_clk_SRC_SEL_MSK        pan_reset_clk__CFG2_SRC_SEL_MASK
#define pan_reset_clk_MODE_MASK          (~(pan_reset_clk_SRC_SEL_MSK))

#if defined(pan_reset_clk__CFG3)
/* CFG3 phase mask */
#define pan_reset_clk_PHASE_MASK         pan_reset_clk__CFG3_PHASE_DLY_MASK
#endif

#endif /* CY_CLOCK_pan_reset_clk_H */


/* [] END OF FILE */
