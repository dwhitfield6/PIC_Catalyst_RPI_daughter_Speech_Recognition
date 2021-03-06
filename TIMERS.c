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
/* Contains functions for timers.
 *
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "EXPAND_IR.h"
#include "MISC.h"
#include "SYSTEM.h"
#include "TIMERS.h"
#include "USER.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
volatile unsigned char Timer1_Timeout = TRUE;

/******************************************************************************/
/* Inline Functions                                                           */
/******************************************************************************/

/******************************************************************************/
/* TMR_EnableTimer1
 *
 * Controls the Timer1 module.
/******************************************************************************/
inline unsigned char TMR_EnableTimer1(unsigned char action)
{
    unsigned char status;
#ifdef ON
    #undef ON
    status = T1CONbits.ON;
    if (action)
    {
        T1CONbits.ON = 1; // Enable Timer
    }
    else
    {
        T1CONbits.ON = 0; // Disable Timer
    }
    #define ON 1
#else
    status = T1CONbits.ON;
    if (action)
    {
        T1CONbits.ON = 1; // Enable Timer
    }
    else
    {
        T1CONbits.ON = 0; // Disable Timer
    }
    return status;
#endif
}

/******************************************************************************/
/* TMR_EnableTimer2
 *
 * Controls the Timer2 module.
/******************************************************************************/
inline unsigned char TMR_EnableTimer2(unsigned char action)
{
    unsigned char status;
#ifdef ON
    #undef ON
    status = T2CONbits.ON;
    if (action)
    {
        T2CONbits.ON = 1; // Enable Timer
    }
    else
    {
        T2CONbits.ON = 0; // Disable Timer
    }
    #define ON 1
#else
    status = T2CONbits.ON;
    if (action)
    {
        T2CONbits.ON = 1; // Enable Timer
    }
    else
    {
        T2CONbits.ON = 0; // Disable Timer
    }
    return status;
#endif
}

/******************************************************************************/
/* TMR_EnableTimer3
 *
 * Controls the Timer3 module.
/******************************************************************************/
inline unsigned char TMR_EnableTimer3(unsigned char action)
{
    unsigned char status;
#ifdef ON
    #undef ON
    status = T3CONbits.ON;
    if (action)
    {
        T3CONbits.ON = 1; // Enable Timer
    }
    else
    {
        T3CONbits.ON = 0; // Disable Timer
    }
    #define ON 1
#else
    status = T3CONbits.ON;
    if (action)
    {
        T3CONbits.ON = 1; // Enable Timer
    }
    else
    {
        T3CONbits.ON = 0; // Disable Timer
    }
    return status;
#endif
}

/******************************************************************************/
/* TMR_EnableTimer4
 *
 * Controls the Timer4 module.
/******************************************************************************/
inline unsigned char TMR_EnableTimer4(unsigned char action)
{
    unsigned char status;
    
#ifdef ON
    #undef ON
    status = T4CONbits.ON;
    if (action)
    {
        T4CONbits.ON = 1; // Enable Timer
    }
    else
    {
        T4CONbits.ON = 0; // Disable Timer
    }
    #define ON 1
#else
    status = T4CONbits.ON;
    if (action)
    {
        T4CONbits.ON = 1; // Enable Timer
    }
    else
    {
        T4CONbits.ON = 0; // Disable Timer
    }
    return status;
#endif
}

/******************************************************************************/
/* TMR_EnableTimer5
 *
 * Controls the Timer5 module.
/******************************************************************************/
inline unsigned char TMR_EnableTimer5(unsigned char action)
{
    unsigned char status;
    
#ifdef ON
    #undef ON
    status = T5CONbits.ON;
    if (action)
    {
        T5CONbits.ON = 1; // Enable Timer
    }
    else
    {
        T5CONbits.ON = 0; // Disable Timer
    }
    #define ON 1
#else
    status = T5CONbits.ON;
    if (action)
    {
        T5CONbits.ON = 1; // Enable Timer
    }
    else
    {
        T5CONbits.ON = 0; // Disable Timer
    }
    return status;
#endif
}

/******************************************************************************/
/* TMR_InterruptTimer1
 *
 * Controls the Timer1 interrupt.
/******************************************************************************/
inline unsigned char TMR_InterruptTimer1(unsigned char action)
{
    unsigned char status = IEC0bits.T1IE;
    if (action)
    {
        IEC0bits.T1IE = 1; // Enable Timer 2 interrupt
    }
    else
    {
        IEC0bits.T1IE = 0; // disenable Timer 2 interrupt
    }
    return status;
}

/******************************************************************************/
/* TMR_InterruptTimer2
 *
 * Controls the Timer2 interrupt.
/******************************************************************************/
inline unsigned char TMR_InterruptTimer2(unsigned char action)
{
    unsigned char status = IEC0bits.T2IE;
    if (action)
    {
        IEC0bits.T2IE = 1; // Enable Timer 2 interrupt
    }
    else
    {
        IEC0bits.T2IE = 0; // disenable Timer 2 interrupt
    }
    return status;
}

/******************************************************************************/
/* TMR_InterruptTimer3
 *
 * Controls the Timer3 interrupt.
/******************************************************************************/
inline unsigned char TMR_InterruptTimer3(unsigned char action)
{
    unsigned char status = IEC0bits.T3IE;
    if (action)
    {
        IEC0bits.T3IE = 1; // Enable Timer 2 interrupt
    }
    else
    {
        IEC0bits.T3IE = 0; // disenable Timer 2 interrupt
    }
    return status;
}

/******************************************************************************/
/* TMR_InterruptTimer4
 *
 * Controls the Timer4 interrupt.
/******************************************************************************/
inline unsigned char TMR_InterruptTimer4(unsigned char action)
{
    unsigned char status = IEC0bits.T4IE;
    if (action)
    {
        IEC0bits.T4IE = 1; // Enable Timer 2 interrupt
    }
    else
    {
        IEC0bits.T4IE = 0; // disenable Timer 2 interrupt
    }
    return status;
}

/******************************************************************************/
/* TMR_InterruptTimer5
 *
 * Controls the Timer5 interrupt.
/******************************************************************************/
inline unsigned char TMR_InterruptTimer5(unsigned char action)
{
    unsigned char status = IEC0bits.T5IE;
    if (action)
    {
        IEC0bits.T5IE = 1; // Enable Timer 2 interrupt
    }
    else
    {
        IEC0bits.T5IE = 0; // disenable Timer 2 interrupt
    }
    return status;
}

/******************************************************************************/
/* TMR_ResetTimer1
 *
 * Restarts Timer 1.
/******************************************************************************/
inline void TMR_ResetTimer1(void)
{
    TMR1 = 0;
}

/******************************************************************************/
/* TMR_ResetTimer2
 *
 * Restarts Timer 2.
/******************************************************************************/
inline void TMR_ResetTimer2(void)
{
    TMR2 = 0;
}

/******************************************************************************/
/* TMR_ResetTimer3
 *
 * Restarts Timer 3.
/******************************************************************************/
inline void TMR_ResetTimer3(void)
{
    TMR3 = 0;
}

/******************************************************************************/
/* TMR_ResetTimer4
 *
 * Restarts Timer 4.
/******************************************************************************/
inline void TMR_ResetTimer4(void)
{
    TMR4 = 0;
}

/******************************************************************************/
/* TMR_ResetTimer5
 *
 * Restarts Timer 5.
/******************************************************************************/
inline void TMR_ResetTimer5(void)
{
    TMR5 = 0;
}

/******************************************************************************/
/* TMR_SetTimer1
 *
 * Sets the PR register for timer 1 to compare.
/******************************************************************************/
inline void TMR_SetTimer1(unsigned short time)
{
    PR1 = time;
}

/******************************************************************************/
/* TMR_SetTimer2
 *
 * Sets the PR register for timer 2 to compare.
/******************************************************************************/
inline void TMR_SetTimer2(unsigned short time)
{
    PR2 = time;
}

/******************************************************************************/
/* TMR_SetTimer3
 *
 * Sets the PR register for timer 3 to compare.
/******************************************************************************/
inline void TMR_SetTimer3(unsigned short time)
{
    PR3 = time;
}

/******************************************************************************/
/* TMR_SetTimer4
 *
 * Sets the PR register for timer 4 to compare.
/******************************************************************************/
inline void TMR_SetTimer4(unsigned short time)
{
    PR4 = time;
}

/******************************************************************************/
/* TMR_SetTimer5
 *
 * Sets the PR register for timer 5 to compare.
/******************************************************************************/
inline void TMR_SetTimer5(unsigned short time)
{
    PR5 = time;
}

/******************************************************************************/
/* Functions
/******************************************************************************/

/******************************************************************************/
/* InitTIMERS
 *
 * The function initializes the timers.
/******************************************************************************/
void InitTIMERS(void)
{
    T1CONbits.SIDL = 0; // Continue operation even in Idle mode
    T2CONbits.SIDL = 0; // Continue operation even in Idle mode
    T3CONbits.SIDL = 0; // Continue operation even in Idle mode
    T4CONbits.SIDL = 0; // Continue operation even in Idle mode
    T5CONbits.SIDL = 0; // Continue operation even in Idle mode
    InitTIMER1();
    InitTIMER2();
    InitTIMER4();
    InitTIMER5();
}

/******************************************************************************/
/* InitTIMER1
 *
 * The function initializes timer 1 which is used for the delay function.
/******************************************************************************/
void InitTIMER1(void)
{
    TMR_EnableTimer1(OFF);
    TMR_InterruptTimer1(OFF);
    IPC1bits.T1IP = 1; // interrupt priority is 1
    IPC1bits.T1IS = 1; // interrupt sub-priority is 1
    T1CONbits.TCS = 0; // Select internal instruction cycle clock
    T1CONbits.TGATE = 0; // Disable Gated Timer mode
    T1CONbits.TCKPS = 0b11; // Select 1:256 Prescaler
}

/******************************************************************************/
/* InitTIMER2
 *
 * The function initializes timer 2 which is used for the RGB LED PWM module.
/******************************************************************************/
void InitTIMER2(void)
{
    TMR_EnableTimer2(OFF);
    TMR_InterruptTimer2(OFF);
    IPC2bits.T2IP = 1; // interrupt priority is 1
    IPC2bits.T2IS = 1; // interrupt sub-priority is 1
    T2CONbits.TCS = 0; // Select internal instruction cycle clock
    T2CONbits.TGATE = 0; // Disable Gated Timer mode
    T2CONbits.TCKPS = 0b110; // Select 1:64 Prescaler
    TMR2 = 0x00; // Clear timer register
    TMR_SetTimer2(1000);
    IFS0bits.T2IF = 0; // Clear Timer 2 Interrupt Flag
    TMR_EnableTimer2(ON);
}

/******************************************************************************/
/* InitTIMER3
 *
 * The function initializes timer 3 which is used for the IR LED PWM module.
/******************************************************************************/
void InitTIMER3(unsigned long ModulationFreq)
{
    double temp;
    
    TMR_EnableTimer3(OFF);
    TMR_InterruptTimer3(OFF);
    IPC3bits.T3IP = 1; // interrupt priority is 1
    IPC3bits.T3IS = 1; // interrupt sub-priority is 1
    T3CONbits.TCS = 0; // Select internal instruction cycle clock
    T3CONbits.TGATE = 0; // Disable Gated Timer mode
    T3CONbits.TCKPS = 0b000; // Select 1:1 Prescaler
    temp = MSC_Round((double)PBCLK /((double)ModulationFreq));
    if(temp > (double)MAX_USHORT)
    {
        temp = (double)MAX_USHORT;
    }
    TMR3 = 0x00; // Clear timer register
    TMR_SetTimer3((unsigned short) temp);
    IR_PWM50 = (unsigned short) temp;
    IR_PWM50 >>= 1;
    IFS0bits.T3IF = 0; // Clear Timer 2 Interrupt Flag
}

/******************************************************************************/
/* InitTIMER4
 *
 * The function initializes timer 4 which is used for the RGB LED functions like
 *  fade and blink.
/******************************************************************************/
void InitTIMER4(void)
{
    TMR_EnableTimer4(OFF);
    TMR_InterruptTimer4(OFF);
    IPC4bits.T4IP = 1; // interrupt priority is 1
    IPC4bits.T4IS = 1; // interrupt sub-priority is 1
    T4CONbits.TCS = 0; // Select internal instruction cycle clock
    T4CONbits.TGATE = 0; // Disable Gated Timer mode
    T4CONbits.TCKPS = 0b111; // Select 1:256 Prescaler
    TMR4 = 0x00; // Clear timer register
    PR4 = 0xFFFF; // Load the period value
    IFS0bits.T4IF = 0; // Clear Timer 4 Interrupt Flag
    TMR_InterruptTimer4(ON);
}

/******************************************************************************/
/* InitTIMER5
 *
 * The function initializes timer 5 which is used for the ADC to sample and
 *  switch channels.
/******************************************************************************/
void InitTIMER5(void)
{
    TMR_EnableTimer5(OFF);
    TMR_InterruptTimer5(OFF);
    IPC5bits.T5IP = 1; // interrupt priority is 1
    IPC5bits.T5IS = 1; // interrupt sub-priority is 1
    T5CONbits.TCS = 0; // Select internal instruction cycle clock
    T5CONbits.TGATE = 0; // Disable Gated Timer mode
    T5CONbits.TCKPS = 0b111; // Select 1:256 Prescaler
    TMR5 = 0x00; // Clear timer register
    PR5 = 0xFFFF; // Load the period value
    IFS0bits.T5IF = 0; // Clear Timer 5 Interrupt Flag
    TMR_InterruptTimer5(ON);
    TMR_EnableTimer5(ON);
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/