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
#ifndef DMA_H
#define	DMA_H

#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "USER.h"

/******************************************************************************/
/* DMA move buffer options
 *
 * This is the options for what ahppens after DMA channel 3 (BufferCopier)
 *  completes.
/******************************************************************************/
#define DMA_MISC            0
#define DMA_SPI_TRANSMIT    1

/******************************************************************************/
/* DMA interrupt options
 *
 * This is the options for creating a DMA interrupt.
/******************************************************************************/
#define DMA_INT_NoChange                0x00008000
#define DMA_INT_AddressError            0x00010000
#define DMA_INT_TransferAbort           0x00020000
#define DMA_INT_CellTransferComplete    0x00040000
#define DMA_INT_BlockTransferComplete   0x00080000
#define DMA_INT_DestinationHalf         0x00100000
#define DMA_INT_DestinationDone         0x00200000
#define DMA_INT_SourceHalf              0x00400000
#define DMA_INT_SourceDone              0x00800000

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
extern unsigned char DMA_TransferType;
extern unsigned short DMA_TransferAmount;
extern unsigned char DMA_BufferCopierComplete;

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
inline unsigned long DMA_VirtToPhys(const void* p);
inline unsigned char DMA_Module(unsigned char state);
inline unsigned char DMA_Suspend(unsigned char state);
inline void DMA_Force(unsigned char channel);
inline void DMA_TransferCompleteWait(unsigned char channel);
inline unsigned char DMA_TransferComplete(unsigned char channel);
inline unsigned char DMA_Busy(void);
void InitDMA(void);
unsigned char DMA_ChannelBusy(unsigned char channel);
unsigned char DMA_ChannelEnable(unsigned char channel, unsigned char state);
unsigned char DMA_ChannelAutoEnable(unsigned char channel, unsigned char state);
unsigned char DMA_ChannelChain(unsigned char channel, unsigned char chained, unsigned char ChainPriority);
unsigned char DMA_ChannelPriority(unsigned char channel, unsigned char priority);
unsigned char DMA_ChannelTransferSource(unsigned char channel, unsigned char IRQsource, unsigned char enable);
unsigned char DMA_ChannelAbortSource(unsigned char channel, unsigned char source, unsigned char enable);
unsigned char DMA_ChannelInterrupt(unsigned char channel, unsigned long Interruptbits, unsigned char Enablebits, unsigned char MasterEnable);
void DMA_ChannelSource(unsigned char channel, unsigned long StartAddress, unsigned short size);
void DMA_ChannelDestination(unsigned char channel, unsigned long StartAddress, unsigned short size);
void DMA_ChannelTransferSize(unsigned char channel, unsigned short size);
void DMA_RBG_Configure(void);
void DMA_BufferCopier_Configure(void);
void DMA_BufferCopy(unsigned long FromAddress, unsigned long ToAddress, unsigned long amount, unsigned char type);

#endif	/* DMA_H */