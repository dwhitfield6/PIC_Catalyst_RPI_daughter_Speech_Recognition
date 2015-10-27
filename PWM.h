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
#ifndef PWM_H
#define	PWM_H

#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "USER.h"

/******************************************************************************/
/* COLORS
 *
 * This is the available Colors to set the LED.
/******************************************************************************/
#define NONE        0
#define RED         1
#define YELLOW      2
#define GREEN       3
#define BLUE        4
#define PURPLE      5
#define WHITE       6
#define TURQUOISE   7

/******************************************************************************/
/* Actions
 *
 * This is the available actions to set the LED.
/******************************************************************************/
#define NUM_ACTIONS 5

#define NOTHING     0
#define FADEUP      1
#define FADEDOWN    2
#define FADEUPDOWN  3
#define BLINK       4

/******************************************************************************/
/* Speeds
 *
 * This is the available speeds for each action to set the LED.
/******************************************************************************/
#define PWM_VERYSLOW    50000
#define PWM_SLOW        30000
#define PWM_MEDIUM      20000
#define PWM_FAST        10000
#define PWM_VERYFAST    5000

/******************************************************************************/
/* Special PWM commands
 *
 * 255 is a non modify marker (NMM) meaning dont change the value.
/******************************************************************************/
#define NMM        255

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
extern unsigned long Red_Duty;
extern unsigned long Green_Duty;
extern unsigned long Blue_Duty;
extern unsigned char PWM_Action;
extern unsigned char RedAction;
extern unsigned char GreenAction;
extern unsigned char BlueAction;
extern const unsigned char RGB_Functions[3][NUM_ACTIONS][25];
extern unsigned char PWM_Place;

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

/******************************************************************************/
/* Macro Functions                                                            */
/******************************************************************************/

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
void InitPWM(void);
unsigned char PWM_Module(unsigned char state);
inline void PWM_SetRed(unsigned short Value);
inline void PWM_SetGreen(unsigned short Value);
inline void PWM_SetBlue(unsigned short Value);
inline void PWM_SetRGB(unsigned short Red, unsigned short Green, unsigned short Blue);
void PWM_SetColor(unsigned short Color, unsigned char type, unsigned short speed);
void PWM_SetAction(unsigned char type, unsigned short speed);

#endif	/* PWM_H */