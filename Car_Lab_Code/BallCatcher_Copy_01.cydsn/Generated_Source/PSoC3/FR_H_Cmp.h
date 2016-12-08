/*******************************************************************************
* File Name: FR_H_Cmp.h  
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

#if !defined(CY_PINS_FR_H_Cmp_H) /* Pins FR_H_Cmp_H */
#define CY_PINS_FR_H_Cmp_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "FR_H_Cmp_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    FR_H_Cmp_Write(uint8 value) ;
void    FR_H_Cmp_SetDriveMode(uint8 mode) ;
uint8   FR_H_Cmp_ReadDataReg(void) ;
uint8   FR_H_Cmp_Read(void) ;
uint8   FR_H_Cmp_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define FR_H_Cmp_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define FR_H_Cmp_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define FR_H_Cmp_DM_RES_UP          PIN_DM_RES_UP
#define FR_H_Cmp_DM_RES_DWN         PIN_DM_RES_DWN
#define FR_H_Cmp_DM_OD_LO           PIN_DM_OD_LO
#define FR_H_Cmp_DM_OD_HI           PIN_DM_OD_HI
#define FR_H_Cmp_DM_STRONG          PIN_DM_STRONG
#define FR_H_Cmp_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define FR_H_Cmp_MASK               FR_H_Cmp__MASK
#define FR_H_Cmp_SHIFT              FR_H_Cmp__SHIFT
#define FR_H_Cmp_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define FR_H_Cmp_PS                     (* (reg8 *) FR_H_Cmp__PS)
/* Data Register */
#define FR_H_Cmp_DR                     (* (reg8 *) FR_H_Cmp__DR)
/* Port Number */
#define FR_H_Cmp_PRT_NUM                (* (reg8 *) FR_H_Cmp__PRT) 
/* Connect to Analog Globals */                                                  
#define FR_H_Cmp_AG                     (* (reg8 *) FR_H_Cmp__AG)                       
/* Analog MUX bux enable */
#define FR_H_Cmp_AMUX                   (* (reg8 *) FR_H_Cmp__AMUX) 
/* Bidirectional Enable */                                                        
#define FR_H_Cmp_BIE                    (* (reg8 *) FR_H_Cmp__BIE)
/* Bit-mask for Aliased Register Access */
#define FR_H_Cmp_BIT_MASK               (* (reg8 *) FR_H_Cmp__BIT_MASK)
/* Bypass Enable */
#define FR_H_Cmp_BYP                    (* (reg8 *) FR_H_Cmp__BYP)
/* Port wide control signals */                                                   
#define FR_H_Cmp_CTL                    (* (reg8 *) FR_H_Cmp__CTL)
/* Drive Modes */
#define FR_H_Cmp_DM0                    (* (reg8 *) FR_H_Cmp__DM0) 
#define FR_H_Cmp_DM1                    (* (reg8 *) FR_H_Cmp__DM1)
#define FR_H_Cmp_DM2                    (* (reg8 *) FR_H_Cmp__DM2) 
/* Input Buffer Disable Override */
#define FR_H_Cmp_INP_DIS                (* (reg8 *) FR_H_Cmp__INP_DIS)
/* LCD Common or Segment Drive */
#define FR_H_Cmp_LCD_COM_SEG            (* (reg8 *) FR_H_Cmp__LCD_COM_SEG)
/* Enable Segment LCD */
#define FR_H_Cmp_LCD_EN                 (* (reg8 *) FR_H_Cmp__LCD_EN)
/* Slew Rate Control */
#define FR_H_Cmp_SLW                    (* (reg8 *) FR_H_Cmp__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define FR_H_Cmp_PRTDSI__CAPS_SEL       (* (reg8 *) FR_H_Cmp__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define FR_H_Cmp_PRTDSI__DBL_SYNC_IN    (* (reg8 *) FR_H_Cmp__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define FR_H_Cmp_PRTDSI__OE_SEL0        (* (reg8 *) FR_H_Cmp__PRTDSI__OE_SEL0) 
#define FR_H_Cmp_PRTDSI__OE_SEL1        (* (reg8 *) FR_H_Cmp__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define FR_H_Cmp_PRTDSI__OUT_SEL0       (* (reg8 *) FR_H_Cmp__PRTDSI__OUT_SEL0) 
#define FR_H_Cmp_PRTDSI__OUT_SEL1       (* (reg8 *) FR_H_Cmp__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define FR_H_Cmp_PRTDSI__SYNC_OUT       (* (reg8 *) FR_H_Cmp__PRTDSI__SYNC_OUT) 


#if defined(FR_H_Cmp__INTSTAT)  /* Interrupt Registers */

    #define FR_H_Cmp_INTSTAT                (* (reg8 *) FR_H_Cmp__INTSTAT)
    #define FR_H_Cmp_SNAP                   (* (reg8 *) FR_H_Cmp__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins FR_H_Cmp_H */


/* [] END OF FILE */
