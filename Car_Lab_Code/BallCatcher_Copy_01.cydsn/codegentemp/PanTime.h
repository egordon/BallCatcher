/*******************************************************************************
* File Name: PanTime.h
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

#if !defined(CY_Timer_v2_30_PanTime_H)
#define CY_Timer_v2_30_PanTime_H

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

#define PanTime_Resolution                 16u
#define PanTime_UsingFixedFunction         0u
#define PanTime_UsingHWCaptureCounter      0u
#define PanTime_SoftwareCaptureMode        0u
#define PanTime_SoftwareTriggerMode        0u
#define PanTime_UsingHWEnable              0u
#define PanTime_EnableTriggerMode          1u
#define PanTime_InterruptOnCaptureCount    1u
#define PanTime_RunModeUsed                2u
#define PanTime_ControlRegRemoved          0u


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct PanTime_backupStruct
{
    uint8 TimerEnableState;
    #if(!PanTime_UsingFixedFunction)
        #if (CY_PSOC5A)
            uint16 TimerUdb;                 /* Timer internal counter value */
            uint16 TimerPeriod;              /* Timer Period value       */
            uint8 InterruptMaskValue;       /* Timer Compare Value */
            #if (PanTime_UsingHWCaptureCounter)
                uint16 TimerCaptureCounter;  /* Timer Capture Counter Value */
            #endif /* variable declaration for backing up Capture Counter value*/
        #endif /* variables for non retention registers in PSoC5A */

        #if (CY_PSOC3 || CY_PSOC5LP)
            uint16 TimerUdb;
            uint8 InterruptMaskValue;
            #if (PanTime_UsingHWCaptureCounter)
                uint16 TimerCaptureCounter;
            #endif /* variable declarations for backing up non retention registers in PSoC3 or PSoC5LP */
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */

        #if (!PanTime_ControlRegRemoved)
        uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */
}PanTime_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    PanTime_Start(void) ;
void    PanTime_Stop(void) ;

void    PanTime_SetInterruptMode(uint8 interruptMode) ;
uint8   PanTime_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define PanTime_GetInterruptSource() PanTime_ReadStatusRegister()

#if(!PanTime_ControlRegRemoved)
    uint8   PanTime_ReadControlRegister(void) ;
    void    PanTime_WriteControlRegister(uint8 control) \
        ;
#endif /* (!PanTime_ControlRegRemoved) */

uint16  PanTime_ReadPeriod(void) ;
void    PanTime_WritePeriod(uint16 period) \
    ;
uint16  PanTime_ReadCounter(void) ;
void    PanTime_WriteCounter(uint16 counter) \
    ;
uint16  PanTime_ReadCapture(void) ;
void    PanTime_SoftwareCapture(void) ;


#if(!PanTime_UsingFixedFunction) /* UDB Prototypes */
    #if (PanTime_SoftwareCaptureMode)
        void    PanTime_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!PanTime_UsingFixedFunction) */

    #if (PanTime_SoftwareTriggerMode)
        void    PanTime_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (PanTime_SoftwareTriggerMode) */
    #if (PanTime_EnableTriggerMode)
        void    PanTime_EnableTrigger(void) ;
        void    PanTime_DisableTrigger(void) ;
    #endif /* (PanTime_EnableTriggerMode) */

    #if(PanTime_InterruptOnCaptureCount)
        #if(!PanTime_ControlRegRemoved)
            void    PanTime_SetInterruptCount(uint8 interruptCount) \
                ;
        #endif /* (!PanTime_ControlRegRemoved) */
    #endif /* (PanTime_InterruptOnCaptureCount) */

    #if (PanTime_UsingHWCaptureCounter)
        void    PanTime_SetCaptureCount(uint8 captureCount) \
            ;
        uint8   PanTime_ReadCaptureCount(void) ;
    #endif /* (PanTime_UsingHWCaptureCounter) */

    void PanTime_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void PanTime_Init(void)          ;
void PanTime_Enable(void)        ;
void PanTime_SaveConfig(void)    ;
void PanTime_RestoreConfig(void) ;
void PanTime_Sleep(void)         ;
void PanTime_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define PanTime__B_TIMER__CM_NONE 0
#define PanTime__B_TIMER__CM_RISINGEDGE 1
#define PanTime__B_TIMER__CM_FALLINGEDGE 2
#define PanTime__B_TIMER__CM_EITHEREDGE 3
#define PanTime__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define PanTime__B_TIMER__TM_NONE 0x00u
#define PanTime__B_TIMER__TM_RISINGEDGE 0x04u
#define PanTime__B_TIMER__TM_FALLINGEDGE 0x08u
#define PanTime__B_TIMER__TM_EITHEREDGE 0x0Cu
#define PanTime__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define PanTime_INIT_PERIOD             9999u
#define PanTime_INIT_CAPTURE_MODE       (2u << PanTime_CTRL_CAP_MODE_SHIFT)
#define PanTime_INIT_TRIGGER_MODE       (1u << PanTime_CTRL_TRIG_MODE_SHIFT)
#if (PanTime_UsingFixedFunction)
    #define PanTime_INIT_INTERRUPT_MODE ((0u << PanTime_STATUS_TC_INT_MASK_SHIFT) | \
                                                  (1 << PanTime_STATUS_CAPTURE_INT_MASK_SHIFT))
#else
    #define PanTime_INIT_INTERRUPT_MODE ((0u << PanTime_STATUS_TC_INT_MASK_SHIFT) | \
                                                 (1 << PanTime_STATUS_CAPTURE_INT_MASK_SHIFT) | \
                                                 (0 << PanTime_STATUS_FIFOFULL_INT_MASK_SHIFT))
#endif /* (PanTime_UsingFixedFunction) */
#define PanTime_INIT_CAPTURE_COUNT      (2u)
#define PanTime_INIT_INT_CAPTURE_COUNT  ((1u - 1) << PanTime_CTRL_INTCNT_SHIFT)


/***************************************
*           Registers
***************************************/

#if (PanTime_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define PanTime_STATUS         (*(reg8 *) PanTime_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define PanTime_STATUS_MASK    (*(reg8 *) PanTime_TimerHW__SR0 )
    #define PanTime_CONTROL        (*(reg8 *) PanTime_TimerHW__CFG0)
    #define PanTime_CONTROL2       (*(reg8 *) PanTime_TimerHW__CFG1)
    #define PanTime_CONTROL2_PTR   ( (reg8 *) PanTime_TimerHW__CFG1)
    #define PanTime_RT1            (*(reg8 *) PanTime_TimerHW__RT1)
    #define PanTime_RT1_PTR        ( (reg8 *) PanTime_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define PanTime_CONTROL3       (*(reg8 *) PanTime_TimerHW__CFG2)
        #define PanTime_CONTROL3_PTR   ( (reg8 *) PanTime_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define PanTime_GLOBAL_ENABLE  (*(reg8 *) PanTime_TimerHW__PM_ACT_CFG)
    #define PanTime_GLOBAL_STBY_ENABLE  (*(reg8 *) PanTime_TimerHW__PM_STBY_CFG)

    #define PanTime_CAPTURE_LSB         (* (reg16 *) PanTime_TimerHW__CAP0 )
    #define PanTime_CAPTURE_LSB_PTR       ((reg16 *) PanTime_TimerHW__CAP0 )
    #define PanTime_PERIOD_LSB          (* (reg16 *) PanTime_TimerHW__PER0 )
    #define PanTime_PERIOD_LSB_PTR        ((reg16 *) PanTime_TimerHW__PER0 )
    #define PanTime_COUNTER_LSB         (* (reg16 *) PanTime_TimerHW__CNT_CMP0 )
    #define PanTime_COUNTER_LSB_PTR       ((reg16 *) PanTime_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define PanTime_BLOCK_EN_MASK                     PanTime_TimerHW__PM_ACT_MSK
    #define PanTime_BLOCK_STBY_EN_MASK                PanTime_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define PanTime_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define PanTime_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define PanTime_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define PanTime_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define PanTime_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define PanTime_CTRL_ENABLE                        (0x01u << PanTime_CTRL_ENABLE_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define PanTime_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define PanTime_CTRL2_IRQ_SEL                      (0x01u << PanTime_CTRL2_IRQ_SEL_SHIFT)

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define PanTime_CTRL_MODE_SHIFT                     0x01u
        #define PanTime_CTRL_MODE_MASK                     (0x07u << PanTime_CTRL_MODE_SHIFT)
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define PanTime_CTRL_RCOD_SHIFT        0x02u
        #define PanTime_CTRL_ENBL_SHIFT        0x00u
        #define PanTime_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define PanTime_CTRL_RCOD_MASK  (0x03u << PanTime_CTRL_RCOD_SHIFT) /* ROD and COD bit masks */
        #define PanTime_CTRL_ENBL_MASK  (0x80u << PanTime_CTRL_ENBL_SHIFT) /* HW_EN bit mask */
        #define PanTime_CTRL_MODE_MASK  (0x03u << PanTime_CTRL_MODE_SHIFT) /* Run mode bit mask */

        #define PanTime_CTRL_RCOD       (0x03u << PanTime_CTRL_RCOD_SHIFT)
        #define PanTime_CTRL_ENBL       (0x80u << PanTime_CTRL_ENBL_SHIFT)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define PanTime_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define PanTime_RT1_MASK                        (0x03u << PanTime_RT1_SHIFT)
    #define PanTime_SYNC                            (0x03u << PanTime_RT1_SHIFT)
    #define PanTime_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define PanTime_SYNCDSI_MASK                    (0x0Fu << PanTime_SYNCDSI_SHIFT)
    /* Sync all DSI inputs */
    #define PanTime_SYNCDSI_EN                      (0x0Fu << PanTime_SYNCDSI_SHIFT)

    #define PanTime_CTRL_MODE_PULSEWIDTH            (0x01u << PanTime_CTRL_MODE_SHIFT)
    #define PanTime_CTRL_MODE_PERIOD                (0x02u << PanTime_CTRL_MODE_SHIFT)
    #define PanTime_CTRL_MODE_CONTINUOUS            (0x00u << PanTime_CTRL_MODE_SHIFT)

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define PanTime_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define PanTime_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define PanTime_STATUS_TC_INT_MASK_SHIFT        (PanTime_STATUS_TC_SHIFT - 4)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define PanTime_STATUS_CAPTURE_INT_MASK_SHIFT   (PanTime_STATUS_CAPTURE_SHIFT - 4)

    /* Status Register Bit Masks */
    #define PanTime_STATUS_TC                       (0x01u << PanTime_STATUS_TC_SHIFT)
    #define PanTime_STATUS_CAPTURE                  (0x01u << PanTime_STATUS_CAPTURE_SHIFT)
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define PanTime_STATUS_TC_INT_MASK              (0x01u << PanTime_STATUS_TC_INT_MASK_SHIFT)
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define PanTime_STATUS_CAPTURE_INT_MASK         (0x01u << PanTime_STATUS_CAPTURE_INT_MASK_SHIFT)

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define PanTime_STATUS              (* (reg8 *) PanTime_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define PanTime_STATUS_MASK         (* (reg8 *) PanTime_TimerUDB_rstSts_stsreg__MASK_REG)
    #define PanTime_STATUS_AUX_CTRL     (* (reg8 *) PanTime_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define PanTime_CONTROL             (* (reg8 *) PanTime_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )

    #define PanTime_CAPTURE_LSB         (* (reg16 *) PanTime_TimerUDB_sT16_timerdp_u0__F0_REG )
    #define PanTime_CAPTURE_LSB_PTR       ((reg16 *) PanTime_TimerUDB_sT16_timerdp_u0__F0_REG )
    #define PanTime_PERIOD_LSB          (* (reg16 *) PanTime_TimerUDB_sT16_timerdp_u0__D0_REG )
    #define PanTime_PERIOD_LSB_PTR        ((reg16 *) PanTime_TimerUDB_sT16_timerdp_u0__D0_REG )
    #define PanTime_COUNTER_LSB         (* (reg16 *) PanTime_TimerUDB_sT16_timerdp_u0__A0_REG )
    #define PanTime_COUNTER_LSB_PTR       ((reg16 *) PanTime_TimerUDB_sT16_timerdp_u0__A0_REG )

    #if (PanTime_UsingHWCaptureCounter)
        #define PanTime_CAP_COUNT              (*(reg8 *) PanTime_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define PanTime_CAP_COUNT_PTR          ( (reg8 *) PanTime_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define PanTime_CAPTURE_COUNT_CTRL     (*(reg8 *) PanTime_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define PanTime_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) PanTime_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (PanTime_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define PanTime_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define PanTime_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define PanTime_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define PanTime_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define PanTime_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define PanTime_CTRL_INTCNT_MASK               (0x03u << PanTime_CTRL_INTCNT_SHIFT)
    #define PanTime_CTRL_TRIG_MODE_MASK            (0x03u << PanTime_CTRL_TRIG_MODE_SHIFT)
    #define PanTime_CTRL_TRIG_EN                   (0x01u << PanTime_CTRL_TRIG_EN_SHIFT)
    #define PanTime_CTRL_CAP_MODE_MASK             (0x03u << PanTime_CTRL_CAP_MODE_SHIFT)
    #define PanTime_CTRL_ENABLE                    (0x01u << PanTime_CTRL_ENABLE_SHIFT)

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define PanTime_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define PanTime_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define PanTime_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define PanTime_STATUS_TC_INT_MASK_SHIFT       PanTime_STATUS_TC_SHIFT
    #define PanTime_STATUS_CAPTURE_INT_MASK_SHIFT  PanTime_STATUS_CAPTURE_SHIFT
    #define PanTime_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define PanTime_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define PanTime_STATUS_FIFOFULL_INT_MASK_SHIFT PanTime_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define PanTime_STATUS_TC                      (0x01u << PanTime_STATUS_TC_SHIFT)
    /* Sticky Capture Event Bit-Mask */
    #define PanTime_STATUS_CAPTURE                 (0x01u << PanTime_STATUS_CAPTURE_SHIFT)
    /* Interrupt Enable Bit-Mask */
    #define PanTime_STATUS_TC_INT_MASK             (0x01u << PanTime_STATUS_TC_SHIFT)
    /* Interrupt Enable Bit-Mask */
    #define PanTime_STATUS_CAPTURE_INT_MASK        (0x01u << PanTime_STATUS_CAPTURE_SHIFT)
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define PanTime_STATUS_FIFOFULL                (0x01u << PanTime_STATUS_FIFOFULL_SHIFT)
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define PanTime_STATUS_FIFONEMP                (0x01u << PanTime_STATUS_FIFONEMP_SHIFT)
    /* Interrupt Enable Bit-Mask */
    #define PanTime_STATUS_FIFOFULL_INT_MASK       (0x01u << PanTime_STATUS_FIFOFULL_SHIFT)

    #define PanTime_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define PanTime_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define PanTime_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define PanTime_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define PanTime_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define PanTime_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_Timer_v2_30_PanTime_H */


/* [] END OF FILE */
