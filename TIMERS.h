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
/* Files to Include                                                           */
/******************************************************************************/
#ifndef TIMERS_H
#define	TIMERS_H

#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "USER.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
inline unsigned char TMR_EnableTimer2(unsigned char action);
inline unsigned char TMR_EnableTimer4(unsigned char action);
inline unsigned char TMR_EnableTimer5(unsigned char action);
inline unsigned char TMR_InterruptTimer2(unsigned char action);
inline unsigned char TMR_InterruptTimer4(unsigned char action);
inline unsigned char TMR_InterruptTimer5(unsigned char action);
inline void TMR_ResetTimer2(void);
inline void TMR_ResetTimer4(void);
inline void TMR_ResetTimer5(void);
inline void TMR_SetTimer4(unsigned short time);
inline void TMR_SetTimer5(unsigned short time);
void InitTIMERS(void);
void InitTIMER2(void);
void InitTIMER4(void);
void InitTIMER5(void);

#endif	/* TIMERS_H */