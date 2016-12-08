/*******************************************************************************
* File Name: BL_H.h  
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

#if !defined(CY_PINS_BL_H_H) /* Pins BL_H_H */
#define CY_PINS_BL_H_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "BL_H_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    BL_H_Write(uint8 value) ;
void    BL_H_SetDriveMode(uint8 mode) ;
uint8   BL_H_ReadDataReg(void) ;
uint8   BL_H_Read(void) ;
uint8   BL_H_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define BL_H_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define BL_H_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define BL_H_DM_RES_UP          PIN_DM_RES_UP
#define BL_H_DM_RES_DWN         PIN_DM_RES_DWN
#define BL_H_DM_OD_LO           PIN_DM_OD_LO
#define BL_H_DM_OD_HI           PIN_DM_OD_HI
#define BL_H_DM_STRONG          PIN_DM_STRONG
#define BL_H_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define BL_H_MASK               BL_H__MASK
#define BL_H_SHIFT              BL_H__SHIFT
#define BL_H_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define BL_H_PS                     (* (reg8 *) BL_H__PS)
/* Data Register */
#define BL_H_DR                     (* (reg8 *) BL_H__DR)
/* Port Number */
#define BL_H_PRT_NUM                (* (reg8 *) BL_H__PRT) 
/* Connect to Analog Globals */                                                  
#define BL_H_AG                     (* (reg8 *) BL_H__AG)                       
/* Analog MUX bux enable */
#define BL_H_AMUX                   (* (reg8 *) BL_H__AMUX) 
/* Bidirectional Enable */                                                        
#define BL_H_BIE                    (* (reg8 *) BL_H__BIE)
/* Bit-mask for Aliased Register Access */
#define BL_H_BIT_MASK               (* (reg8 *) BL_H__BIT_MASK)
/* Bypass Enable */
#define BL_H_BYP                    (* (reg8 *) BL_H__BYP)
/* Port wide control signals */                                                   
#define BL_H_CTL                    (* (reg8 *) BL_H__CTL)
/* Drive Modes */
#define BL_H_DM0                    (* (reg8 *) BL_H__DM0) 
#define BL_H_DM1                    (* (reg8 *) BL_H__DM1)
#define BL_H_DM2                    (* (reg8 *) BL_H__DM2) 
/* Input Buffer Disable Override */
#define BL_H_INP_DIS                (* (reg8 *) BL_H__INP_DIS)
/* LCD Common or Segment Drive */
#define BL_H_LCD_COM_SEG            (* (reg8 *) BL_H__LCD_COM_SEG)
/* Enable Segment LCD */
#define BL_H_LCD_EN                 (* (reg8 *) BL_H__LCD_EN)
/* Slew Rate Control */
#define BL_H_SLW                    (* (reg8 *) BL_H__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define BL_H_PRTDSI__CAPS_SEL       (* (reg8 *) BL_H__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define BL_H_PRTDSI__DBL_SYNC_IN    (* (reg8 *) BL_H__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define BL_H_PRTDSI__OE_SEL0        (* (reg8 *) BL_H__PRTDSI__OE_SEL0) 
#define BL_H_PRTDSI__OE_SEL1        (* (reg8 *) BL_H__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define BL_H_PRTDSI__OUT_SEL0       (* (reg8 *) BL_H__PRTDSI__OUT_SEL0) 
#define BL_H_PRTDSI__OUT_SEL1       (* (reg8 *) BL_H__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define BL_H_PRTDSI__SYNC_OUT       (* (reg8 *) BL_H__PRTDSI__SYNC_OUT) 


#if defined(BL_H__INTSTAT)  /* Interrupt Registers */

    #define BL_H_INTSTAT                (* (reg8 *) BL_H__INTSTAT)
    #define BL_H_SNAP                   (* (reg8 *) BL_H__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins BL_H_H */


/* [] END OF FILE */
