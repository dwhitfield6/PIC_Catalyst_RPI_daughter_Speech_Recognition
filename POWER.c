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
/* Contains functions for the power scheme including the supercap.
 *
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "MISC.h"
#include "USER.h"
#include "POWER.h"
#include "ADC.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
RAILSTYPE RailStatus;

/******************************************************************************/
/* Inline Functions                                                           */
/******************************************************************************/

/******************************************************************************/
/* PWR_Charge
 *
 * Controls the charging of the SuperCap.
/******************************************************************************/
inline void PWR_Charge(unsigned char action)
{
    if (action)
    {
        LATD |= ChargeCap;
        RailStatus.Supercap_state = CHARGING;
    }
    else
    {
        LATD &= ~ChargeCap;
        RailStatus.Supercap_state = DISCHARGING;
    }
}

/******************************************************************************/
/* PWR_ExternalInterrupt
 *
 * This function enables/disables an external interrupt.
/******************************************************************************/
inline unsigned char PWR_ExternalInterrupt(unsigned char channel, unsigned char action)
{
    unsigned char status;
    if(channel == 0)
    {
        status = IEC0bits.INT0IE;
        if(action)
        {
            IEC0bits.INT0IE = 1; // enable external interrupt
        }
        else
        {
            IEC0bits.INT0IE = 0; // disable external interrupt
        }
    }
    else if(channel == 1)
    {
        status = IEC0bits.INT1IE;
        if(action)
        {
            IEC0bits.INT1IE = 1; // enable external interrupt
        }
        else
        {
            IEC0bits.INT1IE = 0; // disable external interrupt
        }
    }
    else if(channel == 2)
    {
        status = IEC0bits.INT2IE;
        if(action)
        {
            IEC0bits.INT2IE = 1; // enable external interrupt
        }
        else
        {
            IEC0bits.INT2IE = 0; // disable external interrupt
        }
    }
    else if(channel == 3)
    {
        status = IEC0bits.INT3IE;
        if(action)
        {
            IEC0bits.INT3IE = 1; // enable external interrupt
        }
        else
        {
            IEC0bits.INT3IE = 0; // disable external interrupt
        }
    }
    else
    {
        status = IEC0bits.INT4IE;
        if(action)
        {
            IEC0bits.INT4IE = 1; // enable external interrupt
        }
        else
        {
            IEC0bits.INT4IE = 0; // disable external interrupt
        }
    }
    return status;
}

/******************************************************************************/
/* PWR_ExternalInterruptEdge
 *
 * This function sets the interrupt for either a rising or falling edge.
/******************************************************************************/
inline unsigned char PWR_ExternalInterruptEdge(unsigned char channel, unsigned char edge)
{
    unsigned char status;
    if(channel == 0)
    {
        status = INTCONbits.INT0EP;
        INTCONbits.INT0EP = edge;
    }
    else if(channel == 1)
    {
        status = INTCONbits.INT1EP;
        INTCONbits.INT1EP = edge;
    }
    else if(channel == 2)
    {
        status = INTCONbits.INT2EP;
        INTCONbits.INT2EP = edge;
    }
    else if(channel == 3)
    {
        status = INTCONbits.INT3EP;
        INTCONbits.INT3EP = edge;
    }
    else
    {
        status = INTCONbits.INT4EP;
        INTCONbits.INT4EP = edge;
    }
    return status;
}

/******************************************************************************/
/* PWR_Volts5
 *
 * Controls the 5 volt DCDC converter.
/******************************************************************************/
inline void PWR_Volts5(unsigned char action)
{
    if (action)
    {       
        LATD &= ~Inhibit;
        RailStatus.DCDC_state = ON;
    }
    else
    {
        LATD |= Inhibit;
        RailStatus.DCDC_state = OFF;
    }
}

/******************************************************************************/
/* PWR_RASP
 *
 * Controls the 5 volt rail to the raspberry pi.
/******************************************************************************/
inline void PWR_RASP(unsigned char action)
{
    if (action)
    {       
        LATA |= RASP_ON;
        RailStatus.Rasp_state = ON;
    }
    else
    {
        LATA &= ~RASP_ON;
        RailStatus.Rasp_state = OFF;
    }
}

/******************************************************************************/
/* PWR_RASP_PowerStatus
 *
 * Reads the state of the raspberry pi voltage rail.
/******************************************************************************/
inline unsigned char PWR_RASP_PowerStatus(void)
{
    if (PORTA & RASP_ON2)
    {       
        RailStatus.Rasp_state = ON;
        return ON;
    }
    RailStatus.Rasp_state = OFF;
    return OFF;
}

/******************************************************************************/
/* PWR_RASP_Connected
 *
 * Detects if a raspberry pi is present or not.
/******************************************************************************/
inline unsigned char PWR_RASP_Connected(void)
{
    if (PORTG & RASP_Connected)
    {       
        RailStatus.Rasp_connected = FALSE;
        return OFF;
    }
    RailStatus.Rasp_connected = TRUE;
    return ON;
}

/******************************************************************************/
/* PWR_RASP_SPIReady
 *
 * Detects if a raspberry pi is present and if the SPI module is ready.
/******************************************************************************/
inline unsigned char PWR_RASP_SPIReady(void)
{
    if(PWR_RASP_Connected())
    {
        if (PORTB & RASP_SPI_Configured)
        {       
            return PASS;
        }
    }
    return FAIL;
}

/******************************************************************************/
/* Functions
/******************************************************************************/

/******************************************************************************/
/* InitLowPower
 *
 * The function turn off any unused modules to save power.
/******************************************************************************/
void InitLowPower(void)
{
    SYS_SystemUnlock();
    CFGCONbits.PMDLOCK = 0; // allow PMD module to be configured
    Nop();
    Nop();
    PMD1bits.CTMUMD = ON;   // disable the CTMU module
    PMD1bits.CVRMD = ON;    // disable the Comparator Voltage Reference
    PMD2bits.CMP1MD = ON;   // disable the Comparator 1
    PMD2bits.CMP2MD = ON;   // disable the Comparator 2
    PMD5bits.SPI1MD = ON;   // disable SPI module 1
    PMD6bits.PMPMD = ON;    // disable PMP module
    PMD6bits.REFOMD = ON;   // disable Reference Clock Output
    CFGCONbits.PMDLOCK = 1; // do not allow PMD module to be configured
    SYS_SystemLock();
}

/******************************************************************************/
/* InitI2C
 *
 * The function initializes the external comparator for monitoring voltage
 *  rail extremes.
/******************************************************************************/
void InitComparator(void)
{
    /* Set remappable inputs */
    INT1R = COMP_VCAP_Warn3_Pin;        // INT1
    INT2R = COMP_VIN_Warn_Pin;          // INT2
    INT3R = COMP_VCAP_Warn2_Pin;        // INT3
    
    PWR_ExternalInterruptEdge(COMP_VIN_Warn_INT,RISING);
    PWR_ExternalInterruptEdge(COMP_VCAP_Warn1_INT,RISING);
    PWR_ExternalInterruptEdge(COMP_VCAP_Warn2_INT,RISING);
    PWR_ExternalInterruptEdge(COMP_VCAP_Warn3_INT,FALLING);
    PWR_ReadComparator();
    PWR_ExternalInterrupt(COMP_VIN_Warn_INT,ON);
    PWR_ExternalInterrupt(COMP_VCAP_Warn1_INT,ON);
    PWR_ExternalInterrupt(COMP_VCAP_Warn3_INT,ON);
}
    
/******************************************************************************/
/* PWR_StatusUpdate
 *
 * The function reads the compared voltages from the comparator.
/******************************************************************************/
void PWR_ReadComparator(void)
{
    if(PORTB & COMP_VIN_Warn)
    {
        /* VIN is less than 9 volts */
        RailStatus.WarnVIN = 1;
    }
    else
    {
        /* VIN is greater than 9 volts */
        RailStatus.WarnVIN = 0;
    }
    
    if(PORTF & COMP_VCAP_Warn1)
    {
        /* Supercap is less than 4.1 volts */
        RailStatus.Warn4_1 = 1;
    }
    else
    {
        /* Supercap is greater than 4.1 volts */
        RailStatus.Warn4_1 = 0;
    }
    
    if(PORTD & COMP_VCAP_Warn2)
    {
        /* Supercap is less than 4.4 volts */
        RailStatus.Warn4_4 = 1;
    }
    else
    {
        /* Supercap is greater than 4.4 volts */
        RailStatus.Warn4_4 = 0;
    }
    
    if(PORTF & COMP_VCAP_Warn3)
    {
        /* Supercap is less than the potentiometer voltage */
        RailStatus.WarnPOT = 1;
    }
    else
    {
        /* Supercap is greater than the potentiometer voltage */
        RailStatus.WarnPOT = 0;
    }
}

/******************************************************************************/
/* PWR_StatusUpdate
 *
 * The function updates the status of the raspberry pi.
/******************************************************************************/
void PWR_StatusUpdate(void)
{
    PWR_RASP_PowerStatus();
    PWR_RASP_Connected();
    PWR_ReadComparator();
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/