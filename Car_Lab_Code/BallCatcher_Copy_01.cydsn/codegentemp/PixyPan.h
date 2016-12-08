/*******************************************************************************
* File Name: PixyPan.h  
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

#if !defined(CY_PINS_PixyPan_H) /* Pins PixyPan_H */
#define CY_PINS_PixyPan_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "PixyPan_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    PixyPan_Write(uint8 value) ;
void    PixyPan_SetDriveMode(uint8 mode) ;
uint8   PixyPan_ReadDataReg(void) ;
uint8   PixyPan_Read(void) ;
uint8   PixyPan_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define PixyPan_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define PixyPan_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define PixyPan_DM_RES_UP          PIN_DM_RES_UP
#define PixyPan_DM_RES_DWN         PIN_DM_RES_DWN
#define PixyPan_DM_OD_LO           PIN_DM_OD_LO
#define PixyPan_DM_OD_HI           PIN_DM_OD_HI
#define PixyPan_DM_STRONG          PIN_DM_STRONG
#define PixyPan_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define PixyPan_MASK               PixyPan__MASK
#define PixyPan_SHIFT              PixyPan__SHIFT
#define PixyPan_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define PixyPan_PS                     (* (reg8 *) PixyPan__PS)
/* Data Register */
#define PixyPan_DR                     (* (reg8 *) PixyPan__DR)
/* Port Number */
#define PixyPan_PRT_NUM                (* (reg8 *) PixyPan__PRT) 
/* Connect to Analog Globals */                                                  
#define PixyPan_AG                     (* (reg8 *) PixyPan__AG)                       
/* Analog MUX bux enable */
#define PixyPan_AMUX                   (* (reg8 *) PixyPan__AMUX) 
/* Bidirectional Enable */                                                        
#define PixyPan_BIE                    (* (reg8 *) PixyPan__BIE)
/* Bit-mask for Aliased Register Access */
#define PixyPan_BIT_MASK               (* (reg8 *) PixyPan__BIT_MASK)
/* Bypass Enable */
#define PixyPan_BYP                    (* (reg8 *) PixyPan__BYP)
/* Port wide control signals */                                                   
#define PixyPan_CTL                    (* (reg8 *) PixyPan__CTL)
/* Drive Modes */
#define PixyPan_DM0                    (* (reg8 *) PixyPan__DM0) 
#define PixyPan_DM1                    (* (reg8 *) PixyPan__DM1)
#define PixyPan_DM2                    (* (reg8 *) PixyPan__DM2) 
/* Input Buffer Disable Override */
#define PixyPan_INP_DIS                (* (reg8 *) PixyPan__INP_DIS)
/* LCD Common or Segment Drive */
#define PixyPan_LCD_COM_SEG            (* (reg8 *) PixyPan__LCD_COM_SEG)
/* Enable Segment LCD */
#define PixyPan_LCD_EN                 (* (reg8 *) PixyPan__LCD_EN)
/* Slew Rate Control */
#define PixyPan_SLW                    (* (reg8 *) PixyPan__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define PixyPan_PRTDSI__CAPS_SEL       (* (reg8 *) PixyPan__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define PixyPan_PRTDSI__DBL_SYNC_IN    (* (reg8 *) PixyPan__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define PixyPan_PRTDSI__OE_SEL0        (* (reg8 *) PixyPan__PRTDSI__OE_SEL0) 
#define PixyPan_PRTDSI__OE_SEL1        (* (reg8 *) PixyPan__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define PixyPan_PRTDSI__OUT_SEL0       (* (reg8 *) PixyPan__PRTDSI__OUT_SEL0) 
#define PixyPan_PRTDSI__OUT_SEL1       (* (reg8 *) PixyPan__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define PixyPan_PRTDSI__SYNC_OUT       (* (reg8 *) PixyPan__PRTDSI__SYNC_OUT) 


#if defined(PixyPan__INTSTAT)  /* Interrupt Registers */

    #define PixyPan_INTSTAT                (* (reg8 *) PixyPan__INTSTAT)
    #define PixyPan_SNAP                   (* (reg8 *) PixyPan__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins PixyPan_H */


/* [] END OF FILE */
