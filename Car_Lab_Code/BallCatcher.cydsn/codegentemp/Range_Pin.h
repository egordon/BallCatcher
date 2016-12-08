/*******************************************************************************
* File Name: Range_Pin.h  
* Version 1.70
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PINS_Range_Pin_H) /* Pins Range_Pin_H */
#define CY_PINS_Range_Pin_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Range_Pin_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Range_Pin_Write(uint8 value) ;
void    Range_Pin_SetDriveMode(uint8 mode) ;
uint8   Range_Pin_ReadDataReg(void) ;
uint8   Range_Pin_Read(void) ;
uint8   Range_Pin_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Range_Pin_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Range_Pin_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Range_Pin_DM_RES_UP          PIN_DM_RES_UP
#define Range_Pin_DM_RES_DWN         PIN_DM_RES_DWN
#define Range_Pin_DM_OD_LO           PIN_DM_OD_LO
#define Range_Pin_DM_OD_HI           PIN_DM_OD_HI
#define Range_Pin_DM_STRONG          PIN_DM_STRONG
#define Range_Pin_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Range_Pin_MASK               Range_Pin__MASK
#define Range_Pin_SHIFT              Range_Pin__SHIFT
#define Range_Pin_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Range_Pin_PS                     (* (reg8 *) Range_Pin__PS)
/* Data Register */
#define Range_Pin_DR                     (* (reg8 *) Range_Pin__DR)
/* Port Number */
#define Range_Pin_PRT_NUM                (* (reg8 *) Range_Pin__PRT) 
/* Connect to Analog Globals */                                                  
#define Range_Pin_AG                     (* (reg8 *) Range_Pin__AG)                       
/* Analog MUX bux enable */
#define Range_Pin_AMUX                   (* (reg8 *) Range_Pin__AMUX) 
/* Bidirectional Enable */                                                        
#define Range_Pin_BIE                    (* (reg8 *) Range_Pin__BIE)
/* Bit-mask for Aliased Register Access */
#define Range_Pin_BIT_MASK               (* (reg8 *) Range_Pin__BIT_MASK)
/* Bypass Enable */
#define Range_Pin_BYP                    (* (reg8 *) Range_Pin__BYP)
/* Port wide control signals */                                                   
#define Range_Pin_CTL                    (* (reg8 *) Range_Pin__CTL)
/* Drive Modes */
#define Range_Pin_DM0                    (* (reg8 *) Range_Pin__DM0) 
#define Range_Pin_DM1                    (* (reg8 *) Range_Pin__DM1)
#define Range_Pin_DM2                    (* (reg8 *) Range_Pin__DM2) 
/* Input Buffer Disable Override */
#define Range_Pin_INP_DIS                (* (reg8 *) Range_Pin__INP_DIS)
/* LCD Common or Segment Drive */
#define Range_Pin_LCD_COM_SEG            (* (reg8 *) Range_Pin__LCD_COM_SEG)
/* Enable Segment LCD */
#define Range_Pin_LCD_EN                 (* (reg8 *) Range_Pin__LCD_EN)
/* Slew Rate Control */
#define Range_Pin_SLW                    (* (reg8 *) Range_Pin__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Range_Pin_PRTDSI__CAPS_SEL       (* (reg8 *) Range_Pin__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Range_Pin_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Range_Pin__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Range_Pin_PRTDSI__OE_SEL0        (* (reg8 *) Range_Pin__PRTDSI__OE_SEL0) 
#define Range_Pin_PRTDSI__OE_SEL1        (* (reg8 *) Range_Pin__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Range_Pin_PRTDSI__OUT_SEL0       (* (reg8 *) Range_Pin__PRTDSI__OUT_SEL0) 
#define Range_Pin_PRTDSI__OUT_SEL1       (* (reg8 *) Range_Pin__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Range_Pin_PRTDSI__SYNC_OUT       (* (reg8 *) Range_Pin__PRTDSI__SYNC_OUT) 


#if defined(Range_Pin__INTSTAT)  /* Interrupt Registers */

    #define Range_Pin_INTSTAT                (* (reg8 *) Range_Pin__INTSTAT)
    #define Range_Pin_SNAP                   (* (reg8 *) Range_Pin__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins Range_Pin_H */


/* [] END OF FILE */
