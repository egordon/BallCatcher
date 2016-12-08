/*******************************************************************************
* File Name: Gyro_Pin.h  
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

#if !defined(CY_PINS_Gyro_Pin_H) /* Pins Gyro_Pin_H */
#define CY_PINS_Gyro_Pin_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Gyro_Pin_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Gyro_Pin_Write(uint8 value) ;
void    Gyro_Pin_SetDriveMode(uint8 mode) ;
uint8   Gyro_Pin_ReadDataReg(void) ;
uint8   Gyro_Pin_Read(void) ;
uint8   Gyro_Pin_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Gyro_Pin_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Gyro_Pin_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Gyro_Pin_DM_RES_UP          PIN_DM_RES_UP
#define Gyro_Pin_DM_RES_DWN         PIN_DM_RES_DWN
#define Gyro_Pin_DM_OD_LO           PIN_DM_OD_LO
#define Gyro_Pin_DM_OD_HI           PIN_DM_OD_HI
#define Gyro_Pin_DM_STRONG          PIN_DM_STRONG
#define Gyro_Pin_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Gyro_Pin_MASK               Gyro_Pin__MASK
#define Gyro_Pin_SHIFT              Gyro_Pin__SHIFT
#define Gyro_Pin_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Gyro_Pin_PS                     (* (reg8 *) Gyro_Pin__PS)
/* Data Register */
#define Gyro_Pin_DR                     (* (reg8 *) Gyro_Pin__DR)
/* Port Number */
#define Gyro_Pin_PRT_NUM                (* (reg8 *) Gyro_Pin__PRT) 
/* Connect to Analog Globals */                                                  
#define Gyro_Pin_AG                     (* (reg8 *) Gyro_Pin__AG)                       
/* Analog MUX bux enable */
#define Gyro_Pin_AMUX                   (* (reg8 *) Gyro_Pin__AMUX) 
/* Bidirectional Enable */                                                        
#define Gyro_Pin_BIE                    (* (reg8 *) Gyro_Pin__BIE)
/* Bit-mask for Aliased Register Access */
#define Gyro_Pin_BIT_MASK               (* (reg8 *) Gyro_Pin__BIT_MASK)
/* Bypass Enable */
#define Gyro_Pin_BYP                    (* (reg8 *) Gyro_Pin__BYP)
/* Port wide control signals */                                                   
#define Gyro_Pin_CTL                    (* (reg8 *) Gyro_Pin__CTL)
/* Drive Modes */
#define Gyro_Pin_DM0                    (* (reg8 *) Gyro_Pin__DM0) 
#define Gyro_Pin_DM1                    (* (reg8 *) Gyro_Pin__DM1)
#define Gyro_Pin_DM2                    (* (reg8 *) Gyro_Pin__DM2) 
/* Input Buffer Disable Override */
#define Gyro_Pin_INP_DIS                (* (reg8 *) Gyro_Pin__INP_DIS)
/* LCD Common or Segment Drive */
#define Gyro_Pin_LCD_COM_SEG            (* (reg8 *) Gyro_Pin__LCD_COM_SEG)
/* Enable Segment LCD */
#define Gyro_Pin_LCD_EN                 (* (reg8 *) Gyro_Pin__LCD_EN)
/* Slew Rate Control */
#define Gyro_Pin_SLW                    (* (reg8 *) Gyro_Pin__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Gyro_Pin_PRTDSI__CAPS_SEL       (* (reg8 *) Gyro_Pin__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Gyro_Pin_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Gyro_Pin__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Gyro_Pin_PRTDSI__OE_SEL0        (* (reg8 *) Gyro_Pin__PRTDSI__OE_SEL0) 
#define Gyro_Pin_PRTDSI__OE_SEL1        (* (reg8 *) Gyro_Pin__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Gyro_Pin_PRTDSI__OUT_SEL0       (* (reg8 *) Gyro_Pin__PRTDSI__OUT_SEL0) 
#define Gyro_Pin_PRTDSI__OUT_SEL1       (* (reg8 *) Gyro_Pin__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Gyro_Pin_PRTDSI__SYNC_OUT       (* (reg8 *) Gyro_Pin__PRTDSI__SYNC_OUT) 


#if defined(Gyro_Pin__INTSTAT)  /* Interrupt Registers */

    #define Gyro_Pin_INTSTAT                (* (reg8 *) Gyro_Pin__INTSTAT)
    #define Gyro_Pin_SNAP                   (* (reg8 *) Gyro_Pin__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins Gyro_Pin_H */


/* [] END OF FILE */
