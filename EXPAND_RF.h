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
/* Code sizes for each configuration
 *
 * This is the number of 'bits' for each configuration.
/******************************************************************************/
#define CONF1_SIZE 12
#define CONF2_SIZE 16

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
extern const unsigned char Conf1_ChannelD[];
extern long LivingroomLight[RF_CODE_SIZE];
extern const unsigned char Conf2_ChannelH_2_ON[];
extern const unsigned char Conf2_ChannelH_2_OFF[];
extern long LivingroomFanOn[RF_CODE_SIZE];
extern long LivingroomFanOff[RF_CODE_SIZE];
extern const unsigned char Conf2_ChannelD_ON[];
extern const unsigned char Conf2_ChannelD_OFF[];
extern long ChristmasTreeWhiteOn[RF_CODE_SIZE];
extern long ChristmasTreeWhiteOff[RF_CODE_SIZE];
extern const unsigned char Conf1_ChannelF[];
extern long ChristmasTreeColor[RF_CODE_SIZE];
extern const unsigned char Conf1_ChannelE[];
extern long BedroomLight[RF_CODE_SIZE];
extern const unsigned char Conf2_ChannelH_3_ON[];
extern const unsigned char Conf2_ChannelH_3_OFF[];
extern long BedroomFanOn[RF_CODE_SIZE];
extern long BedroomFanOff[RF_CODE_SIZE];
extern const unsigned char Conf2_ChannelH_1_ON[];
extern const unsigned char Conf2_ChannelH_1_OFF[];
extern long BedroomBedOn[RF_CODE_SIZE];
extern long BedroomBedOff[RF_CODE_SIZE];
extern const unsigned char Conf2_ChannelB_ON[];
extern const unsigned char Conf2_ChannelB_OFF[];
extern long JoesroomLightOn[RF_CODE_SIZE];
extern long JoesroomLightOff[RF_CODE_SIZE];
extern unsigned char LivingroomFanLast;
extern unsigned char ChristmasTreeWhiteLast;
extern unsigned char BedroomFanLast;
extern unsigned char BedroomBedLast;
extern unsigned char JoesroomLightLast;

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define CODE_END    -1
#define f           2

/******************************************************************************/
/* Macro Functions                                                            */
/******************************************************************************/

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
inline unsigned char RF_SetState(unsigned char state);
inline unsigned char RF_GetState(void);
void InitRF(void);
void RF_CalculateConf1Codes(const unsigned char* code, long *timing);
void RF_CalculateConf2Codes(const unsigned char* code, long *timing);
void RF_CalculateCodes(void);
void RF_SendCode(unsigned long *code);
void RF_SendCode_CMD(void);

#endif	/* EXPAND_RF_H */