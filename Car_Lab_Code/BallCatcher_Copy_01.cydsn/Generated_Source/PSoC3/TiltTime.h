/*******************************************************************************
* File Name: TiltTime.h
* Version 2.30
*
*  Description:
*     Contains the function prototypes and constants available to the timer
*     user module.
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_Timer_v2_30_TiltTime_H)
#define CY_Timer_v2_30_TiltTime_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define TiltTime_Resolution                 16u
#define TiltTime_UsingFixedFunction         0u
#define TiltTime_UsingHWCaptureCounter      0u
#define TiltTime_SoftwareCaptureMode        0u
#define TiltTime_SoftwareTriggerMode        0u
#define TiltTime_UsingHWEnable              0u
#define TiltTime_EnableTriggerMode          1u
#define TiltTime_InterruptOnCaptureCount    1u
#define TiltTime_RunModeUsed                2u
#define TiltTime_ControlRegRemoved          0u


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct TiltTime_backupStruct
{
    uint8 TimerEnableState;
    #if(!TiltTime_UsingFixedFunction)
        #if (CY_PSOC5A)
            uint16 TimerUdb;                 /* Timer internal counter value */
            uint16 TimerPeriod;              /* Timer Period value       */
            uint8 InterruptMaskValue;       /* Timer Compare Value */
            #if (TiltTime_UsingHWCaptureCounter)
                uint16 TimerCaptureCounter;  /* Timer Capture Counter Value */
            #endif /* variable declaration for backing up Capture Counter value*/
        #endif /* variables for non retention registers in PSoC5A */

        #if (CY_PSOC3 || CY_PSOC5LP)
            uint16 TimerUdb;
            uint8 InterruptMaskValue;
            #if (TiltTime_UsingHWCaptureCounter)
                uint16 TimerCaptureCounter;
            #endif /* variable declarations for backing up non retention registers in PSoC3 or PSoC5LP */
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */

        #if (!TiltTime_ControlRegRemoved)
        uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */
}TiltTime_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    TiltTime_Start(void) ;
void    TiltTime_Stop(void) ;

void    TiltTime_SetInterruptMode(uint8 interruptMode) ;
uint8   TiltTime_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define TiltTime_GetInterruptSource() TiltTime_ReadStatusRegister()

#if(!TiltTime_ControlRegRemoved)
    uint8   TiltTime_ReadControlRegister(void) ;
    void    TiltTime_WriteControlRegister(uint8 control) \
        ;
#endif /* (!TiltTime_ControlRegRemoved) */

uint16  TiltTime_ReadPeriod(void) ;
void    TiltTime_WritePeriod(uint16 period) \
    ;
uint16  TiltTime_ReadCounter(void) ;
void    TiltTime_WriteCounter(uint16 counter) \
    ;
uint16  TiltTime_ReadCapture(void) ;
void    TiltTime_SoftwareCapture(void) ;


#if(!TiltTime_UsingFixedFunction) /* UDB Prototypes */
    #if (TiltTime_SoftwareCaptureMode)
        void    TiltTime_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!TiltTime_UsingFixedFunction) */

    #if (TiltTime_SoftwareTriggerMode)
        void    TiltTime_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (TiltTime_SoftwareTriggerMode) */
    #if (TiltTime_EnableTriggerMode)
        void    TiltTime_EnableTrigger(void) ;
        void    TiltTime_DisableTrigger(void) ;
    #endif /* (TiltTime_EnableTriggerMode) */

    #if(TiltTime_InterruptOnCaptureCount)
        #if(!TiltTime_ControlRegRemoved)
            void    TiltTime_SetInterruptCount(uint8 interruptCount) \
                ;
        #endif /* (!TiltTime_ControlRegRemoved) */
    #endif /* (TiltTime_InterruptOnCaptureCount) */

    #if (TiltTime_UsingHWCaptureCounter)
        void    TiltTime_SetCaptureCount(uint8 captureCount) \
            ;
        uint8   TiltTime_ReadCaptureCount(void) ;
    #endif /* (TiltTime_UsingHWCaptureCounter) */

    void TiltTime_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void TiltTime_Init(void)          ;
void TiltTime_Enable(void)        ;
void TiltTime_SaveConfig(void)    ;
void TiltTime_RestoreConfig(void) ;
void TiltTime_Sleep(void)         ;
void TiltTime_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define TiltTime__B_TIMER__CM_NONE 0
#define TiltTime__B_TIMER__CM_RISINGEDGE 1
#define TiltTime__B_TIMER__CM_FALLINGEDGE 2
#define TiltTime__B_TIMER__CM_EITHEREDGE 3
#define TiltTime__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define TiltTime__B_TIMER__TM_NONE 0x00u
#define TiltTime__B_TIMER__TM_RISINGEDGE 0x04u
#define TiltTime__B_TIMER__TM_FALLINGEDGE 0x08u
#define TiltTime__B_TIMER__TM_EITHEREDGE 0x0Cu
#define TiltTime__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define TiltTime_INIT_PERIOD             9999u
#define TiltTime_INIT_CAPTURE_MODE       (2u << TiltTime_CTRL_CAP_MODE_SHIFT)
#define TiltTime_INIT_TRIGGER_MODE       (1u << TiltTime_CTRL_TRIG_MODE_SHIFT)
#if (TiltTime_UsingFixedFunction)
    #define TiltTime_INIT_INTERRUPT_MODE ((0u << TiltTime_STATUS_TC_INT_MASK_SHIFT) | \
                                                  (1 << TiltTime_STATUS_CAPTURE_INT_MASK_SHIFT))
#else
    #define TiltTime_INIT_INTERRUPT_MODE ((0u << TiltTime_STATUS_TC_INT_MASK_SHIFT) | \
                                                 (1 << TiltTime_STATUS_CAPTURE_INT_MASK_SHIFT) | \
                                                 (0 << TiltTime_STATUS_FIFOFULL_INT_MASK_SHIFT))
#endif /* (TiltTime_UsingFixedFunction) */
#define TiltTime_INIT_CAPTURE_COUNT      (2u)
#define TiltTime_INIT_INT_CAPTURE_COUNT  ((1u - 1) << TiltTime_CTRL_INTCNT_SHIFT)


/***************************************
*           Registers
***************************************/

#if (TiltTime_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define TiltTime_STATUS         (*(reg8 *) TiltTime_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define TiltTime_STATUS_MASK    (*(reg8 *) TiltTime_TimerHW__SR0 )
    #define TiltTime_CONTROL        (*(reg8 *) TiltTime_TimerHW__CFG0)
    #define TiltTime_CONTROL2       (*(reg8 *) TiltTime_TimerHW__CFG1)
    #define TiltTime_CONTROL2_PTR   ( (reg8 *) TiltTime_TimerHW__CFG1)
    #define TiltTime_RT1            (*(reg8 *) TiltTime_TimerHW__RT1)
    #define TiltTime_RT1_PTR        ( (reg8 *) TiltTime_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define TiltTime_CONTROL3       (*(reg8 *) TiltTime_TimerHW__CFG2)
        #define TiltTime_CONTROL3_PTR   ( (reg8 *) TiltTime_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define TiltTime_GLOBAL_ENABLE  (*(reg8 *) TiltTime_TimerHW__PM_ACT_CFG)
    #define TiltTime_GLOBAL_STBY_ENABLE  (*(reg8 *) TiltTime_TimerHW__PM_STBY_CFG)

    #define TiltTime_CAPTURE_LSB         (* (reg16 *) TiltTime_TimerHW__CAP0 )
    #define TiltTime_CAPTURE_LSB_PTR       ((reg16 *) TiltTime_TimerHW__CAP0 )
    #define TiltTime_PERIOD_LSB          (* (reg16 *) TiltTime_TimerHW__PER0 )
    #define TiltTime_PERIOD_LSB_PTR        ((reg16 *) TiltTime_TimerHW__PER0 )
    #define TiltTime_COUNTER_LSB         (* (reg16 *) TiltTime_TimerHW__CNT_CMP0 )
    #define TiltTime_COUNTER_LSB_PTR       ((reg16 *) TiltTime_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define TiltTime_BLOCK_EN_MASK                     TiltTime_TimerHW__PM_ACT_MSK
    #define TiltTime_BLOCK_STBY_EN_MASK                TiltTime_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define TiltTime_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define TiltTime_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define TiltTime_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define TiltTime_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define TiltTime_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define TiltTime_CTRL_ENABLE                        (0x01u << TiltTime_CTRL_ENABLE_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define TiltTime_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define TiltTime_CTRL2_IRQ_SEL                      (0x01u << TiltTime_CTRL2_IRQ_SEL_SHIFT)

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define TiltTime_CTRL_MODE_SHIFT                     0x01u
        #define TiltTime_CTRL_MODE_MASK                     (0x07u << TiltTime_CTRL_MODE_SHIFT)
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define TiltTime_CTRL_RCOD_SHIFT        0x02u
        #define TiltTime_CTRL_ENBL_SHIFT        0x00u
        #define TiltTime_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define TiltTime_CTRL_RCOD_MASK  (0x03u << TiltTime_CTRL_RCOD_SHIFT) /* ROD and COD bit masks */
        #define TiltTime_CTRL_ENBL_MASK  (0x80u << TiltTime_CTRL_ENBL_SHIFT) /* HW_EN bit mask */
        #define TiltTime_CTRL_MODE_MASK  (0x03u << TiltTime_CTRL_MODE_SHIFT) /* Run mode bit mask */

        #define TiltTime_CTRL_RCOD       (0x03u << TiltTime_CTRL_RCOD_SHIFT)
        #define TiltTime_CTRL_ENBL       (0x80u << TiltTime_CTRL_ENBL_SHIFT)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define TiltTime_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define TiltTime_RT1_MASK                        (0x03u << TiltTime_RT1_SHIFT)
    #define TiltTime_SYNC                            (0x03u << TiltTime_RT1_SHIFT)
    #define TiltTime_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define TiltTime_SYNCDSI_MASK                    (0x0Fu << TiltTime_SYNCDSI_SHIFT)
    /* Sync all DSI inputs */
    #define TiltTime_SYNCDSI_EN                      (0x0Fu << TiltTime_SYNCDSI_SHIFT)

    #define TiltTime_CTRL_MODE_PULSEWIDTH            (0x01u << TiltTime_CTRL_MODE_SHIFT)
    #define TiltTime_CTRL_MODE_PERIOD                (0x02u << TiltTime_CTRL_MODE_SHIFT)
    #define TiltTime_CTRL_MODE_CONTINUOUS            (0x00u << TiltTime_CTRL_MODE_SHIFT)

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define TiltTime_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define TiltTime_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define TiltTime_STATUS_TC_INT_MASK_SHIFT        (TiltTime_STATUS_TC_SHIFT - 4)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define TiltTime_STATUS_CAPTURE_INT_MASK_SHIFT   (TiltTime_STATUS_CAPTURE_SHIFT - 4)

    /* Status Register Bit Masks */
    #define TiltTime_STATUS_TC                       (0x01u << TiltTime_STATUS_TC_SHIFT)
    #define TiltTime_STATUS_CAPTURE                  (0x01u << TiltTime_STATUS_CAPTURE_SHIFT)
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define TiltTime_STATUS_TC_INT_MASK              (0x01u << TiltTime_STATUS_TC_INT_MASK_SHIFT)
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define TiltTime_STATUS_CAPTURE_INT_MASK         (0x01u << TiltTime_STATUS_CAPTURE_INT_MASK_SHIFT)

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define TiltTime_STATUS              (* (reg8 *) TiltTime_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define TiltTime_STATUS_MASK         (* (reg8 *) TiltTime_TimerUDB_rstSts_stsreg__MASK_REG)
    #define TiltTime_STATUS_AUX_CTRL     (* (reg8 *) TiltTime_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define TiltTime_CONTROL             (* (reg8 *) TiltTime_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )

    #define TiltTime_CAPTURE_LSB         (* (reg16 *) TiltTime_TimerUDB_sT16_timerdp_u0__F0_REG )
    #define TiltTime_CAPTURE_LSB_PTR       ((reg16 *) TiltTime_TimerUDB_sT16_timerdp_u0__F0_REG )
    #define TiltTime_PERIOD_LSB          (* (reg16 *) TiltTime_TimerUDB_sT16_timerdp_u0__D0_REG )
    #define TiltTime_PERIOD_LSB_PTR        ((reg16 *) TiltTime_TimerUDB_sT16_timerdp_u0__D0_REG )
    #define TiltTime_COUNTER_LSB         (* (reg16 *) TiltTime_TimerUDB_sT16_timerdp_u0__A0_REG )
    #define TiltTime_COUNTER_LSB_PTR       ((reg16 *) TiltTime_TimerUDB_sT16_timerdp_u0__A0_REG )

    #if (TiltTime_UsingHWCaptureCounter)
        #define TiltTime_CAP_COUNT              (*(reg8 *) TiltTime_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define TiltTime_CAP_COUNT_PTR          ( (reg8 *) TiltTime_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define TiltTime_CAPTURE_COUNT_CTRL     (*(reg8 *) TiltTime_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define TiltTime_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) TiltTime_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (TiltTime_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define TiltTime_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define TiltTime_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define TiltTime_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define TiltTime_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define TiltTime_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define TiltTime_CTRL_INTCNT_MASK               (0x03u << TiltTime_CTRL_INTCNT_SHIFT)
    #define TiltTime_CTRL_TRIG_MODE_MASK            (0x03u << TiltTime_CTRL_TRIG_MODE_SHIFT)
    #define TiltTime_CTRL_TRIG_EN                   (0x01u << TiltTime_CTRL_TRIG_EN_SHIFT)
    #define TiltTime_CTRL_CAP_MODE_MASK             (0x03u << TiltTime_CTRL_CAP_MODE_SHIFT)
    #define TiltTime_CTRL_ENABLE                    (0x01u << TiltTime_CTRL_ENABLE_SHIFT)

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define TiltTime_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define TiltTime_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define TiltTime_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define TiltTime_STATUS_TC_INT_MASK_SHIFT       TiltTime_STATUS_TC_SHIFT
    #define TiltTime_STATUS_CAPTURE_INT_MASK_SHIFT  TiltTime_STATUS_CAPTURE_SHIFT
    #define TiltTime_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define TiltTime_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define TiltTime_STATUS_FIFOFULL_INT_MASK_SHIFT TiltTime_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define TiltTime_STATUS_TC                      (0x01u << TiltTime_STATUS_TC_SHIFT)
    /* Sticky Capture Event Bit-Mask */
    #define TiltTime_STATUS_CAPTURE                 (0x01u << TiltTime_STATUS_CAPTURE_SHIFT)
    /* Interrupt Enable Bit-Mask */
    #define TiltTime_STATUS_TC_INT_MASK             (0x01u << TiltTime_STATUS_TC_SHIFT)
    /* Interrupt Enable Bit-Mask */
    #define TiltTime_STATUS_CAPTURE_INT_MASK        (0x01u << TiltTime_STATUS_CAPTURE_SHIFT)
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define TiltTime_STATUS_FIFOFULL                (0x01u << TiltTime_STATUS_FIFOFULL_SHIFT)
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define TiltTime_STATUS_FIFONEMP                (0x01u << TiltTime_STATUS_FIFONEMP_SHIFT)
    /* Interrupt Enable Bit-Mask */
    #define TiltTime_STATUS_FIFOFULL_INT_MASK       (0x01u << TiltTime_STATUS_FIFOFULL_SHIFT)

    #define TiltTime_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define TiltTime_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define TiltTime_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define TiltTime_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define TiltTime_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define TiltTime_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_Timer_v2_30_TiltTime_H */


/* [] END OF FILE */
