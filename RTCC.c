/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 09/15/15     1.0_DW0a    Initial project make.
/******************************************************************************/

/******************************************************************************/
/* Contains functions for the Real time clock and calender.
 *
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "MISC.h"
#include "USER.h"
#include "RTCC.h"
#include "SYSTEM.h"
#include "EXCEPTIONS.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
TIMETYPE StartTime;
TIMETYPE EndTime;
DIFFTIMETYPE DiffTime;
TIMETYPE CurrentTime;
TIMETYPE SetTime;
TIMETYPE CurrentAlarm;
TIMETYPE SetAlarm;

/******************************************************************************/
/* Inline Functions
/******************************************************************************/

/******************************************************************************/
/* Functions
/******************************************************************************/

/******************************************************************************/
/* InitRTCC
 *
 * The function initializes the real time clock and calender.
/******************************************************************************/
void InitRTCC(void)
{
    RTCC_ClearTime(&CurrentTime);
    RTCC_ClearTime(&SetTime);
    IPC6bits.RTCCIP= 6;  // interrupt priority is 6
    IPC6bits.RTCCIS = 3; // interrupt sub-priority is 3
    IFS0bits.RTCCIF = 0;            // clear interrupt 
    if(!RTCC_ConfigOscillator())
    {
        return;
    }
    RTCC_Access(YES);
    RTCC_Module(ON);
    RTCCONbits.SIDL = 0; // Continue normal operation in Idle mode
    RTCCONbits.RTSECSEL = 0; // RTCC Alarm Pulse is selected for the RTCC pin
    RTCC_SetDefaultTime();
    RTCC_SetDefaultAlarm();
    RTCC_Alarm(ON, TIME, YEAR);
    RTCC_Access(NO);
    RTCC_StartTiming();
    
    EndTime.Second = 10;
    EndTime.Minute = 25;
    EndTime.Hour_24 = 3;
    EndTime.Date = 7;
    EndTime.Month = March;
    EndTime.Year = 2020;
    
    RTCC_TimingDiff();
}

/******************************************************************************/
/* RTCC_ConfigOscillator
 *
 * The function configures the secondary oscillator used for the RTCC.
/******************************************************************************/
unsigned char RTCC_ConfigOscillator(void)
{
    unsigned long timer = 0;
    
    SYS_SystemUnlock();
    OSCCONbits.SOSCEN = 1; // Enable Secondary Oscillator
    SYS_SystemLock();
    while(OSCCONbits.SOSCRDY)
    {
        timer++;
        if(timer > RTCC_OSCILLATOR_TIMEROUT)
        {
            Fault.RTCC_Oscillator_Fail = TRUE;
            return FAIL;
        }
    }
    Fault.RTCC_Oscillator_Fail = FALSE;
    return PASS;
}

/******************************************************************************/
/* RTCC_ClearTime
 *
 * The function clears the struct TIMETYPE.
/******************************************************************************/
void RTCC_ClearTime(TIMETYPE* Time)
{
    Time->Year = 0;
    Time->Month = 0;
    Time->Date = 0;
    Time->Weekday = 0;
    Time->Hour_24 = 0;
    RTCC_CalculateHours(Time,MILITARY);
    Time->Minute = 0;
    Time->Second = 0;
}

/******************************************************************************/
/* RTCC_CalculateHours
 *
 * The function calculates the 12 hour time and AM or PM from military time.
/******************************************************************************/
void RTCC_CalculateHours(TIMETYPE* Time, unsigned char type)
{
    if(type == MILITARY)
    {
        if(Time->Hour_24 > 12)
        {
            Time->Hour_12 = Time->Hour_24 - 12;
        }
        else
        {
            if(Time->Hour_24 == 0)
            {
                Time->Hour_12 = 12;
            }
            else
            {
                Time->Hour_12 = Time->Hour_24;
            }
        }
        if(Time->Hour_24 >=12)
        {
            Time->PM_nAM = PM;
        }
        else
        {
            Time->PM_nAM = AM;
        }
    }
    else
    {
        if(Time->PM_nAM == AM)
        {
                Time->Hour_24 = Time->Hour_12;
        }
        else
        {
            if(Time->Hour_12 > 11)
            {
                Time->Hour_24 = 0;
            }
            else
            {
                Time->Hour_24 = Time->Hour_12 + 12;
            }
        }
    }
}

/******************************************************************************/
/* RTCC_SetDefaultTime
 *
 * The function sets the default time to the current time.
/******************************************************************************/
void RTCC_SetDefaultTime(void)
{
    SetTime.Year = 2015;
    SetTime.Month = September;
    SetTime.Date = 16;
    SetTime.Weekday = Wednesday;
    SetTime.Hour_12 = 10;
    SetTime.PM_nAM = AM;
    SetTime.Minute = 25;
    SetTime.Second = 0;
    RTCC_Write(&SetTime);
}

/******************************************************************************/
/* RTCC_SetDefaultAlarm
 *
 * The function sets the default alarm to the current alarm.
/******************************************************************************/
void RTCC_SetDefaultAlarm(void)
{
    SetAlarm.Year = 2015;
    SetAlarm.Month = September;
    SetAlarm.Date = 16;
    SetAlarm.Hour_12 = 10;
    SetAlarm.PM_nAM = AM;
    SetAlarm.Minute = 25;
    SetAlarm.Second = 18;
    RTCC_SetAlarm(&SetAlarm);
}

/******************************************************************************/
/* RTCC_Module
 *
 * The function controls the real time clock and calender.
/******************************************************************************/
unsigned char RTCC_Module(unsigned char state)
{
    unsigned char temp;
    unsigned char access;

    access = RTCC_Access(YES);
    
#ifdef ON
    #undef ON
    temp = RTCCONbits.ON;
    if(state)
    {
        RTCCONbits.ON = 1; // Turn on the RTCC module
    }
    else
    {
        RTCCONbits.ON = 0; // Turn off the RTCC module
    }
    #define ON 1
#else
    temp = RTCCONbits.ON;
    if(state)
    {
        RTCCONbits.ON = 1; // Turn on the RTCC module
    }
    else
    {
        RTCCONbits.ON = 0; // Turn off the RTCC module
    }
#endif
    if(!access)
    {
        RTCC_Access(NO);
    }
    return temp;
}

/******************************************************************************/
/* RTCC_Status
 *
 * The function returns the status of the RTCC clock.
/******************************************************************************/
unsigned char RTCC_Status(void)
{
    return RTCCONbits.RTCCLKON;
}

/******************************************************************************/
/* RTCC_Access
 *
 * The function allows the user to update the time and date by unlocking the
 *   RTC.
/******************************************************************************/
unsigned char RTCC_Access(unsigned char Yes_No)
{
    unsigned char temp;    
    temp = RTCCONbits.RTCWREN;
    
    if((temp && (Yes_No == YES)) || ((temp == 0) && (Yes_No == NO)))
    {
        return temp;
    }
    
    if(Yes_No == YES )
    {
        SYSKEY = 0xAA996655; // write first unlock key to SYSKEY
        SYSKEY = 0x556699AA; // write second unlock key to SYSKEY
        RTCCONSET = 0x0008; // set RTCWREN in RTCCONSET
    }
    else
    {
        RTCCONCLR = 0x0008;
    }
    return temp;
}

/******************************************************************************/
/* RTCC_Read
 *
 * The function reads the RTC.
/******************************************************************************/
void RTCC_Read(TIMETYPE* Time)
{
    unsigned long temp =0;

    temp = RTCTIME;
    Time->Second = MSC_BCDtoHEX(((temp & 0x00007F00) >> 8));
    Time->Minute = MSC_BCDtoHEX(((temp & 0x007F0000)>> 16));
    Time->Hour_24 = MSC_BCDtoHEX(((temp & 0x3F000000) >> 24));
    RTCC_CalculateHours(Time,MILITARY);
    
    temp = RTCDATE;
    Time->Weekday = MSC_BCDtoHEX(temp & 0x00000007);
    Time->Date = (MSC_BCDtoHEX(((temp & 0x00003F00) >> 8))) + 1;
    Time->Month = (MSC_BCDtoHEX(((temp & 0x001F0000) >> 16))) + 1;
    Time->Year = (MSC_BCDtoHEX(((temp & 0xFF000000) >> 24)) + 2000);           
}

/******************************************************************************/
/* RTCC_Write
 *
 * The function allows the user to update the time and date by unlocking the
 *   RTC.
/******************************************************************************/
void RTCC_Write(TIMETYPE* Time)
{
    unsigned long temp =0;
    unsigned char status;
    
    RTCC_Access(TRUE);
    status = RTCC_Module(OFF);
    while(RTCC_Status()) // wait till the clock stops
    {
        RTCC_Module(OFF);
    }
   
    temp = MSC_HEXtoBCD( Time->Year - 2000) << 24;
    temp |= MSC_HEXtoBCD( (unsigned int) (Time->Month - 1)) << 16;
    temp |= MSC_HEXtoBCD( (unsigned int) (Time->Date - 1)) << 8;
    temp |= MSC_HEXtoBCD( (unsigned int) Time->Weekday);
    RTCDATE = temp;
    
    RTCC_CalculateHours(Time,CIVILIAN);
    temp = MSC_HEXtoBCD( (unsigned int) Time->Hour_24) << 24;
    temp |= MSC_HEXtoBCD( (unsigned int) Time->Minute) << 16;
    temp |= MSC_HEXtoBCD( (unsigned int) Time->Second) << 8;
    RTCTIME = temp;

    if(status)
    {
        RTCC_Module(ON);
    }
}

/******************************************************************************/
/* RTCC_SetAlarmFuture
 *
 * The function allows the user to set the alarm a certain number of days,
 *  hours, minutes, and seconds in the future.
/******************************************************************************/
void RTCC_SetAlarmFuture(short days, char hours, char minutes, char seconds)
{
    short temp;
    char month,years;
    char AccumMonths= 0;
    
    RTCC_Read(&CurrentTime);
    
    /* seconds */
    temp = seconds + CurrentTime.Second;
    if(temp > 59)
    {
        temp -= 60;
        minutes++;
    }
    SetAlarm.Second = temp;
    
    /* minutes */
    temp = minutes + CurrentTime.Minute;
    if(temp > 59)
    {
        temp -= 60;
        hours++;
    }
    SetAlarm.Minute = temp;
    
    /* hours */
    temp = hours + CurrentTime.Hour_24;
    if(temp > 23)
    {
        temp -= 24;
        days++;
    }
    SetAlarm.Hour_24 = temp;
    RTCC_CalculateHours(&SetAlarm,CIVILIAN);
    
    month = CurrentTime.Month;
    while(days >= 0)
    {
        switch (month)
        {
            case January:
                if((CurrentTime.Date + days) > DAYS_January) 
                {
                    AccumMonths++;
                }
                else
                {
                    SetAlarm.Date = CurrentTime.Date + days;
                }
                days -= DAYS_January;
                break;
            case February:
                if(RTCC_LeapYear(CurrentTime.Year + AccumMonths / 12))
                {
                    if((CurrentTime.Date + days) > (DAYS_February + 1))  
                    {
                        AccumMonths++;
                    }
                    else
                    {
                        SetAlarm.Date = CurrentTime.Date + days;
                    }
                    days -= DAYS_February;
                }
                else
                {
                    if((CurrentTime.Date + days) > DAYS_February) 
                    {
                        AccumMonths++;
                    }
                    else
                    {
                        SetAlarm.Date = CurrentTime.Date + days;
                    }
                    days -= DAYS_February + 1;
                }               
                break;
            case March:
                if((CurrentTime.Date + days) > DAYS_March) 
                {
                    AccumMonths++;
                }
                else
                {
                    SetAlarm.Date = CurrentTime.Date + days;
                }
                days -= DAYS_March;
                break;
            case April:
                if((CurrentTime.Date + days) > DAYS_April) 
                {
                    AccumMonths++;
                }
                else
                {
                    SetAlarm.Date = CurrentTime.Date + days;
                }
                days -= DAYS_April;
                break;
            case May:
                if((CurrentTime.Date + days) > DAYS_May) 
                {
                    AccumMonths++;
                }
                else
                {
                    SetAlarm.Date = CurrentTime.Date + days;
                }
                days -= DAYS_May;
                break;
            case June:
                if((CurrentTime.Date + days) > DAYS_June) 
                {
                    AccumMonths++;
                }
                else
                {
                    SetAlarm.Date = CurrentTime.Date + days;
                }
                days -= DAYS_June;
                break;
            case July:
                if((CurrentTime.Date + days) > DAYS_July) 
                {
                    AccumMonths++;
                }
                else
                {
                    SetAlarm.Date = CurrentTime.Date + days;
                }
                days -= DAYS_July;
                break;
            case August:
                if((CurrentTime.Date + days) > DAYS_August) 
                {
                    AccumMonths++;
                }
                else
                {
                    SetAlarm.Date = CurrentTime.Date + days;
                }
                days -= DAYS_August;
                break;
            case September:
                if((CurrentTime.Date + days) > DAYS_September) 
                {
                    AccumMonths++;
                }
                else
                {
                    SetAlarm.Date = CurrentTime.Date + days;
                }
                days -= DAYS_September;
                break;
            case October:
                if((CurrentTime.Date + days) > DAYS_October) 
                {
                    AccumMonths++;
                }
                else
                {
                    SetAlarm.Date = CurrentTime.Date + days;
                }
                days -= DAYS_October;
                break;
            case November:
                if((CurrentTime.Date + days) > DAYS_November) 
                {
                    AccumMonths++;
                }
                else
                {
                    SetAlarm.Date = CurrentTime.Date + days;
                }
                days -= DAYS_November;
                break;
            default:
                if((CurrentTime.Date + days) > DAYS_March) 
                {
                    AccumMonths++;
                }
                else
                {
                    SetAlarm.Date = CurrentTime.Date + days;
                }
                days -= DAYS_March;
                break;
                
                
        }
    }
        
    temp = CurrentTime.Month + AccumMonths;
    while(temp >12)
    {
        temp -= 12;
        years++;
    }
    SetAlarm.Month = CurrentTime.Month + temp;
    SetAlarm.Year = SetAlarm.Year + years;
    
    RTCC_SetAlarm(&SetAlarm);
    RTCC_Alarm(ON, TIME, YEAR);
}

/******************************************************************************/
/* RTCC_SetAlarm
 *
 * The function sets the alarm time.
/******************************************************************************/
void RTCC_SetAlarm(TIMETYPE* Time)
{
    unsigned char status1, status2;
    unsigned long temp =0;
    
    status1 = RTCC_Alarm(OFF,NO_CHANGE ,NO_CHANGE);
    while(RTCALRMbits.ALRMSYNC);
   
    RTCC_CalculateHours(Time,CIVILIAN);
    temp = MSC_HEXtoBCD( (unsigned int) Time->Hour_24) << 24;
    temp |= MSC_HEXtoBCD( (unsigned int) Time->Minute) << 16;
    temp |= MSC_HEXtoBCD( (unsigned int) Time->Second) << 8;
    ALRMTIME = temp;
            
    temp = MSC_HEXtoBCD( (unsigned int) (Time->Month - 1)) << 16;
    temp |= MSC_HEXtoBCD( (unsigned int) (Time->Date - 1)) << 8;
    ALRMDATE = temp;
    
    Time->Year = SetAlarm.Year;
    
    if(status1)
    {
        RTCC_Alarm(ON, NO_CHANGE, NO_CHANGE);
    }
    if(status2)
    {
        RTCC_Module(ON);
    }
}

/******************************************************************************/
/* RTCC_SetAlarm
 *
 * The function sets the alarm time and alarm mode.
/******************************************************************************/
unsigned char RTCC_CheckAlarmYear(void)
{
    RTCC_Read(&CurrentTime);
    RTCC_ReadAlarm(&CurrentAlarm);
    if(CurrentTime.Year == CurrentAlarm.Year)
    {
        return TRUE;
    }
    return FALSE;
}

/******************************************************************************/
/* RTCC_SetAlarm
 *
 * The function sets the alarm time and alarm mode.
/******************************************************************************/
unsigned char RTCC_ReadAlarm(TIMETYPE* Time)
{
    unsigned long temp =0;
    
    temp = ALRMTIME;
    Time->Second = MSC_BCDtoHEX(((temp & 0x00007F00) >> 8));
    Time->Minute = MSC_BCDtoHEX(((temp & 0x007F0000)>> 16));
    Time->Hour_24 = MSC_BCDtoHEX(((temp & 0x3F000000) >> 24));
    RTCC_CalculateHours(Time,MILITARY);
    
    temp = ALRMDATE;
    Time->Weekday = MSC_BCDtoHEX(temp & 0x00000007);
    Time->Date = (MSC_BCDtoHEX(((temp & 0x00003F00) >> 8))) + 1;
    Time->Month = (MSC_BCDtoHEX(((temp & 0x001F0000) >> 16))) + 1;
    Time->Year = SetAlarm.Year;
    
    return RTCALRMbits.ALRMEN;
}

/******************************************************************************/
/* RTCC_Alarm
 *
 * The function controls the alarm.
/******************************************************************************/
unsigned char RTCC_Alarm(unsigned char state, unsigned char type, unsigned char resolution)
{
    unsigned char status;
    
    status = RTCALRMbits.ALRMEN;
    while(RTCALRMbits.ALRMSYNC);
    
    if(state)
    {
        RTCALRMbits.ALRMEN = 1; // Alarm is enabled
        RTCC_AlarmInterrupts(ON);
    }
    else
    {
        RTCALRMbits.ALRMEN = 0; // Alarm is disabled
        RTCC_AlarmInterrupts(OFF);
    }
    
    if(type == NO_CHANGE || resolution == NO_CHANGE)
    {
        return status;
    }
    if(type == TIME)
    {
        RTCALRMbits.CHIME = 0;
    }
    else
    {
        RTCALRMbits.CHIME = 1;
    }
        
    if(resolution == HALF_SECOND)
    {
        RTCALRMbits.AMASK = 0b0000;
    }
    else if(resolution == SECOND)
    {
        RTCALRMbits.AMASK = 0b0001;
    }
    else if(resolution == SECONDS_10)
    {
        RTCALRMbits.AMASK = 0b0010;
    }
    else if(resolution == MINUTE)
    {
        RTCALRMbits.AMASK = 0b0011;
    }
    else if(resolution == MINUTES_10)
    {
        RTCALRMbits.AMASK = 0b0100;
    }
    else if(resolution == HOUR)
    {
        RTCALRMbits.AMASK = 0b0101;
    }
    else if(resolution == DAY)
    {
        RTCALRMbits.AMASK = 0b0110;
    }
    else if(resolution == WEEK)
    {
        RTCALRMbits.AMASK = 0b0111;
    }
    else if(resolution == MONTH)
    {
        RTCALRMbits.AMASK = 0b1000;
    }
    else
    {
        RTCALRMbits.AMASK = 0b1001;
    }
    return status;
}

/******************************************************************************/
/* RTCC_Alarm
 *
 * The function controls the alarm.
/******************************************************************************/
unsigned char RTCC_AlarmInterrupts(unsigned char state)
{
    if(state)
    {
        IFS0bits.RTCCIF = 0;
        IEC0bits.RTCCIE = 1;
    }
    else
    {
        IEC0bits.RTCCIE = 0;
    }  
}

/******************************************************************************/
/* RTCC_StartTiming
 *
 * The function logs the time to start the timing difference.
/******************************************************************************/
void RTCC_StartTiming(void)
{
    RTCC_Read(&StartTime);
}

/******************************************************************************/
/* RTCC_StopTiming
 *
 * The function logs the time to end the timing difference.
/******************************************************************************/
void RTCC_StopTiming(void)
{
    RTCC_Read(&EndTime);
    RTCC_TimingDiff();
}

/******************************************************************************/
/* RTCC_LeapYear
 *
 * The function return true if the input year is a leap year.
/******************************************************************************/
unsigned char RTCC_LeapYear(short year)
{
    if(year % 4 == 0)
    {
        return TRUE;
    }
    return FALSE;
}

/******************************************************************************/
/* RTCC_StopTiming
 *
 * The function logs the time to end the timing difference.
/******************************************************************************/
void RTCC_TimingDiff(void)
{
    char temp;
    char month,StartMonth;
    short year;
    
    TIMETYPE TempTime;
    MSC_BufferCopy(&EndTime, &TempTime, sizeof(TIMETYPE));
    
    DiffTime.Days = 0;
    
    /* seconds */
    temp = TempTime.Second - StartTime.Second;
    if(temp < 0)
    {        
        TempTime.Minute--;
        TempTime.Second += 60;
    }
    DiffTime.Seconds = TempTime.Second - StartTime.Second;
    
    /* minutes */
    if(TempTime.Minute < 0)
    {
        TempTime.Hour_24--;
        TempTime.Minute += 60;
    }
    temp = TempTime.Minute - StartTime.Minute;
    if(temp < 0)
    {
        TempTime.Hour_24--;
        TempTime.Minute += 60;
    }
    DiffTime.Minutes = TempTime.Minute - StartTime.Minute;
    
    /* hours */
    if(TempTime.Hour_24 < 0)
    {
        TempTime.Date--;
        DiffTime.Days--;
        TempTime.Hour_24 += 24;
    }
    temp = TempTime.Hour_24 - StartTime.Hour_24;
    if(temp < 0)
    {
        TempTime.Date--;
        DiffTime.Days--;
        TempTime.Hour_24 += 24;
    }
    DiffTime.Hours= TempTime.Hour_24 - StartTime.Hour_24;
    
    /* Days */
    StartMonth = StartTime.Month;
    if(StartTime.Month == EndTime.Month && StartTime.Year == EndTime.Year)
    {
        DiffTime.Days += EndTime.Date - StartTime.Date;
    }
    else
    {
        /* first partial month */
        month = StartTime.Month;
        switch (month)
        {
            case January:
                DiffTime.Days += DAYS_January - StartTime.Date;  
                break;
            case February:
                if(RTCC_LeapYear(year))
                {
                    DiffTime.Days += (DAYS_February +1) - StartTime.Date;
                }
                else
                {
                    DiffTime.Days += DAYS_February - StartTime.Date;
                }
                break;
            case March:
                DiffTime.Days += DAYS_March - StartTime.Date;  
                break;
            case April:
                DiffTime.Days += DAYS_April - StartTime.Date;  
                break;
            case May:
                DiffTime.Days += DAYS_May - StartTime.Date;  
                break;
            case June:
                DiffTime.Days += DAYS_June - StartTime.Date;  
                break;
            case July:
                DiffTime.Days += DAYS_July - StartTime.Date;  
                break;
            case August:
                DiffTime.Days += DAYS_August - StartTime.Date;  
                break;
            case September:
                DiffTime.Days += DAYS_September - StartTime.Date;  
                break;
            case October:
                DiffTime.Days += DAYS_October - StartTime.Date;  
                break;
            case November:
                DiffTime.Days += DAYS_November - StartTime.Date;  
                break;
            default:
                DiffTime.Days += DAYS_December - StartTime.Date;  
                break;
        }
        
        /* full months */
        StartMonth = StartTime.Month + 1;
        if(TempTime.Month > December)
        {
            StartMonth = January;
            TempTime.Year--;
        } 
        for(year = StartTime.Year; year <= TempTime.Year; year++)
        {
            for(month = StartMonth;month <=12;month++)
            {
                if(year == TempTime.Year && month == EndTime.Month)
                {
                    break;
                }
                switch (month)
                {
                    case January:
                        DiffTime.Days += DAYS_January;  
                        break;
                    case February:
                        if(RTCC_LeapYear(year))
                        {
                            DiffTime.Days += (DAYS_February +1);
                        }
                        else
                        {
                            DiffTime.Days += DAYS_February;
                        }
                        break;
                    case March:
                        DiffTime.Days += DAYS_March;  
                        break;
                    case April:
                        DiffTime.Days += DAYS_April;
                        break;
                    case May:
                        DiffTime.Days += DAYS_May; 
                        break;
                    case June:
                        DiffTime.Days += DAYS_June; 
                        break;
                    case July:
                        DiffTime.Days += DAYS_July;  
                        break;
                    case August:
                        DiffTime.Days += DAYS_August; 
                        break;
                    case September:
                        DiffTime.Days += DAYS_September;
                        break;
                    case October:
                        DiffTime.Days += DAYS_October;
                        break;
                    case November:
                        DiffTime.Days += DAYS_November;  
                        break;
                    default:
                        DiffTime.Days += DAYS_December;  
                        break; 
                }
            }
            StartMonth = January;
        }
        
        /* last partial month */
        DiffTime.Days += EndTime.Date;
    }        
    DiffTime.TotalSeconds =  DiffTime.Seconds + DiffTime.Minutes * 60.0 + DiffTime.Hours * 60.0 * 60.0 + DiffTime.Days * 24.0 * 60.0 * 60.0;
    DiffTime.TotalMinutes = DiffTime.TotalSeconds / 60.0;
    DiffTime.TotalHours = DiffTime.TotalMinutes / 60.0;
    DiffTime.TotalDays = DiffTime.TotalHours / 24.0;
    DiffTime.TotalWeeks = DiffTime.TotalDays / 7.0;
    DiffTime.TotalMonths = DiffTime.TotalDays / (365.25/12.0);
    DiffTime.TotalYears = DiffTime.TotalDays / (365.25);      
}

    
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/