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
/* Contains system functions.
 *
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "SYSTEM.h"
#include "EXCEPTIONS.h"

/******************************************************************************/
/* Functions
/******************************************************************************/

/******************************************************************************/
/* SYS_ConfigureOscillator
 *
 * The function waits for the high frequency oscillator to be working and stable.
/******************************************************************************/
void SYS_ConfigureOscillator(void)
{    
    /* Clock configuration is done in configuration bits in Configuration.c */
    
    /* wait for PLL to be stable if it is used */
    if(OSCCONbits.COSC = 0b011)
    {
        /* Primary Oscillator with PLL module (XTPLL, HSPLL or ECPLL) */
        while(!OSCCONbits.SLOCK);        
        /* PLL module is in lock or PLL module start-up timer is satisfied */
    }
}

/******************************************************************************/
/* MSC_SystemUnlock
 *
 * The system will be unlocked so that sensitive registers can be modified like
 *  OSCCON.
/******************************************************************************/
void SYS_SystemUnlock(void)
{
    SYSKEY = 0;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
}

/******************************************************************************/
/* MSC_SystemLock
 *
 * The system will be locked so that sensitive registers can't be modified like
 *  OSCCON.
/******************************************************************************/
void SYS_SystemLock(void)
{
    SYSKEY = 0x33333333;
}

/******************************************************************************/
/* SYS_Interrupts
 *
 * The function controls the interrupts.
/******************************************************************************/
void SYS_Interrupts(unsigned char state)
{
    if(state)
    {
        __builtin_enable_interrupts();
    }
    else
    {
        __builtin_disable_interrupts();
    }
}

/******************************************************************************/
/* SYS_Sleep
 *
 * The function puts the system to sleep.
/******************************************************************************/
void SYS_Sleep(void)
{
    SYS_SystemUnlock();
    OSCCONbits.SLPEN = 1; // Device will enter Sleep mode when a WAIT instruction is executed
    SYS_SystemLock();
    _wait();
}

/******************************************************************************/
/* SYS_Idle
 *
 * The function puts the system in idle.
/******************************************************************************/
void SYS_Idle(void)
{
    SYS_SystemUnlock();
    OSCCONbits.SLPEN = 0; // Device will enter Idle mode when a WAIT instruction is executed
    SYS_SystemLock();
    _wait();
}

/******************************************************************************/
/* Init_Watchdog
 *
 * The function initializes the watchdog timer.
/******************************************************************************/
void Init_Watchdog(void)
{
    /* does not enable when in debug mode */
    WDTCONbits.WDTWINEN = 0; // Disable windowed Watchdog Timer
    SYS_Watchdog(OFF);
}

/******************************************************************************/
/* SYS_SoftwareReset
 *
 * The function does a software reset.
/******************************************************************************/
void SYS_SoftwareReset(void)
{
    unsigned long dummy;
    SYS_SystemUnlock(); 
    DMA_Suspend(ON);
    RSWRSTbits.SWRST = 1;
    dummy = RSWRSTbits.SWRST; // read the register to start the reset
    while(1)
    {
        Nop();
    }
}

/******************************************************************************/
/* SYS_Watchdog
 *
 * The function enables/disables the watchdog timer.
/******************************************************************************/
unsigned char SYS_Watchdog(unsigned char state)
{
    unsigned char status;
    
#ifdef ON
    #undef ON
    status = WDTCONbits.ON;
    if(state)
    {
        SYS_PetFluffyPuppy();
        Nop();
        Nop();
        WDTCONbits.ON = 1; // Enables the WDT if it is not enabled by the device configuration
    }
    else
    {
        WDTCONbits.ON = 0; // Disable the WDT if it was enabled in software
    }
    #define ON 1
#else
    status = WDTCONbits.ON;
    if(state)
    {
        SYS_PetFluffyPuppy();
        Nop();
        Nop();
        WDTCONbits.ON = 1; // Enables the WDT if it is not enabled by the device configuration
    }
    else
    {
        WDTCONbits.ON = 0; // Disable the WDT if it was enabled in software
    }
#endif
    return status;
}

/******************************************************************************/
/* SYS_PetFluffyPuppy
 *
 * The function pets the watchdog otherwise known as a the "fluffy puppy".
/******************************************************************************/
void SYS_PetFluffyPuppy(void)
{
    WDTCONbits.WDTCLR = 1; // clear the watchdog
}

/******************************************************************************/
/* SYS_PetFluffyPuppy
 *
 * The function pets the watchdog otherwise known as a the "fluffy puppy".
/******************************************************************************/
void SYS_PetWatchDog(void)
{
    SYS_PetFluffyPuppy();
}

/******************************************************************************/
/* SYS_CheckWatchdogReset
 *
 * The function checks for a watchdog reset.
/******************************************************************************/
unsigned char SYS_CheckWatchdogReset(void)
{
    if(RCONbits.WDTO)
    {
        Fault.WatchDog_Reset = 1;
        RCONbits.WDTO = 0;
        return TRUE;
    }
    return FALSE;
}

/******************************************************************************/
/* SYS_WakeEvent
 *
 * The function checks to see if the device was in sleep mode or idle mode
 *  when it woke-up.
/******************************************************************************/
unsigned char SYS_WakeEvent(unsigned char* event)
{
    if(RCONbits.SLEEP)
    {
        /* Device was in Sleep mode during wake */
        *event = WAKE_SLEEP;
        RCONbits.SLEEP = 0;
        RCONbits.IDLE = 0;
        return TRUE;
    }
    else if(RCONbits.IDLE)
    {
        /* Device was in Sleep mode during wake */
        *event = WAKE_IDLE;
        RCONbits.SLEEP = 0;
        RCONbits.IDLE = 0;
        return TRUE;        
    }
    *event = WAKE_ACTIVE;
    return FALSE;
}

/******************************************************************************/
/* SYS_CheckReset
 *
 * The function checks for the cause of the reset.
/******************************************************************************/
unsigned char SYS_CheckReset(void)
{
    unsigned char event = 0;
    
    if(RCONbits.HVDR)
    {
        /* High Voltage Detect (HVD) Reset has occurred */
        Fault.HighVoltage_Reset = 1;
        RCONbits.HVDR = 0;
        event |= RESET_HVDR;
    }
    if(RCONbits.CMR)
    {
        /* Configuration mismatch Reset has occurred */
        Fault.ConfigMissMatch_Reset = 1;
        RCONbits.CMR = 0;
        event |= RESET_CMR;
    }
    if(RCONbits.VREGS)
    {
        /* Regulator is enabled and is on during Sleep mode */
        RCONbits.VREGS = 0;
        event |= RESET_VREGS;
    }
    if(RCONbits.EXTR)
    {
        /* Master Clear (pin) Reset has occurred */
        RCONbits.EXTR = 0;
        event |= RESET_EXTR;
    }
    if(RCONbits.SWR)
    {
        /* Software Reset was executed */
        Fault.Software_Reset = 1;
        RCONbits.SWR = 0;
        event |= RESET_EXTR;
    }
    if(SYS_CheckWatchdogReset())
    {
        event |= RESET_WDTO;
    }
    if(RCONbits.BOR)
    {
        /* Brown-out Reset has occurred  */
        Fault.Brownout_Reset = 1;
        RCONbits.BOR = 0;
        event |= RESET_BOR;
    }
    if(RCONbits.POR)
    {
        /* Power-on Reset has occurred  */
        Fault.Poweron_Reset = 1;
        RCONbits.POR = 0;
        event |= RESET_POR;
    }
    return event;
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/