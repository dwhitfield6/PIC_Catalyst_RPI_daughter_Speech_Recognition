/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
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

#include "SYSTEM.h"
#include "USER.h"
#include "MISC.h"
#include "ADC.h"
#include "SPI.h"
#include "RTCC.h"
#include "PWM.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
unsigned char data[] = "David is sending this over SPI and is an SPI slave";  
    
/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

short main (void)
{
    unsigned long i;
 
    /* Initialize */
    SYS_ConfigureOscillator();
    Init_App();
    Init_System();
    PWR_StatusUpdate();        
    RDI_GetProduct();
    
    /* Flash LEDs */
    for (i=0;i<24;i++)
    {
        PWM_SetColor(i/3, NOTHING,NOTHING);
        MSC_DelayUS(100000);      
    }
    
    /* Read the current alarm */
    RTCC_ReadAlarm(&CurrentAlarm);
    
    /* set the Red LED to fade up */
    PWM_SetColor(RED, FADEUP,PWM_MEDIUM);
    
    /* enable the watchdog */
    SYS_Watchdog(ON);
        
    while(1)
    {
        SYS_PetFluffyPuppy(); // pet the watchdog
        RDI_SendToRaspberry(data,MSC_SizeOfString(data)); // send data over SPI if ready
    }
}
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
