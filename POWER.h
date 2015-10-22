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
#ifndef POWER_H
#define	POWER_H

#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "USER.h"

/******************************************************************************/
/* Structures                                                                 */
/******************************************************************************/
typedef struct rails
{
    double VCAP;                    // Voltage of rail VCAP
    double Volt5_0;                 // Voltage of 5 volt rail
    double Volt3_3;                 // Voltage of 3.3 volt rail
    double Volt4_1;                 // Voltage of 4.1 volt rail
    double VIN;                     // Voltage of 4.1 volt rail
    double POT;                     // Voltage of 4.1 volt rail
    unsigned char Warn4_4;          // Warning VCAP is under 4.4 volts
    unsigned char Warn4_1;          // Warning VCAP is under 4.1 volts
    unsigned char WarnPOT;          // Warning VCAP is under the potentiometer voltage
    unsigned char WarnVIN;          // Warning VIN is under 9 volts
    unsigned char Supercap_state;   // Status of charging or discharging cap
    unsigned char DCDC_state;       // Status of DCDC converter for 5 volt rail
    unsigned char Rasp_state;       // Status of the raspberry pi voltage rail
    unsigned char Rasp_connected;   // Raspberry pi present detector
}RAILSTYPE;

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define CHARGING        1
#define DISCHARGING     0
#define RISING          1
#define FALLING         0

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
extern RAILSTYPE RailStatus;

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
inline void PWR_Charge(unsigned char action);
inline unsigned char PWR_ExternalInterrupt(unsigned char channel, unsigned char action);
inline unsigned char PWR_ExternalInterruptEdge(unsigned char channel, unsigned char edge);
inline void PWR_Volts5(unsigned char action);
inline void PWR_RASP(unsigned char action);
inline unsigned char PWR_RASP_SPIReady(void);
inline unsigned char PWR_RASP_PowerStatus(void);
inline unsigned char PWR_RASP_Connected(void);
void InitComparator(void);
void PWR_ReadComparator(void);
void PWR_StatusUpdate(void);
void InitLowPower(void);

#endif	/* POWER_H */