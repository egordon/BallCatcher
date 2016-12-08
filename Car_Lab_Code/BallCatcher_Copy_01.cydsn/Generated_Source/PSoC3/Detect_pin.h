/*******************************************************************************
* File Name: Detect_pin.h  
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

#if !defined(CY_PINS_Detect_pin_H) /* Pins Detect_pin_H */
#define CY_PINS_Detect_pin_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Detect_pin_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Detect_pin_Write(uint8 value) ;
void    Detect_pin_SetDriveMode(uint8 mode) ;
uint8   Detect_pin_ReadDataReg(void) ;
uint8   Detect_pin_Read(void) ;
uint8   Detect_pin_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Detect_pin_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Detect_pin_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Detect_pin_DM_RES_UP          PIN_DM_RES_UP
#define Detect_pin_DM_RES_DWN         PIN_DM_RES_DWN
#define Detect_pin_DM_OD_LO           PIN_DM_OD_LO
#define Detect_pin_DM_OD_HI           PIN_DM_OD_HI
#define Detect_pin_DM_STRONG          PIN_DM_STRONG
#define Detect_pin_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Detect_pin_MASK               Detect_pin__MASK
#define Detect_pin_SHIFT              Detect_pin__SHIFT
#define Detect_pin_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Detect_pin_PS                     (* (reg8 *) Detect_pin__PS)
/* Data Register */
#define Detect_pin_DR                     (* (reg8 *) Detect_pin__DR)
/* Port Number */
#define Detect_pin_PRT_NUM                (* (reg8 *) Detect_pin__PRT) 
/* Connect to Analog Globals */                                                  
#define Detect_pin_AG                     (* (reg8 *) Detect_pin__AG)                       
/* Analog MUX bux enable */
#define Detect_pin_AMUX                   (* (reg8 *) Detect_pin__AMUX) 
/* Bidirectional Enable */                                                        
#define Detect_pin_BIE                    (* (reg8 *) Detect_pin__BIE)
/* Bit-mask for Aliased Register Access */
#define Detect_pin_BIT_MASK               (* (reg8 *) Detect_pin__BIT_MASK)
/* Bypass Enable */
#define Detect_pin_BYP                    (* (reg8 *) Detect_pin__BYP)
/* Port wide control signals */                                                   
#define Detect_pin_CTL                    (* (reg8 *) Detect_pin__CTL)
/* Drive Modes */
#define Detect_pin_DM0                    (* (reg8 *) Detect_pin__DM0) 
#define Detect_pin_DM1                    (* (reg8 *) Detect_pin__DM1)
#define Detect_pin_DM2                    (* (reg8 *) Detect_pin__DM2) 
/* Input Buffer Disable Override */
#define Detect_pin_INP_DIS                (* (reg8 *) Detect_pin__INP_DIS)
/* LCD Common or Segment Drive */
#define Detect_pin_LCD_COM_SEG            (* (reg8 *) Detect_pin__LCD_COM_SEG)
/* Enable Segment LCD */
#define Detect_pin_LCD_EN                 (* (reg8 *) Detect_pin__LCD_EN)
/* Slew Rate Control */
#define Detect_pin_SLW                    (* (reg8 *) Detect_pin__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Detect_pin_PRTDSI__CAPS_SEL       (* (reg8 *) Detect_pin__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Detect_pin_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Detect_pin__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Detect_pin_PRTDSI__OE_SEL0        (* (reg8 *) Detect_pin__PRTDSI__OE_SEL0) 
#define Detect_pin_PRTDSI__OE_SEL1        (* (reg8 *) Detect_pin__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Detect_pin_PRTDSI__OUT_SEL0       (* (reg8 *) Detect_pin__PRTDSI__OUT_SEL0) 
#define Detect_pin_PRTDSI__OUT_SEL1       (* (reg8 *) Detect_pin__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Detect_pin_PRTDSI__SYNC_OUT       (* (reg8 *) Detect_pin__PRTDSI__SYNC_OUT) 


#if defined(Detect_pin__INTSTAT)  /* Interrupt Registers */

    #define Detect_pin_INTSTAT                (* (reg8 *) Detect_pin__INTSTAT)
    #define Detect_pin_SNAP                   (* (reg8 *) Detect_pin__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins Detect_pin_H */


/* [] END OF FILE */
