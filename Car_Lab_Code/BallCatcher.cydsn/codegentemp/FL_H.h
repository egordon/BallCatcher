/*******************************************************************************
* File Name: FL_H.h  
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

#if !defined(CY_PINS_FL_H_H) /* Pins FL_H_H */
#define CY_PINS_FL_H_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "FL_H_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    FL_H_Write(uint8 value) ;
void    FL_H_SetDriveMode(uint8 mode) ;
uint8   FL_H_ReadDataReg(void) ;
uint8   FL_H_Read(void) ;
uint8   FL_H_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define FL_H_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define FL_H_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define FL_H_DM_RES_UP          PIN_DM_RES_UP
#define FL_H_DM_RES_DWN         PIN_DM_RES_DWN
#define FL_H_DM_OD_LO           PIN_DM_OD_LO
#define FL_H_DM_OD_HI           PIN_DM_OD_HI
#define FL_H_DM_STRONG          PIN_DM_STRONG
#define FL_H_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define FL_H_MASK               FL_H__MASK
#define FL_H_SHIFT              FL_H__SHIFT
#define FL_H_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define FL_H_PS                     (* (reg8 *) FL_H__PS)
/* Data Register */
#define FL_H_DR                     (* (reg8 *) FL_H__DR)
/* Port Number */
#define FL_H_PRT_NUM                (* (reg8 *) FL_H__PRT) 
/* Connect to Analog Globals */                                                  
#define FL_H_AG                     (* (reg8 *) FL_H__AG)                       
/* Analog MUX bux enable */
#define FL_H_AMUX                   (* (reg8 *) FL_H__AMUX) 
/* Bidirectional Enable */                                                        
#define FL_H_BIE                    (* (reg8 *) FL_H__BIE)
/* Bit-mask for Aliased Register Access */
#define FL_H_BIT_MASK               (* (reg8 *) FL_H__BIT_MASK)
/* Bypass Enable */
#define FL_H_BYP                    (* (reg8 *) FL_H__BYP)
/* Port wide control signals */                                                   
#define FL_H_CTL                    (* (reg8 *) FL_H__CTL)
/* Drive Modes */
#define FL_H_DM0                    (* (reg8 *) FL_H__DM0) 
#define FL_H_DM1                    (* (reg8 *) FL_H__DM1)
#define FL_H_DM2                    (* (reg8 *) FL_H__DM2) 
/* Input Buffer Disable Override */
#define FL_H_INP_DIS                (* (reg8 *) FL_H__INP_DIS)
/* LCD Common or Segment Drive */
#define FL_H_LCD_COM_SEG            (* (reg8 *) FL_H__LCD_COM_SEG)
/* Enable Segment LCD */
#define FL_H_LCD_EN                 (* (reg8 *) FL_H__LCD_EN)
/* Slew Rate Control */
#define FL_H_SLW                    (* (reg8 *) FL_H__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define FL_H_PRTDSI__CAPS_SEL       (* (reg8 *) FL_H__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define FL_H_PRTDSI__DBL_SYNC_IN    (* (reg8 *) FL_H__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define FL_H_PRTDSI__OE_SEL0        (* (reg8 *) FL_H__PRTDSI__OE_SEL0) 
#define FL_H_PRTDSI__OE_SEL1        (* (reg8 *) FL_H__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define FL_H_PRTDSI__OUT_SEL0       (* (reg8 *) FL_H__PRTDSI__OUT_SEL0) 
#define FL_H_PRTDSI__OUT_SEL1       (* (reg8 *) FL_H__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define FL_H_PRTDSI__SYNC_OUT       (* (reg8 *) FL_H__PRTDSI__SYNC_OUT) 


#if defined(FL_H__INTSTAT)  /* Interrupt Registers */

    #define FL_H_INTSTAT                (* (reg8 *) FL_H__INTSTAT)
    #define FL_H_SNAP                   (* (reg8 *) FL_H__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins FL_H_H */


/* [] END OF FILE */
