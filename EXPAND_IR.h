/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 10/26/15     10.0_DW0b    Initial project make.
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#ifndef EXPAND_IR_H
#define	EXPAND_IR_H

#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "USER.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
extern unsigned short IR_PWM50;
extern unsigned char IR_state;

/******************************************************************************/
/* IR codes                          
 *          
 * This is the NEC codes that we send.
 *                              
/******************************************************************************/
extern const unsigned long Sanyo_Power;
extern const unsigned long Sanyo_Sleep;
extern const unsigned long Sanyo_Menu;
extern const unsigned long Sanyo_Volume_Up;
extern const unsigned long Sanyo_Volume_Down;
extern const unsigned long Sanyo_Channel_Up;
extern const unsigned long Sanyo_Channel_Down;
extern const unsigned long Sanyo_Mute;
extern const unsigned long Sanyo_Back;
extern const unsigned long Sanyo_1;
extern const unsigned long Sanyo_2;
extern const unsigned long Sanyo_3;
extern const unsigned long Sanyo_4;
extern const unsigned long Sanyo_5;
extern const unsigned long Sanyo_6;
extern const unsigned long Sanyo_7;
extern const unsigned long Sanyo_8;
extern const unsigned long Sanyo_9;
extern const unsigned long Sanyo_0;
extern const unsigned long Sanyo_Enter;
extern const unsigned long Sanyo_Source;
extern const unsigned long Sanyo_Rewind;
extern const unsigned long Sanyo_Play;
extern const unsigned long Sanyo_Forward;
extern const unsigned long Sanyo_Record;
extern const unsigned long Sanyo_Pause;
extern const unsigned long Sanyo_Stop;

/*~~~~~~~~~~~~~~ Vizio TV ~~~~~~~~~~~~~~~~~~~~~~~~*/
extern const unsigned long Vizio_Power;
extern const unsigned long Vizio_Sleep;
extern const unsigned long Vizio_Menu;
extern const unsigned long Vizio_Volume_Up;
extern const unsigned long Vizio_Volume_Down;

/*~~~~~~~~~~~~~~ Idylis AC unit ~~~~~~~~~~~~~~~~~~~~~~~~*/
extern const unsigned long Idylis_Power;

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

/******************************************************************************/
/* Macro Functions                                                            */
/******************************************************************************/

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
inline unsigned char IR_LEDUse(unsigned char state);
inline void IR_LEDTest(void);
void IR_Module(unsigned char state);
void InitIR(void);
void IR_SendNECRepeat(void);
void IR_SendNEC(unsigned long* NEC);
void IR_SendNEC_Repeat(unsigned long* NEC);
void IR_SendNEC_Repeat_CMD(void);

#endif	/* EXPAND_IR_H */