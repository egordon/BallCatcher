/*******************************************************************************
* File Name: FR_H.h  
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

#if !defined(CY_PINS_FR_H_H) /* Pins FR_H_H */
#define CY_PINS_FR_H_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "FR_H_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    FR_H_Write(uint8 value) ;
void    FR_H_SetDriveMode(uint8 mode) ;
uint8   FR_H_ReadDataReg(void) ;
uint8   FR_H_Read(void) ;
uint8   FR_H_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define FR_H_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define FR_H_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define FR_H_DM_RES_UP          PIN_DM_RES_UP
#define FR_H_DM_RES_DWN         PIN_DM_RES_DWN
#define FR_H_DM_OD_LO           PIN_DM_OD_LO
#define FR_H_DM_OD_HI           PIN_DM_OD_HI
#define FR_H_DM_STRONG          PIN_DM_STRONG
#define FR_H_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define FR_H_MASK               FR_H__MASK
#define FR_H_SHIFT              FR_H__SHIFT
#define FR_H_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define FR_H_PS                     (* (reg8 *) FR_H__PS)
/* Data Register */
#define FR_H_DR                     (* (reg8 *) FR_H__DR)
/* Port Number */
#define FR_H_PRT_NUM                (* (reg8 *) FR_H__PRT) 
/* Connect to Analog Globals */                                                  
#define FR_H_AG                     (* (reg8 *) FR_H__AG)                       
/* Analog MUX bux enable */
#define FR_H_AMUX                   (* (reg8 *) FR_H__AMUX) 
/* Bidirectional Enable */                                                        
#define FR_H_BIE                    (* (reg8 *) FR_H__BIE)
/* Bit-mask for Aliased Register Access */
#define FR_H_BIT_MASK               (* (reg8 *) FR_H__BIT_MASK)
/* Bypass Enable */
#define FR_H_BYP                    (* (reg8 *) FR_H__BYP)
/* Port wide control signals */                                                   
#define FR_H_CTL                    (* (reg8 *) FR_H__CTL)
/* Drive Modes */
#define FR_H_DM0                    (* (reg8 *) FR_H__DM0) 
#define FR_H_DM1                    (* (reg8 *) FR_H__DM1)
#define FR_H_DM2                    (* (reg8 *) FR_H__DM2) 
/* Input Buffer Disable Override */
#define FR_H_INP_DIS                (* (reg8 *) FR_H__INP_DIS)
/* LCD Common or Segment Drive */
#define FR_H_LCD_COM_SEG            (* (reg8 *) FR_H__LCD_COM_SEG)
/* Enable Segment LCD */
#define FR_H_LCD_EN                 (* (reg8 *) FR_H__LCD_EN)
/* Slew Rate Control */
#define FR_H_SLW                    (* (reg8 *) FR_H__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define FR_H_PRTDSI__CAPS_SEL       (* (reg8 *) FR_H__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define FR_H_PRTDSI__DBL_SYNC_IN    (* (reg8 *) FR_H__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define FR_H_PRTDSI__OE_SEL0        (* (reg8 *) FR_H__PRTDSI__OE_SEL0) 
#define FR_H_PRTDSI__OE_SEL1        (* (reg8 *) FR_H__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define FR_H_PRTDSI__OUT_SEL0       (* (reg8 *) FR_H__PRTDSI__OUT_SEL0) 
#define FR_H_PRTDSI__OUT_SEL1       (* (reg8 *) FR_H__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define FR_H_PRTDSI__SYNC_OUT       (* (reg8 *) FR_H__PRTDSI__SYNC_OUT) 


#if defined(FR_H__INTSTAT)  /* Interrupt Registers */

    #define FR_H_INTSTAT                (* (reg8 *) FR_H__INTSTAT)
    #define FR_H_SNAP                   (* (reg8 *) FR_H__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins FR_H_H */


/* [] END OF FILE */
