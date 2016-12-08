/*******************************************************************************
* File Name: Gyro_ADC_INT.c  
* Version 2.30
*
* Description:
*  This file contains the code that operates during the ADC_DelSig interrupt 
*  service routine.  
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


/*******************************************************************************
* Custom Declarations and Variables
* - add user inlcude files, prototypes and variables between the following 
*   #START and #END tags
*******************************************************************************/
/* `#START ADC_SYS_VAR`  */

/* `#END`  */


/*****************************************************************************
* Function Name: Gyro_ADC_ISR1
******************************************************************************
*
* Summary:
*  Handle Interrupt Service Routine.  
*
* Parameters:  
*  void
*
* Return: 
*  void 
*
* Reentrant: 
*  No
*
*****************************************************************************/
CY_ISR( Gyro_ADC_ISR1)
{
    /***************************************************************************
    *  Custom Code
    *  - add user ISR code between the following #START and #END tags
    **************************************************************************/
    /* `#START MAIN_ADC_ISR1`  */

    /* `#END`  */
    
    /* Stop the conversion if conversion mode is single sample and resolution
       is above 16 bits. 
    */
    #if(Gyro_ADC_CFG1_RESOLUTION > 16 && \
        Gyro_ADC_CFG1_CONV_MODE == Gyro_ADC_MODE_SINGLE_SAMPLE) 
        Gyro_ADC_StopConvert(); 
    #endif /* Single sample conversion mode with resolution above 16 bits */         
        
}


/*****************************************************************************
* Function Name: Gyro_ADC_ISR2
******************************************************************************
*
* Summary:
*  Handle Interrupt Service Routine.  
*
* Parameters:  
*  void
*
* Return: 
*  void 
*
* Reentrant: 
*  No
*
*****************************************************************************/
CY_ISR( Gyro_ADC_ISR2)
{
    /***************************************************************************
    *  Custom Code
    *  - add user ISR code between the following #START and #END tags
    **************************************************************************/
    /* `#START MAIN_ADC_ISR2`  */

    /* `#END`  */
    
    /* Stop the conversion conversion mode is single sample and resolution
       is above 16 bits.
    */
    #if(Gyro_ADC_CFG2_RESOLUTION > 16 && \
        Gyro_ADC_CFG2_CONVMODE == Gyro_ADC_MODE_SINGLE_SAMPLE) 
        Gyro_ADC_StopConvert(); 
    #endif /* Single sample conversion mode with resolution above 16 bits */
    
}


/*****************************************************************************
* Function Name: Gyro_ADC_ISR3
******************************************************************************
*
* Summary:
*  Handle Interrupt Service Routine.  
*
* Parameters:  
*  void
*
* Return: 
*  void 
*
* Reentrant: 
*  No
*
*****************************************************************************/
CY_ISR( Gyro_ADC_ISR3)
{
    /***************************************************************************
    *  Custom Code
    *  - add user ISR code between the following #START and #END tags
    **************************************************************************/
    /* `#START MAIN_ADC_ISR3`  */

    /* `#END`  */

    /* Stop the conversion if conversion mode is set to single sample and 
       resolution is above 16 bits. 
    */
    #if(Gyro_ADC_CFG3_RESOLUTION > 16 && \
        Gyro_ADC_CFG3_CONVMODE == Gyro_ADC_MODE_SINGLE_SAMPLE) 
        Gyro_ADC_StopConvert(); 
    #endif /* Single sample conversion mode with resolution above 16 bits */  
}


/*****************************************************************************
* Function Name: Gyro_ADC_ISR4
******************************************************************************
*
* Summary:
*  Handle Interrupt Service Routine.  
*
* Parameters:  
*  void
*
* Return: 
*  void 
*
* Reentrant: 
*  No
*
*****************************************************************************/
CY_ISR( Gyro_ADC_ISR4)
{
    /***************************************************************************
    *  Custom Code
    *  - add user ISR code between the following #START and #END tags
    **************************************************************************/
    /* `#START MAIN_ADC_ISR4`  */

    /* `#END`  */
    
    /* Stop the conversion if conversion mode is set to single sample and 
       resolution is above 16 bits. 
    */
    #if(Gyro_ADC_CFG4_RESOLUTION > 16 && \
        Gyro_ADC_CFG4_CONVMODE == Gyro_ADC_MODE_SINGLE_SAMPLE) 
        Gyro_ADC_StopConvert(); 
    #endif /* Single sample conversion mode with resolution above 16 bits */
}


/* [] END OF FILE */
