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
#ifndef EXPAND_RF_H
#define	EXPAND_RF_H

#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "USER.h"

/******************************************************************************/
/* RF_TRANSMIT_REPEAT
 *
 * This is the number of times that we repeat a send of the RF code.
/******************************************************************************/
#define RF_TRANSMIT_REPEAT 10

/******************************************************************************/
/* RF_CODE_SIZE
 *
 * This is the maximum RF code timing size.
/******************************************************************************/
#define RF_CODE_SIZE 100

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
extern const long LivingroomLight[RF_CODE_SIZE];
extern const long LivingroomFan[RF_CODE_SIZE];
extern const long ChristmasTreeWhite[RF_CODE_SIZE];
extern const long ChristmasTreeColor[RF_CODE_SIZE];
extern const long BedroomLight[RF_CODE_SIZE];
extern const long BedroomFan[RF_CODE_SIZE];
extern const long JoesroomLight[RF_CODE_SIZE];

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define CODE_END -1

/******************************************************************************/
/* Macro Functions                                                            */
/******************************************************************************/

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
inline unsigned char RF_SetState(unsigned char state);
inline unsigned char RF_GetState(void);
void InitRF(void);
void RF_SendCode(unsigned long *code);
void RF_SendCode_CMD(void);

#endif	/* EXPAND_RF_H */