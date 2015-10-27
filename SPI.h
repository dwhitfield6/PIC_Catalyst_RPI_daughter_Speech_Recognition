/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 09/12/15     1.0_DW0a    Initial project make.
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#ifndef SPI_H
#define	SPI_H

#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "USER.h"

/******************************************************************************/
/* SPI modes of operation
 *
 * This is the mode configuration defines.
/******************************************************************************/
#define MASTER  0
#define SLAVE   1
#define BITS8   8
#define BITS16  16
#define BITS32  32

/******************************************************************************/
/* SPI buffer sizes
 *
 * This is the size of the SPI buffers.
/******************************************************************************/
#define TRANSMIT_BUFFER_SIZE  2048
#define RECEIVE_BUFFER_SIZE  2048

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define ENABLE  1
#define DISABLE 0

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
extern unsigned short SPI_TransmitBufferAmount;
extern volatile unsigned char SPI_ReceiveFull;
extern unsigned short SPI_TransmitPlace;
extern unsigned short SPI_ReceivePlace;
extern unsigned char SPI_Transfering;
extern unsigned char SPI_TransmitBuffer[TRANSMIT_BUFFER_SIZE];
extern unsigned char SPI_ReceiveBuffer[RECEIVE_BUFFER_SIZE];

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
inline void SPI_Module(unsigned char state);
void InitSPI(void);
void SPI_Mode(unsigned char master_slave, unsigned char bits, unsigned char mode, unsigned long speed);
void SPI_TransmitterInterrupt(unsigned char state);
void SPI_ReceiverInterrupt(unsigned char state);
unsigned char SPI_WriteRead(unsigned char write, unsigned char* read);
void SPI_CS(unsigned char state);

#endif	/* SPI_H */