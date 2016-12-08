/*******************************************************************************
* File Name: FR_HBridge.h  
* Version 2.20
*
* Description:
*  Contains the prototypes and constants for the functions available to the 
*  PWM user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

#if !defined(CY_PWM_FR_HBridge_H)
#define CY_PWM_FR_HBridge_H


/***************************************
* Conditional Compilation Parameters
***************************************/
#define FR_HBridge_Resolution 16u
#define FR_HBridge_UsingFixedFunction 1u
#define FR_HBridge_DeadBandMode 0u
#define FR_HBridge_KillModeMinTime 0u
#define FR_HBridge_KillMode 1u
#define FR_HBridge_PWMMode 0u
#define FR_HBridge_PWMModeIsCenterAligned 0u
#define FR_HBridge_DeadBandUsed 0u
#define FR_HBridge_DeadBand2_4 0u
#if !defined(FR_HBridge_PWMUDB_sSTSReg_stsreg__REMOVED)
    #define FR_HBridge_UseStatus 0u
#else
    #define FR_HBridge_UseStatus 0u
#endif /* !defined(FR_HBridge_PWMUDB_sSTSReg_stsreg__REMOVED) */
#if !defined(FR_HBridge_PWMUDB_sCTRLReg_ctrlreg__REMOVED)
    #define FR_HBridge_UseControl 1u
#else
    #define FR_HBridge_UseControl 0u
#endif /* !defined(FR_HBridge_PWMUDB_sCTRLReg_ctrlreg__REMOVED) */
#define FR_HBridge_UseOneCompareMode 1u
#define FR_HBridge_MinimumKillTime 1u
#define FR_HBridge_EnableMode 0u

#define FR_HBridge_CompareMode1SW 0u
#define FR_HBridge_CompareMode2SW 0u

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define FR_HBridge__B_PWM__DISABLED 0
#define FR_HBridge__B_PWM__ASYNCHRONOUS 1
#define FR_HBridge__B_PWM__SINGLECYCLE 2
#define FR_HBridge__B_PWM__LATCHED 3
#define FR_HBridge__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define FR_HBridge__B_PWM__DBMDISABLED 0
#define FR_HBridge__B_PWM__DBM_2_4_CLOCKS 1
#define FR_HBridge__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define FR_HBridge__B_PWM__ONE_OUTPUT 0
#define FR_HBridge__B_PWM__TWO_OUTPUTS 1
#define FR_HBridge__B_PWM__DUAL_EDGE 2
#define FR_HBridge__B_PWM__CENTER_ALIGN 3
#define FR_HBridge__B_PWM__DITHER 5
#define FR_HBridge__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define FR_HBridge__B_PWM__LESS_THAN 1
#define FR_HBridge__B_PWM__LESS_THAN_OR_EQUAL 2
#define FR_HBridge__B_PWM__GREATER_THAN 3
#define FR_HBridge__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define FR_HBridge__B_PWM__EQUAL 0
#define FR_HBridge__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct FR_HBridge_backupStruct
{
    
    uint8 PWMEnableState;
       
    #if(!FR_HBridge_UsingFixedFunction)
        #if (CY_PSOC5A)
            uint16 PWMUdb;               /* PWM Current Counter value  */
            uint16 PWMPeriod;            /* PWM Current Period value   */
            #if (FR_HBridge_UseStatus)
                uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
            #endif /* (FR_HBridge_UseStatus) */
            #if(FR_HBridge_UseOneCompareMode)
                uint16 PWMCompareValue;     /* PWM Current Compare value */
            #else
                uint16 PWMCompareValue1;     /* PWM Current Compare value1 */
                uint16 PWMCompareValue2;     /* PWM Current Compare value2 */
            #endif /* (FR_HBridge_UseOneCompareMode) */
            
            /* Backup for Deadband parameters */
            #if(FR_HBridge_DeadBandMode == FR_HBridge__B_PWM__DBM_256_CLOCKS || \
                FR_HBridge_DeadBandMode == FR_HBridge__B_PWM__DBM_2_4_CLOCKS)
                uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
            #endif /* deadband count is either 2-4 clocks or 256 clocks */
            
            /* Backup Kill Mode Counter*/
            #if(FR_HBridge_KillModeMinTime)
                uint8 PWMKillCounterPeriod; /* Kill Mode period value */
            #endif /* (FR_HBridge_KillModeMinTime) */
            
        #endif /* (CY_PSOC5A) */
        
        #if (CY_PSOC3 || CY_PSOC5LP)
            uint16 PWMUdb;               /* PWM Current Counter value  */
            #if(!FR_HBridge_PWMModeIsCenterAligned)
                uint16 PWMPeriod;
            #endif /* (!FR_HBridge_PWMModeIsCenterAligned) */
            #if (FR_HBridge_UseStatus)
                uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
            #endif /* (FR_HBridge_UseStatus) */
            
            /* Backup for Deadband parameters */
            #if(FR_HBridge_DeadBandMode == FR_HBridge__B_PWM__DBM_256_CLOCKS || \
                FR_HBridge_DeadBandMode == FR_HBridge__B_PWM__DBM_2_4_CLOCKS)
                uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
            #endif /* deadband count is either 2-4 clocks or 256 clocks */
            
            /* Backup Kill Mode Counter*/
            #if(FR_HBridge_KillModeMinTime)
                uint8 PWMKillCounterPeriod; /* Kill Mode period value */
            #endif /* (FR_HBridge_KillModeMinTime) */
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
        
        
        /* Backup control register */
        #if(FR_HBridge_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (FR_HBridge_UseControl) */
        
    #endif /* (!FR_HBridge_UsingFixedFunction) */
   
}FR_HBridge_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/
 
void    FR_HBridge_Start(void) ;
void    FR_HBridge_Stop(void) ;
#if (FR_HBridge_UseStatus || FR_HBridge_UsingFixedFunction)
    #define FR_HBridge_SetInterruptMode(interruptMode) CY_SET_REG8(FR_HBridge_STATUS_MASK_PTR, interruptMode)
    #define FR_HBridge_ReadStatusRegister() CY_GET_REG8(FR_HBridge_STATUS_PTR)
#endif /* (FR_HBridge_UseStatus || FR_HBridge_UsingFixedFunction) */
#define FR_HBridge_GetInterruptSource() FR_HBridge_ReadStatusRegister()
#if (FR_HBridge_UseControl)
    #define FR_HBridge_ReadControlRegister() CY_GET_REG8(FR_HBridge_CONTROL_PTR) 
    #define FR_HBridge_WriteControlRegister(control) CY_SET_REG8(FR_HBridge_CONTROL_PTR, control)
#endif /* (FR_HBridge_UseControl) */
#if (FR_HBridge_UseOneCompareMode)
   #if (FR_HBridge_CompareMode1SW)
       void    FR_HBridge_SetCompareMode(uint8 comparemode) ;
   #endif /* (FR_HBridge_CompareMode1SW) */
#else
    #if (FR_HBridge_CompareMode1SW)
        void    FR_HBridge_SetCompareMode1(uint8 comparemode) \
                                                ;
    #endif /* (FR_HBridge_CompareMode1SW) */
    #if (FR_HBridge_CompareMode2SW)
        void    FR_HBridge_SetCompareMode2(uint8 comparemode) \
                                                ;
    #endif /* (FR_HBridge_CompareMode2SW) */
#endif /* (FR_HBridge_UseOneCompareMode) */

#if (!FR_HBridge_UsingFixedFunction)
    uint16   FR_HBridge_ReadCounter(void) ;
    #define FR_HBridge_ReadCapture() CY_GET_REG16(FR_HBridge_CAPTURE_LSB_PTR)
    #if (FR_HBridge_UseStatus)
        void FR_HBridge_ClearFIFO(void) ;
    #endif /* (FR_HBridge_UseStatus) */

    void    FR_HBridge_WriteCounter(uint16 counter) \
                                       ;
#endif /* (!FR_HBridge_UsingFixedFunction) */

void    FR_HBridge_WritePeriod(uint16 period) \
                                     ;
#define FR_HBridge_ReadPeriod() CY_GET_REG16(FR_HBridge_PERIOD_LSB_PTR) 
#if (FR_HBridge_UseOneCompareMode)
    void    FR_HBridge_WriteCompare(uint16 compare) \
                                          ;
    #define FR_HBridge_ReadCompare() CY_GET_REG16(FR_HBridge_COMPARE1_LSB_PTR) 
#else
    void    FR_HBridge_WriteCompare1(uint16 compare) \
                                           ;
    #define FR_HBridge_ReadCompare1() CY_GET_REG16(FR_HBridge_COMPARE1_LSB_PTR) 
    void    FR_HBridge_WriteCompare2(uint16 compare) \
                                           ;
    #define FR_HBridge_ReadCompare2() CY_GET_REG16(FR_HBridge_COMPARE2_LSB_PTR) 
#endif /* (FR_HBridge_UseOneCompareMode) */


#if (FR_HBridge_DeadBandUsed)
    void    FR_HBridge_WriteDeadTime(uint8 deadtime) ;
    uint8   FR_HBridge_ReadDeadTime(void) ;
#endif /* (FR_HBridge_DeadBandUsed) */

#if ( FR_HBridge_KillModeMinTime)
    #define FR_HBridge_WriteKillTime(killtime) CY_SET_REG8(FR_HBridge_KILLMODEMINTIME_PTR, killtime) 
    #define FR_HBridge_ReadKillTime() CY_GET_REG8(FR_HBridge_KILLMODEMINTIME_PTR) 
#endif /* ( FR_HBridge_KillModeMinTime) */

void FR_HBridge_Init(void) ;
void FR_HBridge_Enable(void) ;
void FR_HBridge_Sleep(void) ;
void FR_HBridge_Wakeup(void) ;
void FR_HBridge_SaveConfig(void) ;
void FR_HBridge_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define FR_HBridge_INIT_PERIOD_VALUE        999u
#define FR_HBridge_INIT_COMPARE_VALUE1      500u
#define FR_HBridge_INIT_COMPARE_VALUE2      63u
#define FR_HBridge_INIT_INTERRUPTS_MODE     ((0u << FR_HBridge_STATUS_TC_INT_EN_MASK_SHIFT) | \
                                                  (0 << FR_HBridge_STATUS_CMP2_INT_EN_MASK_SHIFT) | \
                                                  (0 << FR_HBridge_STATUS_CMP1_INT_EN_MASK_SHIFT ) | \
                                                  (0 << FR_HBridge_STATUS_KILL_INT_EN_MASK_SHIFT ))
#define FR_HBridge_DEFAULT_COMPARE2_MODE    (1u << FR_HBridge_CTRL_CMPMODE2_SHIFT)
#define FR_HBridge_DEFAULT_COMPARE1_MODE    (1u << FR_HBridge_CTRL_CMPMODE1_SHIFT)
#define FR_HBridge_INIT_DEAD_TIME           1u


/********************************
*         Registers
******************************** */

#if (FR_HBridge_UsingFixedFunction)
   #define FR_HBridge_PERIOD_LSB          (*(reg16 *) FR_HBridge_PWMHW__PER0)
   #define FR_HBridge_PERIOD_LSB_PTR      ( (reg16 *) FR_HBridge_PWMHW__PER0)
   #define FR_HBridge_COMPARE1_LSB        (*(reg16 *) FR_HBridge_PWMHW__CNT_CMP0)
   #define FR_HBridge_COMPARE1_LSB_PTR    ( (reg16 *) FR_HBridge_PWMHW__CNT_CMP0)
   #define FR_HBridge_COMPARE2_LSB        0x00u
   #define FR_HBridge_COMPARE2_LSB_PTR    0x00u
   #define FR_HBridge_COUNTER_LSB         (*(reg16 *) FR_HBridge_PWMHW__CNT_CMP0)
   #define FR_HBridge_COUNTER_LSB_PTR     ( (reg16 *) FR_HBridge_PWMHW__CNT_CMP0)
   #define FR_HBridge_CAPTURE_LSB         (*(reg16 *) FR_HBridge_PWMHW__CAP0)
   #define FR_HBridge_CAPTURE_LSB_PTR     ( (reg16 *) FR_HBridge_PWMHW__CAP0)
   #define FR_HBridge_RT1                 (*(reg8 *)  FR_HBridge_PWMHW__RT1)
   #define FR_HBridge_RT1_PTR             ( (reg8 *)  FR_HBridge_PWMHW__RT1)
      
#else
   #if(FR_HBridge_PWMModeIsCenterAligned)
       #define FR_HBridge_PERIOD_LSB      (*(reg16 *) FR_HBridge_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define FR_HBridge_PERIOD_LSB_PTR   ((reg16 *) FR_HBridge_PWMUDB_sP16_pwmdp_u0__D1_REG)
   #else
       #define FR_HBridge_PERIOD_LSB      (*(reg16 *) FR_HBridge_PWMUDB_sP16_pwmdp_u0__F0_REG)
       #define FR_HBridge_PERIOD_LSB_PTR   ((reg16 *) FR_HBridge_PWMUDB_sP16_pwmdp_u0__F0_REG)
   #endif /* (FR_HBridge_PWMModeIsCenterAligned) */
   #define FR_HBridge_COMPARE1_LSB    (*(reg16 *) FR_HBridge_PWMUDB_sP16_pwmdp_u0__D0_REG)
   #define FR_HBridge_COMPARE1_LSB_PTR ((reg16 *) FR_HBridge_PWMUDB_sP16_pwmdp_u0__D0_REG)
   #define FR_HBridge_COMPARE2_LSB    (*(reg16 *) FR_HBridge_PWMUDB_sP16_pwmdp_u0__D1_REG)
   #define FR_HBridge_COMPARE2_LSB_PTR ((reg16 *) FR_HBridge_PWMUDB_sP16_pwmdp_u0__D1_REG)
   #define FR_HBridge_COUNTERCAP_LSB   *(reg16 *) FR_HBridge_PWMUDB_sP16_pwmdp_u0__A1_REG)
   #define FR_HBridge_COUNTERCAP_LSB_PTR ((reg16 *) FR_HBridge_PWMUDB_sP16_pwmdp_u0__A1_REG)
   #define FR_HBridge_COUNTER_LSB     (*(reg16 *) FR_HBridge_PWMUDB_sP16_pwmdp_u0__A0_REG)
   #define FR_HBridge_COUNTER_LSB_PTR  ((reg16 *) FR_HBridge_PWMUDB_sP16_pwmdp_u0__A0_REG)
   #define FR_HBridge_CAPTURE_LSB     (*(reg16 *) FR_HBridge_PWMUDB_sP16_pwmdp_u0__F1_REG)
   #define FR_HBridge_CAPTURE_LSB_PTR  ((reg16 *) FR_HBridge_PWMUDB_sP16_pwmdp_u0__F1_REG)
   #define FR_HBridge_AUX_CONTROLDP0      (*(reg8 *) FR_HBridge_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)
   #define FR_HBridge_AUX_CONTROLDP0_PTR  ((reg8 *) FR_HBridge_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)
   #if (FR_HBridge_Resolution == 16)
       #define FR_HBridge_AUX_CONTROLDP1    (*(reg8 *) FR_HBridge_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)
       #define FR_HBridge_AUX_CONTROLDP1_PTR  ((reg8 *) FR_HBridge_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)
   #endif /* (FR_HBridge_Resolution == 16) */
#endif /* (FR_HBridge_UsingFixedFunction) */
   
#if(FR_HBridge_KillModeMinTime )
    #define FR_HBridge_KILLMODEMINTIME      (*(reg8 *) FR_HBridge_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define FR_HBridge_KILLMODEMINTIME_PTR   ((reg8 *) FR_HBridge_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (FR_HBridge_KillModeMinTime ) */

#if(FR_HBridge_DeadBandMode == FR_HBridge__B_PWM__DBM_256_CLOCKS)
    #define FR_HBridge_DEADBAND_COUNT      (*(reg8 *) FR_HBridge_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define FR_HBridge_DEADBAND_COUNT_PTR  ((reg8 *) FR_HBridge_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define FR_HBridge_DEADBAND_LSB_PTR    ((reg8 *) FR_HBridge_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define FR_HBridge_DEADBAND_LSB        (*(reg8 *) FR_HBridge_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(FR_HBridge_DeadBandMode == FR_HBridge__B_PWM__DBM_2_4_CLOCKS)
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (FR_HBridge_UsingFixedFunction)
        #define FR_HBridge_DEADBAND_COUNT        (*(reg8 *) FR_HBridge_PWMHW__CFG0) 
        #define FR_HBridge_DEADBAND_COUNT_PTR     ((reg8 *) FR_HBridge_PWMHW__CFG0)
        #define FR_HBridge_DEADBAND_COUNT_MASK    (0x03u << FR_HBridge_DEADBAND_COUNT_SHIFT)
        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define FR_HBridge_DEADBAND_COUNT_SHIFT   0x06u  
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define FR_HBridge_DEADBAND_COUNT        (*(reg8 *) FR_HBridge_PWMUDB_sDB3_SyncCtl_dbctrlreg__CONTROL_REG)
        #define FR_HBridge_DEADBAND_COUNT_PTR     ((reg8 *) FR_HBridge_PWMUDB_sDB3_SyncCtl_dbctrlreg__CONTROL_REG)
        #define FR_HBridge_DEADBAND_COUNT_MASK    (0x03u << FR_HBridge_DEADBAND_COUNT_SHIFT) 
        /* As defined by the verilog implementation of the Control Register */
        #define FR_HBridge_DEADBAND_COUNT_SHIFT   0x00u 
    #endif /* (FR_HBridge_UsingFixedFunction) */
#endif /* (FR_HBridge_DeadBandMode == FR_HBridge__B_PWM__DBM_256_CLOCKS) */



#if (FR_HBridge_UsingFixedFunction)
    #define FR_HBridge_STATUS                (*(reg8 *) FR_HBridge_PWMHW__SR0)
    #define FR_HBridge_STATUS_PTR            ((reg8 *) FR_HBridge_PWMHW__SR0)
    #define FR_HBridge_STATUS_MASK           (*(reg8 *) FR_HBridge_PWMHW__SR0)
    #define FR_HBridge_STATUS_MASK_PTR       ((reg8 *) FR_HBridge_PWMHW__SR0)
    #define FR_HBridge_CONTROL               (*(reg8 *) FR_HBridge_PWMHW__CFG0)
    #define FR_HBridge_CONTROL_PTR           ((reg8 *) FR_HBridge_PWMHW__CFG0)    
    #define FR_HBridge_CONTROL2              (*(reg8 *) FR_HBridge_PWMHW__CFG1)    
    #if(CY_PSOC3 || CY_PSOC5LP)
        #define FR_HBridge_CONTROL3              (*(reg8 *) FR_HBridge_PWMHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define FR_HBridge_GLOBAL_ENABLE         (*(reg8 *) FR_HBridge_PWMHW__PM_ACT_CFG)
    #define FR_HBridge_GLOBAL_ENABLE_PTR       ( (reg8 *) FR_HBridge_PWMHW__PM_ACT_CFG)
    #define FR_HBridge_GLOBAL_STBY_ENABLE      (*(reg8 *) FR_HBridge_PWMHW__PM_STBY_CFG)
    #define FR_HBridge_GLOBAL_STBY_ENABLE_PTR  ( (reg8 *) FR_HBridge_PWMHW__PM_STBY_CFG)
  
  
    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define FR_HBridge_BLOCK_EN_MASK          FR_HBridge_PWMHW__PM_ACT_MSK
    #define FR_HBridge_BLOCK_STBY_EN_MASK     FR_HBridge_PWMHW__PM_STBY_MSK 
    /* Control Register definitions */
    #define FR_HBridge_CTRL_ENABLE_SHIFT      0x00u
    
    #if(CY_PSOC5A)
        #define FR_HBridge_CTRL_CMPMODE1_SHIFT    0x01u   /* As defined by Register map as MODE_CFG bits in CFG1*/
    #endif /* (CY_PSOC5A) */
    #if(CY_PSOC3 || CY_PSOC5LP)
        #define FR_HBridge_CTRL_CMPMODE1_SHIFT    0x04u  /* As defined by Register map as MODE_CFG bits in CFG2*/
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    
    #define FR_HBridge_CTRL_DEAD_TIME_SHIFT   0x06u   /* As defined by Register map */
    /* Fixed Function Block Only CFG register bit definitions */
    #define FR_HBridge_CFG0_MODE              0x02u   /*  Set to compare mode */
    /* #define FR_HBridge_CFG0_ENABLE            0x01u */  /* Enable the block to run */
    #define FR_HBridge_CFG0_DB                0x20u   /* As defined by Register map as DB bit in CFG0 */

    /* Control Register Bit Masks */
    #define FR_HBridge_CTRL_ENABLE            (0x01u << FR_HBridge_CTRL_ENABLE_SHIFT)
    #define FR_HBridge_CTRL_RESET             (0x01u << FR_HBridge_CTRL_RESET_SHIFT)
    #define FR_HBridge_CTRL_CMPMODE2_MASK     (0x07u << FR_HBridge_CTRL_CMPMODE2_SHIFT)
    #if(CY_PSOC5A)
        #define FR_HBridge_CTRL_CMPMODE1_MASK     (0x07u << FR_HBridge_CTRL_CMPMODE1_SHIFT)
    #endif /* (CY_PSOC5A) */
    #if(CY_PSOC3 || CY_PSOC5LP)
        #define FR_HBridge_CTRL_CMPMODE1_MASK     (0x07u << FR_HBridge_CTRL_CMPMODE1_SHIFT)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    
    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define FR_HBridge_CTRL2_IRQ_SEL_SHIFT    0x00u       
    #define FR_HBridge_CTRL2_IRQ_SEL          (0x01u << FR_HBridge_CTRL2_IRQ_SEL_SHIFT)  
    
    /* Status Register Bit Locations */
    #define FR_HBridge_STATUS_TC_SHIFT            0x07u   /* As defined by Register map as TC in SR0 */
    #define FR_HBridge_STATUS_CMP1_SHIFT          0x06u   /* As defined by the Register map as CAP_CMP in SR0 */
    
    /* Status Register Interrupt Enable Bit Locations */
    #define FR_HBridge_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)    
    #define FR_HBridge_STATUS_TC_INT_EN_MASK_SHIFT            (FR_HBridge_STATUS_TC_SHIFT - 4)
    #define FR_HBridge_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)  
    #define FR_HBridge_STATUS_CMP1_INT_EN_MASK_SHIFT          (FR_HBridge_STATUS_CMP1_SHIFT - 4)
    
    /* Status Register Bit Masks */
    #define FR_HBridge_STATUS_TC              (0x01u << FR_HBridge_STATUS_TC_SHIFT)
    #define FR_HBridge_STATUS_CMP1            (0x01u << FR_HBridge_STATUS_CMP1_SHIFT)
    
    /* Status Register Interrupt Bit Masks*/
    #define FR_HBridge_STATUS_TC_INT_EN_MASK              (FR_HBridge_STATUS_TC >> 4)
    #define FR_HBridge_STATUS_CMP1_INT_EN_MASK            (FR_HBridge_STATUS_CMP1 >> 4)
    
    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP*/
    #define FR_HBridge_RT1_SHIFT             0x04u
    #define FR_HBridge_RT1_MASK              (0x03u << FR_HBridge_RT1_SHIFT)/* Sync TC and CMP bit masks */
    #define FR_HBridge_SYNC                  (0x03u << FR_HBridge_RT1_SHIFT)
    #define FR_HBridge_SYNCDSI_SHIFT         0x00u
    #define FR_HBridge_SYNCDSI_MASK          (0x0Fu << FR_HBridge_SYNCDSI_SHIFT) /* Sync all DSI inputs */
    #define FR_HBridge_SYNCDSI_EN            (0x0Fu << FR_HBridge_SYNCDSI_SHIFT) /* Sync all DSI inputs */
    

#else
    #define FR_HBridge_STATUS                (*(reg8 *) FR_HBridge_PWMUDB_sSTSReg_rstSts_stsreg__STATUS_REG )
    #define FR_HBridge_STATUS_PTR            ((reg8 *) FR_HBridge_PWMUDB_sSTSReg_rstSts_stsreg__STATUS_REG )
    #define FR_HBridge_STATUS_MASK           (*(reg8 *) FR_HBridge_PWMUDB_sSTSReg_rstSts_stsreg__MASK_REG)
    #define FR_HBridge_STATUS_MASK_PTR       ((reg8 *) FR_HBridge_PWMUDB_sSTSReg_rstSts_stsreg__MASK_REG)
    #define FR_HBridge_STATUS_AUX_CTRL       (*(reg8 *) FR_HBridge_PWMUDB_sSTSReg_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define FR_HBridge_CONTROL               (*(reg8 *) FR_HBridge_PWMUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define FR_HBridge_CONTROL_PTR           ((reg8 *) FR_HBridge_PWMUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    
    
    /***********************************
    *          Constants
    ***********************************/

    /* Control Register definitions */
    #define FR_HBridge_CTRL_ENABLE_SHIFT      0x07u
    #define FR_HBridge_CTRL_RESET_SHIFT       0x06u
    #define FR_HBridge_CTRL_CMPMODE2_SHIFT    0x03u
    #define FR_HBridge_CTRL_CMPMODE1_SHIFT    0x00u
    #define FR_HBridge_CTRL_DEAD_TIME_SHIFT   0x00u   /* No Shift Needed for UDB block */
    /* Control Register Bit Masks */
    #define FR_HBridge_CTRL_ENABLE            (0x01u << FR_HBridge_CTRL_ENABLE_SHIFT)
    #define FR_HBridge_CTRL_RESET             (0x01u << FR_HBridge_CTRL_RESET_SHIFT)
    #define FR_HBridge_CTRL_CMPMODE2_MASK     (0x07u << FR_HBridge_CTRL_CMPMODE2_SHIFT)
    #define FR_HBridge_CTRL_CMPMODE1_MASK     (0x07u << FR_HBridge_CTRL_CMPMODE1_SHIFT) 
    
    /* Status Register Bit Locations */
    #define FR_HBridge_STATUS_KILL_SHIFT          0x05u
    #define FR_HBridge_STATUS_FIFONEMPTY_SHIFT    0x04u
    #define FR_HBridge_STATUS_FIFOFULL_SHIFT      0x03u  
    #define FR_HBridge_STATUS_TC_SHIFT            0x02u
    #define FR_HBridge_STATUS_CMP2_SHIFT          0x01u
    #define FR_HBridge_STATUS_CMP1_SHIFT          0x00u
    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define FR_HBridge_STATUS_KILL_INT_EN_MASK_SHIFT          FR_HBridge_STATUS_KILL_SHIFT          
    #define FR_HBridge_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    FR_HBridge_STATUS_FIFONEMPTY_SHIFT    
    #define FR_HBridge_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      FR_HBridge_STATUS_FIFOFULL_SHIFT        
    #define FR_HBridge_STATUS_TC_INT_EN_MASK_SHIFT            FR_HBridge_STATUS_TC_SHIFT            
    #define FR_HBridge_STATUS_CMP2_INT_EN_MASK_SHIFT          FR_HBridge_STATUS_CMP2_SHIFT          
    #define FR_HBridge_STATUS_CMP1_INT_EN_MASK_SHIFT          FR_HBridge_STATUS_CMP1_SHIFT   
    /* Status Register Bit Masks */
    #define FR_HBridge_STATUS_KILL            (0x00u << FR_HBridge_STATUS_KILL_SHIFT )
    #define FR_HBridge_STATUS_FIFOFULL        (0x01u << FR_HBridge_STATUS_FIFOFULL_SHIFT)
    #define FR_HBridge_STATUS_FIFONEMPTY      (0x01u << FR_HBridge_STATUS_FIFONEMPTY_SHIFT)
    #define FR_HBridge_STATUS_TC              (0x01u << FR_HBridge_STATUS_TC_SHIFT)
    #define FR_HBridge_STATUS_CMP2            (0x01u << FR_HBridge_STATUS_CMP2_SHIFT) 
    #define FR_HBridge_STATUS_CMP1            (0x01u << FR_HBridge_STATUS_CMP1_SHIFT)
    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define FR_HBridge_STATUS_KILL_INT_EN_MASK            FR_HBridge_STATUS_KILL
    #define FR_HBridge_STATUS_FIFOFULL_INT_EN_MASK        FR_HBridge_STATUS_FIFOFULL
    #define FR_HBridge_STATUS_FIFONEMPTY_INT_EN_MASK      FR_HBridge_STATUS_FIFONEMPTY
    #define FR_HBridge_STATUS_TC_INT_EN_MASK              FR_HBridge_STATUS_TC
    #define FR_HBridge_STATUS_CMP2_INT_EN_MASK            FR_HBridge_STATUS_CMP2
    #define FR_HBridge_STATUS_CMP1_INT_EN_MASK            FR_HBridge_STATUS_CMP1
                                                          
    /* Datapath Auxillary Control Register definitions */
    #define FR_HBridge_AUX_CTRL_FIFO0_CLR     0x01u
    #define FR_HBridge_AUX_CTRL_FIFO1_CLR     0x02u
    #define FR_HBridge_AUX_CTRL_FIFO0_LVL     0x04u
    #define FR_HBridge_AUX_CTRL_FIFO1_LVL     0x08u
    #define FR_HBridge_STATUS_ACTL_INT_EN_MASK  0x10u /* As defined for the ACTL Register */
#endif /* FR_HBridge_UsingFixedFunction */

#endif  /* CY_PWM_FR_HBridge_H */


/* [] END OF FILE */
