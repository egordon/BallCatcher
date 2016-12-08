/*******************************************************************************
* File Name: RangeTime.h
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

#if !defined(CY_Timer_v2_30_RangeTime_H)
#define CY_Timer_v2_30_RangeTime_H

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

#define RangeTime_Resolution                 16u
#define RangeTime_UsingFixedFunction         0u
#define RangeTime_UsingHWCaptureCounter      0u
#define RangeTime_SoftwareCaptureMode        0u
#define RangeTime_SoftwareTriggerMode        0u
#define RangeTime_UsingHWEnable              0u
#define RangeTime_EnableTriggerMode          1u
#define RangeTime_InterruptOnCaptureCount    1u
#define RangeTime_RunModeUsed                2u
#define RangeTime_ControlRegRemoved          0u


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct RangeTime_backupStruct
{
    uint8 TimerEnableState;
    #if(!RangeTime_UsingFixedFunction)
        #if (CY_PSOC5A)
            uint16 TimerUdb;                 /* Timer internal counter value */
            uint16 TimerPeriod;              /* Timer Period value       */
            uint8 InterruptMaskValue;       /* Timer Compare Value */
            #if (RangeTime_UsingHWCaptureCounter)
                uint16 TimerCaptureCounter;  /* Timer Capture Counter Value */
            #endif /* variable declaration for backing up Capture Counter value*/
        #endif /* variables for non retention registers in PSoC5A */

        #if (CY_PSOC3 || CY_PSOC5LP)
            uint16 TimerUdb;
            uint8 InterruptMaskValue;
            #if (RangeTime_UsingHWCaptureCounter)
                uint16 TimerCaptureCounter;
            #endif /* variable declarations for backing up non retention registers in PSoC3 or PSoC5LP */
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */

        #if (!RangeTime_ControlRegRemoved)
        uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */
}RangeTime_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    RangeTime_Start(void) ;
void    RangeTime_Stop(void) ;

void    RangeTime_SetInterruptMode(uint8 interruptMode) ;
uint8   RangeTime_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define RangeTime_GetInterruptSource() RangeTime_ReadStatusRegister()

#if(!RangeTime_ControlRegRemoved)
    uint8   RangeTime_ReadControlRegister(void) ;
    void    RangeTime_WriteControlRegister(uint8 control) \
        ;
#endif /* (!RangeTime_ControlRegRemoved) */

uint16  RangeTime_ReadPeriod(void) ;
void    RangeTime_WritePeriod(uint16 period) \
    ;
uint16  RangeTime_ReadCounter(void) ;
void    RangeTime_WriteCounter(uint16 counter) \
    ;
uint16  RangeTime_ReadCapture(void) ;
void    RangeTime_SoftwareCapture(void) ;


#if(!RangeTime_UsingFixedFunction) /* UDB Prototypes */
    #if (RangeTime_SoftwareCaptureMode)
        void    RangeTime_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!RangeTime_UsingFixedFunction) */

    #if (RangeTime_SoftwareTriggerMode)
        void    RangeTime_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (RangeTime_SoftwareTriggerMode) */
    #if (RangeTime_EnableTriggerMode)
        void    RangeTime_EnableTrigger(void) ;
        void    RangeTime_DisableTrigger(void) ;
    #endif /* (RangeTime_EnableTriggerMode) */

    #if(RangeTime_InterruptOnCaptureCount)
        #if(!RangeTime_ControlRegRemoved)
            void    RangeTime_SetInterruptCount(uint8 interruptCount) \
                ;
        #endif /* (!RangeTime_ControlRegRemoved) */
    #endif /* (RangeTime_InterruptOnCaptureCount) */

    #if (RangeTime_UsingHWCaptureCounter)
        void    RangeTime_SetCaptureCount(uint8 captureCount) \
            ;
        uint8   RangeTime_ReadCaptureCount(void) ;
    #endif /* (RangeTime_UsingHWCaptureCounter) */

    void RangeTime_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void RangeTime_Init(void)          ;
void RangeTime_Enable(void)        ;
void RangeTime_SaveConfig(void)    ;
void RangeTime_RestoreConfig(void) ;
void RangeTime_Sleep(void)         ;
void RangeTime_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define RangeTime__B_TIMER__CM_NONE 0
#define RangeTime__B_TIMER__CM_RISINGEDGE 1
#define RangeTime__B_TIMER__CM_FALLINGEDGE 2
#define RangeTime__B_TIMER__CM_EITHEREDGE 3
#define RangeTime__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define RangeTime__B_TIMER__TM_NONE 0x00u
#define RangeTime__B_TIMER__TM_RISINGEDGE 0x04u
#define RangeTime__B_TIMER__TM_FALLINGEDGE 0x08u
#define RangeTime__B_TIMER__TM_EITHEREDGE 0x0Cu
#define RangeTime__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define RangeTime_INIT_PERIOD             29999u
#define RangeTime_INIT_CAPTURE_MODE       (2u << RangeTime_CTRL_CAP_MODE_SHIFT)
#define RangeTime_INIT_TRIGGER_MODE       (1u << RangeTime_CTRL_TRIG_MODE_SHIFT)
#if (RangeTime_UsingFixedFunction)
    #define RangeTime_INIT_INTERRUPT_MODE ((0u << RangeTime_STATUS_TC_INT_MASK_SHIFT) | \
                                                  (1 << RangeTime_STATUS_CAPTURE_INT_MASK_SHIFT))
#else
    #define RangeTime_INIT_INTERRUPT_MODE ((0u << RangeTime_STATUS_TC_INT_MASK_SHIFT) | \
                                                 (1 << RangeTime_STATUS_CAPTURE_INT_MASK_SHIFT) | \
                                                 (0 << RangeTime_STATUS_FIFOFULL_INT_MASK_SHIFT))
#endif /* (RangeTime_UsingFixedFunction) */
#define RangeTime_INIT_CAPTURE_COUNT      (2u)
#define RangeTime_INIT_INT_CAPTURE_COUNT  ((1u - 1) << RangeTime_CTRL_INTCNT_SHIFT)


/***************************************
*           Registers
***************************************/

#if (RangeTime_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define RangeTime_STATUS         (*(reg8 *) RangeTime_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define RangeTime_STATUS_MASK    (*(reg8 *) RangeTime_TimerHW__SR0 )
    #define RangeTime_CONTROL        (*(reg8 *) RangeTime_TimerHW__CFG0)
    #define RangeTime_CONTROL2       (*(reg8 *) RangeTime_TimerHW__CFG1)
    #define RangeTime_CONTROL2_PTR   ( (reg8 *) RangeTime_TimerHW__CFG1)
    #define RangeTime_RT1            (*(reg8 *) RangeTime_TimerHW__RT1)
    #define RangeTime_RT1_PTR        ( (reg8 *) RangeTime_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define RangeTime_CONTROL3       (*(reg8 *) RangeTime_TimerHW__CFG2)
        #define RangeTime_CONTROL3_PTR   ( (reg8 *) RangeTime_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define RangeTime_GLOBAL_ENABLE  (*(reg8 *) RangeTime_TimerHW__PM_ACT_CFG)
    #define RangeTime_GLOBAL_STBY_ENABLE  (*(reg8 *) RangeTime_TimerHW__PM_STBY_CFG)

    #define RangeTime_CAPTURE_LSB         (* (reg16 *) RangeTime_TimerHW__CAP0 )
    #define RangeTime_CAPTURE_LSB_PTR       ((reg16 *) RangeTime_TimerHW__CAP0 )
    #define RangeTime_PERIOD_LSB          (* (reg16 *) RangeTime_TimerHW__PER0 )
    #define RangeTime_PERIOD_LSB_PTR        ((reg16 *) RangeTime_TimerHW__PER0 )
    #define RangeTime_COUNTER_LSB         (* (reg16 *) RangeTime_TimerHW__CNT_CMP0 )
    #define RangeTime_COUNTER_LSB_PTR       ((reg16 *) RangeTime_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define RangeTime_BLOCK_EN_MASK                     RangeTime_TimerHW__PM_ACT_MSK
    #define RangeTime_BLOCK_STBY_EN_MASK                RangeTime_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define RangeTime_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define RangeTime_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define RangeTime_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define RangeTime_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define RangeTime_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define RangeTime_CTRL_ENABLE                        (0x01u << RangeTime_CTRL_ENABLE_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define RangeTime_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define RangeTime_CTRL2_IRQ_SEL                      (0x01u << RangeTime_CTRL2_IRQ_SEL_SHIFT)

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define RangeTime_CTRL_MODE_SHIFT                     0x01u
        #define RangeTime_CTRL_MODE_MASK                     (0x07u << RangeTime_CTRL_MODE_SHIFT)
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define RangeTime_CTRL_RCOD_SHIFT        0x02u
        #define RangeTime_CTRL_ENBL_SHIFT        0x00u
        #define RangeTime_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define RangeTime_CTRL_RCOD_MASK  (0x03u << RangeTime_CTRL_RCOD_SHIFT) /* ROD and COD bit masks */
        #define RangeTime_CTRL_ENBL_MASK  (0x80u << RangeTime_CTRL_ENBL_SHIFT) /* HW_EN bit mask */
        #define RangeTime_CTRL_MODE_MASK  (0x03u << RangeTime_CTRL_MODE_SHIFT) /* Run mode bit mask */

        #define RangeTime_CTRL_RCOD       (0x03u << RangeTime_CTRL_RCOD_SHIFT)
        #define RangeTime_CTRL_ENBL       (0x80u << RangeTime_CTRL_ENBL_SHIFT)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define RangeTime_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define RangeTime_RT1_MASK                        (0x03u << RangeTime_RT1_SHIFT)
    #define RangeTime_SYNC                            (0x03u << RangeTime_RT1_SHIFT)
    #define RangeTime_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define RangeTime_SYNCDSI_MASK                    (0x0Fu << RangeTime_SYNCDSI_SHIFT)
    /* Sync all DSI inputs */
    #define RangeTime_SYNCDSI_EN                      (0x0Fu << RangeTime_SYNCDSI_SHIFT)

    #define RangeTime_CTRL_MODE_PULSEWIDTH            (0x01u << RangeTime_CTRL_MODE_SHIFT)
    #define RangeTime_CTRL_MODE_PERIOD                (0x02u << RangeTime_CTRL_MODE_SHIFT)
    #define RangeTime_CTRL_MODE_CONTINUOUS            (0x00u << RangeTime_CTRL_MODE_SHIFT)

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define RangeTime_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define RangeTime_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define RangeTime_STATUS_TC_INT_MASK_SHIFT        (RangeTime_STATUS_TC_SHIFT - 4)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define RangeTime_STATUS_CAPTURE_INT_MASK_SHIFT   (RangeTime_STATUS_CAPTURE_SHIFT - 4)

    /* Status Register Bit Masks */
    #define RangeTime_STATUS_TC                       (0x01u << RangeTime_STATUS_TC_SHIFT)
    #define RangeTime_STATUS_CAPTURE                  (0x01u << RangeTime_STATUS_CAPTURE_SHIFT)
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define RangeTime_STATUS_TC_INT_MASK              (0x01u << RangeTime_STATUS_TC_INT_MASK_SHIFT)
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define RangeTime_STATUS_CAPTURE_INT_MASK         (0x01u << RangeTime_STATUS_CAPTURE_INT_MASK_SHIFT)

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define RangeTime_STATUS              (* (reg8 *) RangeTime_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define RangeTime_STATUS_MASK         (* (reg8 *) RangeTime_TimerUDB_rstSts_stsreg__MASK_REG)
    #define RangeTime_STATUS_AUX_CTRL     (* (reg8 *) RangeTime_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define RangeTime_CONTROL             (* (reg8 *) RangeTime_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )

    #define RangeTime_CAPTURE_LSB         (* (reg16 *) RangeTime_TimerUDB_sT16_timerdp_u0__F0_REG )
    #define RangeTime_CAPTURE_LSB_PTR       ((reg16 *) RangeTime_TimerUDB_sT16_timerdp_u0__F0_REG )
    #define RangeTime_PERIOD_LSB          (* (reg16 *) RangeTime_TimerUDB_sT16_timerdp_u0__D0_REG )
    #define RangeTime_PERIOD_LSB_PTR        ((reg16 *) RangeTime_TimerUDB_sT16_timerdp_u0__D0_REG )
    #define RangeTime_COUNTER_LSB         (* (reg16 *) RangeTime_TimerUDB_sT16_timerdp_u0__A0_REG )
    #define RangeTime_COUNTER_LSB_PTR       ((reg16 *) RangeTime_TimerUDB_sT16_timerdp_u0__A0_REG )

    #if (RangeTime_UsingHWCaptureCounter)
        #define RangeTime_CAP_COUNT              (*(reg8 *) RangeTime_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define RangeTime_CAP_COUNT_PTR          ( (reg8 *) RangeTime_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define RangeTime_CAPTURE_COUNT_CTRL     (*(reg8 *) RangeTime_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define RangeTime_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) RangeTime_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (RangeTime_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define RangeTime_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define RangeTime_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define RangeTime_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define RangeTime_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define RangeTime_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define RangeTime_CTRL_INTCNT_MASK               (0x03u << RangeTime_CTRL_INTCNT_SHIFT)
    #define RangeTime_CTRL_TRIG_MODE_MASK            (0x03u << RangeTime_CTRL_TRIG_MODE_SHIFT)
    #define RangeTime_CTRL_TRIG_EN                   (0x01u << RangeTime_CTRL_TRIG_EN_SHIFT)
    #define RangeTime_CTRL_CAP_MODE_MASK             (0x03u << RangeTime_CTRL_CAP_MODE_SHIFT)
    #define RangeTime_CTRL_ENABLE                    (0x01u << RangeTime_CTRL_ENABLE_SHIFT)

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define RangeTime_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define RangeTime_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define RangeTime_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define RangeTime_STATUS_TC_INT_MASK_SHIFT       RangeTime_STATUS_TC_SHIFT
    #define RangeTime_STATUS_CAPTURE_INT_MASK_SHIFT  RangeTime_STATUS_CAPTURE_SHIFT
    #define RangeTime_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define RangeTime_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define RangeTime_STATUS_FIFOFULL_INT_MASK_SHIFT RangeTime_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define RangeTime_STATUS_TC                      (0x01u << RangeTime_STATUS_TC_SHIFT)
    /* Sticky Capture Event Bit-Mask */
    #define RangeTime_STATUS_CAPTURE                 (0x01u << RangeTime_STATUS_CAPTURE_SHIFT)
    /* Interrupt Enable Bit-Mask */
    #define RangeTime_STATUS_TC_INT_MASK             (0x01u << RangeTime_STATUS_TC_SHIFT)
    /* Interrupt Enable Bit-Mask */
    #define RangeTime_STATUS_CAPTURE_INT_MASK        (0x01u << RangeTime_STATUS_CAPTURE_SHIFT)
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define RangeTime_STATUS_FIFOFULL                (0x01u << RangeTime_STATUS_FIFOFULL_SHIFT)
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define RangeTime_STATUS_FIFONEMP                (0x01u << RangeTime_STATUS_FIFONEMP_SHIFT)
    /* Interrupt Enable Bit-Mask */
    #define RangeTime_STATUS_FIFOFULL_INT_MASK       (0x01u << RangeTime_STATUS_FIFOFULL_SHIFT)

    #define RangeTime_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define RangeTime_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define RangeTime_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define RangeTime_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define RangeTime_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define RangeTime_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_Timer_v2_30_RangeTime_H */


/* [] END OF FILE */
