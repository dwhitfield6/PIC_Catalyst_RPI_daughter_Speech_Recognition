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
/* Files to Include                                                           */
/******************************************************************************/
#ifndef EXCEPTIONS_H
#define	EXCEPTIONS_H

#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "USER.h"

/******************************************************************************/
/* Structures                                                                 */
/******************************************************************************/
typedef struct __attribute__((packed)) _fault_
{
    unsigned char LogCreatedString[10];
    unsigned char LogCreated;
    unsigned char RTCC_Oscillator_Fail;
    unsigned char EEPROM_SerialNumber_Read_Fail;
    unsigned char EEPROM_SerialNumber_Write_Fail;
    unsigned char EEPROM_FaultLog_Read_Fail;
    unsigned char EEPROM_FaultLog_Write_Fail;
    unsigned char Exception;
    unsigned short ExceptionCode;
    unsigned short ExceptionAddress;
    unsigned char Flash_ID_FAIL;
    unsigned char Flash_BadBlocksLimit_FAIL;
    unsigned char WatchDog_Reset;
    unsigned char Software_Reset;
    unsigned char Brownout_Reset;
    unsigned char Poweron_Reset;
    unsigned char HighVoltage_Reset;
    unsigned char ConfigMissMatch_Reset;
}FAULTTYPE;

/******************************************************************************/
/* Enumeration                                                               */
/******************************************************************************/
static enum {
	EXCEP_IRQ = 0,			// interrupt
	EXCEP_AdEL = 4,			// address error exception (load or ifetch)
	EXCEP_AdES,				// address error exception (store)
	EXCEP_IBE,				// bus error (ifetch)
	EXCEP_DBE,				// bus error (load/store)
	EXCEP_Sys,				// syscall
	EXCEP_Bp,				// breakpoint
	EXCEP_RI,				// reserved instruction
	EXCEP_CpU,				// coprocessor unusable
	EXCEP_Overflow,			// arithmetic overflow
	EXCEP_Trap,				// trap (possible divide by zero)
	EXCEP_IS1 = 16,			// implementation specfic 1
	EXCEP_CEU,				// CorExtend Unuseable
	EXCEP_C2E				// coprocessor 2
} Exception_Code;

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
extern FAULTTYPE Fault;

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
void InitException(void);
void EXC_ClearFaults(void);
void general_Exception_Handler(void);

#endif	/* EXCEPTIONS_H */