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
/* Files to Include                                                           */
/******************************************************************************/
#ifndef USER_H
#define	USER_H

#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

/******************************************************************************/
/* Firmware Version
/******************************************************************************/
#define CPU_VERSION    "63"
#define CPU_REVISION   "00"
#define CPU_ALPHA      ""                 /* use "" for no alpha.  */ 
#define CPU_BRANCH     "DW0a"             /* use "" for no branch. */

/******************************************************************************/
/* PCB board
 *
 * This code is used with boards:
 * 1. Catalyst_PCB_revA             (use configuration "PCB_A__pic32MX350F128L")
/******************************************************************************/
#define Catalyst_PCB_revA

/******************************************************************************/
/* PCB board Serial Number
 *
 * This is the PIC boards serial number.
/******************************************************************************/
#ifdef Catalyst_PCB_revA
    #define BOARD_SERIAL     "123456789-123456789"
#endif

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define ON 1
#define OFF 0
#define TRUE 1
#define FALSE 0
#define INPUT 1
#define OUTPUT 0
#define PASS 1
#define FAIL 0
#define YES 1
#define NO 0
#define k 1000.0

/******************************************************************************/
/* Pin Defines                                                                */
/******************************************************************************/

/************* Oscillator *************/
/* Connected to the secondary oscillator */
#define SOSCOTris	TRISBbits.TRISB4

/* Connected to the secondary oscillator */
#define SOSCITris	TRISAbits.TRISA4

/************* Red LED *************/
/* Connected to the red LED indicator */
#define RedLEDTris	TRISDbits.TRISD3
#define RedLED 0x00000008 //RD3

/************* RGB LED *************/
/* Connected to the RGB red LED indicator */
#define RGB_RedLEDTris	TRISDbits.TRISD2
#define RGB_RedLED 0x00000004 //RD2 used as OC3

/* Connected to the RGB Green LED indicator */
#define RGB_GreenLEDTris	TRISBbits.TRISB6
#define RGB_GreenLED 0x00000040 //RB6 used as OC2

/* Connected to the RGB Blue LED indicator */
#define RGB_BlueLEDTris	TRISDbits.TRISD1
#define RGB_BlueLED 0x00000002 //RD1 used as OC1

/************* Super Cap *************/
/* Connected to the power switch that charges the super capacitor */
#define ChargeCapTris	TRISDbits.TRISD9
#define ChargeCap 0x00000200 //RD9

/************* DCDC *************/
/* Connected to the DC to DC converters inhibit pin */
#define InhibitTris	TRISDbits.TRISD0
#define Inhibit 0x00000001 //RD0

/************* Raspberry pi *************/
/* Connected to the power switch that controls the raspberry pi */
#define RASP_ONTris	TRISAbits.TRISA5
#define RASP_ON 0x00000020 //RA5

#define RASP_ON2Tris	TRISAbits.TRISA14
#define RASP_ON2 0x00004000 //RA14

/* Connected to pull up pin to detect if a raspberry pi is mounted */
#define RASP_ConnectedTris	TRISGbits.TRISG7
#define RASP_Connected 0x00000080 //RG7

/* Connected to UART RX on Rasberry pi */
#define RASP_UART_TXTris	TRISCbits.TRISC4
#define RASP_UART_TX 0x00000010        // RC4 used as U1TX
#define RASP_UART_TX_Module 0b0011     // RPnR Value for U1TX

#define RASP_UART_TX2Tris	TRISCbits.TRISC3
#define RASP_UART_TX2 0x00000008        // RC3

/* Connected to UART TX on Rasberry pi */
#define RASP_UART_RXTris	TRISBbits.TRISB9
#define RASP_UART_RX 0x00000200        // RB9 used as U1RX
#define RASP_UART_RX_Pin 0b0101        // U1RX is pin RPB9

/* Connected to SPI MOSI on Rasberry pi */
#define RASP_SPI_MOSITris	TRISBbits.TRISB10
#define RASP_SPI_MOSI 0x00000400        // RB10 used as SDO2
#define RASP_SPI_MOSI_Pin 0b0110        // SDO2 is pin RPB10 (master mode)

#define RASP_SPI_MOSI2Tris	TRISGbits.TRISG0
#define RASP_SPI_MOSI2 0x00000001        // RB10 used as SDO2
#define RASP_SPI_MOSI_Module 0b1100     // SDI2 is pin RPG0 (slave mode)

/* Connected to SPI MISO on Rasberry pi */
#define RASP_SPI_MISOTris	TRISDbits.TRISD15
#define RASP_SPI_MISO 0x00008000        // RD15 used as SDI2
#define RASP_SPI_MISO_Module 0b1011     // SDI2 is pin RPD15 (master mode)
#define RASP_SPI_MISO_Pin 0b0110        // SDO2 is pin RPD15 (slave mode)

/* Connected to SPI CLK on Rasberry pi */
#define RASP_SPI_CLKTris	TRISGbits.TRISG6
#define RASP_SPI_CLK 0x00000040        // RG6 used as SCK2

/* Connected to SPI CS on Rasberry pi */
#define RASP_SPI_CSTris	TRISAbits.TRISA0
#define RASP_SPI_CS 0x00000001        // RA0

#define RASP_SPI_CS2Tris	TRISEbits.TRISE8
#define RASP_SPI_CS2 0x00000100        // RE8 used as SS2
#define RASP_SPI_CS2_Pin 0b0110        // SS2 is pin RPE8
#define RASP_SPI_CS2_Module 0b1101     // SS2 is pin RPE8

#define RASP_SPI_CS3Tris	TRISEbits.TRISE9
#define RASP_SPI_CS3 0x00000200        // RE9

/* Connected Raspberry pi GPIO 12 used to get the Raspberry pi to clock the SPI bus */
#define RASP_Slave_Need_ClockingTris	TRISGbits.TRISG8
#define RASP_Slave_Need_Clocking 0x00000100     // RG8

/* Connected Raspberry pi GPIO 18 used to indicate that the SPI bus is configured */
#define RASP_SPI_ConfiguredTris	TRISBbits.TRISB11
#define RASP_SPI_Configured 0x00000800     // RB11

/************* Voltage rails for ADC read *************/
/* Connected to the voltage divider to read the 5 volt rail */
#define ADC_Volt5_0Tris	TRISBbits.TRISB5
#define ADC_Volt5_0 0x00000020    //RB5 used as AN5
#define ADC_Volt5_0_AN 5          //AN5

/* Connected to the voltage divider to read the VIN rail */
#define ADC_VINTris	TRISBbits.TRISB4
#define ADC_VIN 0x00000010    //RB4 used as AN4
#define ADC_VIN_AN 4          //AN4

/* Connected to the voltage divider to read the 3.3 volt rail */
#define ADC_Volt3_3Tris	TRISBbits.TRISB3
#define ADC_Volt3_3 0x00000008    //RB3 used as AN3
#define ADC_Volt3_3_AN 3          //AN3

/* Connected to the voltage divider to read the 4.1 volt rail */
#define ADC_Volt4_1Tris	TRISBbits.TRISB2
#define ADC_Volt4_1 0x00000004    //RB2 used as AN2
#define ADC_Volt4_1_AN 2          //AN2

/* Connected to the voltage divider to read the VCAP rail */
#define ADC_VCAPTris	TRISBbits.TRISB7
#define ADC_VCAP 0x00000080    //RB7 used as AN7
#define ADC_VCAP_AN 7          //AN7

/* Connected to the positive reference voltage */
#define ADC_VREF_posTris	TRISAbits.TRISA10
#define ADC_VREF_pos 0x00000400    //RA10 used as VREF+

/* Connected to the negative reference voltage */
#define ADC_VREF_negTris	TRISAbits.TRISA9
#define ADC_VREF_neg 0x00000200    //RA9 used as VREF-

/* Connected to the voltage divider to read the potentiometer voltage */
#define ADC_POTTris	TRISBbits.TRISB13
#define ADC_POT 0x00000020    //RB13 used as AN13
#define ADC_POT_AN 13          //AN13

/************* RS-232 UART channels *************/
/* Connected to the rs232 driver shutdown for the male DB9 */
#define RS232_MALE_SHUTDOWNTris	TRISAbits.TRISA7
#define RS232_MALE_SHUTDOWN 0x00000080  // RA7

/* Connected to the rs232 driver enable for the male DB9 */
#define RS232_MALE_ENABLETris	TRISDbits.TRISD7
#define RS232_MALE_ENABLE 0x00000080  // RD7

/* Connected to the rs232 driver shutdown for the female DB9 */
#define RS232_FEMALE_SHUTDOWNTris	TRISDbits.TRISD13
#define RS232_FEMALE_SHUTDOWN 0x00002000  // RD13

/* Connected to the rs232 driver enable for the female DB9 */
#define RS232_FEMALE_ENABLETris	TRISDbits.TRISD6
#define RS232_FEMALE_ENABLE 0x00000040  // RD6

/* Connected to the transmit of the JP2 male DB9 */
#define RS232_MALE_TXTris	TRISFbits.TRISF5
#define RS232_MALE_TX 0x00000020        // RF5 used as U2TX
#define RS232_MALE_TX_Module 0b0001     // RPnR Value for U2TX

/* Connected to the receive of the JP2 male DB9 */
#define RS232_MALE_RXTris	TRISFbits.TRISF2
#define RS232_MALE_RX 0x00000004        // RF2 used as U2RX
#define RS232_MALE_RX_Pin 0b1111        // U2RX is pin RPF2

/* Connected to the RTS of the JP2 male DB9 used as second TX */
#define RS232_MALE_RTSTris	TRISDbits.TRISD14
#define RS232_MALE_RTS 0x00004000        // RD14 used as U3TX
#define RS232_MALE_RTS_Module 0b0001     // RPnR Value for U3TX

/* Connected to the RTS of the JP2 male DB9 used as second RX */
#define RS232_MALE_CTSTris	TRISFbits.TRISF7
#define RS232_MALE_CTS 0x00000080        // RF7 used as U3RX
#define RS232_MALE_CTS_Pin 0b1111        // U3RX is pin RPF7


/* Connected to the transmit of the JP1 female DB9 */
#define RS232_FEMALE_TXTris	TRISFbits.TRISF12
#define RS232_FEMALE_TX 0x00001000        // RF12 used as U4TX
#define RS232_FEMALE_TX_Module 0b0010     // RPnR Value for U4TX

#define RS232_FEMALE_TX2Tris	TRISBbits.TRISB12
#define RS232_FEMALE_TX2 0x00001000       // RB12

/* Connected to the receive of the JP1 female DB9 */
#define RS232_FEMALE_RXTris	TRISGbits.TRISG9
#define RS232_FEMALE_RX 0x00000200        // RG9 used as U4RX
#define RS232_FEMALE_RX_Pin 0b0001        // U4RX is pin RPG9

/* Connected to the RTS of the JP1 female DB9 used as second TX */
#define RS232_FEMALE_RTSTris	TRISFbits.TRISF8
#define RS232_FEMALE_RTS 0x00000100        // RF8 used as U5TX
#define RS232_FEMALE_RTS_Module 0b0100     // RPnR Value for U5TX

/* Connected to the RTS of the JP1 female DB9 used as second RX */
#define RS232_FEMALE_CTSTris	TRISCbits.TRISC2
#define RS232_FEMALE_CTS 0x00000004        // RC2 used as U5RX
#define RS232_FEMALE_CTS_Pin 0b1100        // U5RX is pin RPC2

#define RS232_FEMALE_CTS2Tris	TRISCbits.TRISC1
#define RS232_FEMALE_CTS2 0x00000002        // RC1

/************* Relay *************/
/* Connected to the MOSFET that controls the relay */
#define RelayTris	TRISAbits.TRISA1
#define Relay 0x00000002  // RA1

/* Connected to the MOSFET that controls the relay */
#define Relay2Tris	TRISFbits.TRISF13
#define Relay2 0x00002000  // RF13

/************* Expander *************/
/* Connected to the Expansion header pin 6 */
#define Expand1_1Tris	TRISAbits.TRISA6
#define Expand1_1 0x00000040  // RA6

#define Expand1_2Tris	TRISGbits.TRISG1
#define Expand1_2 0x00000002  // RG1

/* Connected to the Expansion header pin 5 */
#define Expand2Tris	TRISFbits.TRISF4
#define Expand2 0x00000002  // RF4

/************* External Flash IC *************/
/* Connected to the external Flash IC IO pin 0 */
#define ExtFlash_IO_0Tris	TRISEbits.TRISE0
#define ExtFlash_IO_0 0x00000001  // RE0

/* Connected to the external Flash IC IO pin 1 */
#define ExtFlash_IO_1Tris	TRISEbits.TRISE1
#define ExtFlash_IO_1 0x00000002  // RE1

/* Connected to the external Flash IC IO pin 2 */
#define ExtFlash_IO_2Tris	TRISEbits.TRISE2
#define ExtFlash_IO_2 0x00000004  // RE2

/* Connected to the external Flash IC IO pin 3 */
#define ExtFlash_IO_3Tris	TRISEbits.TRISE3
#define ExtFlash_IO_3 0x00000008  // RE3

/* Connected to the external Flash IC IO pin 4 */
#define ExtFlash_IO_4Tris	TRISEbits.TRISE4
#define ExtFlash_IO_4 0x00000010  // RE4

/* Connected to the external Flash IC IO pin 5 */
#define ExtFlash_IO_5Tris	TRISEbits.TRISE5
#define ExtFlash_IO_5 0x00000020  // RE5

/* Connected to the external Flash IC IO pin 6 */
#define ExtFlash_IO_6Tris	TRISEbits.TRISE6
#define ExtFlash_IO_6 0x00000040  // RE6

/* Connected to the external Flash IC IO pin 7 */
#define ExtFlash_IO_7Tris	TRISEbits.TRISE7
#define ExtFlash_IO_7 0x00000080  // RE7

/* Connected to the external Flash IC RB (read busy) pin */
#define ExtFlash_RBTris	TRISGbits.TRISG13
#define ExtFlash_RB 0x00002000  // RG13

#define ExtFlash_RB2Tris	TRISGbits.TRISG12
#define ExtFlash_RB2 0x00001000  // RG12

/* Connected to the external Flash IC RE (read enable) pin */
#define ExtFlash_RETris	TRISDbits.TRISD5
#define ExtFlash_RE 0x00000020  // RD5

/* Connected to the external Flash IC CE (chip enable) pin */
#define ExtFlash_CETris	TRISDbits.TRISD11
#define ExtFlash_CE 0x00000800  // RD11

/* Connected to the external Flash IC CLE (command latch enable) pin */
#define ExtFlash_CLETris	TRISBbits.TRISB14
#define ExtFlash_CLE 0x00004000  // RB14

/* Connected to the external Flash IC ALE (address latch enable) pin */
#define ExtFlash_ALETris	TRISBbits.TRISB15
#define ExtFlash_ALE 0x00008000  // RB15

/* Connected to the external Flash IC WE (write enable) pin */
#define ExtFlash_WETris	TRISDbits.TRISD4
#define ExtFlash_WE 0x00000010  // RD4

/* Connected to the external Flash IC WP (write protect) pin */
#define ExtFlash_WPTris	TRISGbits.TRISG14
#define ExtFlash_WP 0x00004000  // RG14

/******* I2C bus 1 (connected to EEPROM and raspberry pi I2C only pins) *******/
/* Connected to I2C module 1 clock */
#define I2C_RASP_DRV_ClockTris	TRISGbits.TRISG2
#define I2C_RASP_DRV_Clock 0x00000004  // RG2

/* Connected to I2C module 1 data */
#define I2C_RASP_DRV_DataTris	TRISGbits.TRISG3
#define I2C_RASP_DRV_Data 0x00000008  // RG3

/******* I2C bus 2 (connected to EEPROM and raspberry pi I2C MUX pins) *******/
/* Connected to I2C module 2 clock */
#define I2C_RASP_GEN_ClockTris	TRISAbits.TRISA2
#define I2C_RASP_GEN_Clock 0x00000004  // RA2

/* Connected to I2C module 2 data */
#define I2C_RASP_GEN_DataTris	TRISAbits.TRISA3
#define I2C_RASP_GEN_Data 0x00000008  // RA3

/******* Comparator IC for monitoring voltage extremes on the rails *******/
/* Connected to comparator VIN_WARN warn */
#define COMP_VIN_WarnTris	TRISBbits.TRISB8
#define COMP_VIN_Warn 0x00000100  // RB8 used as INT2
#define COMP_VIN_Warn_INT 2
#define COMP_VIN_Warn_Pin 0b0010        // INT2 is pin RPB8

/* Connected to comparator VCAP_WARN1 warn */
#define COMP_VCAP_Warn1Tris	TRISFbits.TRISF6
#define COMP_VCAP_Warn1 0x00000040  // RF6 used as INT0
#define COMP_VCAP_Warn1_INT 0

/* Connected to comparator VCAP_WARN2 warn */
#define COMP_VCAP_Warn2Tris	TRISDbits.TRISD10
#define COMP_VCAP_Warn2 0x00000400  // RD10 used as INT3
#define COMP_VCAP_Warn2_INT 3
#define COMP_VCAP_Warn2_Pin 0b0110        // INT3 is pin RPD10

/* Connected to comparator VCAP_WARN3 warn */
#define COMP_VCAP_Warn3Tris	TRISFbits.TRISF3
#define COMP_VCAP_Warn3 0x00000008  // RF3 used as INT1
#define COMP_VCAP_Warn3_INT 1
#define COMP_VCAP_Warn3_Pin 0b1000        // INT1 is pin RPF3

/******************************************************************************/
/* Version variables                                                          */
/******************************************************************************/
extern const unsigned char Version[];
extern const unsigned char Revision[];
extern const unsigned char Alpha[];
extern const unsigned char Branch_Version[];
extern const unsigned char PIC_Board_Serial_Number[];

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
extern unsigned char Serial_Number[];

/******************************************************************************/
/* Macro Functions                                                            */
/******************************************************************************/

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
void Init_App(void);        
void Init_System (void);

#endif	/* USER_H */