/*******************************************************************************
* File Name: Gyro_ADC_PM.c  
* Version 2.30
*
* Description:
*  This file provides the power manager source code to the API for the 
*  Delta-Sigma ADC Component.
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

static Gyro_ADC_BACKUP_STRUCT Gyro_ADC_backup = 
{
    Gyro_ADC_DISABLED,
    Gyro_ADC_CFG1_DEC_CR
};


/*******************************************************************************  
* Function Name: Gyro_ADC_SaveConfig
********************************************************************************
*
* Summary:
*  Save the register configuration which are not retention.
* 
* Parameters:
*  void
* 
* Return:
*  void
*
* Global variables:
*  Gyro_ADC_backup:  This global structure variable is used to store 
*  configuration registers which are non retention whenever user wants to go 
*  sleep mode by calling Sleep() API.
*
*******************************************************************************/
void Gyro_ADC_SaveConfig(void) 
{
    Gyro_ADC_backup.deccr = Gyro_ADC_DEC_CR_REG;
}


/*******************************************************************************  
* Function Name: Gyro_ADC_RestoreConfig
********************************************************************************
*
* Summary:
*  Restore the register configurations which are not retention.
* 
* Parameters:
*  void
* 
* Return:
*  void
*
* Global variables:
*  Gyro_ADC_backup:  This global structure variable is used to restore 
*  configuration registers which are non retention whenever user wants to switch 
*  to active power mode by calling Wakeup() API.
*
*******************************************************************************/
void Gyro_ADC_RestoreConfig(void) 
{
    Gyro_ADC_DEC_CR_REG = Gyro_ADC_backup.deccr;
}


/******************************************************************************* 
* Function Name: Gyro_ADC_Sleep
********************************************************************************
*
* Summary:
*  Stops the operation of the block and saves the user configuration. 
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Gyro_ADC_backup:  The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void Gyro_ADC_Sleep(void) 
{
    /* Save ADC enable state */
    if((Gyro_ADC_ACT_PWR_DEC_EN == (Gyro_ADC_PWRMGR_DEC_REG & Gyro_ADC_ACT_PWR_DEC_EN)) &&
       (Gyro_ADC_ACT_PWR_DSM_EN == (Gyro_ADC_PWRMGR_DSM_REG & Gyro_ADC_ACT_PWR_DSM_EN)))
    {
        /* Component is enabled */
        Gyro_ADC_backup.enableState = Gyro_ADC_ENABLED;
    }
    else
    {
        /* Component is disabled */
        Gyro_ADC_backup.enableState = Gyro_ADC_DISABLED;
    }

    /* Stop the configuration */
    Gyro_ADC_Stop();

    /* Save the user configuration */
    Gyro_ADC_SaveConfig();
}


/******************************************************************************* 
* Function Name: Gyro_ADC_Wakeup
********************************************************************************
*
* Summary:
*  Restores the user configuration and enables the power to the block.
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Gyro_ADC_backup:  The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void Gyro_ADC_Wakeup(void) 
{
    /* Restore the configuration */
    Gyro_ADC_RestoreConfig();

    /* Enable's the component operation */
    if(Gyro_ADC_backup.enableState == Gyro_ADC_ENABLED)
    {
        Gyro_ADC_Enable();

        /* Start the conversion only if conversion is not triggered by the hardware */
        if(!(Gyro_ADC_DEC_CR_REG & Gyro_ADC_DEC_XSTART_EN))
        {
            Gyro_ADC_StartConvert();
        }

    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */
