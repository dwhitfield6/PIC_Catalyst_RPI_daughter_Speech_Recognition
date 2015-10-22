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
/* Contains functions for the I2C bus.
 *
 * Module 1 is for the Raspberry Pi driver install I2C bus described below.
 * Module 2 is for the General purpose I2C bus connected to memory and the
 *  Raspberry Pi's MUX pins.
 * 
 * Special note:
 * The ID EEPROM contains data that identifies the board, tells the B+ how the
 *  GPIOs need to be set up and what hardware is on the board. This allows the
 *  add-on board to be automatically identified and set up by the Pi software
 *  at boot time including loading all the necessary drivers.
 * 
 * The board contains a CAT24C256 on each I2C bus.
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "USER.h"
#include "I2C.h"
#include "SYSTEM.h"
#include "MISC.h"
#include "EXCEPTIONS.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/******************************************************************************/
/* Inline Functions
/******************************************************************************/

/******************************************************************************/
/* I2C_RASP_DRV_Start
 *
 * The function sends a start condition over I2C module 1.
/******************************************************************************/
inline void I2C_RASP_DRV_Start(void)
{
    I2C1CONbits.SEN = 1;
}

/******************************************************************************/
/* I2C_RASP_GEN_Start
 *
 * The function sends a start condition over I2C module 2.
/******************************************************************************/
inline void I2C_RASP_GEN_Start(void)
{
    I2C2CONbits.SEN = 1;
}

/******************************************************************************/
/* I2C_RASP_DRV_Stop
 *
 * The function sends a stop condition over I2C module 1.
/******************************************************************************/
inline void I2C_RASP_DRV_Stop(void)
{
    I2C1CONbits.PEN = 1;
}

/******************************************************************************/
/* I2C_RASP_GEN_Stop
 *
 * The function sends a stop condition over I2C module 2.
/******************************************************************************/
inline void I2C_RASP_GEN_Stop(void)
{
    I2C2CONbits.PEN = 1;
}

/******************************************************************************/
/* I2C_RASP_DRV_Restart
 *
 * The function sends a restart condition over I2C module 1.
/******************************************************************************/
inline void I2C_RASP_DRV_Restart(void)
{
    I2C1CONbits.RSEN = 1;
}

/******************************************************************************/
/* I2C_RASP_GEN_Restart
 *
 * The function sends a restart condition over I2C module 2.
/******************************************************************************/
inline void I2C_RASP_GEN_Restart(void)
{
    I2C2CONbits.RSEN = 1;
}

/******************************************************************************/
/* I2C_RASP_DRV_Ack
 *
 * The function sends an ACK or a NACK restart condition over I2C module 1.
/******************************************************************************/
inline void I2C_RASP_DRV_Ack(unsigned char state)
{
    if(state == ACK)
    {
        I2C1CONbits.ACKDT = 0; // ACK is sent
    }
    else
    {
        I2C1CONbits.ACKDT = 1; // NACK is sent
    }
    I2C1CONbits.ACKEN = 1;
}

/******************************************************************************/
/* I2C_RASP_GEN_Ack
 *
 * The function sends an ACK or a NACK restart condition over I2C module 2.
/******************************************************************************/
inline void I2C_RASP_GEN_Ack(unsigned char state)
{
    if(state == ACK)
    {
        I2C2CONbits.ACKDT = 0; // ACK is sent
    }
    else
    {
        I2C2CONbits.ACKDT = 1; // NACK is sent
    }
    I2C2CONbits.ACKEN = 1;
}

/******************************************************************************/
/* I2C_RASP_DRV_Address
 *
 * The function sets the address when in slave mode.
/******************************************************************************/
inline unsigned char I2C_RASP_DRV_Address(unsigned char address, unsigned char Read_Write)
{
    unsigned char temp;
    
    if(Read_Write == WRITE)
    {
        temp = 0;
    }
    else
    {
        temp = 1;
    }
    I2C1ADD = (address << 1) + temp;
    return (address << 1) + temp;
}

/******************************************************************************/
/* I2C_RASP_GEN_Address
 *
 * The function sets the address.
/******************************************************************************/
inline unsigned char I2C_RASP_GEN_Address(unsigned char address, unsigned char Read_Write)
{
    unsigned char temp;
    
    if(Read_Write == WRITE)
    {
        temp = 0;
    }
    else
    {
        temp = 1;
    }
    I2C2ADD = (address << 1) + temp;
    return (address << 1) + temp;
}

/******************************************************************************/
/* Functions
/******************************************************************************/

/******************************************************************************/
/* InitI2C
 *
 * The function initializes the I2C module.
/******************************************************************************/
void InitI2C(void)
{
    I2C_RASP_DRV_Module(OFF);
    I2C_RASP_GEN_Module(OFF);
    I2C1CONbits.SIDL = 0; // Continue module operation in Idle mode
    I2C2CONbits.SIDL = 0; // Continue module operation in Idle mode
    I2C1CONbits.A10M = 0; // I2CxADD register is a 7-bit slave address
    I2C2CONbits.A10M = 0; // I2CxADD register is a 7-bit slave address
    IPC8bits.I2C1IP = 3; // interrupt priority is 3
    IPC8bits.I2C1IS = 1; // interrupt sub-priority is 1
    IPC9bits.I2C2IP = 3; // interrupt priority is 3
    IPC9bits.I2C2IS = 1; // interrupt sub-priority is 1
    I2C_RASP_DRV_SetParameters(_100kHz);
    I2C_RASP_GEN_SetParameters(_100kHz);
    I2C_RASP_DRV_CTP();
    I2C_RASP_GEN_CTP();
    I2C_RASP_DRV_Module(ON);
    I2C_RASP_GEN_Module(ON);
    I2C_RASP_DRV_Interrupt(OFF,OFF,ON);
    I2C_RASP_GEN_Interrupt(OFF,OFF,ON);
    I2C_RASP_DRV_EEPROMWriteSerialNumbers();
    I2C_RASP_GEN_EEPROMWriteString(0,"David Whitfield");
}

/******************************************************************************/
/* I2C_RASP_DRV_CTP
 *
 * This function "clocks through the problem" on the module 1 I2C bus.
/******************************************************************************/
void I2C_RASP_DRV_CTP(void)
{ 
    unsigned char i;
    unsigned char status = I2C_RASP_DRV_Module(OFF);
    
    I2C_RASP_DRV_ClockTris  = OUTPUT;
    for(i=0;i<20;i++)
    {
        LATG ^= I2C_RASP_DRV_Clock;
        MSC_DelayUS(10);
    }
    I2C_RASP_DRV_ClockTris  = INPUT;
    if(status)
    {
        I2C_RASP_DRV_Module(ON);
    }
}

/******************************************************************************/
/* I2C_RASP_GEN_CTP
 *
 * This function "clocks through the problem" on the module 2 I2C bus.
/******************************************************************************/
void I2C_RASP_GEN_CTP(void)
{ 
    unsigned char i;
    unsigned char status = I2C_RASP_GEN_Module(OFF);
    
    I2C_RASP_GEN_ClockTris  = OUTPUT;
    for(i=0;i<20;i++)
    {
        LATA ^= I2C_RASP_GEN_Clock;
        MSC_DelayUS(10);
    }
    I2C_RASP_GEN_ClockTris  = INPUT;
    if(status)
    {
        I2C_RASP_GEN_Module(ON);
    }
}

/******************************************************************************/
/* I2C_RASP_DRV_Interrupt
 *
 * This function controls the interrupts for the I2C bus connected to the 
 *  Raspberry Pi's driver install bus.
/******************************************************************************/
void I2C_RASP_DRV_Interrupt(unsigned char master, unsigned char slave, unsigned char bus)
{  
    if(master)
    {
        IEC1bits.I2C1MIE = 1; // enable the I2C master interrupt
    }
    else
    {
        IEC1bits.I2C1MIE = 0; // disable the I2C master interrupt
    }
    if(slave)
    {
        IEC1bits.I2C1SIE = 1; // enable the I2C slave interrupt
    }
    else
    {
        IEC1bits.I2C1SIE = 0; // disable the I2C slave interrupt
    }
    if(bus)
    {
        IEC1bits.I2C1BIE = 1; // enable the I2C bus collision interrupt
    }
    else
    {
        IEC1bits.I2C1BIE = 0; // disable the I2C bus collision interrupt
    }
}

/******************************************************************************/
/* I2C_RASP_GEN_Interrupt
 *
 * This function controls the interrupts for the I2C bus connected to the
 *  Raspberry Pi's general purpose I2C bus.
/******************************************************************************/
void I2C_RASP_GEN_Interrupt(unsigned char master, unsigned char slave, unsigned char bus)
{  
    if(master)
    {
        IEC1bits.I2C2MIE = 1; // enable the I2C master interrupt
    }
    else
    {
        IEC1bits.I2C2MIE = 0; // disable the I2C master interrupt
    }
    if(slave)
    {
        IEC1bits.I2C2SIE = 1; // enable the I2C slave interrupt
    }
    else
    {
        IEC1bits.I2C2SIE = 0; // disable the I2C slave interrupt
    }
    if(bus)
    {
        IEC1bits.I2C2BIE = 1; // enable the I2C bus collision interrupt
    }
    else
    {
        IEC1bits.I2C2BIE = 0; // disable the I2C bus collision interrupt
    }
}

/******************************************************************************/
/* I2C_RASP_DRV_Module
 *
 * This function controls the I2C module connected to the Raspberry Pi's
 *  driver install bus.
/******************************************************************************/
unsigned char I2C_RASP_DRV_Module(unsigned char state)
{
#ifdef ON
    #undef ON
    unsigned char status = I2C1CONbits.ON;
    if(state)
    {
        I2C1CONbits.ON = 1; // Turn on the I2C module
    }
    else
    {
        I2C1CONbits.ON = 0; // Turn off the I2C module
    }
    #define ON 1
#else
    unsigned char status = I2C1CONbits.ON;
    if(state)
    {
        I2C1CONbits.ON = 1; // Turn on the I2C module
    }
    else
    {
        I2C1CONbits.ON = 0; // Turn off the I2C module
    }
#endif    
    return status;
}

/******************************************************************************/
/* I2C_RASP_GEN_Module
 *
 * This function controls the I2C module connected to the Raspberry Pi's 
 *  general purpose I2C bus.
/******************************************************************************/
unsigned char I2C_RASP_GEN_Module(unsigned char state)
{
#ifdef ON
    #undef ON
    unsigned char status = I2C2CONbits.ON;
    if(state)
    {
        I2C2CONbits.ON = 1; // Turn on the I2C module
    }
    else
    {
        I2C2CONbits.ON = 0; // Turn off the I2C module
    }
    #define ON 1
#else
    unsigned char status = I2C2CONbits.ON;
    if(state)
    {
        I2C2CONbits.ON = 1; // Turn on the I2C module
    }
    else
    {
        I2C2CONbits.ON = 0; // Turn off the I2C module
    }
#endif    
    return status;
}

/******************************************************************************/
/* I2C_RASP_DRV_SetParameters
 *
 * This function configures the I2C module connected to the Raspberry Pi's
 *  driver install bus.
/******************************************************************************/
void I2C_RASP_DRV_SetParameters(unsigned long Clock)
{
    unsigned long temp;
    
    if(Clock >= 400000)
    {
        I2C1CONbits.DISSLW = 0; // Slew rate control enabled for High Speed mode (400 kHz)
    }
    else
    {
        I2C1CONbits.DISSLW = 1; // Slew rate control disabled for Standard Speed mode (100 kHz)
    }
    
    /* Set clock */
    temp = (unsigned long) MSC_Round((( 1.0 /(2*(double)Clock) - TPGD)* (double) PBCLK) - 2);
    if(temp < 2)
    {
        temp = 2;
    }
    I2C1BRG = temp;
}

/******************************************************************************/
/* I2C_RASP_GEN_SetParameters
 *
 * This function configures the I2C module connected to the Raspberry Pi's 
 *  general purpose I2C bus.
/******************************************************************************/
void I2C_RASP_GEN_SetParameters(unsigned long Clock)
{
    unsigned long temp;
    
    if(Clock >= 400000)
    {
        I2C2CONbits.DISSLW = 0; // Slew rate control enabled for High Speed mode (400 kHz)
    }
    else
    {
        I2C2CONbits.DISSLW = 1; // Slew rate control disabled for Standard Speed mode (100 kHz)
    }
    
    /* Set clock */
    temp = (unsigned long) MSC_Round((( 1.0 /(2*(double)Clock) - TPGD)* (double) PBCLK) - 2);
    if(temp < 2)
    {
        temp = 2;
    }
    I2C2BRG = temp;
}

/******************************************************************************/
/* I2C_RASP_DRV_Write
 *
 * This function writes a byte of data to the EEPROM IC on the Raspberry Pi
 *  driver install bus.
/******************************************************************************/
unsigned char I2C_RASP_DRV_Write(unsigned char data)
{
    unsigned long timer = 0;
   
    while(!I2C1STATbits.TBF) // wait for master transmit to start
    {
        I2C1STATbits.BCL = 0; // clear I2C collide status
        I2C1STATbits.IWCOL = 0; // clear I2C collide status
        timer++;
        if(timer > I2C_SEND_TIMEOUT)
        {
            return FAIL;
        }
        I2C1TRN = data; 
    }
    
    timer = 0;
    while(I2C1STATbits.TBF) // wait for master transmit to finish
    {
        timer++;
        if(timer > I2C_SEND_TIMEOUT)
        {
            return FAIL;
        }
    }
    if(I2C1STATbits.ACKSTAT)
    {
        return FAIL;
    }
    MSC_DelayUS(50);
    return PASS;
}

/******************************************************************************/
/* I2C_RASP_GEN_Write
 *
 * This function writes a byte of data to the EEPROM IC on the Raspberry Pi's 
 *  general purpose I2C bus.
/******************************************************************************/
unsigned char I2C_RASP_GEN_Write(unsigned char data)
{
    unsigned long timer = 0;
   
    while(!I2C2STATbits.TBF) // wait for master transmit to start
    {
        I2C2STATbits.BCL = 0; // clear I2C collide status
        I2C2STATbits.IWCOL = 0; // clear I2C collide status
        timer++;
        if(timer > I2C_SEND_TIMEOUT)
        {
            return FAIL;
        }
        I2C2TRN = data; 
    }
    
    timer = 0;
    while(I2C2STATbits.TBF) // wait for master transmit to finish
    {
        timer++;
        if(timer > I2C_SEND_TIMEOUT)
        {
            return FAIL;
        }
    }
    if(I2C2STATbits.ACKSTAT)
    {
        return FAIL;
    }
    MSC_DelayUS(50);
    return PASS;
}

/******************************************************************************/
/* I2C_RASP_DRV_Read
 *
 * This function reads a byte of data from the EEPROM IC on the Raspberry Pi
 *  driver install bus.
/******************************************************************************/
unsigned char I2C_RASP_DRV_Read(unsigned char* data)
{
    unsigned long timer;
    unsigned char dummy1;
    timer=0;
    dummy1 = I2C1RCV;
    while(!I2C1STATbits.RBF) // wait for acknowledge
    {
        timer++;
        if(timer > I2C_RECEIVE_TIMEOUT)
        {
            *data =0;
            return FAIL;
        }
    }
    *data = I2C1RCV;
    return PASS;
}

/******************************************************************************/
/* I2C_RASP_GEN_Read
 *
 * This function reads a byte of data from the EEPROM IC on the Raspberry Pi's
 *  general purpose I2C bus.
/******************************************************************************/
unsigned char I2C_RASP_GEN_Read(unsigned char* data)
{
    unsigned long timer;
    unsigned char dummy1;
    timer=0;
    dummy1 = I2C2RCV;
    while(!I2C2STATbits.RBF) // wait for acknowledge
    {
        timer++;
        if(timer > I2C_RECEIVE_TIMEOUT)
        {
            *data =0;
            return FAIL;
        }
    }
    *data = I2C2RCV;
    return PASS;
}

/******************************************************************************/
/* I2C_RASP_DRV_EEPROMWriteByte
 *
 * This function writes a byte of data to the EEPROM IC on the Raspberry Pi
 *  driver install bus.
/******************************************************************************/
unsigned char I2C_RASP_DRV_EEPROMWriteByte(unsigned short DataAddress, unsigned char data)
{
    unsigned char I2C_address;
    
    I2C_RASP_DRV_Start();
    I2C_address = I2C_RASP_DRV_Address(I2C_RASP_DRV_EEPROM_ADDRESS, WRITE);
    if(!I2C_RASP_DRV_Write(I2C_address))
    {
        return FAIL;
    }
    if(!I2C_RASP_DRV_Write((unsigned char)(DataAddress & 0x00FF)))
    {
        return FAIL;
    }
    if(!I2C_RASP_DRV_Write((unsigned char)((DataAddress & 0xFF00) >> 8)))
    {
        return FAIL;
    }
    if(!I2C_RASP_DRV_Write(data))
    {
        return FAIL;
    }
    I2C_RASP_DRV_Stop();
    MSC_DelayUS(10000);
    return PASS;
}

/******************************************************************************/
/* I2C_RASP_GEN_EEPROMWriteByte
 *
 * This function writes a byte of data to the EEPROM IC on the Raspberry Pi's
 *  general purpose I2C bus.
/******************************************************************************/
unsigned char I2C_RASP_GEN_EEPROMWriteByte(unsigned short DataAddress, unsigned char data)
{
    unsigned char I2C_address;
    
    I2C_RASP_GEN_Start();
    I2C_address = I2C_RASP_GEN_Address(I2C_RASP_GEN_EEPROM_ADDRESS, WRITE);
    if(!I2C_RASP_GEN_Write(I2C_address))
    {
        return FAIL;
    }
    if(!I2C_RASP_GEN_Write((unsigned char)(DataAddress & 0x00FF)))
    {
        return FAIL;
    }
    if(!I2C_RASP_GEN_Write((unsigned char)((DataAddress & 0xFF00) >> 8)))
    {
        return FAIL;
    }
    if(!I2C_RASP_GEN_Write(data))
    {
        return FAIL;
    }
    I2C_RASP_GEN_Stop();
    MSC_DelayUS(10000);
    return PASS;
}

/******************************************************************************/
/* I2C_RASP_DRV_EEPROMReadByte
 *
 * This function reads a byte of data from the EEPROM IC on the Raspberry Pi
 *  driver install bus.
/******************************************************************************/
unsigned char I2C_RASP_DRV_EEPROMReadByte(unsigned short DataAddress, unsigned char* data)
{
    unsigned char I2C_address;
    
    I2C_RASP_DRV_Start();
    I2C_address = I2C_RASP_DRV_Address(I2C_RASP_DRV_EEPROM_ADDRESS, WRITE);
    if(!I2C_RASP_DRV_Write(I2C_address))
    {
        return FAIL;
    }
    if(!I2C_RASP_DRV_Write((unsigned char)(DataAddress & 0x00FF)))
    {
        return FAIL;
    }
    if(!I2C_RASP_DRV_Write((unsigned char)((DataAddress & 0xFF00) >> 8)))
    {
        return FAIL;
    }
    I2C_RASP_DRV_Restart();
    I2C_address = I2C_RASP_DRV_Address(I2C_RASP_DRV_EEPROM_ADDRESS, READ);
    if(!I2C_RASP_DRV_Write(I2C_address))
    {
        return FAIL;
    }
    while(!I2C1CONbits.RCEN)
    {
        I2C1CONbits.RCEN = 1; //  Enables Receive mode for I2C, automatically cleared by module at end of 8-bit receive data byte
    }
    if(!I2C_RASP_DRV_Read(data))
    {
        return FAIL;
    }
    I2C_RASP_DRV_Ack(NACK);
    I2C_RASP_DRV_Stop();
    MSC_DelayUS(100);
    return PASS;
}

/******************************************************************************/
/* I2C_RASP_GEN_EEPROMReadByte
 *
 * This function reads a byte of data from the EEPROM IC on the Raspberry Pi
 *  general purpose I2C bus.
/******************************************************************************/
unsigned char I2C_RASP_GEN_EEPROMReadByte(unsigned short DataAddress, unsigned char* data)
{
    unsigned char I2C_address;
    
    I2C_RASP_GEN_Start();
    I2C_address = I2C_RASP_GEN_Address(I2C_RASP_DRV_EEPROM_ADDRESS, WRITE);
    if(!I2C_RASP_GEN_Write(I2C_address))
    {
        return FAIL;
    }
    if(!I2C_RASP_GEN_Write((unsigned char)(DataAddress & 0x00FF)))
    {
        return FAIL;
    }
    if(!I2C_RASP_GEN_Write((unsigned char)((DataAddress & 0xFF00) >> 8)))
    {
        return FAIL;
    }
    I2C_RASP_GEN_Restart();
    I2C_address = I2C_RASP_GEN_Address(I2C_RASP_DRV_EEPROM_ADDRESS, READ);
    if(!I2C_RASP_GEN_Write(I2C_address))
    {
        return FAIL;
    }
    while(!I2C2CONbits.RCEN)
    {
        I2C2CONbits.RCEN = 1; //  Enables Receive mode for I2C, automatically cleared by module at end of 8-bit receive data byte
    }
    if(!I2C_RASP_GEN_Read(data))
    {
        return FAIL;
    }
    I2C_RASP_GEN_Ack(NACK);
    I2C_RASP_GEN_Stop();
    MSC_DelayUS(100);
    return PASS;
}

/******************************************************************************/
/* I2C_RASP_DRV_EEPROMReadByteVerify
 *
 * This function tries to read a byte of data the number of times saved in
 *  Macro I2C_ReadLimit on the driver install bus.
/******************************************************************************/
unsigned char I2C_RASP_DRV_EEPROMReadByteVerify(unsigned short DataAddress, unsigned char* data)
{
    unsigned char i;
    
    for(i=0;i<I2C_ReadLimit;i++)
    {
        if(I2C_RASP_DRV_EEPROMReadByte(DataAddress,data))
        {
            return PASS;
        }
        I2C_RASP_DRV_CTP();
    }
    return FAIL;
}

/******************************************************************************/
/* I2C_RASP_GEN_EEPROMReadByteVerify
 *
 * This function tries to read a byte of data the number of times saved in
 *  Macro I2C_ReadLimit on the general purpose bus.
/******************************************************************************/
unsigned char I2C_RASP_GEN_EEPROMReadByteVerify(unsigned short DataAddress, unsigned char* data)
{
    unsigned char i;
    
    for(i=0;i<I2C_ReadLimit;i++)
    {
        if(I2C_RASP_GEN_EEPROMReadByte(DataAddress,data))
        {
            return PASS;
        }
        I2C_RASP_GEN_CTP();
    }
    return FAIL;
}

/******************************************************************************/
/* I2C_RASP_DRV_EEPROMWriteByteVerify
 *
 * This function tries to write a byte of data the number of times saved in
 *  Macro I2C_WriteLimit on the driver install bus. 
/******************************************************************************/
unsigned char I2C_RASP_DRV_EEPROMWriteByteVerify(unsigned short DataAddress, unsigned char data)
{
    unsigned char i;
    unsigned char ReadData;
    for(i=0;i<I2C_WriteLimit;i++)
    {
        I2C_RASP_DRV_EEPROMWriteByte(DataAddress,data);
        if(I2C_RASP_DRV_EEPROMReadByteVerify(DataAddress,&ReadData))
        {
            if(ReadData == data)
            {
                return PASS;
            }
        }
        I2C_RASP_DRV_CTP();
    }
    return FAIL;   
}

/******************************************************************************/
/* I2C_RASP_GEN_EEPROMWriteByteVerify
 *
 * This function tries to write a byte of data the number of times saved in
 *  Macro I2C_WriteLimit on the general purpose bus.
/******************************************************************************/
unsigned char I2C_RASP_GEN_EEPROMWriteByteVerify(unsigned short DataAddress, unsigned char data)
{
    unsigned char i;
    unsigned char ReadData;
    for(i=0;i<I2C_WriteLimit;i++)
    {
        I2C_RASP_GEN_EEPROMWriteByte(DataAddress,data);
        if(I2C_RASP_GEN_EEPROMReadByteVerify(DataAddress,&ReadData))
        {
            if(ReadData == data)
            {
                return PASS;
            }
        }
        I2C_RASP_GEN_CTP();
    }
    return FAIL;   
}

/******************************************************************************/
/* I2C_RASP_DRV_EEPROMReadArray
 *
 * This function reads an array from the EEPROM on the driver install bus.
/******************************************************************************/
unsigned char I2C_RASP_DRV_EEPROMReadArray(unsigned short DataAddressStart, unsigned short Amount, void* data)
{
    unsigned short address;
    unsigned short i; 
    
    address = DataAddressStart;
    for(i=0;i<Amount;i++)
    {
        if(!I2C_RASP_DRV_EEPROMReadByteVerify(address,(unsigned char*)data))
        {
            return FAIL;
        }
        address++;
        (unsigned char*) data++;
    }
    return PASS;    
}

/******************************************************************************/
/* I2C_RASP_GEN_EEPROMReadArray
 *
 * This function reads an array from the EEPROM on the general purpose bus.
/******************************************************************************/
unsigned char I2C_RASP_GEN_EEPROMReadArray(unsigned short DataAddressStart, unsigned short Amount, void* data)
{
    unsigned short address;
    unsigned short i; 
    
    address = DataAddressStart;
    for(i=0;i<Amount;i++)
    {
        if(!I2C_RASP_GEN_EEPROMReadByteVerify(address,(unsigned char*)data))
        {
            return FAIL;
        }
        address++;
        (unsigned char*) data++;
    }
    return PASS;    
}

/******************************************************************************/
/* I2C_RASP_DRV_EEPROMWriteArray
 *
 * This function writes an array to the EEPROM on the driver install bus.
/******************************************************************************/
unsigned char I2C_RASP_DRV_EEPROMWriteArray(unsigned short DataAddressStart, unsigned short Amount, void* data)
{
    unsigned short address;
    unsigned short i; 
    
    address = DataAddressStart;
    for(i=0;i<Amount;i++)
    {
        if(!I2C_RASP_DRV_EEPROMWriteByteVerify(address, *(unsigned char*)data))
        {
            return FAIL;
        }
        address++;
        (unsigned char*) data++;
    }
    return PASS;    
}

/******************************************************************************/
/* I2C_RASP_GEN_EEPROMWriteArray
 *
 * This function writes an array to the EEPROM on the general purpose bus.
/******************************************************************************/
unsigned char I2C_RASP_GEN_EEPROMWriteArray(unsigned short DataAddressStart, unsigned short Amount, void* data)
{
    unsigned short address;
    unsigned short i; 
    
    address = DataAddressStart;
    for(i=0;i<Amount;i++)
    {
        if(!I2C_RASP_GEN_EEPROMWriteByteVerify(address, *(unsigned char*)data))
        {
            return FAIL;
        }
        address++;
        (unsigned char*) data++;
    }
    return PASS;    
}

/******************************************************************************/
/* I2C_RASP_DRV_EEPROMWriteString
 *
 * This function writes a string to the EEPROM on the driver install bus.
/******************************************************************************/
unsigned char I2C_RASP_DRV_EEPROMWriteString(unsigned short DataAddressStart, unsigned char* data)
{
    unsigned short address; 
    
    address = DataAddressStart;
    while(1)
    {
        if(!I2C_RASP_DRV_EEPROMWriteByteVerify(address,*data))
        {
            return FAIL;
        }
        if(*data == 0)
        {
            return PASS;  
        }
        address++;
        data++;
    }
    return PASS;    
}

/******************************************************************************/
/* I2C_RASP_GEN_EEPROMWriteString
 *
 * This function writes a string to the EEPROM on the general purpose bus.
/******************************************************************************/
unsigned char I2C_RASP_GEN_EEPROMWriteString(unsigned short DataAddressStart, unsigned char* data)
{
    unsigned short address; 
    
    address = DataAddressStart;
    while(1)
    {
        if(!I2C_RASP_GEN_EEPROMWriteByteVerify(address,*data))
        {
            return FAIL;
        }
        if(*data == 0)
        {
            return PASS;  
        }
        address++;
        data++;
    }
    return PASS;    
}

/******************************************************************************/
/* I2C_RASP_DRV_EEPROMWriteSerialNumbers
 *
 * This function writes the board serial numbers and info to the EEPROM on the
 *  driver install bus.
/******************************************************************************/
unsigned char I2C_RASP_DRV_EEPROMWriteSerialNumbers(void)
{
    unsigned char status;
    
    status = I2C_RASP_DRV_EEPROMWriteString(SERIAL_NUMBER_ADDRESS,(unsigned char*)PIC_Board_Serial_Number);
    
    if(!status)
    {
        Fault.EEPROM_SerialNumber_Write_Fail = TRUE;
    }
    return status;
}

/******************************************************************************/
/* I2C_RASP_DRV_EEPROMReadSerialNumbers
 *
 * This function reads the board serial numbers and info from the EEPROM on
 *  the driver install bus.
/******************************************************************************/
unsigned char I2C_RASP_DRV_EEPROMReadSerialNumbers(unsigned char* data)
{
    unsigned char i = 0;
    unsigned char status;
    unsigned char amount;
    
    while(1)
    {
        if(PIC_Board_Serial_Number[i] == 0)
        {
            amount = i + 1;
            break;
        }
        i++;
    }
    
    status = I2C_RASP_DRV_EEPROMReadArray(SERIAL_NUMBER_ADDRESS,amount, data);
    
    if(!status)
    {
        Fault.EEPROM_SerialNumber_Read_Fail = TRUE;
    }
    return status;
}

/******************************************************************************/
/* I2C_RASP_GEN_EEPROMWriteFaultLog
 *
 * This function writes the fault log to the EEPROM on the general purpose bus.
/******************************************************************************/
unsigned char I2C_RASP_GEN_EEPROMWriteFaultLog(void)
{
    unsigned char status;
    unsigned short size;
    
    size = sizeof(Fault);
    status = I2C_RASP_GEN_EEPROMWriteArray(FAULT_LOG_ADDRESS,size,&Fault);
    
    if(!status)
    {
        Fault.EEPROM_FaultLog_Write_Fail = TRUE;
    }
    return status;
}

/******************************************************************************/
/* I2C_RASP_GEN_EEPROMReadFaultLog
 *
 * This function reads the fault log to the EEPROM on the general purpose bus.
/******************************************************************************/
unsigned char I2C_RASP_GEN_EEPROMReadFaultLog(void)
{
    unsigned char status;
    unsigned short size;
    
    size = sizeof(Fault);
    
    status = I2C_RASP_GEN_EEPROMReadArray(FAULT_LOG_ADDRESS,size, &Fault);
    
    if(!status)
    {
        Fault.EEPROM_FaultLog_Read_Fail = TRUE;
    }
    return status;
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/