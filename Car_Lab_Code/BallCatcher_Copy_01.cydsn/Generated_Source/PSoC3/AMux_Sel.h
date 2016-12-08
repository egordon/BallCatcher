/*******************************************************************************
* File Name: AMux_Sel.h  
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

#if !defined(CY_PWM_AMux_Sel_H)
#define CY_PWM_AMux_Sel_H


/***************************************
* Conditional Compilation Parameters
***************************************/
#define AMux_Sel_Resolution 8u
#define AMux_Sel_UsingFixedFunction 0u
#define AMux_Sel_DeadBandMode 0u
#define AMux_Sel_KillModeMinTime 0u
#define AMux_Sel_KillMode 0u
#define AMux_Sel_PWMMode 1u
#define AMux_Sel_PWMModeIsCenterAligned 0u
#define AMux_Sel_DeadBandUsed 0u
#define AMux_Sel_DeadBand2_4 0u
#if !defined(AMux_Sel_PWMUDB_sSTSReg_stsreg__REMOVED)
    #define AMux_Sel_UseStatus 0u
#else
    #define AMux_Sel_UseStatus 0u
#endif /* !defined(AMux_Sel_PWMUDB_sSTSReg_stsreg__REMOVED) */
#if !defined(AMux_Sel_PWMUDB_sCTRLReg_ctrlreg__REMOVED)
    #define AMux_Sel_UseControl 1u
#else
    #define AMux_Sel_UseControl 0u
#endif /* !defined(AMux_Sel_PWMUDB_sCTRLReg_ctrlreg__REMOVED) */
#define AMux_Sel_UseOneCompareMode 0u
#define AMux_Sel_MinimumKillTime 1u
#define AMux_Sel_EnableMode 0u

#define AMux_Sel_CompareMode1SW 0u
#define AMux_Sel_CompareMode2SW 0u

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define AMux_Sel__B_PWM__DISABLED 0
#define AMux_Sel__B_PWM__ASYNCHRONOUS 1
#define AMux_Sel__B_PWM__SINGLECYCLE 2
#define AMux_Sel__B_PWM__LATCHED 3
#define AMux_Sel__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define AMux_Sel__B_PWM__DBMDISABLED 0
#define AMux_Sel__B_PWM__DBM_2_4_CLOCKS 1
#define AMux_Sel__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define AMux_Sel__B_PWM__ONE_OUTPUT 0
#define AMux_Sel__B_PWM__TWO_OUTPUTS 1
#define AMux_Sel__B_PWM__DUAL_EDGE 2
#define AMux_Sel__B_PWM__CENTER_ALIGN 3
#define AMux_Sel__B_PWM__DITHER 5
#define AMux_Sel__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define AMux_Sel__B_PWM__LESS_THAN 1
#define AMux_Sel__B_PWM__LESS_THAN_OR_EQUAL 2
#define AMux_Sel__B_PWM__GREATER_THAN 3
#define AMux_Sel__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define AMux_Sel__B_PWM__EQUAL 0
#define AMux_Sel__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct AMux_Sel_backupStruct
{
    
    uint8 PWMEnableState;
       
    #if(!AMux_Sel_UsingFixedFunction)
        #if (CY_PSOC5A)
            uint8 PWMUdb;               /* PWM Current Counter value  */
            uint8 PWMPeriod;            /* PWM Current Period value   */
            #if (AMux_Sel_UseStatus)
                uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
            #endif /* (AMux_Sel_UseStatus) */
            #if(AMux_Sel_UseOneCompareMode)
                uint8 PWMCompareValue;     /* PWM Current Compare value */
            #else
                uint8 PWMCompareValue1;     /* PWM Current Compare value1 */
                uint8 PWMCompareValue2;     /* PWM Current Compare value2 */
            #endif /* (AMux_Sel_UseOneCompareMode) */
            
            /* Backup for Deadband parameters */
            #if(AMux_Sel_DeadBandMode == AMux_Sel__B_PWM__DBM_256_CLOCKS || \
                AMux_Sel_DeadBandMode == AMux_Sel__B_PWM__DBM_2_4_CLOCKS)
                uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
            #endif /* deadband count is either 2-4 clocks or 256 clocks */
            
            /* Backup Kill Mode Counter*/
            #if(AMux_Sel_KillModeMinTime)
                uint8 PWMKillCounterPeriod; /* Kill Mode period value */
            #endif /* (AMux_Sel_KillModeMinTime) */
            
        #endif /* (CY_PSOC5A) */
        
        #if (CY_PSOC3 || CY_PSOC5LP)
            uint8 PWMUdb;               /* PWM Current Counter value  */
            #if(!AMux_Sel_PWMModeIsCenterAligned)
                uint8 PWMPeriod;
            #endif /* (!AMux_Sel_PWMModeIsCenterAligned) */
            #if (AMux_Sel_UseStatus)
                uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
            #endif /* (AMux_Sel_UseStatus) */
            
            /* Backup for Deadband parameters */
            #if(AMux_Sel_DeadBandMode == AMux_Sel__B_PWM__DBM_256_CLOCKS || \
                AMux_Sel_DeadBandMode == AMux_Sel__B_PWM__DBM_2_4_CLOCKS)
                uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
            #endif /* deadband count is either 2-4 clocks or 256 clocks */
            
            /* Backup Kill Mode Counter*/
            #if(AMux_Sel_KillModeMinTime)
                uint8 PWMKillCounterPeriod; /* Kill Mode period value */
            #endif /* (AMux_Sel_KillModeMinTime) */
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
        
        
        /* Backup control register */
        #if(AMux_Sel_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (AMux_Sel_UseControl) */
        
    #endif /* (!AMux_Sel_UsingFixedFunction) */
   
}AMux_Sel_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/
 
void    AMux_Sel_Start(void) ;
void    AMux_Sel_Stop(void) ;
#if (AMux_Sel_UseStatus || AMux_Sel_UsingFixedFunction)
    #define AMux_Sel_SetInterruptMode(interruptMode) CY_SET_REG8(AMux_Sel_STATUS_MASK_PTR, interruptMode)
    #define AMux_Sel_ReadStatusRegister() CY_GET_REG8(AMux_Sel_STATUS_PTR)
#endif /* (AMux_Sel_UseStatus || AMux_Sel_UsingFixedFunction) */
#define AMux_Sel_GetInterruptSource() AMux_Sel_ReadStatusRegister()
#if (AMux_Sel_UseControl)
    #define AMux_Sel_ReadControlRegister() CY_GET_REG8(AMux_Sel_CONTROL_PTR) 
    #define AMux_Sel_WriteControlRegister(control) CY_SET_REG8(AMux_Sel_CONTROL_PTR, control)
#endif /* (AMux_Sel_UseControl) */
#if (AMux_Sel_UseOneCompareMode)
   #if (AMux_Sel_CompareMode1SW)
       void    AMux_Sel_SetCompareMode(uint8 comparemode) ;
   #endif /* (AMux_Sel_CompareMode1SW) */
#else
    #if (AMux_Sel_CompareMode1SW)
        void    AMux_Sel_SetCompareMode1(uint8 comparemode) \
                                                ;
    #endif /* (AMux_Sel_CompareMode1SW) */
    #if (AMux_Sel_CompareMode2SW)
        void    AMux_Sel_SetCompareMode2(uint8 comparemode) \
                                                ;
    #endif /* (AMux_Sel_CompareMode2SW) */
#endif /* (AMux_Sel_UseOneCompareMode) */

#if (!AMux_Sel_UsingFixedFunction)
    uint8   AMux_Sel_ReadCounter(void) ;
    #define AMux_Sel_ReadCapture() CY_GET_REG8(AMux_Sel_CAPTURE_LSB_PTR)
    #if (AMux_Sel_UseStatus)
        void AMux_Sel_ClearFIFO(void) ;
    #endif /* (AMux_Sel_UseStatus) */

    void    AMux_Sel_WriteCounter(uint8 counter) \
                                       ;
#endif /* (!AMux_Sel_UsingFixedFunction) */

void    AMux_Sel_WritePeriod(uint8 period) \
                                     ;
#define AMux_Sel_ReadPeriod() CY_GET_REG8(AMux_Sel_PERIOD_LSB_PTR) 
#if (AMux_Sel_UseOneCompareMode)
    void    AMux_Sel_WriteCompare(uint8 compare) \
                                          ;
    #define AMux_Sel_ReadCompare() CY_GET_REG8(AMux_Sel_COMPARE1_LSB_PTR) 
#else
    void    AMux_Sel_WriteCompare1(uint8 compare) \
                                           ;
    #define AMux_Sel_ReadCompare1() CY_GET_REG8(AMux_Sel_COMPARE1_LSB_PTR) 
    void    AMux_Sel_WriteCompare2(uint8 compare) \
                                           ;
    #define AMux_Sel_ReadCompare2() CY_GET_REG8(AMux_Sel_COMPARE2_LSB_PTR) 
#endif /* (AMux_Sel_UseOneCompareMode) */


#if (AMux_Sel_DeadBandUsed)
    void    AMux_Sel_WriteDeadTime(uint8 deadtime) ;
    uint8   AMux_Sel_ReadDeadTime(void) ;
#endif /* (AMux_Sel_DeadBandUsed) */

#if ( AMux_Sel_KillModeMinTime)
    #define AMux_Sel_WriteKillTime(killtime) CY_SET_REG8(AMux_Sel_KILLMODEMINTIME_PTR, killtime) 
    #define AMux_Sel_ReadKillTime() CY_GET_REG8(AMux_Sel_KILLMODEMINTIME_PTR) 
#endif /* ( AMux_Sel_KillModeMinTime) */

void AMux_Sel_Init(void) ;
void AMux_Sel_Enable(void) ;
void AMux_Sel_Sleep(void) ;
void AMux_Sel_Wakeup(void) ;
void AMux_Sel_SaveConfig(void) ;
void AMux_Sel_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define AMux_Sel_INIT_PERIOD_VALUE        9u
#define AMux_Sel_INIT_COMPARE_VALUE1      5u
#define AMux_Sel_INIT_COMPARE_VALUE2      4u
#define AMux_Sel_INIT_INTERRUPTS_MODE     ((0u << AMux_Sel_STATUS_TC_INT_EN_MASK_SHIFT) | \
                                                  (0 << AMux_Sel_STATUS_CMP2_INT_EN_MASK_SHIFT) | \
                                                  (0 << AMux_Sel_STATUS_CMP1_INT_EN_MASK_SHIFT ) | \
                                                  (0 << AMux_Sel_STATUS_KILL_INT_EN_MASK_SHIFT ))
#define AMux_Sel_DEFAULT_COMPARE2_MODE    (0u << AMux_Sel_CTRL_CMPMODE2_SHIFT)
#define AMux_Sel_DEFAULT_COMPARE1_MODE    (1u << AMux_Sel_CTRL_CMPMODE1_SHIFT)
#define AMux_Sel_INIT_DEAD_TIME           1u


/********************************
*         Registers
******************************** */

#if (AMux_Sel_UsingFixedFunction)
   #define AMux_Sel_PERIOD_LSB          (*(reg16 *) AMux_Sel_PWMHW__PER0)
   #define AMux_Sel_PERIOD_LSB_PTR      ( (reg16 *) AMux_Sel_PWMHW__PER0)
   #define AMux_Sel_COMPARE1_LSB        (*(reg16 *) AMux_Sel_PWMHW__CNT_CMP0)
   #define AMux_Sel_COMPARE1_LSB_PTR    ( (reg16 *) AMux_Sel_PWMHW__CNT_CMP0)
   #define AMux_Sel_COMPARE2_LSB        0x00u
   #define AMux_Sel_COMPARE2_LSB_PTR    0x00u
   #define AMux_Sel_COUNTER_LSB         (*(reg16 *) AMux_Sel_PWMHW__CNT_CMP0)
   #define AMux_Sel_COUNTER_LSB_PTR     ( (reg16 *) AMux_Sel_PWMHW__CNT_CMP0)
   #define AMux_Sel_CAPTURE_LSB         (*(reg16 *) AMux_Sel_PWMHW__CAP0)
   #define AMux_Sel_CAPTURE_LSB_PTR     ( (reg16 *) AMux_Sel_PWMHW__CAP0)
   #define AMux_Sel_RT1                 (*(reg8 *)  AMux_Sel_PWMHW__RT1)
   #define AMux_Sel_RT1_PTR             ( (reg8 *)  AMux_Sel_PWMHW__RT1)
      
#else
   #if(AMux_Sel_PWMModeIsCenterAligned)
       #define AMux_Sel_PERIOD_LSB      (*(reg8 *) AMux_Sel_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define AMux_Sel_PERIOD_LSB_PTR   ((reg8 *) AMux_Sel_PWMUDB_sP8_pwmdp_u0__D1_REG)
   #else
       #define AMux_Sel_PERIOD_LSB      (*(reg8 *) AMux_Sel_PWMUDB_sP8_pwmdp_u0__F0_REG)
       #define AMux_Sel_PERIOD_LSB_PTR   ((reg8 *) AMux_Sel_PWMUDB_sP8_pwmdp_u0__F0_REG)
   #endif /* (AMux_Sel_PWMModeIsCenterAligned) */
   #define AMux_Sel_COMPARE1_LSB    (*(reg8 *) AMux_Sel_PWMUDB_sP8_pwmdp_u0__D0_REG)
   #define AMux_Sel_COMPARE1_LSB_PTR ((reg8 *) AMux_Sel_PWMUDB_sP8_pwmdp_u0__D0_REG)
   #define AMux_Sel_COMPARE2_LSB    (*(reg8 *) AMux_Sel_PWMUDB_sP8_pwmdp_u0__D1_REG)
   #define AMux_Sel_COMPARE2_LSB_PTR ((reg8 *) AMux_Sel_PWMUDB_sP8_pwmdp_u0__D1_REG)
   #define AMux_Sel_COUNTERCAP_LSB   *(reg8 *) AMux_Sel_PWMUDB_sP8_pwmdp_u0__A1_REG)
   #define AMux_Sel_COUNTERCAP_LSB_PTR ((reg8 *) AMux_Sel_PWMUDB_sP8_pwmdp_u0__A1_REG)
   #define AMux_Sel_COUNTER_LSB     (*(reg8 *) AMux_Sel_PWMUDB_sP8_pwmdp_u0__A0_REG)
   #define AMux_Sel_COUNTER_LSB_PTR  ((reg8 *) AMux_Sel_PWMUDB_sP8_pwmdp_u0__A0_REG)
   #define AMux_Sel_CAPTURE_LSB     (*(reg8 *) AMux_Sel_PWMUDB_sP8_pwmdp_u0__F1_REG)
   #define AMux_Sel_CAPTURE_LSB_PTR  ((reg8 *) AMux_Sel_PWMUDB_sP8_pwmdp_u0__F1_REG)
   #define AMux_Sel_AUX_CONTROLDP0      (*(reg8 *) AMux_Sel_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)
   #define AMux_Sel_AUX_CONTROLDP0_PTR  ((reg8 *) AMux_Sel_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)
   #if (AMux_Sel_Resolution == 16)
       #define AMux_Sel_AUX_CONTROLDP1    (*(reg8 *) AMux_Sel_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)
       #define AMux_Sel_AUX_CONTROLDP1_PTR  ((reg8 *) AMux_Sel_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)
   #endif /* (AMux_Sel_Resolution == 16) */
#endif /* (AMux_Sel_UsingFixedFunction) */
   
#if(AMux_Sel_KillModeMinTime )
    #define AMux_Sel_KILLMODEMINTIME      (*(reg8 *) AMux_Sel_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define AMux_Sel_KILLMODEMINTIME_PTR   ((reg8 *) AMux_Sel_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (AMux_Sel_KillModeMinTime ) */

#if(AMux_Sel_DeadBandMode == AMux_Sel__B_PWM__DBM_256_CLOCKS)
    #define AMux_Sel_DEADBAND_COUNT      (*(reg8 *) AMux_Sel_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define AMux_Sel_DEADBAND_COUNT_PTR  ((reg8 *) AMux_Sel_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define AMux_Sel_DEADBAND_LSB_PTR    ((reg8 *) AMux_Sel_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define AMux_Sel_DEADBAND_LSB        (*(reg8 *) AMux_Sel_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(AMux_Sel_DeadBandMode == AMux_Sel__B_PWM__DBM_2_4_CLOCKS)
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (AMux_Sel_UsingFixedFunction)
        #define AMux_Sel_DEADBAND_COUNT        (*(reg8 *) AMux_Sel_PWMHW__CFG0) 
        #define AMux_Sel_DEADBAND_COUNT_PTR     ((reg8 *) AMux_Sel_PWMHW__CFG0)
        #define AMux_Sel_DEADBAND_COUNT_MASK    (0x03u << AMux_Sel_DEADBAND_COUNT_SHIFT)
        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define AMux_Sel_DEADBAND_COUNT_SHIFT   0x06u  
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define AMux_Sel_DEADBAND_COUNT        (*(reg8 *) AMux_Sel_PWMUDB_sDB3_SyncCtl_dbctrlreg__CONTROL_REG)
        #define AMux_Sel_DEADBAND_COUNT_PTR     ((reg8 *) AMux_Sel_PWMUDB_sDB3_SyncCtl_dbctrlreg__CONTROL_REG)
        #define AMux_Sel_DEADBAND_COUNT_MASK    (0x03u << AMux_Sel_DEADBAND_COUNT_SHIFT) 
        /* As defined by the verilog implementation of the Control Register */
        #define AMux_Sel_DEADBAND_COUNT_SHIFT   0x00u 
    #endif /* (AMux_Sel_UsingFixedFunction) */
#endif /* (AMux_Sel_DeadBandMode == AMux_Sel__B_PWM__DBM_256_CLOCKS) */



#if (AMux_Sel_UsingFixedFunction)
    #define AMux_Sel_STATUS                (*(reg8 *) AMux_Sel_PWMHW__SR0)
    #define AMux_Sel_STATUS_PTR            ((reg8 *) AMux_Sel_PWMHW__SR0)
    #define AMux_Sel_STATUS_MASK           (*(reg8 *) AMux_Sel_PWMHW__SR0)
    #define AMux_Sel_STATUS_MASK_PTR       ((reg8 *) AMux_Sel_PWMHW__SR0)
    #define AMux_Sel_CONTROL               (*(reg8 *) AMux_Sel_PWMHW__CFG0)
    #define AMux_Sel_CONTROL_PTR           ((reg8 *) AMux_Sel_PWMHW__CFG0)    
    #define AMux_Sel_CONTROL2              (*(reg8 *) AMux_Sel_PWMHW__CFG1)    
    #if(CY_PSOC3 || CY_PSOC5LP)
        #define AMux_Sel_CONTROL3              (*(reg8 *) AMux_Sel_PWMHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define AMux_Sel_GLOBAL_ENABLE         (*(reg8 *) AMux_Sel_PWMHW__PM_ACT_CFG)
    #define AMux_Sel_GLOBAL_ENABLE_PTR       ( (reg8 *) AMux_Sel_PWMHW__PM_ACT_CFG)
    #define AMux_Sel_GLOBAL_STBY_ENABLE      (*(reg8 *) AMux_Sel_PWMHW__PM_STBY_CFG)
    #define AMux_Sel_GLOBAL_STBY_ENABLE_PTR  ( (reg8 *) AMux_Sel_PWMHW__PM_STBY_CFG)
  
  
    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define AMux_Sel_BLOCK_EN_MASK          AMux_Sel_PWMHW__PM_ACT_MSK
    #define AMux_Sel_BLOCK_STBY_EN_MASK     AMux_Sel_PWMHW__PM_STBY_MSK 
    /* Control Register definitions */
    #define AMux_Sel_CTRL_ENABLE_SHIFT      0x00u
    
    #if(CY_PSOC5A)
        #define AMux_Sel_CTRL_CMPMODE1_SHIFT    0x01u   /* As defined by Register map as MODE_CFG bits in CFG1*/
    #endif /* (CY_PSOC5A) */
    #if(CY_PSOC3 || CY_PSOC5LP)
        #define AMux_Sel_CTRL_CMPMODE1_SHIFT    0x04u  /* As defined by Register map as MODE_CFG bits in CFG2*/
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    
    #define AMux_Sel_CTRL_DEAD_TIME_SHIFT   0x06u   /* As defined by Register map */
    /* Fixed Function Block Only CFG register bit definitions */
    #define AMux_Sel_CFG0_MODE              0x02u   /*  Set to compare mode */
    /* #define AMux_Sel_CFG0_ENABLE            0x01u */  /* Enable the block to run */
    #define AMux_Sel_CFG0_DB                0x20u   /* As defined by Register map as DB bit in CFG0 */

    /* Control Register Bit Masks */
    #define AMux_Sel_CTRL_ENABLE            (0x01u << AMux_Sel_CTRL_ENABLE_SHIFT)
    #define AMux_Sel_CTRL_RESET             (0x01u << AMux_Sel_CTRL_RESET_SHIFT)
    #define AMux_Sel_CTRL_CMPMODE2_MASK     (0x07u << AMux_Sel_CTRL_CMPMODE2_SHIFT)
    #if(CY_PSOC5A)
        #define AMux_Sel_CTRL_CMPMODE1_MASK     (0x07u << AMux_Sel_CTRL_CMPMODE1_SHIFT)
    #endif /* (CY_PSOC5A) */
    #if(CY_PSOC3 || CY_PSOC5LP)
        #define AMux_Sel_CTRL_CMPMODE1_MASK     (0x07u << AMux_Sel_CTRL_CMPMODE1_SHIFT)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    
    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define AMux_Sel_CTRL2_IRQ_SEL_SHIFT    0x00u       
    #define AMux_Sel_CTRL2_IRQ_SEL          (0x01u << AMux_Sel_CTRL2_IRQ_SEL_SHIFT)  
    
    /* Status Register Bit Locations */
    #define AMux_Sel_STATUS_TC_SHIFT            0x07u   /* As defined by Register map as TC in SR0 */
    #define AMux_Sel_STATUS_CMP1_SHIFT          0x06u   /* As defined by the Register map as CAP_CMP in SR0 */
    
    /* Status Register Interrupt Enable Bit Locations */
    #define AMux_Sel_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)    
    #define AMux_Sel_STATUS_TC_INT_EN_MASK_SHIFT            (AMux_Sel_STATUS_TC_SHIFT - 4)
    #define AMux_Sel_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)  
    #define AMux_Sel_STATUS_CMP1_INT_EN_MASK_SHIFT          (AMux_Sel_STATUS_CMP1_SHIFT - 4)
    
    /* Status Register Bit Masks */
    #define AMux_Sel_STATUS_TC              (0x01u << AMux_Sel_STATUS_TC_SHIFT)
    #define AMux_Sel_STATUS_CMP1            (0x01u << AMux_Sel_STATUS_CMP1_SHIFT)
    
    /* Status Register Interrupt Bit Masks*/
    #define AMux_Sel_STATUS_TC_INT_EN_MASK              (AMux_Sel_STATUS_TC >> 4)
    #define AMux_Sel_STATUS_CMP1_INT_EN_MASK            (AMux_Sel_STATUS_CMP1 >> 4)
    
    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP*/
    #define AMux_Sel_RT1_SHIFT             0x04u
    #define AMux_Sel_RT1_MASK              (0x03u << AMux_Sel_RT1_SHIFT)/* Sync TC and CMP bit masks */
    #define AMux_Sel_SYNC                  (0x03u << AMux_Sel_RT1_SHIFT)
    #define AMux_Sel_SYNCDSI_SHIFT         0x00u
    #define AMux_Sel_SYNCDSI_MASK          (0x0Fu << AMux_Sel_SYNCDSI_SHIFT) /* Sync all DSI inputs */
    #define AMux_Sel_SYNCDSI_EN            (0x0Fu << AMux_Sel_SYNCDSI_SHIFT) /* Sync all DSI inputs */
    

#else
    #define AMux_Sel_STATUS                (*(reg8 *) AMux_Sel_PWMUDB_sSTSReg_rstSts_stsreg__STATUS_REG )
    #define AMux_Sel_STATUS_PTR            ((reg8 *) AMux_Sel_PWMUDB_sSTSReg_rstSts_stsreg__STATUS_REG )
    #define AMux_Sel_STATUS_MASK           (*(reg8 *) AMux_Sel_PWMUDB_sSTSReg_rstSts_stsreg__MASK_REG)
    #define AMux_Sel_STATUS_MASK_PTR       ((reg8 *) AMux_Sel_PWMUDB_sSTSReg_rstSts_stsreg__MASK_REG)
    #define AMux_Sel_STATUS_AUX_CTRL       (*(reg8 *) AMux_Sel_PWMUDB_sSTSReg_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define AMux_Sel_CONTROL               (*(reg8 *) AMux_Sel_PWMUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define AMux_Sel_CONTROL_PTR           ((reg8 *) AMux_Sel_PWMUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    
    
    /***********************************
    *          Constants
    ***********************************/

    /* Control Register definitions */
    #define AMux_Sel_CTRL_ENABLE_SHIFT      0x07u
    #define AMux_Sel_CTRL_RESET_SHIFT       0x06u
    #define AMux_Sel_CTRL_CMPMODE2_SHIFT    0x03u
    #define AMux_Sel_CTRL_CMPMODE1_SHIFT    0x00u
    #define AMux_Sel_CTRL_DEAD_TIME_SHIFT   0x00u   /* No Shift Needed for UDB block */
    /* Control Register Bit Masks */
    #define AMux_Sel_CTRL_ENABLE            (0x01u << AMux_Sel_CTRL_ENABLE_SHIFT)
    #define AMux_Sel_CTRL_RESET             (0x01u << AMux_Sel_CTRL_RESET_SHIFT)
    #define AMux_Sel_CTRL_CMPMODE2_MASK     (0x07u << AMux_Sel_CTRL_CMPMODE2_SHIFT)
    #define AMux_Sel_CTRL_CMPMODE1_MASK     (0x07u << AMux_Sel_CTRL_CMPMODE1_SHIFT) 
    
    /* Status Register Bit Locations */
    #define AMux_Sel_STATUS_KILL_SHIFT          0x05u
    #define AMux_Sel_STATUS_FIFONEMPTY_SHIFT    0x04u
    #define AMux_Sel_STATUS_FIFOFULL_SHIFT      0x03u  
    #define AMux_Sel_STATUS_TC_SHIFT            0x02u
    #define AMux_Sel_STATUS_CMP2_SHIFT          0x01u
    #define AMux_Sel_STATUS_CMP1_SHIFT          0x00u
    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define AMux_Sel_STATUS_KILL_INT_EN_MASK_SHIFT          AMux_Sel_STATUS_KILL_SHIFT          
    #define AMux_Sel_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    AMux_Sel_STATUS_FIFONEMPTY_SHIFT    
    #define AMux_Sel_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      AMux_Sel_STATUS_FIFOFULL_SHIFT        
    #define AMux_Sel_STATUS_TC_INT_EN_MASK_SHIFT            AMux_Sel_STATUS_TC_SHIFT            
    #define AMux_Sel_STATUS_CMP2_INT_EN_MASK_SHIFT          AMux_Sel_STATUS_CMP2_SHIFT          
    #define AMux_Sel_STATUS_CMP1_INT_EN_MASK_SHIFT          AMux_Sel_STATUS_CMP1_SHIFT   
    /* Status Register Bit Masks */
    #define AMux_Sel_STATUS_KILL            (0x00u << AMux_Sel_STATUS_KILL_SHIFT )
    #define AMux_Sel_STATUS_FIFOFULL        (0x01u << AMux_Sel_STATUS_FIFOFULL_SHIFT)
    #define AMux_Sel_STATUS_FIFONEMPTY      (0x01u << AMux_Sel_STATUS_FIFONEMPTY_SHIFT)
    #define AMux_Sel_STATUS_TC              (0x01u << AMux_Sel_STATUS_TC_SHIFT)
    #define AMux_Sel_STATUS_CMP2            (0x01u << AMux_Sel_STATUS_CMP2_SHIFT) 
    #define AMux_Sel_STATUS_CMP1            (0x01u << AMux_Sel_STATUS_CMP1_SHIFT)
    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define AMux_Sel_STATUS_KILL_INT_EN_MASK            AMux_Sel_STATUS_KILL
    #define AMux_Sel_STATUS_FIFOFULL_INT_EN_MASK        AMux_Sel_STATUS_FIFOFULL
    #define AMux_Sel_STATUS_FIFONEMPTY_INT_EN_MASK      AMux_Sel_STATUS_FIFONEMPTY
    #define AMux_Sel_STATUS_TC_INT_EN_MASK              AMux_Sel_STATUS_TC
    #define AMux_Sel_STATUS_CMP2_INT_EN_MASK            AMux_Sel_STATUS_CMP2
    #define AMux_Sel_STATUS_CMP1_INT_EN_MASK            AMux_Sel_STATUS_CMP1
                                                          
    /* Datapath Auxillary Control Register definitions */
    #define AMux_Sel_AUX_CTRL_FIFO0_CLR     0x01u
    #define AMux_Sel_AUX_CTRL_FIFO1_CLR     0x02u
    #define AMux_Sel_AUX_CTRL_FIFO0_LVL     0x04u
    #define AMux_Sel_AUX_CTRL_FIFO1_LVL     0x08u
    #define AMux_Sel_STATUS_ACTL_INT_EN_MASK  0x10u /* As defined for the ACTL Register */
#endif /* AMux_Sel_UsingFixedFunction */

#endif  /* CY_PWM_AMux_Sel_H */


/* [] END OF FILE */
