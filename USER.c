/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 07/28/15     1.0_DW0a    Initial project make.
/******************************************************************************/

/******************************************************************************/
/* Contains Functions for PIC initialization
 *
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include <stdio.h>         /* For sprintf definition */

#include "ADC.h"
#include "CMD.h"
#include "DMA.h"
#include "EXCEPTIONS.h"
#include "FLASH.h"
#include "I2C.h"
#include "MISC.h"
#include "POWER.h"
#include "PWM.h"
#include "RTCC.h"
#include "SPI.h"
#include "SYSTEM.h"
#include "TIMERS.h"
#include "UART.h"
#include "USER.h"


/******************************************************************************/
/* Version variables                                                          */
/******************************************************************************/
const unsigned char Version[]        = CPU_VERSION;
const unsigned char Revision[]       = CPU_REVISION;
const unsigned char Alpha[]          = CPU_ALPHA;
const unsigned char Branch_Version[] = CPU_BRANCH;
const unsigned char PIC_Board_Serial_Number[] = BOARD_SERIAL;

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/******************************************************************************/
/* Inline Functions
/******************************************************************************/

/******************************************************************************/
/* Functions
/******************************************************************************/

/******************************************************************************/
/* Init_App
 *
 * The function initializes the application. It sets the pins and pull-ups.
/******************************************************************************/
void Init_App(void)
{
    LATA = 0;
    LATB = 0;
    LATC = 0;
    LATD = 0;
    LATE = 0;
    LATF = 0;
    LATG = 0;
    
    /* Set all pins as digital */
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;
    ANSELF = 0;
    ANSELG = 0;
    
    /* initialize pins */
    PWR_Volts5(ON);     // turn on the 5 volt rail
    PWR_Charge(TRUE);   // charge the capacitor
    PWR_RASP(OFF);       // power off the raspberry pi
    
    /*~~~~~~~~~~~~~ Red LED ~~~~~~~~~~~~~~~~~*/
    MSC_RedLEDOFF();
    RedLEDTris          = OUTPUT;
    
    /*~~~~~~~~~~~~~ RGB LED ~~~~~~~~~~~~~~~~~*/
    MSC_RGBRedLEDOFF();
    MSC_RGBGreenLEDOFF();
    MSC_RGBBlueLEDOFF();
    RGB_RedLEDTris      = OUTPUT;
    RGB_GreenLEDTris    = OUTPUT;
    RGB_BlueLEDTris     = OUTPUT;
    
    /*~~~~~~~~~~~~~ SuperCap ~~~~~~~~~~~~~~~~~*/
    ChargeCapTris       = OUTPUT;
    
    /*~~~~~~~~~~~~~ DCDC ~~~~~~~~~~~~~~~~~*/
    InhibitTris         = OUTPUT;
    
    /*~~~~~~~~~~~~~ Raspberry pi ~~~~~~~~~~~~~~~~~*/
    /* Raspberry pi rail */
    RASP_ONTris         = OUTPUT;
    RASP_ON2Tris        = INPUT;
    
    /* Raspberry pi connected detector */
    RASP_ConnectedTris  = INPUT;
    PWR_RASP_Connected();
    
    /* Raspberry pi UART */
    RASP_UART_RXTris    = INPUT;
    RASP_UART_TX2Tris   = INPUT;
    RASP_UART_TXTris    = OUTPUT;
    
    /* Raspberry pi SPI */
    RASP_SPI_MOSITris   = INPUT;
#ifndef Catalyst_Expander_IR_RF_revA
    RASP_SPI_MOSI2Tris  = INPUT;
    RASP_SPI_MOSI2_1Tris    = INPUT;
    RASP_SPI_MOSI2_2Tris    = INPUT;
#endif
    RASP_SPI_MISOTris   = INPUT;
    RASP_SPI_CLKTris    = INPUT;
    RASP_SPI_CSTris     = INPUT;
    RASP_SPI_CS2Tris    = INPUT;
    RASP_SPI_CS3Tris    = INPUT;
    RASP_SPI_ConfiguredTris = INPUT;
    RASP_Slave_Need_ClockingTris = INPUT;
    RASP_GPIO5Tris      = INPUT;
            
            
    /*~~~~~~~~~~~~~ Voltages rails for ADC ~~~~~~~~~~~~~~~~~*/
    ADC_Volt5_0Tris     = INPUT;
    ADC_Volt3_3Tris     = INPUT;
    ADC_Volt4_1Tris     = INPUT;
    ADC_VINTris         = INPUT;
    ADC_VCAPTris        = INPUT;
    ADC_VREF_posTris    = INPUT;
    ADC_VREF_negTris    = INPUT;
    ADC_POTTris         = INPUT;
    
    /* select the ADC channels and references as analog */
    ANSELB              |= (ADC_Volt5_0 + ADC_Volt3_3 + ADC_Volt4_1 + ADC_VIN + ADC_VCAP);
    ANSELA              |= (ADC_VREF_pos + ADC_VREF_neg);
    
    /*~~~~~~~~~~~~~ R-232 DB9 male and female ~~~~~~~~~~~~~~~~~*/
    /* RS-232 male */
    RS232_MALE_TXTris       = OUTPUT;
    RS232_MALE_RXTris       = INPUT;
    RS232_MALE_RTSTris      = OUTPUT;
    RS232_MALE_CTSTris      = INPUT;
    
    /* RS-232 female */
    RS232_FEMALE_TXTris     = OUTPUT;
    RS232_FEMALE_TX2Tris    = INPUT;
    RS232_FEMALE_RXTris     = INPUT;
    RS232_FEMALE_RTSTris    = OUTPUT;
    RS232_FEMALE_CTSTris    = INPUT;
    RS232_FEMALE_CTS2Tris   = INPUT;
    
    RS232_MALE_SHUTDOWNTris     = OUTPUT;
    RS232_FEMALE_SHUTDOWNTris   = OUTPUT;
    RS232_MALE_ENABLETris       = OUTPUT;
    RS232_FEMALE_ENABLETris     = OUTPUT;
    
    /*~~~~~~~~~~~~~ Relay ~~~~~~~~~~~~~~~~~*/
    RelayTris           = OUTPUT;
    Relay2Tris          = INPUT;
    
    /*~~~~~~~~~~~~~ External Flash IC ~~~~~~~~~~~~~~~~~*/
    FLH_EXT_ChipSelect(OFF);
    ExtFlash_IO_0Tris   = INPUT;
    ExtFlash_IO_1Tris   = INPUT;
    ExtFlash_IO_2Tris   = INPUT;
    ExtFlash_IO_3Tris   = INPUT;
    ExtFlash_IO_4Tris   = INPUT;
    ExtFlash_IO_5Tris   = INPUT;
    ExtFlash_IO_6Tris   = INPUT;
    ExtFlash_IO_7Tris   = INPUT;
    ExtFlash_RBTris     = INPUT;
    ExtFlash_RB2Tris    = INPUT;
    ExtFlash_RETris     = OUTPUT;
    ExtFlash_CETris     = OUTPUT;
    ExtFlash_CLETris    = OUTPUT;
    ExtFlash_ALETris    = OUTPUT;
    ExtFlash_WETris     = OUTPUT;
    ExtFlash_WPTris     = OUTPUT;
    
    /*~~~~~~~~~~~~~ Secondary Oscillator for RTCC ~~~~~~~~~~~~~~~~~*/
    SOSCOTris           = INPUT;
    SOSCITris           = INPUT;
    
    /*~~~~~~~~~~~~~ I2C busses ~~~~~~~~~~~~~~~~~*/
    I2C_RASP_DRV_ClockTris  = INPUT;
    I2C_RASP_DRV_DataTris   = INPUT;
    I2C_RASP_GEN_ClockTris  = INPUT;
    I2C_RASP_GEN_DataTris   = INPUT;
    I2C_RASP_GEN_Data1Tris  = INPUT;
    
    /*~~~~~~~~~~~~~ Comparator ~~~~~~~~~~~~~~~~~*/
    COMP_VIN_WarnTris   = INPUT;
    COMP_VCAP_Warn1Tris = INPUT;
    COMP_VCAP_Warn2Tris = INPUT;
    COMP_VCAP_Warn3Tris = INPUT;
    
#ifdef Catalyst_Expander_IR_RF_revA
    /*~~~~~~~~~~~~~ IR/RF expansion header ~~~~~~~~~~~~~~~~~*/
    IR_LEDUse(OFF);
    IR_LED_Tris         = INPUT;
    IR_LED1_Tris        = OUTPUT;
    IR_LED2_Tris        = INPUT;
    RF_TRANSMIT_Tris    = INPUT;
#endif

    /*~~~~~~~~~~~~~ Optional High Power LDO ~~~~~~~~~~~~~~~~~*/
    RTCC_OUT_Tris       = OUTPUT;
    
    /*~~~~~~~~~~~~~ RTCC ~~~~~~~~~~~~~~~~~*/
    RTCC_OUT_Tris       = OUTPUT;
    RTCC_INT_Tris       = INPUT;
    
    /*~~~~~~~~~~~~~ Debug UART ~~~~~~~~~~~~~~~~~*/
    DEBUG_UART_TXTris   = OUTPUT;
    DEBUG_UART_RXTris   = INPUT;
}

/******************************************************************************/
/* Init_System
 *
 * The function initializes the modules.
/******************************************************************************/
void Init_System(void)
{
    INTCONbits.MVEC = TRUE; // Multi-vectored interrupts
    if(PMD6bits.REFOMD != ON)
    {
        /* put in low power mode if not already */
        InitLowPower();
    }
    InitTIMERS();
    InitPWM();
    InitUART();
    InitException();
    InitDMA();
    InitCMD();
#ifdef Catalyst_Expander_IR_RF_revA
    InitIR();
    InitRF();
#endif
    SYS_Interrupts(ON);
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/