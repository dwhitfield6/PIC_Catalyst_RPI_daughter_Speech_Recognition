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
/* Files to Include                                                           */
/******************************************************************************/
#ifndef RTCC_H
#define	RTCC_H

#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "USER.h"

/******************************************************************************/
/* Structures                                                                 */
/******************************************************************************/
typedef struct _time
{
    char Second;
    char Minute;
    char Hour_24;
    char Hour_12;
    char PM_nAM;
    char Weekday;
    char Date;
    char Month;
    short Year;
}TIMETYPE;

typedef struct _difftime
{
    char Seconds;
    char Minutes;
    char Hours;
    long Days;
    double TotalSeconds;
    double TotalMinutes;
    double TotalHours;
    double TotalDays;
    double TotalWeeks;
    double TotalMonths;
    double TotalYears;
}DIFFTIMETYPE;

/******************************************************************************/
/* Secondary oscillator startup timeout counts                                */
/******************************************************************************/
#define RTCC_OSCILLATOR_TIMEROUT 100000
        
/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define MILITARY    1
#define CIVILIAN    2
#define PM          1
#define AM          0
#define NO_CHANGE   0
#define TIME        12
#define INTERVAL    13
#define HALF_SECOND 2
#define SECOND      3
#define SECONDS_10  4
#define MINUTE      5
#define MINUTES_10  6
#define HOUR        7
#define DAY         8
#define WEEK        9
#define MONTH       10
#define YEAR        11

/******************************************************************************/
/* Time Macros                                                                */
/******************************************************************************/
#define January 1
#define February 2
#define March 3
#define April 4
#define May 5
#define June 6
#define July 7
#define August 8
#define September 9
#define October 10
#define November 11
#define December 12

#define Sunday 0
#define Monday 1
#define Tuesday 2
#define Wednesday 3
#define Thursday 4
#define Friday 5
#define Saturday 6

/******************************************************************************/
/* Month days                                                                 */
/******************************************************************************/
#define DAYS_January        31
#define DAYS_February       28
#define DAYS_March          31
#define DAYS_April          30
#define DAYS_May            31
#define DAYS_June           30
#define DAYS_July           31
#define DAYS_August         31
#define DAYS_September      30
#define DAYS_October        31
#define DAYS_November       30
#define DAYS_December       31

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
extern TIMETYPE StartTime;
extern TIMETYPE EndTime;
extern DIFFTIMETYPE DiffTime;
extern TIMETYPE CurrentTime;
extern TIMETYPE SetTime;
extern TIMETYPE CurrentAlarm;
extern TIMETYPE SetAlarm;

/******************************************************************************/
/* Macro Functions                                                            */
/******************************************************************************/

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
void InitRTCC(void);
unsigned char RTCC_ConfigOscillator(void);
void RTCC_ClearTime(TIMETYPE* Time);
void RTCC_CalculateHours(TIMETYPE* Time, unsigned char type);
void RTCC_SetDefaultTime(void);
void RTCC_SetDefaultAlarm(void);
unsigned char RTCC_Module(unsigned char state);
unsigned char RTCC_Status(void);
unsigned char RTCC_Access(unsigned char Yes_No);
void RTCC_Read(TIMETYPE* Time);
void RTCC_Write(TIMETYPE* Time);
void RTCC_SetAlarmFuture(short days, char hours, char minutes, char seconds);
void RTCC_SetAlarm(TIMETYPE* Time);
unsigned char RTCC_CheckAlarmYear(void);
unsigned char RTCC_ReadAlarm(TIMETYPE* Time);
unsigned char RTCC_Alarm(unsigned char state, unsigned char type, unsigned char resolution);
unsigned char RTCC_AlarmInterrupts(unsigned char state);
void RTCC_StartTiming(void);
void RTCC_StopTiming(void);
unsigned char RTCC_LeapYear(short year);
void RTCC_TimingDiff(void);


#endif	/* RTCC_H */