/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 08/21/15     1.0_DW0a    Initial project make.
 *                          Derived from project 'PIC_PS2_to_UART'.
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#ifndef SYSTEM_H
#define	SYSTEM_H

#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "USER.h"

/******************************************************************************/
/* System frequency
 *
 * This is the CPU clock frequency.
 *
 * For this system the clock is 96MHz and the internal PLL is used.
 *
/******************************************************************************/
#define FOSC        96000000L

/******************************************************************************/
/* Reset reason                                                               */
/******************************************************************************/
#define RESET_HVDR    0x01
#define RESET_CMR     0x02
#define RESET_VREGS   0x04
#define RESET_EXTR    0x08
#define RESET_SWR     0x10
#define RESET_WDTO    0x20
#define RESET_BOR     0x40
#define RESET_POR     0x80

/******************************************************************************/
/* Wake-up scenario                                                           */
/******************************************************************************/
#define WAKE_ACTIVE 0
#define WAKE_SLEEP  1
#define WAKE_IDLE   2

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define SYSCLK      FOSC
#define PBCLK       SYSCLK/2

/******************************************************************************/
/* Macro Functions                                                            */
/******************************************************************************/
#define RESET() SYS_SoftwareReset();
#define SLEEP() SYS_Sleep();
#define IDLE() SYS_Idle();

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
void SYS_ConfigureOscillator(void);
void SYS_SystemUnlock(void);
void SYS_SystemLock(void);
void SYS_Interrupts(unsigned char state);
void SYS_Sleep(void);
void SYS_Idle(void);
void Init_Watchdog(void);
void SYS_SoftwareReset(void);
unsigned char SYS_Watchdog(unsigned char state);
void SYS_PetFluffyPuppy(void);
void SYS_PetWatchDog(void);
unsigned char SYS_CheckWatchdogReset(void);
unsigned char SYS_WakeEvent(unsigned char* event);
unsigned char SYS_CheckReset(void);

#endif	/* SYSTEM_H */