/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 08/21/15     1.0_DW0a    Initial project make.
/******************************************************************************/

/******************************************************************************/
/* Contains functions for Analog to Digital converter.
 *
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "USER.h"
#include "ADC.h"
#include "POWER.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
unsigned char CurrentChannel = ADC_VIN_AN;
volatile unsigned char ADC_ReadRailStatus = FALSE;

/******************************************************************************/
/* Inline Functions
/******************************************************************************/

/******************************************************************************/
/* ADC_Interrupt
 *
 * The function controls the ADC interrupt.
/******************************************************************************/
inline unsigned char ADC_Interrupt(unsigned char state)
{
    unsigned char status = IEC0bits.AD1IE;
    if(state)
    {       
        IEC0bits.AD1IE = 1; // enable ADC interrupt
    }
    else
    {
        IEC0bits.AD1IE = 0; // disable ADC interrupt 
    }
    return status;
}

/******************************************************************************/
/* ADC_Module
 *
 * The function controls the ADC module.
/******************************************************************************/
inline unsigned char ADC_Module(unsigned char state)
{
#ifdef ON
    #undef ON
    unsigned char status = AD1CON1bits.ON;
    if(state)
    {       
        AD1CON1bits.ON = 1; // turn on the ADC module
    }
    else
    {
        AD1CON1bits.ON = 0; // turn off the ADC module 
    }
    #define ON 1
#else
    unsigned char status = AD1CON1bits.ON;
    if(state)
    {       
        AD1CON1bits.ON = 1; // turn on the ADC module
    }
    else
    {
        AD1CON1bits.ON = 0; // turn off the ADC module 
    }
#endif
    return status;
}

/******************************************************************************/
/* Functions
/******************************************************************************/

/******************************************************************************/
/* InitADC
 *
 * The function initializes the analog to digital converter.
/******************************************************************************/
void InitADC(void)
{
    ADC_Module(OFF);
    AD1CON1bits.ADSIDL = 0; // Continue module operation in Idle mode
    AD1CON2bits.VCFG = 0x000; // reference voltage is AVDD and AVSS
    AD1CON3bits.ADCS = 0b00111111; // 64 * TCY ~1.6uS
    AD1CON2bits.SMPI = 0b0000; // Interrupts at the completion of conversion for each sample/convert sequence
    IPC5bits.AD1IP = 1; // interrupt priority is 1
    IPC5bits.AD1IS = 1; // interrupt sub-priority is 1
    ADC_Interrupt(OFF);
    ADC_Module(ON);
}

/******************************************************************************/
/* ADC_SetSample
 *
 * The function sets the ADC channel and starts the sampling.
/******************************************************************************/
void ADC_SetSample(unsigned char channel)
{
    AD1CHSbits.CH0SB = channel;
    AD1CHSbits.CH0SA = channel;
    AD1CON1bits.SAMP = TRUE; // sample the input
}

/******************************************************************************/
/* ADC_StopSample
 *
 * The function stops the ADC sampling and starts a conversion.
/******************************************************************************/
void ADC_StopSample(void)
{
    AD1CON1bits.SAMP = FALSE; // sample the input
    AD1CON1bits.DONE = 0;
}

/******************************************************************************/
/* ADC_ReadRails
 *
 * The function reads the 5 voltage rails.
/******************************************************************************/
void ADC_ReadRails(void)
{
    unsigned long timer =0;
    TMR_EnableTimer5(OFF);
    TMR_ResetTimer5();
    CurrentChannel = ADC_VIN_AN;
    ADC_ReadRailStatus = FALSE;
    TMR_EnableTimer5(ON);
    while(!ADC_ReadRailStatus)
    {
        timer++;
        if(timer > 100000)
        {
            /* Rail reading timeout occurred */
        }
    }
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/