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
/* Contains functions for the SPI controller.
 *
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "USER.h"
#include "SPI.h"
#include "SYSTEM.h"
#include "MISC.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
unsigned short SPI_TransmitBufferAmount = 0;
unsigned volatile char SPI_ReceiveFull = FALSE;
unsigned short SPI_TransmitPlace = 0;
unsigned short SPI_ReceivePlace = 0;
unsigned char SPI_Transfering = FALSE;
unsigned char SPI_TransmitBuffer[TRANSMIT_BUFFER_SIZE];
unsigned char SPI_ReceiveBuffer[RECEIVE_BUFFER_SIZE];

/******************************************************************************/
/* Inline Functions
/******************************************************************************/

/******************************************************************************/
/* SPI_Module
 *
 * The function controls the SPI module 2.
/******************************************************************************/
inline void SPI_Module(unsigned char state)
{

#ifdef ON
    #undef ON
    if(state)
    {
        SPI2CONbits.ON = 1; // Turn on the SPI module
    }
    else
    {
        SPI2CONbits.ON = 0; // Turn off the SPI module
    }
    #define ON 1
#else
    if(state)
    {
        SPI2CONbits.ON = 1; // Turn on the SPI module
    }
    else
    {
        SPI2CONbits.ON = 0; // Turn off the SPI module
    }
#endif    
}

/******************************************************************************/
/* Functions
/******************************************************************************/

/******************************************************************************/
/* InitSPI
 *
 * The function initializes the SPI module.
/******************************************************************************/
void InitSPI(void)
{
    SPI_TransmitterInterrupt(OFF);
    SPI_ReceiverInterrupt(OFF);
    SPI_Module(OFF);
    SPI2CONbits.SIDL = 0; // Continue operation in Idle mode
    SPI2CONbits.ENHBUF = 1; // Enhanced Buffer mode is enabled
    SPI2CONbits.MSSEN = 1;  // Slave select SPI support enabled (master mode)
    SPI2CONbits.SSEN = 1;   // Slave select pin used for Slave mode
    SPI2CONbits.DISSDO = 0; // SDO pin is controlled by the module
    SPI2CONbits.DISSDI = 0; // SDI pin is controlled by the module 
    IPC8bits.SPI2IP = 6; // interrupt priority is 6
    IPC8bits.SPI2IS = 3; // interrupt sub-priority is 3
    IFS1bits.SPI2RXIF = 0;            // clear interrupt
    IFS1bits.SPI2TXIF = 0;            // clear interrupt
    SPI2CONbits.STXISEL = 0b11; // Interrupt is generated when the buffer is not full (has one or more empty elements)
    SPI2CONbits.SRXISEL = 0b01; // Interrupt is generated when the buffer is not empty
    SPI_Mode(SLAVE, BITS8, 0, 3000);    
    SPI2STATbits.SPIROV = 0; // no overflow has occurred
    SPI_Module(ON);
    if(PWR_RASP_SPIReady())
    {
        SPI_ReceiverInterrupt(ON);
    }
}

/******************************************************************************/
/* SPI_Mode
 *
 * The function configures the SPI module.
/******************************************************************************/
void SPI_Mode(unsigned char master_slave, unsigned char bits, unsigned char mode, unsigned long speedKhz)
{
    double speedKhzDB = (double) speedKhz;
    speedKhzDB *= 1000.0;
    
    if(master_slave == MASTER)
    {
        SPI2CONbits.MSTEN = 1; // master mode
        RASP_SPI_MOSITris   = OUTPUT;
        RASP_SPI_MOSI2Tris  = INPUT;
        RASP_SPI_MISOTris   = INPUT;
        RASP_SPI_CLKTris    = OUTPUT;
        RASP_SPI_CSTris     = OUTPUT;
        RASP_SPI_CS2Tris    = INPUT;
        RASP_SPI_CS3Tris    = INPUT;
            
        /* Set remappable outputs */
        RPB10R = RASP_SPI_MOSI_Pin;         // SDO2 aka MOSI
        RPE8R = RASP_SPI_CS2_Pin;           // SS2 aka CS
    
        /* Set remappable inputs */
        SDI2R = RASP_SPI_MISO_Module;       // SDI2 aka MISO
    }
    else
    {
        SPI2CONbits.MSTEN = 0; // slave mode  
        RASP_SPI_MOSITris   = INPUT;
        RASP_SPI_MOSI2Tris  = INPUT;
        RASP_SPI_MISOTris   = OUTPUT;
        RASP_SPI_CLKTris    = INPUT;
        RASP_SPI_CSTris     = INPUT;
        RASP_SPI_CS2Tris    = INPUT;
        RASP_SPI_CS3Tris    = INPUT;
                
        /* Set remappable outputs */
        RPB15R = RASP_SPI_MISO_Pin;         // SDO2 aka MOSI
    
        /* Set remappable inputs */
        SDI2R = RASP_SPI_MOSI_Module;       // SDI2 aka MISO
        SS2R = RASP_SPI_CS2_Module;         // SS2 aka CS
    }
    
    if(bits == 8)
    {
        SPI2CONbits.MODE16 = 0;
        SPI2CONbits.MODE32 = 0;
    }
    else if(bits == 16)
    {
        SPI2CONbits.MODE16 = 1;
        SPI2CONbits.MODE32 = 0;
    }
    else
    {
        SPI2CONbits.MODE16 = 1;
        SPI2CONbits.MODE32 = 1;
    }
    
    if(mode == 0)
    {
        SPI2CONbits.CKP = 0;
        SPI2CONbits.CKE = 1;
    }
    else if(mode == 1)
    {
        SPI2CONbits.CKP = 0;
        SPI2CONbits.CKE = 0;
    }
    else if(mode == 2)
    {
        SPI2CONbits.CKP = 1;
        SPI2CONbits.CKE = 1;
    }
    else
    {
        SPI2CONbits.CKP = 1;
        SPI2CONbits.CKE = 0;
    }
    
    /* 
     * Transmit interrupt is generated when the last transfer is shifted out
     *  of SPISR and transmit operations are complete
     */
    SPI2CONbits.STXISEL = 0b00; 
    
    /* 
     * Receive interrupt is generated when the last word in the receive
     *  buffer is read (i.e., buffer is empty)
     */
    SPI2CONbits.SRXISEL = 0b00; 
    
    SPI2CONbits.MCLKSEL = 0; // PBCLK is used by the Baud Rate Generator
    SPI2BRG = (unsigned long) MSC_Round(((double)PBCLK/(2.0 * (double) speedKhzDB)) - 1.0);   
}

/******************************************************************************/
/* SPI_TransmitterInterrupt
 *
 * The function controls the SPI module 2 transmitter interrupt.
/******************************************************************************/
void SPI_TransmitterInterrupt(unsigned char state)
{
    if(state)
    {
        IEC1bits.SPI2TXIE = 1; // Turn on the SPI module transmitter interrupt
    }
    else
    {
        IEC1bits.SPI2TXIE = 0; // Turn off the SPI module transmitter interrupt
    }
}

/******************************************************************************/
/* SPI_ReceiverInterrupt
 *
 * The function controls the SPI module 2 receiver interrupt.
/******************************************************************************/
void SPI_ReceiverInterrupt(unsigned char state)
{
    if(state)
    {
        IEC1bits.SPI2RXIE = 1; // Turn on the SPI module receiver interrupt
    }
    else
    {
        IEC1bits.SPI2RXIE = 0; // Turn off the SPI module receiver interrupt
    }
}

/******************************************************************************/
/* SPI_WriteRead
 *
 * The function writes a byte over the SPI and listens for a response.
/******************************************************************************/
unsigned char SPI_WriteRead(unsigned char write, unsigned char* read)
{
    unsigned short dummy;

    /* dummy read */
    dummy = SPI2BUF;
    
    while(SPI2STATbits.SPITBF); 
    while(!SPI2STATbits.SRMT);
    /* send data */
    SPI2BUF = write;  
    
    /* read data */
    *read = dummy;
    
    if(SPI2STATbits.SPIRBE)
    {
        /* RX FIFO is not empty (CRPTR != SWPTR) */
        return 0;
    }
    return 1;
}

/******************************************************************************/
/* SPI_WriteRead
 *
 * The function writes a byte over the SPI and listens for a response.
/******************************************************************************/
void SPI_CS(unsigned char state)
{
    if(state == ENABLE)
    {
        LATA &= ~RASP_SPI_CS;
    }
    else
    {
        LATA |= RASP_SPI_CS;
    }
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/