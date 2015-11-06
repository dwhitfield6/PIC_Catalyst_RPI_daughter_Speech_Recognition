/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 11/06/15     10.1        Version change only.
 * 10/29/15     10.0_DW0b   Fixed bug that always made the system a SPI Master.
 *                          Added disable for PWM function so it turns off the
 *                            RGB during sleep.
 *                          Added template for expander card to do IR/RF.
 *                          Added IR functionality.
 *                          Added phrase searching for the pi UART parsing.
 *                          Added RF functionality.
 *                          Added raspberry pi integration with sphinx.
 * 10/22/15     10.0_DW0a   Initial project make ported from 
 *                            "Catalyst_RPI_daughter" tag '9.0_DW0a'.
 * 10/02/15     1.0_DW0a    Initial project make.
 *                          Added RGB LED functionality.
 *                          Added internal ADC read functionality.
 *                          Added UART functionality.
 *                          Fixed UART bugs.
 *                          Added UART TX software FIFOs.
 *                          Parse the RDI banner and input the catalyst 
 *                            extention.
 *                          Implement SPI mater and slave mode.
 *                          Added external Flash IC functionality.
 *                          Added RTCC functionality.
 *                          Added RGB LED fade and blink functionality.
 *                          Added RTC alarm capability.
 *                          Added I2C functionality.
 *                          Improved RGB LED functionality.
 *                          Added DMA framework.
 *                          Added Red, Green, Blue PWM channels DMA
 *                            functionality. This eases use of the CPU for
 *                            important things.
 *                          Burn the fault log to external EEPROM so that it
 *                            is harder to lose.
 *                          Added timer to automatically take ADC readings of
 *                            the voltage rails.
 *                          Added flash IC read/write functions.
 *                          Wrote function to check for bad blocks on the
 *                            flash IC.
 *                          Added functions to calculate time differences from
 *                            the RTCC.      
 *                          Added Raspberry pi UART pass-through mode to allow
 *                            the user to debug the raspberry pi. 
 *                          Added DMA buffer copier function which gets used by
 *                            the hardware modules to send data.              
/******************************************************************************/

/******************************************************************************/
/* Contains main function.
 *
/******************************************************************************/


/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>         /* XC32 General Include File */

#include <stdint.h>        
#include <stdbool.h>       
#include <stdio.h>         
#include <string.h>

#include "ADC.h"
#include "CMD.h"
#include "EXPAND_RF.h"
#include "EXPAND_IR.h"
#include "MISC.h"
#include "PWM.h"
#include "RTCC.h"
#include "SPI.h"
#include "SYSTEM.h"
#include "UART.h"
#include "USER.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define LED_TIMEOUT 0xFFFFF

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/  

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

short main (void)
{
    unsigned long i;
    unsigned char index = 0;
    unsigned long LED_Counter = LED_TIMEOUT;
    
    /* Initialize */
    SYS_Watchdog(OFF);
    SYS_ConfigureOscillator();
    Init_App();
    Init_System();        
    
    /* Flash LEDs */
    for (i=3;i<24;i++)
    {
        PWM_SetColor(i/3, NOTHING,NOTHING);
        MSC_DelayUS(100000);      
    }  
    PWM_SetColor(BLUE, NOTHING,NOTHING); // LED is BLUE while linux boots
    UART_RS232_FemalePrintBanner(); // print project banner
       
    /* login to the system after boot */
    CMD_PhraseChecking(ON);
    CMD_PhraseCheckingReset(0);
    CMD_StreamingPhraseSet("raspberrypi login:", 0);
    while(!CMD_StreamingPhraseFound(0));
    CMD_PhraseCheckingClear(0);
    UART_RaspSendString("root\r\n"); // pi
    CMD_StreamingPhraseSet("Password", 0);
    while(!CMD_StreamingPhraseFound(0));
    CMD_PhraseCheckingClear(0);
    UART_RaspSendString("pi\r\n"); // raspberry
    PWM_SetColor(PURPLE, NOTHING,NOTHING); // LED is PURPLE after linux boots
    
    /* start sphinx */
    UART_RaspSendString("cd ..\r\n"); // go to home directory
    UART_RaspSendString("cd /home/pi/Speech_Programs/\r\n"); // go where directory is
    UART_RaspSendString("export LD_LIBRARY_PATH=/usr/local/lib\r\n"); // export library
    UART_RaspSendString("export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig\r\n"); // export configuration
    UART_RaspSendString("pocketsphinx_continuous -hmm /usr/local/share/pocketsphinx/model/en-us/en-us"
            " -lm 5394.lm -dict 5394.dic -samprate 16000/8000/48000 -inmic yes -adcdev sysdefault\r\n");
    
    /* start checking for the speech command breaker "Listening..." */
    CMD_PhraseCheckingReset(0);
    CMD_StreamingPhraseSet(CHECK_PHRASE, 0);
    
    while(1)
    {
        /* search for command */
        if(CMD_StreamingPhraseFound(0))
        {
            if(CMD_Match(RX1_Buffer, &COMMANDS[0], &index))
            {
                UART_RS232_FemaleSendString(COMMANDS[index].command);
                UART_RS232_FemaleSendString(CRLN);
                LED_Counter = 0;
            }
            CMD_PhraseCheckingReset(0);
        }
        
        /* LED timing */
        if(LED_Counter < LED_TIMEOUT)
        {
            if(LED_Counter == 0)
            {
                PWM_SetColor(GREEN, NOTHING,NOTHING);
            }
            LED_Counter++;
        }
        else
        {
            if(LED_Counter == LED_TIMEOUT)
            {
                PWM_SetColor(RED, NOTHING,NOTHING);
                LED_Counter++;
            }
        }
    }
}
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
