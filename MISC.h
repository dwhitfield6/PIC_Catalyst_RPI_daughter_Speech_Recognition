/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 08/21/15     1.0_DW0a    Initial project make.
 *                          Derived from project 'PIC_Sitcom_Generator'.
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#ifndef MISC_H
#define	MISC_H

#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "SYSTEM.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define BIG     1
#define LITTLE  2
#define MIDDLE  3

/******************************************************************************/
/* Macro Functions                                                            */
/******************************************************************************/

/*~~~~~~~~~~~~~~~~~~~~~~~~~ RED LED ~~~~~~~~~~~~~~~~~~~~~~~~~*/
/******************************************************************************/
/* MSC_RedLEDON
 *
 * The function turns on the Red LED.
/******************************************************************************/
#define MSC_RedLEDON()  (LATD |= RedLED)

/******************************************************************************/
/* MSC_RedLEDOFF
 *
 * The function turns off the Red LED.
/******************************************************************************/
#define MSC_RedLEDOFF()  (LATD &= ~RedLED)

/******************************************************************************/
/* MSC_RedLEDTOGGLE
 *
 * The function toggles the Red LED.
/******************************************************************************/
#define MSC_RedLEDTOGGLE()  (LATD ^= RedLED)

/*~~~~~~~~~~~~~~~~~~~~~~~~~ RGB LED ~~~~~~~~~~~~~~~~~~~~~~~~~*/
/******************************************************************************/
/* MSC_RGBRedLEDON
 *
 * The function turns on the RGB Red LED.
/******************************************************************************/
#define MSC_RGBRedLEDON()  (LATD |= RGB_RedLED)

/******************************************************************************/
/* MSC_RGBGreenLEDON
 *
 * The function turns on the RGB Green LED.
/******************************************************************************/
#define MSC_RGBGreenLEDON()  (LATB |= RGB_GreenLED)

/******************************************************************************/
/* MSC_RGBBlueLEDON
 *
 * The function turns on the RGB Blue LED.
/******************************************************************************/
#define MSC_RGBBlueLEDON()  (LATD |= RGB_BlueLED)

/******************************************************************************/
/* MSC_RGBRedLEDOFF
 *
 * The function turns off the RGB Red LED.
/******************************************************************************/
#define MSC_RGBRedLEDOFF()  (LATD &= ~RGB_RedLED)

/******************************************************************************/
/* MSC_RGBGreenLEDOFF
 *
 * The function turns off the RGB Green LED.
/******************************************************************************/
#define MSC_RGBGreenLEDOFF()  (LATB &= ~RGB_GreenLED)

/******************************************************************************/
/* MSC_RGBBlueLEDOFF
 *
 * The function turns off the RGB Blue LED.
/******************************************************************************/
#define MSC_RGBBlueLEDOFF()  (LATD &= ~RGB_BlueLED)

/******************************************************************************/
/* MSC_RGBRedLEDTOGGLE
 *
 * The function toggles the RGB Red LED.
/******************************************************************************/
#define MSC_RGBRedLEDTOGGLE()  (LATD ^= RGB_RedLED)

/******************************************************************************/
/* MSC_RGBGreenLEDTOGGLE
 *
 * The function toggles the RGB Green LED.
/******************************************************************************/
#define MSC_RGBGreenLEDTOGGLE()  (LATB ^= RGB_GreenLED)

/******************************************************************************/
/* MSC_RGBBlueLEDTOGGLE
 *
 * The function toggles the RGB Blue LED.
/******************************************************************************/
#define MSC_RGBBlueLEDTOGGLE()  (LATD ^= RGB_BlueLED)

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
inline void MSC_Relay(unsigned char state);
void MSC_DelayUS(long US);
void MSC_DelayNOP(unsigned char NOPs);
unsigned char IsLetter(unsigned char data);
unsigned char MSC_IsNumber(unsigned char data);
unsigned char MSC_IsCharacter(unsigned char data);
unsigned char MSC_IsAlphaNumeric(unsigned char data);
unsigned char MSC_IsAlphaNumericString(unsigned char* data);
void MSC_CleanBuffer(void* data, unsigned short bytes);
double MSC_Round(double input);
void MSC_BufferFill(void* buffer, unsigned long data, unsigned char bits, unsigned short bytes);
unsigned short MSC_SizeOfString(unsigned char* string);
void MSC_StringCopy(unsigned char* from,unsigned char* to);
unsigned char MSC_StringMatch(void* This, void* That);
unsigned char MSC_BufferMatch(void* buffer1, void* buffer2, unsigned short bytes);
void MSC_BufferCopy(void* From, void* To, unsigned short bytes);
void MSC_LowercaseString(unsigned char* Input);
void MSC_LowercaseChar(unsigned char* Input);
unsigned short MSC_HEXtoBCD(unsigned short input);
unsigned short MSC_BCDtoHEX(unsigned short input);
unsigned long MSC_Endian(unsigned long number, unsigned char bits, unsigned char style);

#endif	/* MISC_H */