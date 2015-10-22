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
/* Contains functions for pulse width modulation controller.
 *
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "USER.h"
#include "PWM.h"
#include "TIMERS.h"

/******************************************************************************/
/* RGB LED functions    
 * 
 * RGB_Functions[color][pattern][PWM data]                                   
/******************************************************************************/
const unsigned char RGB_Functions[3][NUM_ACTIONS][25] = 
{
    /******** Red ********/
    { //   1,  2,  3,  4,  5,  6,  7 , 8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 
        {NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM}, // nothing
        {  1,  2,  3,  5,  8, 11, 16, 21, 27, 34, 41, 50, 59, 69, 79, 91,103,116,130,145,161,177,194,212,230}, //FADEUP
        {230,212,194,177,161,145,130,116,103, 91, 79, 69, 59, 50, 41, 34, 27, 21, 16, 11,  8,  5,  3,  2,  1}, //FADEDOWN
        {  1,  3,  8, 11, 16, 27, 41, 59, 79,103,130,161,194,194,161,130,103, 79, 59, 41, 27, 16, 11,  8,  3}, //FADEUPDOWN
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59}  //BLINK


    },
    /******** Green ********/
    { //   1,  2,  3,  4,  5,  6,  7 , 8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 
        {NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,  1}, // nothing
        {  1,  2,  3,  5,  7, 10, 14, 18, 24, 29, 36, 43, 51, 60, 69, 79, 90,101,113,126,140,154,169,184,200}, //FADEUP
        {200,184,169,154,140,126,113,101, 90, 79, 69, 60, 51, 43, 36, 29, 24, 18, 14, 10,  7,  5,  3,  2,  1}, //FADEDOWN
        {  1,  3,  7, 10, 14, 24, 36, 51, 69, 90,113,140,169,169,140,113, 90, 69, 51, 36, 24, 14, 10,  7,  3}, //FADEUPDOWN
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}  //BLINK
    },
    /******** Blue ********/
    { //   1,  2,  3,  4,  5,  6,  7 , 8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 
        {NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,NMM,  1}, // nothing
        {  1,  2,  2,  3,  4,  6,  8, 10, 12, 15, 19, 22, 26, 31, 35, 40, 45, 51, 57, 64, 70, 77, 85, 92,100}, //FADEUP
        {100, 92, 85, 77, 70, 64, 57, 51, 45, 40, 35, 31, 26, 22, 19, 15, 12, 10,  8,  6,  4,  3,  2,  2,  1}, //FADEDOWN
        {  1,  2,  4,  6,  8, 12, 19, 26, 35, 45, 57, 70, 85, 85, 70, 57, 45, 35, 26, 19, 12,  8,  6,  4,  2}, //FADEUPDOWN
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26}  //BLINK
    }       
};

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
unsigned long Red_Duty = 0;
unsigned long Green_Duty = 0;
unsigned long Blue_Duty = 0;
unsigned char PWM_Action = FADEUP;
unsigned char RedAction = FALSE;
unsigned char GreenAction = FALSE;
unsigned char BlueAction = FALSE;
unsigned char PWM_Place = 0;

/******************************************************************************/
/* Inline Functions                                                           */
/******************************************************************************/

/******************************************************************************/
/* PWM_SetRed
 *
 * Sets the duty cycle for the Red LED.
/******************************************************************************/
inline void PWM_SetRed(unsigned short Value)
{
    Red_Duty = Value;
}

/******************************************************************************/
/* PWM_SetGreen
 *
 * Sets the duty cycle for the Green LED.
/******************************************************************************/
inline void PWM_SetGreen(unsigned short Value)
{
    Green_Duty = Value;
}

/******************************************************************************/
/* PWM_SetBlue
 *
 * Sets the duty cycle for the Blue LED.
/******************************************************************************/
inline void PWM_SetBlue(unsigned short Value)
{
    Blue_Duty = Value;
}

/******************************************************************************/
/* PWM_SetRGB
 *
 * Sets the LED Color via 3 color maps.
/******************************************************************************/
inline void PWM_SetRGB(unsigned short Red, unsigned short Green, unsigned short Blue)
{
    PWM_SetRed(Red);
    PWM_SetGreen(Green);
    PWM_SetBlue(Blue);
}

/******************************************************************************/
/* Functions
/******************************************************************************/

/******************************************************************************/
/* InitPIR
 *
 * The function initializes the 3 PWM channels. This must be called after a
 * a call to initialize the timers.
/******************************************************************************/
void InitPWM(void)
{
    /* Map the remappable pins */
    /* Red led is OC3 */
    /* Green led is OC2 */
    /* Blue led is OC1 */      
    RPD2R    = 0b1011;    // RD2 = OC3 aka Red on RGB LED
    RPB6R    = 0b1011;    // RB6 = OC2 aka Green on RGB LED
    RPD1R    = 0b1100;    // RD1 = OC1 aka Blue on RGB LED
    
    // Initialize Output Compare Module
#ifdef ON
    #undef ON
    OC1CONbits.ON = TRUE;    // Turn off output compare peripheral
    OC2CONbits.ON = TRUE;    // Turn off output compare peripheral
    OC3CONbits.ON = TRUE;    // Turn off output compare peripheral
    #define ON 1
#else
    OC1CONbits.ON = TRUE;    // Turn off output compare peripheral
    OC2CONbits.ON = TRUE;    // Turn off output compare peripheral
    OC3CONbits.ON = TRUE;    // Turn off output compare peripheral
#endif
    OC1CONbits.SIDL = 0; // Continue operation in Idle mode
    OC2CONbits.SIDL = 0; // Continue operation in Idle mode
    OC3CONbits.SIDL = 0; // Continue operation in Idle mode
    
    OC1CONbits.OCM = 0b110; // PWM mode on OC1; Fault pin disabled
    OC2CONbits.OCM = 0b110; // PWM mode on OC2; Fault pin disabled
    OC3CONbits.OCM = 0b110; // PWM mode on OC3; Fault pin disabled
    
    OC1R = 0; // Write the duty cycle for the first PWM pulse of Blue LED
    OC2R = 0; // Write the duty cycle for the first PWM pulse of Green LED
    OC3R = 0; // Write the duty cycle for the first PWM pulse of Red LED
    OC1RS = 0; // Write the duty cycle for the second PWM pulse
    OC2RS = 0; // Write the duty cycle for the second PWM pulse
    OC3RS = 0; // Write the duty cycle for the second PWM pulse
    
    OC1CONbits.OCTSEL = 0; // Select Timer 2 as output compare time base
    OC2CONbits.OCTSEL = 0; // Select Timer 2 as output compare time base
    OC3CONbits.OCTSEL = 0; // Select Timer 2 as output compare time base
    
    TMR_ResetTimer2();
    TMR_InterruptTimer2(ON);
}

/******************************************************************************/
/* PWM_SetAction
 *
 * Sets the LED action like fade or blink and sets the speed.
/******************************************************************************/
void PWM_SetAction(unsigned char type, unsigned short speed)
{
    TMR_EnableTimer4(OFF);
    PWM_Place = 0;
    PWM_Action = type;
    RedAction = FALSE;
    GreenAction = FALSE;
    BlueAction = FALSE;
    if(Red_Duty)
    {
        RedAction = TRUE;
    }
    if(Green_Duty)
    {
        GreenAction = TRUE;
    }
    if(Blue_Duty)
    {
        BlueAction = TRUE;
    }
    TMR_ResetTimer4();
    TMR_SetTimer4(speed);
    if(type)
    {
        TMR_EnableTimer4(ON);
    }
}

/******************************************************************************/
/* PWM_SetColor
 *
 * Sets the LED Color by name.
/******************************************************************************/
void PWM_SetColor(unsigned short Color, unsigned char type, unsigned short speed)
{
    TMR_EnableTimer4(OFF);
    switch (Color)
    {
        case RED:
            PWM_SetRGB(32,0,0);
            break;
        case GREEN:
            PWM_SetRGB(0,16,0);
            break;
        case BLUE:
            PWM_SetRGB(0,0,8);
            break;
        case PURPLE:
            PWM_SetRGB(32,0,8);
            break;
        case YELLOW:
            PWM_SetRGB(16,16,0);
            break;
         case WHITE:
            PWM_SetRGB(16,16,8);
            break;
        case TURQUOISE:
            PWM_SetRGB(0,16,8);
            break;
        default:
            PWM_SetRGB(0,0,0);
            break;
    }
    PWM_SetAction(type, speed);
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/