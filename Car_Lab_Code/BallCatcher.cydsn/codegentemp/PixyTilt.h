/*******************************************************************************
* File Name: PixyTilt.h  
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

#if !defined(CY_PINS_PixyTilt_H) /* Pins PixyTilt_H */
#define CY_PINS_PixyTilt_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "PixyTilt_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    PixyTilt_Write(uint8 value) ;
void    PixyTilt_SetDriveMode(uint8 mode) ;
uint8   PixyTilt_ReadDataReg(void) ;
uint8   PixyTilt_Read(void) ;
uint8   PixyTilt_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define PixyTilt_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define PixyTilt_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define PixyTilt_DM_RES_UP          PIN_DM_RES_UP
#define PixyTilt_DM_RES_DWN         PIN_DM_RES_DWN
#define PixyTilt_DM_OD_LO           PIN_DM_OD_LO
#define PixyTilt_DM_OD_HI           PIN_DM_OD_HI
#define PixyTilt_DM_STRONG          PIN_DM_STRONG
#define PixyTilt_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define PixyTilt_MASK               PixyTilt__MASK
#define PixyTilt_SHIFT              PixyTilt__SHIFT
#define PixyTilt_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define PixyTilt_PS                     (* (reg8 *) PixyTilt__PS)
/* Data Register */
#define PixyTilt_DR                     (* (reg8 *) PixyTilt__DR)
/* Port Number */
#define PixyTilt_PRT_NUM                (* (reg8 *) PixyTilt__PRT) 
/* Connect to Analog Globals */                                                  
#define PixyTilt_AG                     (* (reg8 *) PixyTilt__AG)                       
/* Analog MUX bux enable */
#define PixyTilt_AMUX                   (* (reg8 *) PixyTilt__AMUX) 
/* Bidirectional Enable */                                                        
#define PixyTilt_BIE                    (* (reg8 *) PixyTilt__BIE)
/* Bit-mask for Aliased Register Access */
#define PixyTilt_BIT_MASK               (* (reg8 *) PixyTilt__BIT_MASK)
/* Bypass Enable */
#define PixyTilt_BYP                    (* (reg8 *) PixyTilt__BYP)
/* Port wide control signals */                                                   
#define PixyTilt_CTL                    (* (reg8 *) PixyTilt__CTL)
/* Drive Modes */
#define PixyTilt_DM0                    (* (reg8 *) PixyTilt__DM0) 
#define PixyTilt_DM1                    (* (reg8 *) PixyTilt__DM1)
#define PixyTilt_DM2                    (* (reg8 *) PixyTilt__DM2) 
/* Input Buffer Disable Override */
#define PixyTilt_INP_DIS                (* (reg8 *) PixyTilt__INP_DIS)
/* LCD Common or Segment Drive */
#define PixyTilt_LCD_COM_SEG            (* (reg8 *) PixyTilt__LCD_COM_SEG)
/* Enable Segment LCD */
#define PixyTilt_LCD_EN                 (* (reg8 *) PixyTilt__LCD_EN)
/* Slew Rate Control */
#define PixyTilt_SLW                    (* (reg8 *) PixyTilt__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define PixyTilt_PRTDSI__CAPS_SEL       (* (reg8 *) PixyTilt__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define PixyTilt_PRTDSI__DBL_SYNC_IN    (* (reg8 *) PixyTilt__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define PixyTilt_PRTDSI__OE_SEL0        (* (reg8 *) PixyTilt__PRTDSI__OE_SEL0) 
#define PixyTilt_PRTDSI__OE_SEL1        (* (reg8 *) PixyTilt__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define PixyTilt_PRTDSI__OUT_SEL0       (* (reg8 *) PixyTilt__PRTDSI__OUT_SEL0) 
#define PixyTilt_PRTDSI__OUT_SEL1       (* (reg8 *) PixyTilt__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define PixyTilt_PRTDSI__SYNC_OUT       (* (reg8 *) PixyTilt__PRTDSI__SYNC_OUT) 


#if defined(PixyTilt__INTSTAT)  /* Interrupt Registers */

    #define PixyTilt_INTSTAT                (* (reg8 *) PixyTilt__INTSTAT)
    #define PixyTilt_SNAP                   (* (reg8 *) PixyTilt__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins PixyTilt_H */


/* [] END OF FILE */
