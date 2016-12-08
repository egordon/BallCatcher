/*******************************************************************************
* File Name: BL_H2.h  
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

#if !defined(CY_PINS_BL_H2_H) /* Pins BL_H2_H */
#define CY_PINS_BL_H2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "BL_H2_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    BL_H2_Write(uint8 value) ;
void    BL_H2_SetDriveMode(uint8 mode) ;
uint8   BL_H2_ReadDataReg(void) ;
uint8   BL_H2_Read(void) ;
uint8   BL_H2_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define BL_H2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define BL_H2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define BL_H2_DM_RES_UP          PIN_DM_RES_UP
#define BL_H2_DM_RES_DWN         PIN_DM_RES_DWN
#define BL_H2_DM_OD_LO           PIN_DM_OD_LO
#define BL_H2_DM_OD_HI           PIN_DM_OD_HI
#define BL_H2_DM_STRONG          PIN_DM_STRONG
#define BL_H2_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define BL_H2_MASK               BL_H2__MASK
#define BL_H2_SHIFT              BL_H2__SHIFT
#define BL_H2_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define BL_H2_PS                     (* (reg8 *) BL_H2__PS)
/* Data Register */
#define BL_H2_DR                     (* (reg8 *) BL_H2__DR)
/* Port Number */
#define BL_H2_PRT_NUM                (* (reg8 *) BL_H2__PRT) 
/* Connect to Analog Globals */                                                  
#define BL_H2_AG                     (* (reg8 *) BL_H2__AG)                       
/* Analog MUX bux enable */
#define BL_H2_AMUX                   (* (reg8 *) BL_H2__AMUX) 
/* Bidirectional Enable */                                                        
#define BL_H2_BIE                    (* (reg8 *) BL_H2__BIE)
/* Bit-mask for Aliased Register Access */
#define BL_H2_BIT_MASK               (* (reg8 *) BL_H2__BIT_MASK)
/* Bypass Enable */
#define BL_H2_BYP                    (* (reg8 *) BL_H2__BYP)
/* Port wide control signals */                                                   
#define BL_H2_CTL                    (* (reg8 *) BL_H2__CTL)
/* Drive Modes */
#define BL_H2_DM0                    (* (reg8 *) BL_H2__DM0) 
#define BL_H2_DM1                    (* (reg8 *) BL_H2__DM1)
#define BL_H2_DM2                    (* (reg8 *) BL_H2__DM2) 
/* Input Buffer Disable Override */
#define BL_H2_INP_DIS                (* (reg8 *) BL_H2__INP_DIS)
/* LCD Common or Segment Drive */
#define BL_H2_LCD_COM_SEG            (* (reg8 *) BL_H2__LCD_COM_SEG)
/* Enable Segment LCD */
#define BL_H2_LCD_EN                 (* (reg8 *) BL_H2__LCD_EN)
/* Slew Rate Control */
#define BL_H2_SLW                    (* (reg8 *) BL_H2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define BL_H2_PRTDSI__CAPS_SEL       (* (reg8 *) BL_H2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define BL_H2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) BL_H2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define BL_H2_PRTDSI__OE_SEL0        (* (reg8 *) BL_H2__PRTDSI__OE_SEL0) 
#define BL_H2_PRTDSI__OE_SEL1        (* (reg8 *) BL_H2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define BL_H2_PRTDSI__OUT_SEL0       (* (reg8 *) BL_H2__PRTDSI__OUT_SEL0) 
#define BL_H2_PRTDSI__OUT_SEL1       (* (reg8 *) BL_H2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define BL_H2_PRTDSI__SYNC_OUT       (* (reg8 *) BL_H2__PRTDSI__SYNC_OUT) 


#if defined(BL_H2__INTSTAT)  /* Interrupt Registers */

    #define BL_H2_INTSTAT                (* (reg8 *) BL_H2__INTSTAT)
    #define BL_H2_SNAP                   (* (reg8 *) BL_H2__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins BL_H2_H */


/* [] END OF FILE */
