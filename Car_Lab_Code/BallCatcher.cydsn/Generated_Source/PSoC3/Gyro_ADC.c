/*******************************************************************************
* File Name: Gyro_ADC.c  
* Version 2.30
*
* Description:
*  This file provides the source code to the API for the Delta-Sigma ADC
*  Component.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "Gyro_ADC.h"

#if(Gyro_ADC_DEFAULT_INTERNAL_CLK)
    #include "Gyro_ADC_theACLK.h"
#endif /* Gyro_ADC_DEFAULT_INTERNAL_CLK */

#include "Gyro_ADC_Ext_CP_Clk.h"

#if(Gyro_ADC_DEFAULT_INPUT_MODE)
    #include "Gyro_ADC_AMux.h"
#endif /* Gyro_ADC_DEFAULT_INPUT_MODE */

/* Software flag for checking conversion completed or not */
volatile uint8 Gyro_ADC_convDone = 0u;

/* Software flag to stop conversion for single sample conversion mode 
   with resolution above 16 bits */
volatile uint8 Gyro_ADC_stopConversion = 0u;

/* To run the initialization block only at the start up */
uint8 Gyro_ADC_initVar = 0u;

/* To check whether ADC started or not before switching the configuration */
volatile uint8 Gyro_ADC_started = 0u;

/* Flag to hold ADC config number. By default active config is 1. */
volatile uint8 Gyro_ADC_Config = 1u;

volatile int32 Gyro_ADC_Offset = 0u;
volatile int32 Gyro_ADC_CountsPerVolt = (uint32)Gyro_ADC_CFG1_COUNTS_PER_VOLT;

/* Only valid for PSoC5A */
/* Variable to decide whether or not to restore the register values from
    backup structure */
#if (CY_PSOC5A)
    uint8 Gyro_ADC_restoreReg = 0u;
#endif /* CY_PSOC5A */

/* Valid only for PSoC5A silicon */
#if (CY_PSOC5A)
    /* Backup struct for power mode registers */
    static Gyro_ADC_POWERMODE_BACKUP_STRUCT Gyro_ADC_powerModeBackup = 
    {
        /* Initializing the structure fields with power mode registers of 
           config1 */
        Gyro_ADC_CFG1_DSM_CR14,
        Gyro_ADC_CFG1_DSM_CR15,
        Gyro_ADC_CFG1_DSM_CR16,
        Gyro_ADC_CFG1_DSM_CR17,
        Gyro_ADC_CFG1_DSM_REF0,
        Gyro_ADC_CFG1_DSM_BUF0,
        Gyro_ADC_CFG1_DSM_BUF1,
        Gyro_ADC_CFG1_DSM_CLK,
        Gyro_ADC_BYPASS_DISABLED
    };
#endif /* CY_PSOC5A */


/****************************************************************************** 
* Function Name: Gyro_ADC_Init
*******************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
* Gyro_ADC_Start().
*  
*
* Parameters:  
*  void
*
* Return: 
*  void 
*
*******************************************************************************/
void Gyro_ADC_Init(void) 
{
    /* Initialize the registers with default customizer settings for config1 */
    Gyro_ADC_InitConfig(1);

    /* This is only valid if there is an internal clock */
    #if(Gyro_ADC_DEFAULT_INTERNAL_CLK)
        Gyro_ADC_theACLK_SetMode(CYCLK_DUTY);
    #endif /* Gyro_ADC_DEFAULT_INTERNAL_CLK */
    
    /* Set the duty cycle for charge pump clock */
    Gyro_ADC_Ext_CP_Clk_SetMode(CYCLK_DUTY);

    /* To perform ADC calibration */
    Gyro_ADC_GainCompensation(Gyro_ADC_CFG1_RANGE, 
                                      Gyro_ADC_CFG1_IDEAL_DEC_GAIN, 
                                      Gyro_ADC_CFG1_IDEAL_ODDDEC_GAIN, 
                                      Gyro_ADC_CFG1_RESOLUTION);        
}


/******************************************************************************
* Function Name: Gyro_ADC_Enable
*******************************************************************************
*
* Summary: 
*  Enables the ADC DelSig block operation.
*  
*
* Parameters:  
*  void
*
* Return: 
*  void 
*
*******************************************************************************/
void Gyro_ADC_Enable(void) 
{
    /* Restore the power registers if silicon used is PSoC5A */
    #if (CY_PSOC5A)
        if(Gyro_ADC_restoreReg && 
           (!(Gyro_ADC_powerModeBackup.bypassRestore)))
        {
            Gyro_ADC_RestorePowerRegisters();
            Gyro_ADC_restoreReg = 0u;
        }
        Gyro_ADC_powerModeBackup.bypassRestore = Gyro_ADC_BYPASS_DISABLED;
    #endif /* CY_PSOC5A */

    /* Enable active mode power for ADC */
    Gyro_ADC_PWRMGR_DEC_REG |= Gyro_ADC_ACT_PWR_DEC_EN;
    Gyro_ADC_PWRMGR_DSM_REG |= Gyro_ADC_ACT_PWR_DSM_EN;
    
     /* Enable alternative active mode power for ADC */
    Gyro_ADC_STBY_PWRMGR_DEC_REG |= Gyro_ADC_STBY_PWR_DEC_EN;
    #if (CY_PSOC3 || CY_PSOC5LP)
    Gyro_ADC_STBY_PWRMGR_DSM_REG |= Gyro_ADC_STBY_PWR_DSM_EN;
    #endif /* CY_PSOC3 || CY_PSOC5LP */

    /* Config selected is 1, then before enablign the REFBUF0, REFBUF1 and 
	   VCMBUF's enable and press circuit and then after a delay of 3
	   microseconds, disable the press circuit. */
    if (Gyro_ADC_Config == 1u)
	{
	    /* Disable PRES, Enable power to VCMBUF0, REFBUF0 and REFBUF1, enable 
	       PRES */
	    #if (CY_PSOC3 || CY_PSOC5LP)
	        Gyro_ADC_RESET_CR4_REG |= Gyro_ADC_IGNORE_PRESA1;
	        Gyro_ADC_RESET_CR5_REG |= Gyro_ADC_IGNORE_PRESA2;
	    #elif (CY_PSOC5A)
	        Gyro_ADC_RESET_CR1_REG |= Gyro_ADC_DIS_PRES1;
	        Gyro_ADC_RESET_CR3_REG |= Gyro_ADC_DIS_PRES2;
	    #endif /* CY_PSOC5A */
	    
	    Gyro_ADC_DSM_CR17_REG |= (Gyro_ADC_DSM_EN_BUF_VREF | Gyro_ADC_DSM_EN_BUF_VCM);
	    #if ((Gyro_ADC_CFG1_REFERENCE == Gyro_ADC_EXT_REF_ON_P03) || \
	         (Gyro_ADC_CFG1_REFERENCE == Gyro_ADC_EXT_REF_ON_P32))
	        Gyro_ADC_DSM_CR17_REG &= ~Gyro_ADC_DSM_EN_BUF_VREF;
	    #endif /* Check for exteranl reference option */
	    #if ((Gyro_ADC_CFG1_RANGE == Gyro_ADC_IR_VSSA_TO_2VREF) && \
	          (CY_PSOC3 || CY_PSOC5LP) && \
	          ((Gyro_ADC_CFG1_REFERENCE != Gyro_ADC_EXT_REF_ON_P03) && \
	         (Gyro_ADC_CFG1_REFERENCE != Gyro_ADC_EXT_REF_ON_P32)))
	        Gyro_ADC_DSM_REF0_REG |= Gyro_ADC_DSM_EN_BUF_VREF_INN;
	    #endif /* Gyro_ADC_IR_VSSA_TO_2VREF */
	    
	        /* Wait for 3 microseconds */
	    CyDelayUs(3);
        /* Enable the press circuit */
	    #if (CY_PSOC3 || CY_PSOC5LP)
	        Gyro_ADC_RESET_CR4_REG &= ~Gyro_ADC_IGNORE_PRESA1;
	        Gyro_ADC_RESET_CR5_REG &= ~Gyro_ADC_IGNORE_PRESA2;
	    #elif (CY_PSOC5A)
	        Gyro_ADC_RESET_CR1_REG &= ~Gyro_ADC_DIS_PRES1;
	        Gyro_ADC_RESET_CR3_REG &= ~Gyro_ADC_DIS_PRES2;
	    #endif /* CY_PSOC5A */
	}
    
    /* Enable negative pumps for DSM  */
    Gyro_ADC_PUMP_CR1_REG  |= ( Gyro_ADC_PUMP_CR1_CLKSEL | Gyro_ADC_PUMP_CR1_FORCE );
 
    /* This is only valid if there is an internal clock */
    #if(Gyro_ADC_DEFAULT_INTERNAL_CLK)
        Gyro_ADC_PWRMGR_CLK_REG |= Gyro_ADC_ACT_PWR_CLK_EN;        
        Gyro_ADC_STBY_PWRMGR_CLK_REG |= Gyro_ADC_STBY_PWR_CLK_EN;
    #endif /* Gyro_ADC_DEFAULT_INTERNAL_CLK */
    
    /* Enable the active and alternate active power for charge pump clock */
    Gyro_ADC_PWRMGR_CHARGE_PUMP_CLK_REG |= Gyro_ADC_ACT_PWR_CHARGE_PUMP_CLK_EN;
    Gyro_ADC_STBY_PWRMGR_CHARGE_PUMP_CLK_REG |= Gyro_ADC_STBY_PWR_CHARGE_PUMP_CLK_EN;
        
}


#if (CY_PSOC5A) /* valid only for PSoC5A silicon */
    /******************************************************************************* 
    * Function Name: Gyro_ADC_RestorePowerRegisters
    ********************************************************************************
    *
    * Summary: 
    *  Restores the power registers on calling Start() API. This API is only 
    *   for internal use and valid only for PSoC5A.
    *  
    *
    * Parameters:  
    *  void
    *
    * Return: 
    *  void 
    *
    ***************************************************************************/
    void Gyro_ADC_RestorePowerRegisters(void) 
    {
        Gyro_ADC_DSM_CR14_REG = Gyro_ADC_powerModeBackup.DSM_CR_14;
        Gyro_ADC_DSM_CR15_REG = Gyro_ADC_powerModeBackup.DSM_CR_15;
        Gyro_ADC_DSM_CR16_REG = Gyro_ADC_powerModeBackup.DSM_CR_16;
        Gyro_ADC_DSM_CR17_REG = Gyro_ADC_powerModeBackup.DSM_CR_17;
        Gyro_ADC_DSM_REF0_REG = Gyro_ADC_powerModeBackup.DSM_REF0;
        Gyro_ADC_DSM_BUF0_REG = Gyro_ADC_powerModeBackup.DSM_BUF0;
        Gyro_ADC_DSM_BUF1_REG = Gyro_ADC_powerModeBackup.DSM_BUF1;
        Gyro_ADC_DSM_CLK_REG = Gyro_ADC_powerModeBackup.DSM_CLK;
    }


    /************************************************************************** 
    * Function Name: Gyro_ADC_SavePowerRegisters
    ***************************************************************************
    *
    * Summary: 
    *  Save the power registers before stopping the block operation. This is 
    *  called by Stop() API. This API is only for internal use and valid only 
    *  for PSoC5A.
    *  
    *
    * Parameters:  
    *  void
    *
    * Return: 
    *  void 
    *
    **************************************************************************/
    void Gyro_ADC_SavePowerRegisters(void) 
    {
        Gyro_ADC_powerModeBackup.DSM_CR_14 = Gyro_ADC_DSM_CR14_REG;
        Gyro_ADC_powerModeBackup.DSM_CR_15 = Gyro_ADC_DSM_CR15_REG;
        Gyro_ADC_powerModeBackup.DSM_CR_16 = Gyro_ADC_DSM_CR16_REG;
        Gyro_ADC_powerModeBackup.DSM_CR_17 = Gyro_ADC_DSM_CR17_REG;
        Gyro_ADC_powerModeBackup.DSM_REF0 = Gyro_ADC_DSM_REF0_REG;
        Gyro_ADC_powerModeBackup.DSM_BUF0 = Gyro_ADC_DSM_BUF0_REG;
        Gyro_ADC_powerModeBackup.DSM_BUF1 = Gyro_ADC_DSM_BUF1_REG;
        Gyro_ADC_powerModeBackup.DSM_CLK = Gyro_ADC_DSM_CLK_REG;
    }


    /************************************************************************* 
    * Function Name: Gyro_ADC_SetLowPower
    ***************************************************************************
    *
    * Summary: 
    *  Set all the power registers of DSM block to low power mode. This API is
    *   called by Stop() API. This API is only for internal use and valid for 
    *   only PSoC5A.
    *  
    *
    * Parameters:  
    *  void
    *
    * Return: 
    *  void 
    *
    ***************************************************************************/
    void Gyro_ADC_SetLowPower(void) 
    {
        Gyro_ADC_DSM_CR14_REG &= ~Gyro_ADC_DSM_POWER1_MASK;
        Gyro_ADC_DSM_CR14_REG |= Gyro_ADC_DSM_POWER1_44UA;
        
        Gyro_ADC_DSM_CR15_REG &= ~(Gyro_ADC_DSM_POWER2_3_MASK | Gyro_ADC_DSM_POWER_COMP_MASK);
        Gyro_ADC_DSM_CR15_REG |= (Gyro_ADC_DSM_POWER2_3_LOW | Gyro_ADC_DSM_POWER_VERYLOW);
        
        Gyro_ADC_DSM_CR16_REG &= ~(Gyro_ADC_DSM_CP_PWRCTL_MASK | Gyro_ADC_DSM_POWER_SUM_MASK |
                                           Gyro_ADC_DSM_CP_ENABLE);
        Gyro_ADC_DSM_CR16_REG |= (Gyro_ADC_DSM_POWER_SUM_LOW | Gyro_ADC_DSM_CP_PWRCTL_LOW);
        
        Gyro_ADC_DSM_CR17_REG &= ~(Gyro_ADC_DSM_EN_BUF_VREF | Gyro_ADC_DSM_PWR_CTRL_VCM_MASK |
                                           Gyro_ADC_DSM_PWR_CTRL_VREF_MASK | Gyro_ADC_DSM_EN_BUF_VCM |
                                           Gyro_ADC_DSM_PWR_CTRL_VREF_INN_MASK);
        Gyro_ADC_DSM_CR17_REG |= (Gyro_ADC_DSM_PWR_CTRL_VREF_0 | Gyro_ADC_DSM_PWR_CTRL_VCM_0 |
                                           Gyro_ADC_DSM_PWR_CTRL_VREF_INN_0);
        
        /* Disable reference buffers */
        Gyro_ADC_DSM_REF0_REG &= ~(Gyro_ADC_DSM_EN_BUF_VREF_INN | Gyro_ADC_DSM_VREF_RES_DIV_EN |
                                           Gyro_ADC_DSM_VCM_RES_DIV_EN);
            
        /* Disable the positive input buffer */
        Gyro_ADC_DSM_BUF0_REG &= ~Gyro_ADC_DSM_ENABLE_P;
        /* Disable the negative input buffer */
        Gyro_ADC_DSM_BUF1_REG &= ~Gyro_ADC_DSM_ENABLE_N;
        /* Disable the clock to DSM */
        Gyro_ADC_DSM_CLK_REG &= ~(Gyro_ADC_DSM_CLK_CLK_EN | Gyro_ADC_DSM_CLK_BYPASS_SYNC);
    }
#endif /* CY_PSOC5A */


/******************************************************************************* 
* Function Name: Gyro_ADC_Start
********************************************************************************
*
* Summary:
*  The start function initializes the Delta Sigma Modulator with the default  
*  values, and sets the power to the given level.  A power level of 0, is the 
*  same as executing the stop function.
*
* Parameters:  
*  None
*
* Return: 
*  void 
*
* Global variables:
*  Gyro_ADC_initVar:  Used to check the initial configuration,
*  modified when this function is called for the first time.
*
*******************************************************************************/
void Gyro_ADC_Start() 
{
    if(Gyro_ADC_initVar == 0u)
    {
        if(!(Gyro_ADC_started))
        {
            Gyro_ADC_Init();
        }
        Gyro_ADC_initVar = 1u;
    }

    /* Enable the ADC */
    Gyro_ADC_Enable();
}


/*******************************************************************************
* Function Name: Gyro_ADC_Stop
********************************************************************************
*
* Summary:
*  Stops and powers down ADC to lowest power state.
*
* Parameters:  
*  void
*
* Return: 
*  void 
*
*******************************************************************************/
void Gyro_ADC_Stop(void) 
{
    /* Stop conversions */
    Gyro_ADC_DEC_CR_REG &= ~Gyro_ADC_DEC_START_CONV;
    Gyro_ADC_DEC_SR_REG |=  Gyro_ADC_DEC_INTR_CLEAR;
    
    /* Disable PRES, Disable power to VCMBUF0, REFBUF0 and REFBUF1, 
       enable PRES */
    #if (CY_PSOC3 || CY_PSOC5LP)
        Gyro_ADC_RESET_CR4_REG |= Gyro_ADC_IGNORE_PRESA1;
        Gyro_ADC_RESET_CR5_REG |= Gyro_ADC_IGNORE_PRESA2;
    #elif (CY_PSOC5A)
        Gyro_ADC_RESET_CR1_REG |= Gyro_ADC_DIS_PRES1;
        Gyro_ADC_RESET_CR3_REG |= Gyro_ADC_DIS_PRES2;
    #endif /* CY_PSOC5A */
    
    Gyro_ADC_DSM_CR17_REG &= ~(Gyro_ADC_DSM_EN_BUF_VREF | Gyro_ADC_DSM_EN_BUF_VCM);
    Gyro_ADC_DSM_REF0_REG &= ~Gyro_ADC_DSM_EN_BUF_VREF_INN;
    
    /* Wait for 3 microseconds. */
    CyDelayUs(3);
    
	/* Enable the press circuit */
    #if (CY_PSOC3 || CY_PSOC5LP)
        Gyro_ADC_RESET_CR4_REG &= ~Gyro_ADC_IGNORE_PRESA1;
        Gyro_ADC_RESET_CR5_REG &= ~Gyro_ADC_IGNORE_PRESA2;
    #elif (CY_PSOC5A)
        Gyro_ADC_RESET_CR1_REG &= ~Gyro_ADC_DIS_PRES1;
        Gyro_ADC_RESET_CR3_REG &= ~Gyro_ADC_DIS_PRES2;
    #endif /* CY_PSOC5A */
    
    /* If PSoC5A then don't disable the power instead put the block to  
       low power mode. Also, save current state of all the power configuration 
       registers before putting block to low power mode */
    #if (CY_PSOC5A)
        
        /* set the flag */
        Gyro_ADC_restoreReg = 1u;
        
        Gyro_ADC_SavePowerRegisters();
        Gyro_ADC_SetLowPower();
    #else    
        /* Disable power to the ADC */
        Gyro_ADC_PWRMGR_DSM_REG &= ~Gyro_ADC_ACT_PWR_DSM_EN;
    #endif /* CY_PSOC5A */
    
    /* Disable power to Decimator block */
    Gyro_ADC_PWRMGR_DEC_REG &= ~Gyro_ADC_ACT_PWR_DEC_EN;
    
    /* Disable alternative active power to the ADC */
    Gyro_ADC_STBY_PWRMGR_DEC_REG &= ~Gyro_ADC_STBY_PWR_DEC_EN;
    #if (CY_PSOC3 || CY_PSOC5LP)
    Gyro_ADC_STBY_PWRMGR_DSM_REG &= ~Gyro_ADC_STBY_PWR_DSM_EN;
    #endif /* CY_PSOC3 || CY_PSOC5LP */

   /* Disable negative pumps for DSM  */
    Gyro_ADC_PUMP_CR1_REG &= ~(Gyro_ADC_PUMP_CR1_CLKSEL | Gyro_ADC_PUMP_CR1_FORCE );
    
    /* This is only valid if there is an internal clock */
    #if(Gyro_ADC_DEFAULT_INTERNAL_CLK)
        Gyro_ADC_PWRMGR_CLK_REG &= ~Gyro_ADC_ACT_PWR_CLK_EN;
        Gyro_ADC_STBY_PWRMGR_CLK_REG &= ~Gyro_ADC_STBY_PWR_CLK_EN;
    #endif /* Gyro_ADC_DEFAULT_INTERNAL_CLK */
    
    /* Disable power to charge pump clock */
    Gyro_ADC_PWRMGR_CHARGE_PUMP_CLK_REG &= ~Gyro_ADC_ACT_PWR_CHARGE_PUMP_CLK_EN;
    Gyro_ADC_STBY_PWRMGR_CHARGE_PUMP_CLK_REG &= ~Gyro_ADC_STBY_PWR_CHARGE_PUMP_CLK_EN;
}


/*******************************************************************************
* Function Name: Gyro_ADC_SetBufferGain
********************************************************************************
*
* Summary:
*  Set input buffer range.
*
* Parameters:  
*  gain:  Two bit value to select a gain of 1, 2, 4, or 8.
*
* Return: 
*  void
*
*******************************************************************************/
void Gyro_ADC_SetBufferGain(uint8 gain) 
{
    uint8 tmpReg;
    tmpReg = Gyro_ADC_DSM_BUF1_REG & ~Gyro_ADC_DSM_GAIN_MASK;
    tmpReg |= (gain << 2);
    Gyro_ADC_DSM_BUF1_REG = tmpReg;
}


/*******************************************************************************
* Function Name: Gyro_ADC_SetCoherency
********************************************************************************
*
* Summary:
*  Sets the ADC output register coherency bit.
*
* Parameters:  
*  gain:  Two bit value to set the coherency bit.
          00-Coherency checking off
          01-low byte is key byte
          02-middle byte is the key byte
          03-high byte is the key byte
*
* Return: 
*  void
*
* Side Effects:  If the coherency is changed, for any reason, it should be 
*                changed back to the LSB when the provided "GetResult" API 
*                is used.
*
*******************************************************************************/
void Gyro_ADC_SetCoherency(uint8 coherency) 
{
    uint8 tmpReg;    
    tmpReg = Gyro_ADC_DEC_COHER_REG & ~Gyro_ADC_DEC_SAMP_KEY_MASK;
    tmpReg |= coherency;
    Gyro_ADC_DEC_COHER_REG = tmpReg;
}


/*******************************************************************************
* Function Name: Gyro_ADC_SetGCOR
********************************************************************************
*
* Summary:
*  Calculates a new GCOR value and writes it into the GCOR register.
*
* Parameters:  
*  gainAdjust:  floating point value to set GCOR registers.
*
* Return: 
*  uint8:  0-if GCOR value is within the expected range.
           1- if GCOR is outside the expected range.
*
* Side Effects:  The GVAL register is set to the amount of valid bits in the
*                GCOR  register minus one. If GVAL is 15 (0x0F), all 16 bits
*                of the GCOR registers will be valid. If for example GVAL is 
*                11 (0x0B) only 12 bits will be valid. The least 4 bits will
*                be lost when the GCOR value is shifted 4 places to the right.
*
******************************************************************************/
uint8 Gyro_ADC_SetGCOR(float gainAdjust) 
{
    uint16 tmpReg;
    uint8 status;
    float tmpValue;
    
    tmpReg = Gyro_ADC_DEC_GCORH_REG;
    tmpReg = (tmpReg << 8) | Gyro_ADC_DEC_GCOR_REG;
    tmpValue = ((float)tmpReg / Gyro_ADC_IDEAL_GAIN_CONST);
    tmpValue = tmpValue * gainAdjust;
    
    if (tmpValue > 1.9999)
    {
        status = 1;
    }
    else
    {
        tmpReg = (uint16)(tmpValue * Gyro_ADC_IDEAL_GAIN_CONST);
        Gyro_ADC_DEC_GCOR_REG = (uint8)tmpReg;
        Gyro_ADC_DEC_GCORH_REG = (uint8) (tmpReg >> 8);
        status = 0;
    }
    
    return status;
}


/******************************************************************************
* Function Name: Gyro_ADC_ReadGCOR
*******************************************************************************
*
* Summary:
*  This API returns the current GCOR register value, normalized based on the 
*  GVAL register settings.
*
* Parameters:  
*  void
*
* Return: 
*  uint16:  Normalized GCOR value.
*
* Side Effects:  If the GVAL register is set to a value greater than 0x0F, then
                 it gives unexpected value.
*
*******************************************************************************/
uint16 Gyro_ADC_ReadGCOR(void) 
{
    uint8 gValue;
    uint16 gcorValue, gcorRegValue;
    
    gValue = Gyro_ADC_DEC_GVAL_REG;
    gcorRegValue = CY_GET_REG16(Gyro_ADC_DEC_GCOR_PTR);
    
    switch (gValue)
    {
        case Gyro_ADC_GVAL_RESOLUTIN_8:
            gcorValue = gcorRegValue << (Gyro_ADC_MAX_GVAL - gValue);
            break;
        
        case Gyro_ADC_GVAL_RESOLUTIN_9:
            gcorValue = gcorRegValue << (Gyro_ADC_MAX_GVAL - gValue);
            break;
            
        case Gyro_ADC_GVAL_RESOLUTIN_10:
            gcorValue = gcorRegValue << (Gyro_ADC_MAX_GVAL - gValue);
            break;
            
        case Gyro_ADC_GVAL_RESOLUTIN_11:
            gcorValue = gcorRegValue << (Gyro_ADC_MAX_GVAL - gValue);
            break;
            
        case Gyro_ADC_GVAL_RESOLUTIN_12:
            gcorValue = gcorRegValue << (Gyro_ADC_MAX_GVAL - gValue);
            break;
            
        case Gyro_ADC_GVAL_RESOLUTIN_13:
            gcorValue = gcorRegValue << (Gyro_ADC_MAX_GVAL - gValue);
            break;
            
        default:
            gcorValue = gcorRegValue;
            break;
    }
        
    return gcorValue;
}


/*******************************************************************************
* Function Name: Gyro_ADC_SetBufferChop
********************************************************************************
*
* Summary:
*  Sets the Delta Sigma Modulator Buffer chopper mode.
*
* Parameters:  
*  chopen:  If non-zero set the chopper mode.
*  chopFreq:  Chop frequency value.
*
* Return: 
*  void
*
*******************************************************************************/
void Gyro_ADC_SetBufferChop(uint8 chopen, uint8 chopFreq) 
{
    if(chopen != 0u)
    {
        Gyro_ADC_DSM_BUF2_REG = (Gyro_ADC_DSM_BUF_FCHOP_MASK & chopFreq) | \
                                         Gyro_ADC_DSM_BUF_CHOP_EN;
    }
    else
    {
        Gyro_ADC_DSM_BUF2_REG = 0x00u;
    }
}


/*******************************************************************************
* Function Name: Gyro_ADC_StartConvert
********************************************************************************
*
* Summary:
*  Starts ADC conversion using the given mode.
*
* Parameters:  
*  void
*
* Return: 
*  void 
*
*******************************************************************************/
void Gyro_ADC_StartConvert(void) 
{
    /* Start the conversion */
    Gyro_ADC_DEC_CR_REG |= Gyro_ADC_DEC_START_CONV;  
}


/*******************************************************************************
* Function Name: Gyro_ADC_StopConvert
********************************************************************************
*
* Summary:
*  Starts ADC conversion using the given mode.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Gyro_ADC_convDone:  Modified when conversion is complete for single
*   sample mode with resolution is above 16.
*
*******************************************************************************/
void Gyro_ADC_StopConvert(void) 
{
    /* Stop all conversions */
    Gyro_ADC_DEC_CR_REG &= ~Gyro_ADC_DEC_START_CONV; 
    
    /* Software flag for checking conversion complete or not. Will be used when
       resolution is above 16 bits and conversion mode is single sample */
    Gyro_ADC_convDone = 1u;
}


/*******************************************************************************
* Function Name: Gyro_ADC_IsEndConversion
********************************************************************************
*
* Summary:
*  Queries the ADC to see if conversion is complete
*
* Parameters:  
*  wMode:  Wait mode, 0 if return with answer immediately.
*                     1 if wait until conversion is complete, then return.
*
* Return: 
*  uint8 status:  0 =>  Conversion not complete.
*                 1 =>  Conversion complete.
*
* Global variables:
*  Gyro_ADC_convDone:  Used to check whether conversion is complete
*  or not for single sample mode with resolution is above 16
*
*******************************************************************************/
uint8 Gyro_ADC_IsEndConversion(uint8 wMode) 
{
    uint8 status;
        
    /* Check for stop convert if conversion mode is Single Sample with 
       resolution above 16 bit */
    if(Gyro_ADC_stopConversion == 1u)
    {
        do
        {
            status = Gyro_ADC_convDone;
        } while((status != Gyro_ADC_DEC_CONV_DONE) && (wMode == Gyro_ADC_WAIT_FOR_RESULT));
    }
    else
    {
        do 
        {
            status = Gyro_ADC_DEC_SR_REG & Gyro_ADC_DEC_CONV_DONE;
        } while((status != Gyro_ADC_DEC_CONV_DONE) && (wMode == Gyro_ADC_WAIT_FOR_RESULT));
    }
    return(status);
}


/*******************************************************************************
* Function Name: Gyro_ADC_GetResult8
********************************************************************************
*
* Summary:
*  Returns an 8-bit result or the LSB of the last conversion.
*
* Parameters:  
*  void
*
* Return: 
*  int8:  ADC result.
*
*******************************************************************************/
int8 Gyro_ADC_GetResult8( void ) 
{
     return( Gyro_ADC_DEC_SAMP_REG );
}


/*******************************************************************************
* Function Name: Gyro_ADC_GetResult16
********************************************************************************
*
* Summary:
*  Returns a 16-bit result from the last ADC conversion.
*
* Parameters:  
*   void
*
* Return: 
*  int16:  ADC result.
*
*******************************************************************************/
int16 Gyro_ADC_GetResult16(void) 
{
    uint16 result;
    
    #if (CY_PSOC3)
        result = Gyro_ADC_DEC_SAMPM_REG ;
        result = (result << 8 ) | Gyro_ADC_DEC_SAMP_REG;
    #else
        result = (CY_GET_REG16(Gyro_ADC_DEC_SAMP_PTR));
    #endif /* CY_PSOC3 */
    
    return result;
}


/*******************************************************************************
* Function Name: Gyro_ADC_GetResult32
********************************************************************************
*
* Summary:
*  Returns an 32-bit result from ADC
*
* Parameters:  
*  void
*
* Return: 
*  int32:  ADC result.
*
*******************************************************************************/
int32 Gyro_ADC_GetResult32(void) 
{
    uint32 result;

    #if (CY_PSOC3)
        /* to make upper bits fo result to 0 */
        result = (int8) (0xff & Gyro_ADC_DEC_SAMPH_REG); 
        result = (result << 8) | Gyro_ADC_DEC_SAMPM_REG;
        result = (result << 8) | Gyro_ADC_DEC_SAMP_REG;
    #else
        result = CY_GET_REG16(Gyro_ADC_DEC_SAMPH_PTR);
        result = (result << 16) | (CY_GET_REG16(Gyro_ADC_DEC_SAMP_PTR));
    #endif /* CY_PSOC3 */
    
    return result;
}


/*******************************************************************************
* Function Name: Gyro_ADC_SetOffset
********************************************************************************
*
* Summary:
*  This function sets the offset for voltage readings.
*
* Parameters:  
*  int32:  offset  Offset in counts
*
* Return: 
*  void
*
* Global variables:
*  Gyro_ADC_Offset:  Modified to set the user provided offset. This 
*  variable is used for offset calibration purpose.
*  Affects the Gyro_ADC_CountsTo_Volts, 
*  Gyro_ADC_CountsTo_mVolts, Gyro_ADC_CountsTo_uVolts functions 
*  by subtracting the given offset. 
*
*******************************************************************************/
void Gyro_ADC_SetOffset(int32 offset) 
{
 
    Gyro_ADC_Offset = offset;
}


/*******************************************************************************
* Function Name: Gyro_ADC_SetGain
********************************************************************************
*
* Summary:
*  This function sets the ADC gain in counts per volt.
*
* Parameters:  
*  int32:  offset  Offset in counts
*
* Return: 
*  void 
*
* Global variables:
*  Gyro_ADC_CountsPerVolt:  modified to set the ADC gain in counts 
*   per volt.
*
*******************************************************************************/
void Gyro_ADC_SetGain(int32 adcGain) 
{
 
    Gyro_ADC_CountsPerVolt = adcGain;
}


/*******************************************************************************
* Function Name: Gyro_ADC_CountsTo_mVolts
********************************************************************************
*
* Summary:
*  This function converts ADC counts to mVolts.
*
* Parameters:  
*  int32:  adcCounts   Reading from ADC.
*
* Return: 
*  int32:  Result in mVolts
*
* Global variables:
*  Gyro_ADC_CountsPerVolt:  used to convert ADC counts to mVolts.
*  Gyro_ADC_Offset:  Used as the offset while converting ADC counts 
*   to mVolts.
*
*******************************************************************************/
int16 Gyro_ADC_CountsTo_mVolts( int32 adcCounts) 
{

    int32 mVolts = 0;
    int32 A, B;
	uint8 resolution = 16;

    /* Subtract ADC offset */
    adcCounts -= Gyro_ADC_Offset;
	
	/* Set the resolution based on the configuration */
	if (Gyro_ADC_Config == Gyro_ADC_CFG1)
	{
        resolution = Gyro_ADC_CFG1_RESOLUTION;
	}	
	else if (Gyro_ADC_Config == Gyro_ADC_CFG2)
	{
	    resolution = Gyro_ADC_CFG2_RESOLUTION;
	}
	else if (Gyro_ADC_Config == Gyro_ADC_CFG3)
	{
	    resolution = Gyro_ADC_CFG3_RESOLUTION;
	}
	else
	{
	    resolution = Gyro_ADC_CFG4_RESOLUTION;
	}
	
    if(resolution < 17)
    {
        A = 1000;
        B = 1;
    }
    else
    {
        A = 100;
        B = 10;
    }

    mVolts = ((A * adcCounts) / ((int32)Gyro_ADC_CountsPerVolt/B)) ;   

    return( (int16)mVolts );
}


/*******************************************************************************
* Function Name: Gyro_ADC_CountsTo_Volts
********************************************************************************
*
* Summary:
*  This function converts ADC counts to Volts
*
* Parameters:  
*  int32:  adcCounts   Reading from ADC.
*
* Return: 
*  float:  Result in Volts
*
* Global variables:
*  Gyro_ADC_CountsPerVolt:  used to convert to Volts.
*  Gyro_ADC_Offset:  Used as the offset while converting ADC counts 
*   to Volts.
*
*******************************************************************************/
float Gyro_ADC_CountsTo_Volts( int32 adcCounts) 
{

    float Volts = 0;

    /* Subtract ADC offset */
    adcCounts -= Gyro_ADC_Offset;

    Volts = (float)adcCounts / (float)Gyro_ADC_CountsPerVolt;   
    
    return( Volts );
}


/*******************************************************************************
* Function Name: Gyro_ADC_CountsTo_uVolts
********************************************************************************
*
* Summary:
*  This function converts ADC counts to uVolts
*
* Parameters:  
*  int32:  adcCounts   Reading from ADC.
*
* Return: 
*  int32:  Result in uVolts
*
* Global variables:
*  Gyro_ADC_CountsPerVolt:  used to convert ADC counts to mVolts.
*  Gyro_ADC_Offset:  Used as the offset while converting ADC counts 
*   to mVolts.
*
* Theory: 
* Care must be taken to not exceed the maximum value for a 32 bit signed
* number in the conversion to uVolts and at the same time not loose 
* resolution.
*
* uVolts = ((A * adcCounts) / ((int32)Gyro_ADC_CountsPerVolt/B)) ;   
*
*  Resolution       A           B
*   8 - 11      1,000,000         1
*  12 - 14        100,000        10
*  15 - 17         10,000       100
*  18 - 20           1000      1000
*
*******************************************************************************/
int32 Gyro_ADC_CountsTo_uVolts( int32 adcCounts) 
{

    int32 uVolts = 0;
    int32 A, B;
	uint8 resolution = 16;
	
	/* Set the resolution based on the configuration */
	if (Gyro_ADC_Config == Gyro_ADC_CFG1)
	{
        resolution = Gyro_ADC_CFG1_RESOLUTION;
	}	
	else if (Gyro_ADC_Config == Gyro_ADC_CFG2)
	{
	    resolution = Gyro_ADC_CFG2_RESOLUTION;
	}
	else if (Gyro_ADC_Config == Gyro_ADC_CFG3)
	{
	    resolution = Gyro_ADC_CFG3_RESOLUTION;
	}
	else
	{
	    resolution = Gyro_ADC_CFG4_RESOLUTION;
	}
    
    if(resolution < 12)
    {
        A = 1000000;
        B = 1;
    }
    else if(resolution < 15)
    {
        A = 100000;
        B = 10;
    }
    else if(resolution < 18)
    {
        A = 10000;
        B = 100;
    }
    else
    {
        A = 1000;
        B = 1000;
    }

    /* Subtract ADC offset */
    adcCounts -= Gyro_ADC_Offset;

    uVolts = ((A * adcCounts) / ((int32)Gyro_ADC_CountsPerVolt/B)) ;   
  
    return( uVolts );
}


/*******************************************************************************
* Function Name: Gyro_ADC_IRQ_Start(void)
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it. The IRQ_Start() API is included to 
*  support legacy code. The routine has been replaced by the library source 
*  code for interrupts. IRQ_Start() should not be used in new designs.
* 
* Parameters:  
*   void.
*
* Return:
*   void.
*
*******************************************************************************/
void Gyro_ADC_IRQ_Start(void) 
{
    /* For all we know the interrupt is active. */
    CyIntDisable(Gyro_ADC_IRQ__INTC_NUMBER );

    /* Set the ISR to point to the ADC_DelSig_1_IRQ Interrupt. */
    CyIntSetVector(Gyro_ADC_IRQ__INTC_NUMBER, Gyro_ADC_ISR1);

    /* Set the priority. */
    CyIntSetPriority(Gyro_ADC_IRQ__INTC_NUMBER, Gyro_ADC_IRQ_INTC_PRIOR_NUMBER);

    /* Enable interrupt. */
    CyIntEnable(Gyro_ADC_IRQ__INTC_NUMBER);
}


/*******************************************************************************
* Function Name: Gyro_ADC_InitConfig(uint8 config)
********************************************************************************
*
* Summary:
*  Initializes all registers based on customizer settings
*
* Parameters:  
*   void
*
* Return: 
*  void
*
* Global variables:
*  Gyro_ADC_initVar:  used to set the common registers in the beginning.
*  Gyro_ADC_CountsPerVolt:  Used to set the default counts per volt.
*
*******************************************************************************/
void Gyro_ADC_InitConfig(uint8 config) 
{
    Gyro_ADC_stopConversion = 0u;
    
    if(Gyro_ADC_initVar == 0u)
        {
            Gyro_ADC_DSM_DEM0_REG    = Gyro_ADC_CFG1_DSM_DEM0;    
            Gyro_ADC_DSM_DEM1_REG    = Gyro_ADC_CFG1_DSM_DEM1;    
            Gyro_ADC_DSM_MISC_REG    = Gyro_ADC_CFG1_DSM_MISC;    
            Gyro_ADC_DSM_CLK_REG    |= Gyro_ADC_CFG1_DSM_CLK; 
            Gyro_ADC_DSM_REF1_REG    = Gyro_ADC_CFG1_DSM_REF1;    
        
            Gyro_ADC_DSM_OUT0_REG    = Gyro_ADC_CFG1_DSM_OUT0;    
            Gyro_ADC_DSM_OUT1_REG    = Gyro_ADC_CFG1_DSM_OUT1;   
        
            Gyro_ADC_DSM_CR0_REG     = Gyro_ADC_CFG1_DSM_CR0;     
            Gyro_ADC_DSM_CR1_REG     = Gyro_ADC_CFG1_DSM_CR1;     
            Gyro_ADC_DSM_CR2_REG     = Gyro_ADC_CFG1_DSM_CR2;     
            Gyro_ADC_DSM_CR3_REG     = Gyro_ADC_CFG1_DSM_CR3;     
            Gyro_ADC_DSM_CR13_REG    = Gyro_ADC_CFG1_DSM_CR13;     
            
            Gyro_ADC_DEC_SR_REG      = Gyro_ADC_CFG1_DEC_SR;      
            Gyro_ADC_DEC_COHER_REG   = Gyro_ADC_CFG1_DEC_COHER;   
        }

    if (config == 1u)
    {
        /* Default Config */   
        Gyro_ADC_DEC_CR_REG      = Gyro_ADC_CFG1_DEC_CR;      
        Gyro_ADC_DEC_SHIFT1_REG  = Gyro_ADC_CFG1_DEC_SHIFT1;  
        Gyro_ADC_DEC_SHIFT2_REG  = Gyro_ADC_CFG1_DEC_SHIFT2;  
        Gyro_ADC_DEC_DR2_REG     = Gyro_ADC_CFG1_DEC_DR2;     
        Gyro_ADC_DEC_DR2H_REG    = Gyro_ADC_CFG1_DEC_DR2H;    
        Gyro_ADC_DEC_DR1_REG     = Gyro_ADC_CFG1_DEC_DR1;     
        Gyro_ADC_DEC_OCOR_REG    = Gyro_ADC_CFG1_DEC_OCOR;    
        Gyro_ADC_DEC_OCORM_REG   = Gyro_ADC_CFG1_DEC_OCORM;   
        Gyro_ADC_DEC_OCORH_REG   = Gyro_ADC_CFG1_DEC_OCORH;   
        
        Gyro_ADC_DSM_CR4_REG     = Gyro_ADC_CFG1_DSM_CR4;     
        Gyro_ADC_DSM_CR5_REG     = Gyro_ADC_CFG1_DSM_CR5;     
        Gyro_ADC_DSM_CR6_REG     = Gyro_ADC_CFG1_DSM_CR6;     
        Gyro_ADC_DSM_CR7_REG     = Gyro_ADC_CFG1_DSM_CR7;     
        Gyro_ADC_DSM_CR8_REG     = Gyro_ADC_CFG1_DSM_CR8;     
        Gyro_ADC_DSM_CR9_REG     = Gyro_ADC_CFG1_DSM_CR9;     
        Gyro_ADC_DSM_CR10_REG    = Gyro_ADC_CFG1_DSM_CR10;    
        Gyro_ADC_DSM_CR11_REG    = Gyro_ADC_CFG1_DSM_CR11;    
        Gyro_ADC_DSM_CR12_REG    = Gyro_ADC_CFG1_DSM_CR12;    
        Gyro_ADC_DSM_CR14_REG    = Gyro_ADC_CFG1_DSM_CR14;    
        Gyro_ADC_DSM_CR15_REG    = Gyro_ADC_CFG1_DSM_CR15;    
        Gyro_ADC_DSM_CR16_REG    = Gyro_ADC_CFG1_DSM_CR16;    
        Gyro_ADC_DSM_CR17_REG    = Gyro_ADC_CFG1_DSM_CR17;
		/* Set DSM_REF0_REG by disabling and enabling the PRESS cirucit */
		Gyro_ADC_SetDSMRef0Reg(Gyro_ADC_CFG1_DSM_REF0);
        Gyro_ADC_DSM_REF2_REG    = Gyro_ADC_CFG1_DSM_REF2;    
        Gyro_ADC_DSM_REF3_REG    = Gyro_ADC_CFG1_DSM_REF3;    
        
        Gyro_ADC_DSM_BUF0_REG    = Gyro_ADC_CFG1_DSM_BUF0;    
        Gyro_ADC_DSM_BUF1_REG    = Gyro_ADC_CFG1_DSM_BUF1;    
        Gyro_ADC_DSM_BUF2_REG    = Gyro_ADC_CFG1_DSM_BUF2;   
        Gyro_ADC_DSM_BUF3_REG    = Gyro_ADC_CFG1_DSM_BUF3;
        #if (CY_PSOC5A)
            Gyro_ADC_DSM_CLK_REG    |= Gyro_ADC_CFG1_DSM_CLK; 
        #endif /* CY_PSOC5A */
        
        /* To select either Vssa or Vref to -ve input of DSM depending on 
          the input  range selected.
        */
        
        #if(Gyro_ADC_DEFAULT_INPUT_MODE)
            #if (CY_PSOC3 || CY_PSOC5LP)
                #if (Gyro_ADC_CFG1_RANGE == Gyro_ADC_IR_VSSA_TO_2VREF)
                    Gyro_ADC_AMux_Select(1);
                #else
                    Gyro_ADC_AMux_Select(0);
                #endif /* Gyro_ADC_IR_VSSA_TO_2VREF) */
            #elif (CY_PSOC5A)
                Gyro_ADC_AMux_Select(0);
            #endif /* CY_PSOC3 || CY_PSOC5LP */
        #endif /* Gyro_ADC_DEFAULT_INPUT_MODE */
        
        /* Set the Conversion stop if resolution is above 16 bit and conversion 
           mode is Single sample */
        #if(Gyro_ADC_CFG1_RESOLUTION > 16 && \
            Gyro_ADC_CFG1_CONV_MODE == Gyro_ADC_MODE_SINGLE_SAMPLE) 
            Gyro_ADC_stopConversion = 1;
        #endif /* Single sample with resolution above 16 bits. */
        Gyro_ADC_CountsPerVolt = (uint32)Gyro_ADC_CFG1_COUNTS_PER_VOLT;
        
        /* Start and set interrupt vector */
        CyIntSetPriority(Gyro_ADC_IRQ__INTC_NUMBER, Gyro_ADC_IRQ_INTC_PRIOR_NUMBER);
        CyIntSetVector(Gyro_ADC_IRQ__INTC_NUMBER, Gyro_ADC_ISR1 );
        CyIntEnable(Gyro_ADC_IRQ__INTC_NUMBER);
    }
    
    #if(Gyro_ADC_DEFAULT_NUM_CONFIGS > 1)
        if(config == 2u)
        {
            /* Second Config */
            Gyro_ADC_DEC_CR_REG      = Gyro_ADC_CFG2_DEC_CR;      
            Gyro_ADC_DEC_SHIFT1_REG  = Gyro_ADC_CFG2_DEC_SHIFT1;  
            Gyro_ADC_DEC_SHIFT2_REG  = Gyro_ADC_CFG2_DEC_SHIFT2;  
            Gyro_ADC_DEC_DR2_REG     = Gyro_ADC_CFG2_DEC_DR2;     
            Gyro_ADC_DEC_DR2H_REG    = Gyro_ADC_CFG2_DEC_DR2H;    
            Gyro_ADC_DEC_DR1_REG     = Gyro_ADC_CFG2_DEC_DR1;     
            Gyro_ADC_DEC_OCOR_REG    = Gyro_ADC_CFG2_DEC_OCOR;    
            Gyro_ADC_DEC_OCORM_REG   = Gyro_ADC_CFG2_DEC_OCORM;   
            Gyro_ADC_DEC_OCORH_REG   = Gyro_ADC_CFG2_DEC_OCORH;   
        
            Gyro_ADC_DSM_CR4_REG     = Gyro_ADC_CFG2_DSM_CR4;     
            Gyro_ADC_DSM_CR5_REG     = Gyro_ADC_CFG2_DSM_CR5;     
            Gyro_ADC_DSM_CR6_REG     = Gyro_ADC_CFG2_DSM_CR6;     
            Gyro_ADC_DSM_CR7_REG     = Gyro_ADC_CFG2_DSM_CR7;     
            Gyro_ADC_DSM_CR8_REG     = Gyro_ADC_CFG2_DSM_CR8;     
            Gyro_ADC_DSM_CR9_REG     = Gyro_ADC_CFG2_DSM_CR9;     
            Gyro_ADC_DSM_CR10_REG    = Gyro_ADC_CFG2_DSM_CR10;    
            Gyro_ADC_DSM_CR11_REG    = Gyro_ADC_CFG2_DSM_CR11;    
            Gyro_ADC_DSM_CR12_REG    = Gyro_ADC_CFG2_DSM_CR12;    
            Gyro_ADC_DSM_CR14_REG    = Gyro_ADC_CFG2_DSM_CR14;    
            Gyro_ADC_DSM_CR15_REG    = Gyro_ADC_CFG2_DSM_CR15;    
            Gyro_ADC_DSM_CR16_REG    = Gyro_ADC_CFG2_DSM_CR16;    
            Gyro_ADC_DSM_CR17_REG    = Gyro_ADC_CFG2_DSM_CR17;    
			/* Set DSM_REF0_REG by disabling and enabling the PRESS cirucit */
			Gyro_ADC_SetDSMRef0Reg(Gyro_ADC_CFG2_DSM_REF0);
            Gyro_ADC_DSM_REF2_REG    = Gyro_ADC_CFG2_DSM_REF2;    
            Gyro_ADC_DSM_REF3_REG    = Gyro_ADC_CFG2_DSM_REF3;    
        
            Gyro_ADC_DSM_BUF0_REG    = Gyro_ADC_CFG2_DSM_BUF0;    
            Gyro_ADC_DSM_BUF1_REG    = Gyro_ADC_CFG2_DSM_BUF1;    
            Gyro_ADC_DSM_BUF2_REG    = Gyro_ADC_CFG2_DSM_BUF2;    
            Gyro_ADC_DSM_BUF3_REG    = Gyro_ADC_CFG2_DSM_BUF3; 
            #if (CY_PSOC5A)
                Gyro_ADC_DSM_CLK_REG    |= Gyro_ADC_CFG1_DSM_CLK; 
            #endif /* CY_PSOC5A */
            
            /* To select either Vssa or Vref to -ve input of DSM depending on 
               the input range selected.
            */
            
            #if(Gyro_ADC_DEFAULT_INPUT_MODE)
                #if (CY_PSOC3 || CY_PSOC5LP)
                    #if (Gyro_ADC_CFG2_INPUT_RANGE == Gyro_ADC_IR_VSSA_TO_2VREF)
                        Gyro_ADC_AMux_Select(1);
                    #else
                        Gyro_ADC_AMux_Select(0);
                    #endif /* Gyro_ADC_IR_VSSA_TO_2VREF) */
                #elif (CY_PSOC5A)
                    Gyro_ADC_AMux_Select(0);
                #endif /* CY_PSOC3 || CY_PSOC5LP */
            #endif /* Gyro_ADC_DEFAULT_INPUT_MODE */
            
            /* Set the Conversion stop if resolution is above 16 bit and 
               conversion mode is Single sample */
            #if(Gyro_ADC_CFG2_RESOLUTION > 16 && \
                Gyro_ADC_CFG2_CONVMODE == Gyro_ADC_MODE_SINGLE_SAMPLE) 
                Gyro_ADC_stopConversion = 1;
            #endif /* Single sample with resolution above 16 bits. */
            
            Gyro_ADC_CountsPerVolt = (uint32)Gyro_ADC_CFG2_COUNTS_PER_VOLT;
            
            /* Start and set interrupt vector */
            CyIntSetPriority(Gyro_ADC_IRQ__INTC_NUMBER, Gyro_ADC_IRQ_INTC_PRIOR_NUMBER);
            CyIntSetVector(Gyro_ADC_IRQ__INTC_NUMBER, Gyro_ADC_ISR2 );
            CyIntEnable(Gyro_ADC_IRQ__INTC_NUMBER);
        }
    #endif /* Gyro_ADC_DEFAULT_NUM_CONFIGS > 1 */

    #if(Gyro_ADC_DEFAULT_NUM_CONFIGS > 2)
        if(config == 3u)
        {
            /* Third Config */
            Gyro_ADC_DEC_CR_REG      = Gyro_ADC_CFG3_DEC_CR;      
            Gyro_ADC_DEC_SHIFT1_REG  = Gyro_ADC_CFG3_DEC_SHIFT1;  
            Gyro_ADC_DEC_SHIFT2_REG  = Gyro_ADC_CFG3_DEC_SHIFT2;  
            Gyro_ADC_DEC_DR2_REG     = Gyro_ADC_CFG3_DEC_DR2;     
            Gyro_ADC_DEC_DR2H_REG    = Gyro_ADC_CFG3_DEC_DR2H;    
            Gyro_ADC_DEC_DR1_REG     = Gyro_ADC_CFG3_DEC_DR1;     
            Gyro_ADC_DEC_OCOR_REG    = Gyro_ADC_CFG3_DEC_OCOR;    
            Gyro_ADC_DEC_OCORM_REG   = Gyro_ADC_CFG3_DEC_OCORM;   
            Gyro_ADC_DEC_OCORH_REG   = Gyro_ADC_CFG3_DEC_OCORH;   
         
            Gyro_ADC_DSM_CR4_REG     = Gyro_ADC_CFG3_DSM_CR4;     
            Gyro_ADC_DSM_CR5_REG     = Gyro_ADC_CFG3_DSM_CR5;     
            Gyro_ADC_DSM_CR6_REG     = Gyro_ADC_CFG3_DSM_CR6;     
            Gyro_ADC_DSM_CR7_REG     = Gyro_ADC_CFG3_DSM_CR7;     
            Gyro_ADC_DSM_CR8_REG     = Gyro_ADC_CFG3_DSM_CR8;     
            Gyro_ADC_DSM_CR9_REG     = Gyro_ADC_CFG3_DSM_CR9;     
            Gyro_ADC_DSM_CR10_REG    = Gyro_ADC_CFG3_DSM_CR10;    
            Gyro_ADC_DSM_CR11_REG    = Gyro_ADC_CFG3_DSM_CR11;    
            Gyro_ADC_DSM_CR12_REG    = Gyro_ADC_CFG3_DSM_CR12;    
            Gyro_ADC_DSM_CR14_REG    = Gyro_ADC_CFG3_DSM_CR14;    
            Gyro_ADC_DSM_CR15_REG    = Gyro_ADC_CFG3_DSM_CR15;    
            Gyro_ADC_DSM_CR16_REG    = Gyro_ADC_CFG3_DSM_CR16;    
            Gyro_ADC_DSM_CR17_REG    = Gyro_ADC_CFG3_DSM_CR17;    
			/* Set DSM_REF0_REG by disabling and enabling the PRESS cirucit */
			Gyro_ADC_SetDSMRef0Reg(Gyro_ADC_CFG3_DSM_REF0);
            Gyro_ADC_DSM_REF2_REG    = Gyro_ADC_CFG3_DSM_REF2;    
            Gyro_ADC_DSM_REF3_REG    = Gyro_ADC_CFG3_DSM_REF3;    
        
            Gyro_ADC_DSM_BUF0_REG    = Gyro_ADC_CFG3_DSM_BUF0;    
            Gyro_ADC_DSM_BUF1_REG    = Gyro_ADC_CFG3_DSM_BUF1;    
            Gyro_ADC_DSM_BUF2_REG    = Gyro_ADC_CFG3_DSM_BUF2;
            Gyro_ADC_DSM_BUF3_REG    = Gyro_ADC_CFG3_DSM_BUF3;
            #if (CY_PSOC5A)
                Gyro_ADC_DSM_CLK_REG    |= Gyro_ADC_CFG1_DSM_CLK; 
            #endif /* CY_PSOC5A */
            
            /* To select either Vssa or Vref to -ve input of DSM depending on 
               the input range selected.
            */
            
            #if(Gyro_ADC_DEFAULT_INPUT_MODE)
                #if (CY_PSOC3 || CY_PSOC5LP)
                    #if (Gyro_ADC_CFG3_INPUT_RANGE == Gyro_ADC_IR_VSSA_TO_2VREF)
                        Gyro_ADC_AMux_Select(1);
                    #else
                        Gyro_ADC_AMux_Select(0);
                    #endif /* Gyro_ADC_IR_VSSA_TO_2VREF) */
                #elif (CY_PSOC5A)
                    Gyro_ADC_AMux_Select(0);
                #endif /* CY_PSOC3 || CY_PSOC5LP */
            #endif /* Gyro_ADC_DEFAULT_INPUT_MODE */
                       
            /* Set the Conversion stop if resolution is above 16 bit and 
               conversion  mode is Single sample */
            #if(Gyro_ADC_CFG3_RESOLUTION > 16 && \
                Gyro_ADC_CFG3_CONVMODE == Gyro_ADC_MODE_SINGLE_SAMPLE) 
                Gyro_ADC_stopConversion = 1;
            #endif /* Single sample with resolution above 16 bits */
            
            Gyro_ADC_CountsPerVolt = (uint32)Gyro_ADC_CFG3_COUNTS_PER_VOLT;
            
            /* Start and set interrupt vector */
            CyIntSetPriority(Gyro_ADC_IRQ__INTC_NUMBER, Gyro_ADC_IRQ_INTC_PRIOR_NUMBER);
            CyIntSetVector(Gyro_ADC_IRQ__INTC_NUMBER, Gyro_ADC_ISR3 );
            CyIntEnable(Gyro_ADC_IRQ__INTC_NUMBER);
        }
    #endif /* Gyro_ADC_DEFAULT_NUM_CONFIGS > 3 */

    #if(Gyro_ADC_DEFAULT_NUM_CONFIGS == 4)
        if (config == 4u)
        {
            /* Fourth Config */
            Gyro_ADC_DEC_CR_REG      = Gyro_ADC_CFG4_DEC_CR;      
            Gyro_ADC_DEC_SHIFT1_REG  = Gyro_ADC_CFG4_DEC_SHIFT1;  
            Gyro_ADC_DEC_SHIFT2_REG  = Gyro_ADC_CFG4_DEC_SHIFT2;  
            Gyro_ADC_DEC_DR2_REG     = Gyro_ADC_CFG4_DEC_DR2;     
            Gyro_ADC_DEC_DR2H_REG    = Gyro_ADC_CFG4_DEC_DR2H;    
            Gyro_ADC_DEC_DR1_REG     = Gyro_ADC_CFG4_DEC_DR1;     
            Gyro_ADC_DEC_OCOR_REG    = Gyro_ADC_CFG4_DEC_OCOR;    
            Gyro_ADC_DEC_OCORM_REG   = Gyro_ADC_CFG4_DEC_OCORM;   
            Gyro_ADC_DEC_OCORH_REG   = Gyro_ADC_CFG4_DEC_OCORH;   

            Gyro_ADC_DSM_CR4_REG     = Gyro_ADC_CFG4_DSM_CR4;     
            Gyro_ADC_DSM_CR5_REG     = Gyro_ADC_CFG4_DSM_CR5;     
            Gyro_ADC_DSM_CR6_REG     = Gyro_ADC_CFG4_DSM_CR6;     
            Gyro_ADC_DSM_CR7_REG     = Gyro_ADC_CFG4_DSM_CR7;     
            Gyro_ADC_DSM_CR8_REG     = Gyro_ADC_CFG4_DSM_CR8;     
            Gyro_ADC_DSM_CR9_REG     = Gyro_ADC_CFG4_DSM_CR9;     
            Gyro_ADC_DSM_CR10_REG    = Gyro_ADC_CFG4_DSM_CR10;    
            Gyro_ADC_DSM_CR11_REG    = Gyro_ADC_CFG4_DSM_CR11;    
            Gyro_ADC_DSM_CR12_REG    = Gyro_ADC_CFG4_DSM_CR12;    
            Gyro_ADC_DSM_CR14_REG    = Gyro_ADC_CFG4_DSM_CR14;    
            Gyro_ADC_DSM_CR15_REG    = Gyro_ADC_CFG4_DSM_CR15;    
            Gyro_ADC_DSM_CR16_REG    = Gyro_ADC_CFG4_DSM_CR16;    
            Gyro_ADC_DSM_CR17_REG    = Gyro_ADC_CFG4_DSM_CR17;
			/* Set DSM_REF0_REG by disabling and enabling the PRESS cirucit */
			Gyro_ADC_SetDSMRef0Reg(Gyro_ADC_CFG4_DSM_REF0);
            Gyro_ADC_DSM_REF2_REG    = Gyro_ADC_CFG4_DSM_REF2;    
            Gyro_ADC_DSM_REF3_REG    = Gyro_ADC_CFG4_DSM_REF3;    
        
            Gyro_ADC_DSM_BUF0_REG    = Gyro_ADC_CFG4_DSM_BUF0;    
            Gyro_ADC_DSM_BUF1_REG    = Gyro_ADC_CFG4_DSM_BUF1;    
            Gyro_ADC_DSM_BUF2_REG    = Gyro_ADC_CFG4_DSM_BUF2;
            Gyro_ADC_DSM_BUF3_REG    = Gyro_ADC_CFG4_DSM_BUF3;
            #if (CY_PSOC5A)
                Gyro_ADC_DSM_CLK_REG    |= Gyro_ADC_CFG1_DSM_CLK; 
            #endif /* CY_PSOC5A */
            
            /* To select either Vssa or Vref to -ve input of DSM depending on 
               the input range selected.
            */
            
            #if(Gyro_ADC_DEFAULT_INPUT_MODE)
                #if (CY_PSOC3 || CY_PSOC5LP)
                    #if (Gyro_ADC_CFG4_INPUT_RANGE == Gyro_ADC_IR_VSSA_TO_2VREF)
                        Gyro_ADC_AMux_Select(1);
                    #else
                        Gyro_ADC_AMux_Select(0);
                    #endif /* Gyro_ADC_IR_VSSA_TO_2VREF) */
                #elif (CY_PSOC5A)
                    Gyro_ADC_AMux_Select(0);
                #endif /* CY_PSOC3 || CY_PSOC5LP */
            #endif /* Gyro_ADC_DEFAULT_INPUT_MODE */
                       
            /* Set the Conversion stop if resolution is above 16 bit and 
               conversion mode is Single sample */
            #if(Gyro_ADC_CFG4_RESOLUTION > 16 && \
                Gyro_ADC_CFG4_CONVMODE == Gyro_ADC_MODE_SINGLE_SAMPLE) 
                Gyro_ADC_stopConversion = 1;
            #endif /* Single sample with resolution above 16 bits */
            
            Gyro_ADC_CountsPerVolt = (uint32)Gyro_ADC_CFG4_COUNTS_PER_VOLT;

            /* Start and set interrupt vector */
            CyIntSetPriority(Gyro_ADC_IRQ__INTC_NUMBER, Gyro_ADC_IRQ_INTC_PRIOR_NUMBER);
            CyIntSetVector(Gyro_ADC_IRQ__INTC_NUMBER, Gyro_ADC_ISR4 );
            CyIntEnable(Gyro_ADC_IRQ__INTC_NUMBER);
        }
    #endif /* Gyro_ADC_DEFAULT_NUM_CONFIGS > 4 */
}


/*******************************************************************************
* Function Name: Gyro_ADC_RoundValue(uint32 busClockFreq, 
*                                            uint32 clockFreq)
********************************************************************************
*
* Summary:
*  Function to round an integer value.
*
* Parameters:  
*  busClockFreq:  Frequency of the bus clock.
*  clockFreq:  Frequency of the component clock.
*
* Return: 
*  uint16: rounded integer value.
*
*******************************************************************************/
uint16 Gyro_ADC_RoundValue(uint32 busClockFreq, uint32 clockFreq) \
                                  
{
    uint16 divider1, divider2;
    
    divider1 = ((busClockFreq * 10) / clockFreq);
    divider2 = (busClockFreq / clockFreq);
    if ( divider1 - (divider2 * 10) >= 5)
    {
        divider2 += 1;
    }
    return divider2;
}
         

/*******************************************************************************
* Function Name: Gyro_ADC_SelectCofiguration(uint8 config, 
*                                                    uint8 restart)
********************************************************************************
*
* Summary:
*  Selects the user defined configuration. This API first stops the current ADC
*  and then initializes the registers with the default values for that config. 
*  This also performs calibration by writing GCOR registers with trim values 
*  stored in the flash.
*
* Parameters:  
*  config:  configuration user wants to select.
*
* Return: 
*  void
*
*******************************************************************************/
void Gyro_ADC_SelectConfiguration(uint8 config, uint8 restart) \
                                              
{
    uint8 inputRange = 0, resolution = 16;
    uint16 idealGain = 0, adcClockDivider = 1;    
    uint16 cpClockDivider = 1;
    uint16 idealOddGain = 0;
    
    /* Check whether the config is valid or not */
    if( config <= Gyro_ADC_DEFAULT_NUM_CONFIGS)
    {   
       /* Set the flag to ensure start() API dont override the config
           selected if ADC is not already started */
        if(Gyro_ADC_initVar == 0u)
        {
            Gyro_ADC_started = 1u;
        }
		
		/* Update the config flag */
		Gyro_ADC_Config = config;
       
        /* Stop the ADC  */
        Gyro_ADC_Stop();
        
        #if (CY_PSOC5A)
            /* Set the structure field which checks whether or not to
               restore the power registers */
            Gyro_ADC_powerModeBackup.bypassRestore = Gyro_ADC_BYPASS_ENABLED;
        #endif /* CY_PSOC5A */
        
        /* Set the  ADC registers based on the configuration */
        Gyro_ADC_InitConfig(config);
        
        /* Trim value calculation */
        if(config == 1u)
        {
            inputRange = Gyro_ADC_CFG1_RANGE;
            resolution = Gyro_ADC_CFG1_RESOLUTION;
            idealGain = Gyro_ADC_CFG1_IDEAL_DEC_GAIN;
            idealOddGain = Gyro_ADC_CFG1_IDEAL_ODDDEC_GAIN;
            adcClockDivider = Gyro_ADC_RoundValue((uint32)BCLK__BUS_CLK__HZ,
                                                       (uint32)Gyro_ADC_CFG1_CLOCK_FREQ);
            cpClockDivider = Gyro_ADC_RoundValue((uint32)BCLK__BUS_CLK__HZ,
                                                       (uint32)Gyro_ADC_CFG1_CP_CLOCK_FREQ);
        }

        #if (Gyro_ADC_DEFAULT_NUM_CONFIGS > 1)
            if(config == 2u)
            {
                inputRange = Gyro_ADC_CFG2_INPUT_RANGE;
                resolution = Gyro_ADC_CFG2_RESOLUTION;
                idealGain = Gyro_ADC_CFG2_IDEAL_DEC_GAIN;
                idealOddGain = Gyro_ADC_CFG2_IDEAL_ODDDEC_GAIN;
                adcClockDivider = Gyro_ADC_RoundValue((uint32)BCLK__BUS_CLK__HZ,
                                                            (uint32)Gyro_ADC_CFG2_CLOCK_FREQ);
                cpClockDivider = Gyro_ADC_RoundValue((uint32)BCLK__BUS_CLK__HZ,
                                                       (uint32)Gyro_ADC_CFG2_CP_CLOCK_FREQ);
            }
        #endif /* Gyro_ADC_DEFAULT_NUM_CONFIGS > 1 */

        #if(Gyro_ADC_DEFAULT_NUM_CONFIGS > 2)
            if(config == 3u)
            {
                inputRange = Gyro_ADC_CFG3_INPUT_RANGE;
                resolution = Gyro_ADC_CFG3_RESOLUTION;
                idealGain = Gyro_ADC_CFG3_IDEAL_DEC_GAIN;
                idealOddGain = Gyro_ADC_CFG3_IDEAL_ODDDEC_GAIN;
                adcClockDivider = Gyro_ADC_RoundValue((uint32)BCLK__BUS_CLK__HZ,
                                                            (uint32)Gyro_ADC_CFG3_CLOCK_FREQ);
                cpClockDivider = Gyro_ADC_RoundValue((uint32)BCLK__BUS_CLK__HZ,
                                                       (uint32)Gyro_ADC_CFG3_CP_CLOCK_FREQ);
            }
        #endif /* Gyro_ADC_DEFAULT_NUM_CONFIGS > 2 */

        #if (Gyro_ADC_DEFAULT_NUM_CONFIGS > 3)
            if(config == 4u)
            {
                inputRange = Gyro_ADC_CFG4_INPUT_RANGE;
                resolution = Gyro_ADC_CFG4_RESOLUTION;
                idealGain = Gyro_ADC_CFG4_IDEAL_DEC_GAIN;
                idealOddGain = Gyro_ADC_CFG4_IDEAL_ODDDEC_GAIN;
                adcClockDivider = Gyro_ADC_RoundValue((uint32)BCLK__BUS_CLK__HZ,  
                                                            (uint32)Gyro_ADC_CFG4_CLOCK_FREQ);
                cpClockDivider = Gyro_ADC_RoundValue((uint32)BCLK__BUS_CLK__HZ,
                                                       (uint32)Gyro_ADC_CFG4_CP_CLOCK_FREQ);
            }
        #endif /* Gyro_ADC_DEFAULT_NUM_CONFIGS > 3 */
        
        adcClockDivider = adcClockDivider - 1;
        /* Set the proper divider for the internal clock */
        #if(Gyro_ADC_DEFAULT_INTERNAL_CLK)
            Gyro_ADC_theACLK_SetDividerRegister(adcClockDivider, 1);
        #endif /* Gyro_ADC_DEFAULT_INTERNAL_CLK */
        
        cpClockDivider = cpClockDivider - 1;
        /* Set the proper divider for the Charge pump clock */
        Gyro_ADC_Ext_CP_Clk_SetDividerRegister(cpClockDivider, 1);
        
        /* Compensate the gain */
        Gyro_ADC_GainCompensation(inputRange, idealGain, idealOddGain, resolution);
        
        if(restart == 1u)
        {        
            /* Restart the ADC */
            Gyro_ADC_Start();
            
            /* Code to disable the REFBUF0 if reference chosen is External ref */
            #if (((Gyro_ADC_CFG2_REFERENCE == Gyro_ADC_EXT_REF_ON_P03) || \
                 (Gyro_ADC_CFG2_REFERENCE == Gyro_ADC_EXT_REF_ON_P32)) || \
                 ((Gyro_ADC_CFG3_REFERENCE == Gyro_ADC_EXT_REF_ON_P03) || \
                 (Gyro_ADC_CFG3_REFERENCE == Gyro_ADC_EXT_REF_ON_P32)) || \
                 ((Gyro_ADC_CFG4_REFERENCE == Gyro_ADC_EXT_REF_ON_P03) || \
                 (Gyro_ADC_CFG4_REFERENCE == Gyro_ADC_EXT_REF_ON_P32)))
                if (((config == 2) && 
                    ((Gyro_ADC_CFG2_REFERENCE == Gyro_ADC_EXT_REF_ON_P03) ||
                    (Gyro_ADC_CFG2_REFERENCE == Gyro_ADC_EXT_REF_ON_P32))) ||
                    ((config == 3) && 
                    ((Gyro_ADC_CFG3_REFERENCE == Gyro_ADC_EXT_REF_ON_P03) ||
                    (Gyro_ADC_CFG3_REFERENCE == Gyro_ADC_EXT_REF_ON_P32))) ||
                    ((config == 4) && 
                    ((Gyro_ADC_CFG4_REFERENCE == Gyro_ADC_EXT_REF_ON_P03) ||
                    (Gyro_ADC_CFG4_REFERENCE == Gyro_ADC_EXT_REF_ON_P32))))
                {
                    /* Disable PRES, Enable power to VCMBUF0, REFBUF0 and 
                       REFBUF1, enable PRES */
                    #if (CY_PSOC3 || CY_PSOC5LP)
                        Gyro_ADC_RESET_CR4_REG |= Gyro_ADC_IGNORE_PRESA1;
                        Gyro_ADC_RESET_CR5_REG |= Gyro_ADC_IGNORE_PRESA2;
                    #elif (CY_PSOC5A)
                        Gyro_ADC_RESET_CR1_REG |= Gyro_ADC_DIS_PRES1;
                        Gyro_ADC_RESET_CR3_REG |= Gyro_ADC_DIS_PRES2;
                    #endif /* CY_PSOC5A */
        
                    /* Disable the REFBUF0 */
                    Gyro_ADC_DSM_CR17_REG &= ~Gyro_ADC_DSM_EN_BUF_VREF;
                    
                    /* Wait for 3 microseconds */
                    CyDelayUs(3);
                    /* Enable the press circuit */
                    #if (CY_PSOC3 || CY_PSOC5LP)
                        Gyro_ADC_RESET_CR4_REG &= ~Gyro_ADC_IGNORE_PRESA1;
                        Gyro_ADC_RESET_CR5_REG &= ~Gyro_ADC_IGNORE_PRESA2;
                    #elif (CY_PSOC5A)
                        Gyro_ADC_RESET_CR1_REG &= ~Gyro_ADC_DIS_PRES1;
                        Gyro_ADC_RESET_CR3_REG &= ~Gyro_ADC_DIS_PRES2;
                    #endif /* CY_PSOC5A */
                }
            #endif /* */
            
            #if ((CY_PSOC3 || CY_PSOC5LP) && \
                 ((Gyro_ADC_CFG2_INPUT_RANGE == Gyro_ADC_IR_VSSA_TO_2VREF) || \
                  (Gyro_ADC_CFG3_INPUT_RANGE == Gyro_ADC_IR_VSSA_TO_2VREF) || \
                  (Gyro_ADC_CFG4_INPUT_RANGE == Gyro_ADC_IR_VSSA_TO_2VREF)))
                if(((config == 2) && 
                    (Gyro_ADC_CFG2_INPUT_RANGE == Gyro_ADC_IR_VSSA_TO_2VREF) && 
                    ((Gyro_ADC_CFG2_REFERENCE != Gyro_ADC_EXT_REF_ON_P03) && 
                     (Gyro_ADC_CFG2_REFERENCE != Gyro_ADC_EXT_REF_ON_P32))) ||
                     ((config == 3) && 
                      (Gyro_ADC_CFG3_INPUT_RANGE == Gyro_ADC_IR_VSSA_TO_2VREF) && 
                     ((Gyro_ADC_CFG3_REFERENCE != Gyro_ADC_EXT_REF_ON_P03) && 
                     (Gyro_ADC_CFG3_REFERENCE != Gyro_ADC_EXT_REF_ON_P32))) ||
                     ((config == 4) && 
                      (Gyro_ADC_CFG4_INPUT_RANGE == Gyro_ADC_IR_VSSA_TO_2VREF) && 
                     ((Gyro_ADC_CFG4_REFERENCE != Gyro_ADC_EXT_REF_ON_P03) && 
                     (Gyro_ADC_CFG4_REFERENCE != Gyro_ADC_EXT_REF_ON_P32))))
                {
                    /* Disable PRES, Enable power to VCMBUF0, REFBUF0 and 
                       REFBUF1, enable PRES */
                    #if (CY_PSOC3 || CY_PSOC5LP)
                        Gyro_ADC_RESET_CR4_REG |= Gyro_ADC_IGNORE_PRESA1;
                        Gyro_ADC_RESET_CR5_REG |= Gyro_ADC_IGNORE_PRESA2;
                    #elif (CY_PSOC5A)
                        Gyro_ADC_RESET_CR1_REG |= Gyro_ADC_DIS_PRES1;
                        Gyro_ADC_RESET_CR3_REG |= Gyro_ADC_DIS_PRES2;
                    #endif /* CY_PSOC5A */
        
                    /* Enable the REFBUF1 */
                    Gyro_ADC_DSM_REF0_REG |= Gyro_ADC_DSM_EN_BUF_VREF_INN;
                    
                    /* Wait for 3 microseconds */
                    CyDelayUs(3);
                    /* Enable the press circuit */
                    #if (CY_PSOC3 || CY_PSOC5LP)
                        Gyro_ADC_RESET_CR4_REG &= ~Gyro_ADC_IGNORE_PRESA1;
                        Gyro_ADC_RESET_CR5_REG &= ~Gyro_ADC_IGNORE_PRESA2;
                    #elif (CY_PSOC5A)
                        Gyro_ADC_RESET_CR1_REG &= ~Gyro_ADC_DIS_PRES1;
                        Gyro_ADC_RESET_CR3_REG &= ~Gyro_ADC_DIS_PRES2;
                    #endif /* CY_PSOC5A */
                }
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
        
            /* Restart the ADC conversion */
            Gyro_ADC_StartConvert();
        }
    }
}     


/*******************************************************************************
* Function Name: Gyro_ADC_GainCompensation(uint8, uint16, uint16, uint8)
********************************************************************************
*
* Summary:
*  This API calculates the trim value and then loads this to GCOR register.
*
* Parameters:  
*  inputRange:  input range for which trim value is to be calculated.
*  IdealDecGain:  Ideal Decimator gain for the selected resolution and 
*                 conversion  mode.
*  IdealOddDecGain:  Ideal odd decimation gain for the selected resolution and 
                     conversion mode.
*  resolution:  Resolution to select the proper flash location for trim value.
*
* Return: 
*  void
*
*******************************************************************************/
void Gyro_ADC_GainCompensation(uint8 inputRange, uint16 IdealDecGain, uint16 IdealOddDecGain,  \
                                       uint8 resolution) 
{
    int8 flash;
    int16 Normalised;
    uint16 GcorValue;
    uint32 GcorTmp;
    
    switch(inputRange)
    {         
        case Gyro_ADC_IR_VNEG_VREF_DIFF:
        
        /* Normalize the flash Value */
        if(resolution > 15)
        {
            flash = Gyro_ADC_DEC_TRIM_VREF_DIFF_16_20; 
        }    
        else
        {
            flash = Gyro_ADC_DEC_TRIM_VREF_DIFF_8_15;
        }        
        break;
        
        case Gyro_ADC_IR_VNEG_VREF_2_DIFF:
        
        /* Normalize the flash Value */
        if(resolution > 15)
        {
            flash = Gyro_ADC_DEC_TRIM_VREF_2_DIFF_16_20;
        }    
        else
        {
            flash = Gyro_ADC_DEC_TRIM_VREF_2_DIFF_8_15;
        }    
        break;
        
        case Gyro_ADC_IR_VNEG_VREF_4_DIFF:
        
        /* Normalize the flash Value */
        if(resolution > 15)
        {
            flash = Gyro_ADC_DEC_TRIM_VREF_4_DIFF_16_20;
        }    
        else
        {
            flash = Gyro_ADC_DEC_TRIM_VREF_4_DIFF_8_15;
        }    
        break;
        
        case Gyro_ADC_IR_VNEG_VREF_16_DIFF:
        
        /* Normalize the flash Value */
        if(resolution > 15)
        {
            flash = Gyro_ADC_DEC_TRIM_VREF_16_DIFF_16_20;
        }    
        else
        {
            flash = Gyro_ADC_DEC_TRIM_VREF_16_DIFF_8_15;
        }    
        break;
        
        default:
            flash = 0;
        break;
    }    
    if(inputRange == Gyro_ADC_IR_VSSA_TO_VREF || inputRange == Gyro_ADC_IR_VSSA_TO_2VREF || 
       inputRange == Gyro_ADC_IR_VSSA_TO_VDDA || inputRange == Gyro_ADC_IR_VSSA_TO_6VREF || 
       inputRange == Gyro_ADC_IR_VNEG_2VREF_DIFF || inputRange == Gyro_ADC_IR_VNEG_6VREF_DIFF ||
       inputRange == Gyro_ADC_IR_VNEG_VREF_8_DIFF)
    {
        Normalised  = 0;
    }
    else
    {
        Normalised  = ((int16)flash) * 32;
    }
                    
    /* Add two values */
    GcorValue = IdealDecGain + Normalised;  
    GcorTmp = (uint32)GcorValue * (uint32)IdealOddDecGain;
    GcorValue = (uint16)(GcorTmp / Gyro_ADC_IDEAL_GAIN_CONST);
        
    if (resolution < 14)
    {
        GcorValue = (GcorValue >> (15 - (resolution + 1)));
        Gyro_ADC_DEC_GVAL_REG = (resolution + 1);
    }
    else
    {
        /* Use all 16 bits */
        Gyro_ADC_DEC_GVAL_REG = 15u;  
    }
      
    /* Load the gain correction register */    
    Gyro_ADC_DEC_GCOR_REG  = (uint8)GcorValue;
    Gyro_ADC_DEC_GCORH_REG = (uint8)(GcorValue >> 8);    
    
    /* Workaround for 0 to 2*Vref mode with PSoC5A siliocn */
    #if( CY_PSOC5A) 
        if( inputRange == Gyro_ADC_IR_VSSA_TO_2VREF)
        {
            Gyro_ADC_DEC_GCOR_REG = 0x00u;
            Gyro_ADC_DEC_GCORH_REG = 0x00u;
            Gyro_ADC_DEC_GVAL_REG = 0x00u;
        }
    #endif /* CY_PSOC5A */
}


/******************************************************************************
* Function Name: Gyro_ADC_SetDSMRef0Reg(uint8)
******************************************************************************
*
* Summary:
*  This API sets the DSM_REF0 register. This is written for internal use.
*
* Parameters:  
*  value:  Value to be written to DSM_REF0 register.
*
* Return: 
*  void
*
******************************************************************************/
void Gyro_ADC_SetDSMRef0Reg(uint8 value) 
{
    /* Disable PRES, Enable power to VCMBUF0, REFBUF0 and REFBUF1, enable 
       PRES */
    #if (CY_PSOC3 || CY_PSOC5LP)
        Gyro_ADC_RESET_CR4_REG |= (Gyro_ADC_IGNORE_PRESA1 | Gyro_ADC_IGNORE_PRESD1);
        Gyro_ADC_RESET_CR5_REG |= (Gyro_ADC_IGNORE_PRESA2 | Gyro_ADC_IGNORE_PRESD2);
    #elif (CY_PSOC5A)
        Gyro_ADC_RESET_CR1_REG |= Gyro_ADC_DIS_PRES1;
        Gyro_ADC_RESET_CR3_REG |= Gyro_ADC_DIS_PRES2;
    #endif /* CY_PSOC5A */
        Gyro_ADC_DSM_REF0_REG = value;   
		
	/* Wait for 3 microseconds */
    CyDelayUs(3);
    /* Enable the press circuit */
    #if (CY_PSOC3 || CY_PSOC5LP)
        Gyro_ADC_RESET_CR4_REG &= ~(Gyro_ADC_IGNORE_PRESA1 | Gyro_ADC_IGNORE_PRESD1);
        Gyro_ADC_RESET_CR5_REG &= ~(Gyro_ADC_IGNORE_PRESA2 | Gyro_ADC_IGNORE_PRESD2);
    #elif (CY_PSOC5A)
        Gyro_ADC_RESET_CR1_REG &= ~Gyro_ADC_DIS_PRES1;
        Gyro_ADC_RESET_CR3_REG &= ~Gyro_ADC_DIS_PRES2;
    #endif /* CY_PSOC5A */
}


/* [] END OF FILE */
