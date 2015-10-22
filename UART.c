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
/* Contains functions for Universal asynchronous receiver/transmitter.
 *
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "USER.h"
#include "UART.h"
#include "SYSTEM.h"
#include "MISC.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
unsigned short RX1_Buffer_Place = 0;
unsigned short RX2_Buffer_Place = 0;
unsigned short RX3_Buffer_Place = 0;
unsigned short RX4_Buffer_Place = 0;
unsigned short RX5_Buffer_Place = 0;
unsigned char RX1_Buffer[UART1_RECEIVE_SIZE];
unsigned char RX2_Buffer[UART2_RECEIVE_SIZE];
unsigned char RX3_Buffer[UART3_RECEIVE_SIZE];
unsigned char RX4_Buffer[UART4_RECEIVE_SIZE];
unsigned char RX5_Buffer[UART5_RECEIVE_SIZE];
unsigned char TX1_Buffer[UART1_TRANSMIT_SIZE];
unsigned char TX2_Buffer[UART2_TRANSMIT_SIZE];
unsigned char TX4_Buffer[UART4_TRANSMIT_SIZE];
volatile unsigned short TX1_Buffer_ADD_Place = 0;
volatile unsigned short TX2_Buffer_ADD_Place = 0;
volatile unsigned short TX4_Buffer_ADD_Place = 0;
unsigned short TX1_Buffer_REMOVE_Place = 0;
unsigned short TX2_Buffer_REMOVE_Place = 0;
unsigned short TX4_Buffer_REMOVE_Place = 0;
unsigned char UserSentBreak = FALSE;

/******************************************************************************/
/* Inline Functions
/******************************************************************************/

/******************************************************************************/
/* UART_Module1
 *
 * The function controls the UART module 1.
/******************************************************************************/
inline void UART_Module1(unsigned char state)
{
#ifdef ON
    #undef ON
    if(state)
    {
        U1MODEbits.ON = 1; // Turn on the UART module
    }
    else
    {
        U1MODEbits.ON = 0; // Turn off the UART module
    }
    #define ON 1
#else
    if(state)
    {
        U1MODEbits.ON = 1; // Turn on the UART module
    }
    else
    {
        U1MODEbits.ON = 0; // Turn off the UART module
    }
#endif
}

/******************************************************************************/
/* UART_Module2
 *
 * The function controls the UART module 2.
/******************************************************************************/
inline void UART_Module2(unsigned char state)
{
#ifdef ON
    #undef ON
    if(state)
    {
        U2MODEbits.ON = 1; // Turn on the UART module
    }
    else
    {
        U2MODEbits.ON = 0; // Turn off the UART module
    }
    #define ON 1
#else
    if(state)
    {
        U2MODEbits.ON = 1; // Turn on the UART module
    }
    else
    {
        U2MODEbits.ON = 0; // Turn off the UART module
    }
#endif    
}

/******************************************************************************/
/* UART_Module3
 *
 * The function controls the UART module 3.
/******************************************************************************/
inline void UART_Module3(unsigned char state)
{
#ifdef ON
    #undef ON    
    if(state)
    {
        U3MODEbits.ON = 1; // Turn on the UART module
    }
    else
    {
        U3MODEbits.ON = 0; // Turn off the UART module
    }
    #define ON 1
#else
    if(state)
    {
        U3MODEbits.ON = 1; // Turn on the UART module
    }
    else
    {
        U3MODEbits.ON = 0; // Turn off the UART module
    }
#endif    
}

/******************************************************************************/
/* UART_Module4
 *
 * The function controls the UART module 4.
/******************************************************************************/
inline void UART_Module4(unsigned char state)
{
#ifdef ON
    #undef ON    
    if(state)
    {
        U4MODEbits.ON = 1; // Turn on the UART module
    }
    else
    {
        U4MODEbits.ON = 0; // Turn off the UART module
    }
    #define ON 1
#else
    if(state)
    {
        U4MODEbits.ON = 1; // Turn on the UART module
    }
    else
    {
        U4MODEbits.ON = 0; // Turn off the UART module
    }
#endif    
}

/******************************************************************************/
/* UART_Module5
 *
 * The function controls the UART module 5.
/******************************************************************************/
inline void UART_Module5(unsigned char state)
{
#ifdef ON
    #undef ON    
    if(state)
    {
        U5MODEbits.ON = 1; // Turn on the UART module
    }
    else
    {
        U5MODEbits.ON = 0; // Turn off the UART module
    }
    #define ON 1
#else
    if(state)
    {
        U5MODEbits.ON = 1; // Turn on the UART module
    }
    else
    {
        U5MODEbits.ON = 0; // Turn off the UART module
    }
#endif    
}

/******************************************************************************/
/* UART_SendCharacter1
 *
 * The function sends a character over the UART module 1.
/******************************************************************************/
inline void UART_SendCharacter1(unsigned char data)
{
    while(U1STAbits.UTXBF);
    U1TXREG = data;
}

/******************************************************************************/
/* UART_SendCharacter2
 *
 * The function sends a character over the UART module 2.
/******************************************************************************/
inline void UART_SendCharacter2(unsigned char data)
{
    while(U2STAbits.UTXBF);
    U2TXREG = data;
}

/******************************************************************************/
/* UART_SendCharacter3
 *
 * The function sends a character over the UART module 3.
/******************************************************************************/
inline void UART_SendCharacter3(unsigned char data)
{
    while(U3STAbits.UTXBF);
    U3TXREG = data;
}

/******************************************************************************/
/* UART_SendCharacter4
 *
 * The function sends a character over the UART module 4.
/******************************************************************************/
inline void UART_SendCharacter4(unsigned char data)
{
    while(U4STAbits.UTXBF);
    U4TXREG = data;
}

/******************************************************************************/
/* UART_SendCharacter5
 *
 * The function sends a character over the UART module 5.
/******************************************************************************/
inline void UART_SendCharacter5(unsigned char data)
{
    while(U5STAbits.UTXBF);
    U5TXREG = data;
}

/******************************************************************************/
/* Functions
/******************************************************************************/

/******************************************************************************/
/* InitUART
 *
 * The function initializes the UART module.
/******************************************************************************/
void InitUART(void)
{
    U1MODE = 0;
    U2MODE = 0;
    U3MODE = 0;
    U4MODE = 0;
    U5MODE = 0;
    U1STA  = 0;
    U2STA  = 0;
    U3STA  = 0;
    U4STA  = 0;
    U5STA  = 0;
    
    /* Set remappable outputs */
    RPC4R = RASP_UART_TX_Module;        // U1TX    
    RPF5R = RS232_MALE_TX_Module;       // U2TX
    RPF5R = RS232_MALE_RTS_Module;      // U3TX
    RPF12R = RS232_FEMALE_TX_Module;    // U4TX
    RPF8R = RS232_FEMALE_RTS_Module;    // U5TX
    
    /* Set remappable inputs */
    U1RXR = RASP_UART_RX_Pin;           // U1RX
    U2RXR = RS232_MALE_RX_Pin;          // U2RX
    U3RXR = RS232_MALE_CTS_Pin;         // U3RX
    U4RXR = RS232_FEMALE_RX_Pin;        // U4RX
    U5RXR = RS232_FEMALE_CTS_Pin;       // U5RX
    
    /* Operation during Idle mode */
    U1MODEbits.SIDL = 0; // Continue operation in Idle mode
    U2MODEbits.SIDL = 0; // Continue operation in Idle mode
    U3MODEbits.SIDL = 0; // Continue operation in Idle mode
    U4MODEbits.SIDL = 0; // Continue operation in Idle mode
    U5MODEbits.SIDL = 0; // Continue operation in Idle mode
    
    /* Clean receive buffers */
    MSC_CleanBuffer(RX1_Buffer, UART1_RECEIVE_SIZE);
    MSC_CleanBuffer(RX2_Buffer, UART2_RECEIVE_SIZE);
    MSC_CleanBuffer(RX3_Buffer, UART3_RECEIVE_SIZE);
    MSC_CleanBuffer(RX4_Buffer, UART4_RECEIVE_SIZE);
    MSC_CleanBuffer(RX5_Buffer, UART5_RECEIVE_SIZE);

    /* Set up the debug port (module 1) */
    UART_SetParameters1(115200, NO, 1);
    UART_Module1(ON);
    UART_Receiver1(ON);
    UART_Transmitter1(ON);
    IPC7bits.U1IP = 4; // interrupt priority is 4
    IPC7bits.U1IS = 3; // interrupt sub-priority is 3
    IFS1bits.U1RXIF = 0;            // clear interrupt
    IFS1bits.U1TXIF = 0;            // clear interrupt
    UART_ReceiverInterrupt1(ON);
    UART_TransmitterInterrupt1(OFF);
    
    /* Set up the Male RS232 port (module 2) */
    UART_RS232_MaleParameters(115200, NO, 1);
    UART_RS232_Male(ON, ON, ON);
    IPC9bits.U2IP = 4; // interrupt priority is 4
    IPC9bits.U2IS = 3; // interrupt sub-priority is 3
    IFS1bits.U2RXIF = 0;            // clear interrupt
    IFS1bits.U2TXIF = 0;            // clear interrupt
    UART_ReceiverInterrupt2(ON);
    UART_TransmitterInterrupt2(OFF);
    
    /* Set up the Female RS232 port (module 4) */
    UART_RS232_FemaleParameters(115200, NO, 1);
    UART_RS232_Female(ON, ON, ON);
    IPC9bits.U4IP = 4; // interrupt priority is 4
    IPC9bits.U4IS = 3; // interrupt sub-priority is 3
    IFS2bits.U4RXIF = 0;            // clear interrupt
    IFS2bits.U4TXIF = 0;            // clear interrupt
    UART_ReceiverInterrupt4(ON);
    UART_TransmitterInterrupt4(OFF);
    
    /* set passthrough mode so that we can debug the raspberry pi */
    UART_DebugPassthrough(ON);
}

/******************************************************************************/
/* UART_DebugPassthrough
 *
 * The function sets the UART port connected to the raspberry pi debug as
 *  inputs and turns off the module so that the port is passed through to the\
 *  external header.
/******************************************************************************/
void UART_DebugPassthrough(unsigned char state)
{
    if(state)
    {
        /* Set remappable outputs */
        RPC4R = 0;        // No connect   
        RASP_UART_TXTris    = INPUT;
        UART_ReceiverInterrupt1(OFF);
        UART_TransmitterInterrupt1(OFF);
    }
    else
    {
        /* Set remappable outputs */
        RPC4R = RASP_UART_TX_Module;        // U1TX 
        RASP_UART_TXTris    = OUTPUT;
        UART_ReceiverInterrupt1(ON);
        UART_TransmitterInterrupt1(OFF);
    }
}

/******************************************************************************/
/* UART_RS232_FemaleSendChar
 *
 * The function sends a character over the Female rs232 port.
/******************************************************************************/
void UART_RS232_FemaleSendChar(unsigned char data)
{
    /* transmit buffer is full so use interrupts to empty buffer */
    TX4_Buffer[TX4_Buffer_ADD_Place] = data;
    TX4_Buffer_ADD_Place++;
    if(TX4_Buffer_ADD_Place >= UART4_TRANSMIT_SIZE)
    {
        TX4_Buffer_ADD_Place = 0;
    }
    UART_TransmitterInterrupt4(ON);
}

/******************************************************************************/
/* UART_RS232_MaleSendChar
 *
 * The function sends a character over the Male rs232 port.
/******************************************************************************/
void UART_RS232_MaleSendChar(unsigned char data)
{
    /* transmit buffer is full so use interrupts to empty buffer */
    TX2_Buffer[TX2_Buffer_ADD_Place] = data;
    TX2_Buffer_ADD_Place++;
    if(TX2_Buffer_ADD_Place >= UART2_TRANSMIT_SIZE)
    {
        TX2_Buffer_ADD_Place = 0;
    }
    UART_TransmitterInterrupt2(ON);
}

/******************************************************************************/
/* UART_RS232_MaleSendString
 *
 * The function sends a string over the Male rs232 port.
/******************************************************************************/
void UART_RS232_MaleSendString(unsigned char* data)
{
    while(*data)
    {
        UART_RS232_MaleSendChar(*data);
        data++;
    }
}

/******************************************************************************/
/* UART_RS232_FemaleSendString
 *
 * The function sends a string over the Female rs232 port.
/******************************************************************************/
void UART_RS232_FemaleSendString(unsigned char* data)
{
    while(*data)
    {
        UART_RS232_FemaleSendChar(*data);
        data++;
    }
}

/******************************************************************************/
/* UART_RS232_MaleSendConstString
 *
 * The function sends a constant string over the Male rs232 port.
/******************************************************************************/
void UART_RS232_MaleSendConstString(const unsigned char* data)
{
    while(*data)
    {
        UART_RS232_MaleSendChar(*data);
        data++;
    }
}

/******************************************************************************/
/* UART_RS232_FemaleSendConstString
 *
 * The function sends a constant string over the Female rs232 port.
/******************************************************************************/
void UART_RS232_FemaleSendConstString(const unsigned char* data)
{
    while(*data)
    {
        UART_RS232_FemaleSendChar(*data);
        data++;
    }
}

/******************************************************************************/
/* UART_RS232_MaleSendStringCRLN
 *
 * The function sends a string followed by a carriage return over the Male 
 *  rs232 port. 
/******************************************************************************/
void UART_RS232_MaleSendStringCRLN(unsigned char* data)
{
    while(*data)
    {
        UART_RS232_MaleSendChar(*data);
        data++;
    }
    UART_RS232_MaleSendConstString("\r\n");
}

/******************************************************************************/
/* UART_RS232_FemaleSendStringCRLN
 *
 * The function sends a string followed by a carriage return over the Female 
 *  rs232 port. 
/******************************************************************************/
void UART_RS232_FemaleSendStringCRLN(unsigned char* data)
{
    while(*data)
    {
        UART_RS232_FemaleSendChar(*data);
        data++;
    }
    UART_RS232_FemaleSendConstString("\r\n");
}

/******************************************************************************/
/* UART_RS232_MaleSendConstStringCRLN
 *
 * The function sends a constant string followed by a carriage return over the 
 *  Male rs232 port. 
/******************************************************************************/
void UART_RS232_MaleSendConstStringCRLN(const unsigned char* data)
{
    while(*data)
    {
        UART_RS232_MaleSendChar(*data);
        data++;
    }
    UART_RS232_MaleSendConstString("\r\n");
}

/******************************************************************************/
/* UART_RS232_FemaleSendConstStringCRLN
 *
 * The function sends a constant string followed by a carriage return over the 
 *  Female rs232 port. 
/******************************************************************************/
void UART_RS232_FemaleSendConstStringCRLN(const unsigned char* data)
{
    while(*data)
    {
        UART_RS232_FemaleSendChar(*data);
        data++;
    }
    UART_RS232_FemaleSendString("\r\n");
}

/******************************************************************************/
/* UART_Rasp_Comm
 *
 * The function controls the UART port connected to the raspberry pi.
/******************************************************************************/
void UART_Rasp(unsigned char module, unsigned char transmit, unsigned char receive)
{
    UART_Module1(module);
    UART_Receiver1(receive);
    UART_Transmitter1(transmit);
}

/******************************************************************************/
/* UART_Rasp_Comm
 *
 * The function controls the UART port connected to the raspberry pi.
/******************************************************************************/
void UART_RS232_Male(unsigned char module, unsigned char transmit, unsigned char receive)
{
    UART_RS232_MaleDriver(module);
    UART_Module2(module);
    UART_Receiver2(receive);
    UART_Transmitter2(transmit);
}

/******************************************************************************/
/* UART_Rasp_Comm
 *
 * The function controls the UART port connected to the raspberry pi.
/******************************************************************************/
void UART_RS232_Female(unsigned char module, unsigned char transmit, unsigned char receive)
{
    UART_RS232_FemaleDriver(module);
    UART_Module4(module);
    UART_Receiver4(receive);
    UART_Transmitter4(transmit);
}

/******************************************************************************/
/* UART_RS232_MaleParameters
 *
 * The function sets the baud, parity, and stop bits number for the male RS232
 *  port.
/******************************************************************************/
void UART_RS232_MaleParameters(unsigned long Baud,unsigned char Parity, unsigned char Stop)
{
    UART_SetParameters2(Baud, Parity, Stop);
}

/******************************************************************************/
/* UART_RS232_FemaleParameters
 *
 * The function sets the baud, parity, and stop bits number for the female
 *  RS232 port.
/******************************************************************************/
void UART_RS232_FemaleParameters(unsigned long Baud,unsigned char Parity, unsigned char Stop)
{
    UART_SetParameters4(Baud, Parity, Stop);
}

/******************************************************************************/
/* UART_SetParameters1
 *
 * The function initializes the UART module 1.
/******************************************************************************/
void UART_SetParameters1(unsigned long Baud,unsigned char Parity, unsigned char Stop)
{
    /* Set parity option */
    if(Parity == ODD)
    {
        /* Odd parity bit */
        U1MODEbits.PDSEL = 0b10;        
    }
    else if(Parity == EVEN)
    {
        /* Even Parity bit */
        U1MODEbits.PDSEL = 0b01;  
    }
    else
    {
        /* No parity bit */
        U1MODEbits.PDSEL = 0b00;
    }
    
    /* Set stop option */
    if(Stop == 1)
    {
        /* One stop bit */
        U1MODEbits.STSEL = 0;       
    }
    else
    {
        /* Two stop bits */
        U1MODEbits.STSEL = 1;  
    }
    
    /* Set Baud clock */
    U1MODEbits.BRGH = 1; // High speed mode
    U1BRG = (unsigned long) MSC_Round(((double)PBCLK/(4.0* (double)Baud)) - 1.0);
}

/******************************************************************************/
/* UART_SetParameters2
 *
 * The function initializes the UART module 2.
/******************************************************************************/
void UART_SetParameters2(unsigned long Baud,unsigned char Parity, unsigned char Stop)
{
    /* Set parity option */
    if(Parity == ODD)
    {
        /* Odd parity bit */
        U2MODEbits.PDSEL = 0b10;        
    }
    else if(Parity == EVEN)
    {
        /* Even Parity bit */
        U2MODEbits.PDSEL = 0b01;  
    }
    else
    {
        /* No parity bit */
        U2MODEbits.PDSEL = 0b00;
    }
    
    /* Set stop option */
    if(Stop == 1)
    {
        /* One stop bit */
        U2MODEbits.STSEL = 0;       
    }
    else
    {
        /* Two stop bits */
        U2MODEbits.STSEL = 1;  
    }
    
    /* Set Baud clock */
    U2MODEbits.BRGH = 1; // High speed mode
    U2BRG = (unsigned long) MSC_Round(((double)PBCLK/(4.0 * (double) Baud)) - 1.0);
}

/******************************************************************************/
/* UART_SetParameters3
 *
 * The function initializes the UART module 3.
/******************************************************************************/
void UART_SetParameters3(unsigned long Baud,unsigned char Parity, unsigned char Stop)
{
    /* Set parity option */
    if(Parity == ODD)
    {
        /* Odd parity bit */
        U3MODEbits.PDSEL = 0b10;        
    }
    else if(Parity == EVEN)
    {
        /* Even Parity bit */
        U3MODEbits.PDSEL = 0b01;  
    }
    else
    {
        /* No parity bit */
        U3MODEbits.PDSEL = 0b00;
    }
    
    /* Set stop option */
    if(Stop == 1)
    {
        /* One stop bit */
        U3MODEbits.STSEL = 0;       
    }
    else
    {
        /* Two stop bits */
        U3MODEbits.STSEL = 1;  
    }
    
    /* Set Baud clock */
    U3MODEbits.BRGH = 1; // High speed mode
    U3BRG = (unsigned long) MSC_Round(((double)PBCLK/(4.0 * (double) Baud)) - 1.0);
}

/******************************************************************************/
/* UART_SetParameters4
 *
 * The function initializes the UART module 4.
/******************************************************************************/
void UART_SetParameters4(unsigned long Baud,unsigned char Parity, unsigned char Stop)
{
    /* Set parity option */
    if(Parity == ODD)
    {
        /* Odd parity bit */
        U4MODEbits.PDSEL = 0b10;        
    }
    else if(Parity == EVEN)
    {
        /* Even Parity bit */
        U4MODEbits.PDSEL = 0b01;  
    }
    else
    {
        /* No parity bit */
        U4MODEbits.PDSEL = 0b00;
    }
    
    /* Set stop option */
    if(Stop == 1)
    {
        /* One stop bit */
        U4MODEbits.STSEL = 0;       
    }
    else
    {
        /* Two stop bits */
        U4MODEbits.STSEL = 1;  
    }
    
    /* Set Baud clock */
    U4MODEbits.BRGH = 1; // High speed mode
    U4BRG = (unsigned long) MSC_Round(((double)PBCLK/(4.0 * (double) Baud)) - 1.0);
}

/******************************************************************************/
/* UART_SetParameters5
 *
 * The function initializes the UART module 5.
/******************************************************************************/
void UART_SetParameters5(unsigned long Baud,unsigned char Parity, unsigned char Stop)
{
    /* Set parity option */
    if(Parity == ODD)
    {
        /* Odd parity bit */
        U5MODEbits.PDSEL = 0b10;        
    }
    else if(Parity == EVEN)
    {
        /* Even Parity bit */
        U5MODEbits.PDSEL = 0b01;  
    }
    else
    {
        /* No parity bit */
        U5MODEbits.PDSEL = 0b00;
    }
    
    /* Set stop option */
    if(Stop == 1)
    {
        /* One stop bit */
        U5MODEbits.STSEL = 0;       
    }
    else
    {
        /* Two stop bits */
        U5MODEbits.STSEL = 1;  
    }
    
    /* Set Baud clock */
    U5MODEbits.BRGH = 1; // High speed mode
    U5BRG = (unsigned long) MSC_Round(((double)PBCLK/(4.0 * (double) Baud)) - 1.0);
}

/******************************************************************************/
/* UART_ReceiverInterrupt1
 *
 * The function controls the UART module 1 receiver interrupt.
/******************************************************************************/
void UART_ReceiverInterrupt1(unsigned char state)
{
    if(state)
    {
        IEC1bits.U1RXIE = 1; // Turn on the UART module receiver interrupt
    }
    else
    {
        IEC1bits.U1RXIE = 0; // Turn off the UART module receiver interrupt
    }
}

/******************************************************************************/
/* UART_ReceiverInterrupt2
 *
 * The function controls the UART module 2 receiver interrupt.
/******************************************************************************/
void UART_ReceiverInterrupt2(unsigned char state)
{
    if(state)
    {
        IEC1bits.U2RXIE = 1; // Turn on the UART module receiver interrupt
    }
    else
    {
        IEC1bits.U2RXIE = 0; // Turn off the UART module receiver interrupt
    }
}

/******************************************************************************/
/* UART_ReceiverInterrupt3
 *
 * The function controls the UART module 3 receiver interrupt.
/******************************************************************************/
void UART_ReceiverInterrupt3(unsigned char state)
{
    if(state)
    {
        IEC1bits.U3RXIE = 1; // Turn on the UART module receiver interrupt
    }
    else
    {
        IEC1bits.U3RXIE = 0; // Turn off the UART module receiver interrupt
    }
}

/******************************************************************************/
/* UART_ReceiverInterrupt4
 *
 * The function controls the UART module 4 receiver interrupt.
/******************************************************************************/
void UART_ReceiverInterrupt4(unsigned char state)
{
    if(state)
    {
        IEC2bits.U4RXIE = 1; // Turn on the UART module receiver interrupt
    }
    else
    {
        IEC2bits.U4RXIE = 0; // Turn off the UART module receiver interrupt
    }
}

/******************************************************************************/
/* UART_ReceiverInterrupt5
 *
 * The function controls the UART module 5 receiver interrupt.
/******************************************************************************/
void UART_ReceiverInterrupt5(unsigned char state)
{
    if(state)
    {
        IEC2bits.U5RXIE = 1; // Turn on the UART module receiver interrupt
    }
    else
    {
        IEC2bits.U5RXIE = 0; // Turn off the UART module receiver interrupt
    }
}

/******************************************************************************/
/* UART_Receiver1
 *
 * The function controls the UART module 1 receiver.
/******************************************************************************/
void UART_Receiver1(unsigned char state)
{
    if(state)
    {
        U1STAbits.URXEN = 1; // Turn on the UART module receiver
    }
    else
    {
        U1STAbits.URXEN = 0; // Turn off the UART module receiver
    }
}

/******************************************************************************/
/* UART_Receiver2
 *
 * The function controls the UART module 2 receiver.
/******************************************************************************/
void UART_Receiver2(unsigned char state)
{
    if(state)
    {
        U2STAbits.URXEN = 1; // Turn on the UART module receiver
    }
    else
    {
        U2STAbits.URXEN = 0; // Turn off the UART module receiver
    }
}

/******************************************************************************/
/* UART_Receiver3
 *
 * The function controls the UART module 3 receiver.
/******************************************************************************/
void UART_Receiver3(unsigned char state)
{
    if(state)
    {
        U3STAbits.URXEN = 1; // Turn on the UART module receiver
    }
    else
    {
        U3STAbits.URXEN = 0; // Turn off the UART module receiver
    }
}

/******************************************************************************/
/* UART_Receiver4
 *
 * The function controls the UART module 4 receiver.
/******************************************************************************/
void UART_Receiver4(unsigned char state)
{
    if(state)
    {
        U4STAbits.URXEN = 1; // Turn on the UART module receiver
    }
    else
    {
        U4STAbits.URXEN = 0; // Turn off the UART module receiver
    }
}

/******************************************************************************/
/* UART_Receiver5
 *
 * The function controls the UART module 5 receiver.
/******************************************************************************/
void UART_Receiver5(unsigned char state)
{
    if(state)
    {
        U5STAbits.URXEN = 1; // Turn on the UART module receiver
    }
    else
    {
        U5STAbits.URXEN = 0; // Turn off the UART module receiver
    }
}

/******************************************************************************/
/* UART_Receiver1Read
 *
 * The function returns the status of the UART 1 module receiver.
/******************************************************************************/
unsigned char UART_Receiver1Read(void)
{
    return U1STAbits.URXEN;
}

/******************************************************************************/
/* UART_Receiver2Read
 *
 * The function returns the status of the UART 2 module receiver.
/******************************************************************************/
unsigned char UART_Receiver2Read(void)
{
    return U2STAbits.URXEN;
}

/******************************************************************************/
/* UART_Receiver3Read
 *
 * The function returns the status of the UART 3 module receiver.
/******************************************************************************/
unsigned char UART_Receiver3Read(void)
{
    return U3STAbits.URXEN;
}

/******************************************************************************/
/* UART_Receiver4Read
 *
 * The function returns the status of the UART 4 module receiver.
/******************************************************************************/
unsigned char UART_Receiver4Read(void)
{
    return U4STAbits.URXEN;
}

/******************************************************************************/
/* UART_Receiver5Read
 *
 * The function returns the status of the UART 5 module receiver.
/******************************************************************************/
unsigned char UART_Receiver5Read(void)
{
    return U5STAbits.URXEN;
}

/******************************************************************************/
/* UART_Transmitter1
 *
 * The function controls the UART module 1 transmitter.
/******************************************************************************/
void UART_Transmitter1(unsigned char state)
{
    if(state)
    {
        U1STAbits.UTXEN = 1; // Turn on the UART module transmitter
    }
    else
    {
        U1STAbits.UTXEN = 0; // Turn off the UART module transmitter
    }
}

/******************************************************************************/
/* UART_Transmitter2
 *
 * The function controls the UART module 2 transmitter.
/******************************************************************************/
void UART_Transmitter2(unsigned char state)
{
    if(state)
    {
        U2STAbits.UTXEN = 1; // Turn on the UART module transmitter
    }
    else
    {
        U2STAbits.UTXEN = 0; // Turn off the UART module transmitter
    }
}

/******************************************************************************/
/* UART_Transmitter3
 *
 * The function controls the UART module 3 transmitter.
/******************************************************************************/
void UART_Transmitter3(unsigned char state)
{
    if(state)
    {
        U3STAbits.UTXEN = 1; // Turn on the UART module transmitter
    }
    else
    {
        U3STAbits.UTXEN = 0; // Turn off the UART module transmitter
    }
}

/******************************************************************************/
/* UART_Transmitter4
 *
 * The function controls the UART module 4 transmitter.
/******************************************************************************/
void UART_Transmitter4(unsigned char state)
{
    if(state)
    {
        U4STAbits.UTXEN = 1; // Turn on the UART module transmitter
    }
    else
    {
        U4STAbits.UTXEN = 0; // Turn off the UART module transmitter
    }
}

/******************************************************************************/
/* UART_Transmitter5
 *
 * The function controls the UART module 5 transmitter.
/******************************************************************************/
void UART_Transmitter5(unsigned char state)
{
    if(state)
    {
        U5STAbits.UTXEN = 1; // Turn on the UART module transmitter
    }
    else
    {
        U5STAbits.UTXEN = 0; // Turn off the UART module transmitter
    }
}

/******************************************************************************/
/* UART_Transmitter1Read
 *
 * The function returns the status of the UART 1 module transmitter.
/******************************************************************************/
unsigned char UART_Transmitter1Read(void)
{
    return U1STAbits.UTXEN;
}

/******************************************************************************/
/* UART_Transmitter2Read
 *
 * The function returns the status of the UART 2 module transmitter.
/******************************************************************************/
unsigned char UART_Transmitter2Read(void)
{
    return U2STAbits.UTXEN;
}

/******************************************************************************/
/* UART_Transmitter3Read
 *
 * The function returns the status of the UART 3 module transmitter.
/******************************************************************************/
unsigned char UART_Transmitter3Read(void)
{
    return U3STAbits.UTXEN;
}

/******************************************************************************/
/* UART_Transmitter4Read
 *
 * The function returns the status of the UART 4 module transmitter.
/******************************************************************************/
unsigned char UART_Transmitter4Read(void)
{
    return U4STAbits.UTXEN;
}

/******************************************************************************/
/* UART_Transmitter5Read
 *
 * The function returns the status of the UART 5 module transmitter.
/******************************************************************************/
unsigned char UART_Transmitter5Read(void)
{
    return U5STAbits.UTXEN;
}

/******************************************************************************/
/* UART_TransmitterInterrupt1
 *
 * The function controls the UART module 1 transmitter interrupt.
/******************************************************************************/
void UART_TransmitterInterrupt1(unsigned char state)
{
    if(state)
    {
        IEC1bits.U1TXIE = 1; // Turn on the UART module transmitter interrupt
    }
    else
    {
        IEC1bits.U1TXIE = 0; // Turn off the UART module transmitter interrupt
    }
}

/******************************************************************************/
/* UART_TransmitterInterrupt2
 *
 * The function controls the UART module 2 transmitter interrupt.
/******************************************************************************/
void UART_TransmitterInterrupt2(unsigned char state)
{
    if(state)
    {
        IEC1bits.U2TXIE = 1; // Turn on the UART module transmitter interrupt
    }
    else
    {
        IEC1bits.U2TXIE = 0; // Turn off the UART module transmitter interrupt
    }
}

/******************************************************************************/
/* UART_TransmitterInterrupt3
 *
 * The function controls the UART module 3 transmitter interrupt.
/******************************************************************************/
void UART_TransmitterInterrupt3(unsigned char state)
{
    if(state)
    {
        IEC2bits.U3TXIE = 1; // Turn on the UART module transmitter interrupt
    }
    else
    {
        IEC2bits.U3TXIE = 0; // Turn off the UART module transmitter interrupt
    }
}

/******************************************************************************/
/* UART_TransmitterInterrupt4
 *
 * The function controls the UART module 4 transmitter interrupt.
/******************************************************************************/
void UART_TransmitterInterrupt4(unsigned char state)
{
    if(state)
    {
        IEC2bits.U4TXIE = 1; // Turn on the UART module transmitter interrupt
    }
    else
    {
        IEC2bits.U4TXIE = 0; // Turn off the UART module transmitter interrupt
    }
}

/******************************************************************************/
/* UART_TransmitterInterrupt5
 *
 * The function controls the UART module 5 transmitter interrupt.
/******************************************************************************/
void UART_TransmitterInterrupt5(unsigned char state)
{
    if(state)
    {
        IEC2bits.U5TXIE = 1; // Turn on the UART module transmitter interrupt
    }
    else
    {
        IEC2bits.U5TXIE = 0; // Turn off the UART module transmitter interrupt
    }
}

/******************************************************************************/
/* UART_SendShortBreak1
 *
 * The function sends a short break on the next transmission.
/******************************************************************************/
void UART_SendShortBreak1(void)
{
    U1STAbits.UTXBRK = 1;
}

/******************************************************************************/
/* UART_SendShortBreak2
 *
 * The function sends a short break on the next transmission.
/******************************************************************************/
void UART_SendShortBreak2(void)
{
    U2STAbits.UTXBRK = 1;
}

/******************************************************************************/
/* UART_SendShortBreak3
 *
 * The function sends a short break on the next transmission.
/******************************************************************************/
void UART_SendShortBreak3(void)
{
    U3STAbits.UTXBRK = 1;
}

/******************************************************************************/
/* UART_SendShortBreak4
 *
 * The function sends a short break on the next transmission.
/******************************************************************************/
void UART_SendShortBreak4(void)
{
    U4STAbits.UTXBRK = 1;
}

/******************************************************************************/
/* UART_SendShortBreak5
 *
 * The function sends a short break on the next transmission.
/******************************************************************************/
void UART_SendShortBreak5(void)
{
    U5STAbits.UTXBRK = 1;
}

/******************************************************************************/
/* UART_SendLongBreak1
 *
 * The function sends a long break on the next transmission.
/******************************************************************************/
void UART_SendLongBreak1(void)
{
    unsigned char status;
    
    status = UART_Transmitter1Read();
    
    LATC |= RASP_UART_TX;
    RPC4R = 0;      
    UART_Transmitter1(OFF);
    LATC &= ~RASP_UART_TX;
    MSC_DelayUS(LONG_BREAK_US);
    LATC |= RASP_UART_TX;
    RPC4R = RASP_UART_TX_Module;        // U1TX
    
    if(status)
    {
        UART_Transmitter1(ON);
    }
}

/******************************************************************************/
/* UART_SendLongBreak2
 *
 * The function sends a long break on the next transmission.
/******************************************************************************/
void UART_SendLongBreak2(void)
{
    unsigned char status;
    
    status = UART_Transmitter2Read();
    
    LATF |= RS232_MALE_TX;
    RPF5R = 0; 
    UART_Transmitter2(OFF);     
    LATF &= ~RS232_MALE_TX;
    MSC_DelayUS(LONG_BREAK_US);
    LATF |= RS232_MALE_TX;
    RPF5R = RS232_MALE_TX_Module;       // U2TX
    
    if(status)
    {
        UART_Transmitter2(ON);
    }
}

/******************************************************************************/
/* UART_SendLongBreak3
 *
 * The function sends a long break on the next transmission.
/******************************************************************************/
void UART_SendLongBreak3(void)
{
    unsigned char status;
    
    status = UART_Transmitter3Read();   

    LATD |= RS232_MALE_RTS;
    RPF5R = 0;  
    UART_Transmitter3(OFF);
    LATD &= ~RS232_MALE_RTS;
    MSC_DelayUS(LONG_BREAK_US);
    LATD |= RS232_MALE_RTS;
    RPF5R = RS232_MALE_RTS_Module;      // U3TX
    
    if(status)
    {
        UART_Transmitter3(ON);
    }
}

/******************************************************************************/
/* UART_SendLongBreak4
 *
 * The function sends a long break on the next transmission.
/******************************************************************************/
void UART_SendLongBreak4(void)
{
    unsigned char status;
    
    status = UART_Transmitter4Read();
    
    LATF |= RS232_FEMALE_TX;
    RPF5R = 0;   
    UART_Transmitter4(OFF);
    LATF &= ~RS232_FEMALE_TX;
    MSC_DelayUS(LONG_BREAK_US);
        LATF |= RS232_FEMALE_TX;
    RPF12R = RS232_FEMALE_TX_Module;    // U4TX
    
    if(status)
    {
        UART_Transmitter4(ON);
    }
}

/******************************************************************************/
/* UART_SendLongBreak5
 *
 * The function sends a long break on the next transmission.
/******************************************************************************/
void UART_SendLongBreak5(void)
{
    unsigned char status;
    
    status = UART_Transmitter5Read();
    
    LATF |= RS232_FEMALE_RTS;
    RPF5R = 0;    
    UART_Transmitter5(OFF);    
    LATF &= ~RS232_FEMALE_RTS;
    MSC_DelayUS(LONG_BREAK_US);
    LATF |= RS232_FEMALE_RTS;
    RPF8R = RS232_FEMALE_RTS_Module;    // U5TX
    
    if(status)
    {
        UART_Transmitter5(ON);
    }
}

/******************************************************************************/
/* UART_RS232_Male
 *
 * The function controls the RS232 driver for the male DB9.
/******************************************************************************/
void UART_RS232_MaleDriver(unsigned char state)
{
    if(state)
    {
        LATA |= RS232_MALE_SHUTDOWN;    // take the driver out of shutdown
        LATD &= ~RS232_MALE_ENABLE;     // Enable the driver
    }
    else
    {
        LATD &= ~RS232_MALE_ENABLE;     // disable the driver
        LATA &= ~RS232_MALE_SHUTDOWN;    // shutdown the driver
    }
}

/******************************************************************************/
/* UART_RS232_Female
 *
 * The function controls the RS232 driver for the female DB9.
/******************************************************************************/
void UART_RS232_FemaleDriver(unsigned char state)
{
    if(state)
    {
        LATD |= RS232_FEMALE_SHUTDOWN;    // take the driver out of shutdown
        LATD &= ~RS232_FEMALE_ENABLE;     // Enable the driver
    }
    else
    {
        LATD &= ~RS232_FEMALE_ENABLE;     // disable the driver
        LATD &= ~RS232_FEMALE_SHUTDOWN;    // shutdown the driver
    }
}

/******************************************************************************/
/* UART_CleanReceive1
 *
 * The function clears the UART 1 receive buffer.
/******************************************************************************/
void UART_CleanReceive1(void)
{
    RX1_Buffer_Place = 0;
}

/******************************************************************************/
/* UART_CleanReceive2
 *
 * The function clears the UART 2 receive buffer.
/******************************************************************************/
void UART_CleanReceive2(void)
{
    RX2_Buffer_Place = 0;
}

/******************************************************************************/
/* UART_CleanReceive3
 *
 * The function clears the UART 3 receive buffer.
/******************************************************************************/
void UART_CleanReceive3(void)
{
    RX3_Buffer_Place = 0;
}

/******************************************************************************/
/* UART_CleanReceive4
 *
 * The function clears the UART 4 receive buffer.
/******************************************************************************/
void UART_CleanReceive4(void)
{
    RX4_Buffer_Place = 0;
}

/******************************************************************************/
/* UART_CleanReceive5
 *
 * The function clears the UART 5 receive buffer.
/******************************************************************************/
void UART_CleanReceive5(void)
{
    RX5_Buffer_Place = 0;
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/