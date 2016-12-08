/*******************************************************************************
* File Name: RangeIn.h  
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

#if !defined(CY_PINS_RangeIn_H) /* Pins RangeIn_H */
#define CY_PINS_RangeIn_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "RangeIn_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    RangeIn_Write(uint8 value) ;
void    RangeIn_SetDriveMode(uint8 mode) ;
uint8   RangeIn_ReadDataReg(void) ;
uint8   RangeIn_Read(void) ;
uint8   RangeIn_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define RangeIn_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define RangeIn_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define RangeIn_DM_RES_UP          PIN_DM_RES_UP
#define RangeIn_DM_RES_DWN         PIN_DM_RES_DWN
#define RangeIn_DM_OD_LO           PIN_DM_OD_LO
#define RangeIn_DM_OD_HI           PIN_DM_OD_HI
#define RangeIn_DM_STRONG          PIN_DM_STRONG
#define RangeIn_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define RangeIn_MASK               RangeIn__MASK
#define RangeIn_SHIFT              RangeIn__SHIFT
#define RangeIn_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define RangeIn_PS                     (* (reg8 *) RangeIn__PS)
/* Data Register */
#define RangeIn_DR                     (* (reg8 *) RangeIn__DR)
/* Port Number */
#define RangeIn_PRT_NUM                (* (reg8 *) RangeIn__PRT) 
/* Connect to Analog Globals */                                                  
#define RangeIn_AG                     (* (reg8 *) RangeIn__AG)                       
/* Analog MUX bux enable */
#define RangeIn_AMUX                   (* (reg8 *) RangeIn__AMUX) 
/* Bidirectional Enable */                                                        
#define RangeIn_BIE                    (* (reg8 *) RangeIn__BIE)
/* Bit-mask for Aliased Register Access */
#define RangeIn_BIT_MASK               (* (reg8 *) RangeIn__BIT_MASK)
/* Bypass Enable */
#define RangeIn_BYP                    (* (reg8 *) RangeIn__BYP)
/* Port wide control signals */                                                   
#define RangeIn_CTL                    (* (reg8 *) RangeIn__CTL)
/* Drive Modes */
#define RangeIn_DM0                    (* (reg8 *) RangeIn__DM0) 
#define RangeIn_DM1                    (* (reg8 *) RangeIn__DM1)
#define RangeIn_DM2                    (* (reg8 *) RangeIn__DM2) 
/* Input Buffer Disable Override */
#define RangeIn_INP_DIS                (* (reg8 *) RangeIn__INP_DIS)
/* LCD Common or Segment Drive */
#define RangeIn_LCD_COM_SEG            (* (reg8 *) RangeIn__LCD_COM_SEG)
/* Enable Segment LCD */
#define RangeIn_LCD_EN                 (* (reg8 *) RangeIn__LCD_EN)
/* Slew Rate Control */
#define RangeIn_SLW                    (* (reg8 *) RangeIn__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define RangeIn_PRTDSI__CAPS_SEL       (* (reg8 *) RangeIn__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define RangeIn_PRTDSI__DBL_SYNC_IN    (* (reg8 *) RangeIn__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define RangeIn_PRTDSI__OE_SEL0        (* (reg8 *) RangeIn__PRTDSI__OE_SEL0) 
#define RangeIn_PRTDSI__OE_SEL1        (* (reg8 *) RangeIn__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define RangeIn_PRTDSI__OUT_SEL0       (* (reg8 *) RangeIn__PRTDSI__OUT_SEL0) 
#define RangeIn_PRTDSI__OUT_SEL1       (* (reg8 *) RangeIn__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define RangeIn_PRTDSI__SYNC_OUT       (* (reg8 *) RangeIn__PRTDSI__SYNC_OUT) 


#if defined(RangeIn__INTSTAT)  /* Interrupt Registers */

    #define RangeIn_INTSTAT                (* (reg8 *) RangeIn__INTSTAT)
    #define RangeIn_SNAP                   (* (reg8 *) RangeIn__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins RangeIn_H */


/* [] END OF FILE */
