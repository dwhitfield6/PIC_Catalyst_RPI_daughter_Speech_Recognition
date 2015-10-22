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
/* Contains functions for direct memory access controller.
 *
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "USER.h"
#include "TIMERS.h"
#include "DMA.h"
#include "PWM.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
unsigned char DMA_TransferType = DMA_MISC;
unsigned short DMA_TransferAmount = 0;
unsigned char DMA_BufferCopierComplete = TRUE;

/******************************************************************************/
/* Inline Functions
/******************************************************************************/

/******************************************************************************/
/* DMA_VirtToPhys
 *
 * The function converts a virtual address to a physical address.
 * 
 * To translate the kernel address (KSEG0 or KSEG1) to a physical address,
 *  perform a ?Bitwise "AND" operation of the virtual address with 0x1FFFFFFF:
 *  Physical Address = Virtual Address and 0x1FFFFFFF For physical address
 *  to KSEG0 virtual address translation, perform a ?Bitwise OR? operation of 
 *  the physical address with 0x80000000: KSEG0 Virtual Address = Physical
 *  Address | 0x80000000 For physical address to KSEG1 virtual address
 *  translation, perform a ?Bitwise OR? operation of the physical address
 *  with 0xA0000000: KSEG1 Virtual Address = Physical Address | 0xA0000000 
 *  To translate from KSEG0 to KSEG1 virtual address, perform a ?Bitwise OR?
 *  operation of the KSEG0 virtual address with 0x20000000: KSEG1 Virtual
 *  Address = KSEG0 Virtual Address | 0x20000000
/******************************************************************************/
inline unsigned long DMA_VirtToPhys(const void* p) 
{ 
    return (int)p<0?((int)p&0x1fffffffL):(unsigned int)((unsigned char*)p+0x40000000L); 
} 
 
/******************************************************************************/
/* DMA_Module
 *
 * The function enables/disables the DMA controller.
/******************************************************************************/
inline unsigned char DMA_Module(unsigned char state)
{
#ifdef ON
    #undef ON
    unsigned char status = DMACONbits.ON;
    if(state)
    {
        DMACONbits.ON = 1; // DMA module is enabled
    }
    else
    {
        DMACONbits.ON = 0; // DMA module is disabled
    }
    #define ON 1
#else
    unsigned char status = DMACONbits.ON;
    if(state)
    {
        DMACONbits.ON = 1; // DMA module is enabled
    }
    else
    {
        DMACONbits.ON = 0; // DMA module is disabled
    }
#endif    
    return status;
}

/******************************************************************************/
/* DMA_Suspend
 *
 * The function controls the DMA controller.
/******************************************************************************/
inline unsigned char DMA_Suspend(unsigned char state)
{
    unsigned char status = DMACONbits.SUSPEND;
    if(state)
    {
        DMACONbits.SUSPEND = 1; // DMA transfers are suspended to allow CPU uninterrupted access to data bus
    }
    else
    {
        DMACONbits.SUSPEND = 0; // DMA operates normally
    }
    return status;
}

/******************************************************************************/
/* DMA_Force
 *
 * The function forces the start of a transfer.
/******************************************************************************/
inline void DMA_Force(unsigned char channel)
{
    if(channel == 0)
    {
        DCH0ECONbits.CFORCE = 1;
    }
    else if(channel == 1)
    {
        DCH1ECONbits.CFORCE = 1;
    }
    else if(channel == 2)
    {
        DCH2ECONbits.CFORCE = 1;
    }
    else
    {
        DCH3ECONbits.CFORCE = 1;
    }
}

/******************************************************************************/
/* DMA_TransferCompleteWait
 *
 * The function forces the start of a transfer.
/******************************************************************************/
inline void DMA_TransferCompleteWait(unsigned char channel)
{
    while(!DMA_TransferComplete(channel))
    {
        MSC_DelayUS(100);
    }
}

/******************************************************************************/
/* DMA_TransferComplete
 *
 * The function returns the status of the transfer.
/******************************************************************************/
inline unsigned char DMA_TransferComplete(unsigned char channel)
{
    if(channel == 0)
    {
        if(DCH0INTbits.CHSDIF || DCH0INTbits.CHDDIF)
        {
            return TRUE;
        }
    }
    else if(channel == 1)
    {
        if(DCH1INTbits.CHSDIF || DCH1INTbits.CHDDIF)
        {
            return TRUE;
        }
    }
    else if(channel == 2)
    {
        if(DCH2INTbits.CHSDIF || DCH2INTbits.CHDDIF)
        {
            return TRUE;
        }
    }
    else
    {
        if(DCH3INTbits.CHSDIF || DCH3INTbits.CHDDIF)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/******************************************************************************/
/* DMA_Busy
 *
 * The function controls the DMA controller.
/******************************************************************************/
inline unsigned char DMA_Busy(void)
{
    return DMACONbits.DMABUSY;
}

/******************************************************************************/
/* Functions
/******************************************************************************/

/******************************************************************************/
/* InitI2C
 *
 * The function initializes the I2C module.
/******************************************************************************/
void InitDMA(void)
{
    DMA_Module(ON);
    IPC10bits.DMA0IP = 3; // interrupt priority is 3
    IPC10bits.DMA0IS = 1; // interrupt sub-priority is 1
    IPC10bits.DMA1IP = 3; // interrupt priority is 3
    IPC10bits.DMA1IS = 1; // interrupt sub-priority is 1
    IPC11bits.DMA2IP = 3; // interrupt priority is 3
    IPC11bits.DMA2IS = 1; // interrupt sub-priority is 1
    IPC11bits.DMA3IP = 3; // interrupt priority is 3
    IPC11bits.DMA3IS = 1; // interrupt sub-priority is 1
    DMA_RBG_Configure();
    DMA_BufferCopier_Configure();
}

/******************************************************************************/
/* DMA_ChannelBusy
 *
 * The function returns the status of the DMA channel.
/******************************************************************************/
unsigned char DMA_ChannelBusy(unsigned char channel)
{
    if(channel == 0)
    {
        return DCH0CONbits.CHBUSY;
    }
    else if(channel == 1)
    {
        return DCH1CONbits.CHBUSY;
    }
    else if(channel == 2)
    {
        return DCH2CONbits.CHBUSY;
    }
    else
    {
        return DCH3CONbits.CHBUSY;
    }
}

/******************************************************************************/
/* DMA_ChannelEnable
 *
 * The function enables/disables a channel on the DMA module.
/******************************************************************************/
unsigned char DMA_ChannelEnable(unsigned char channel, unsigned char state)
{
    unsigned char status;
    
    if(channel == 0)
    {
        status = DCH0CONbits.CHEN;
        if(state)
        {
            DCH0CONbits.CHEN = 1; // Channel is enabled
        }
        else
        {
            DCH0CONbits.CHEN = 0; // Channel is disabled
        }
    }
    else if(channel == 1)
    {
        status = DCH1CONbits.CHEN;
        if(state)
        {
            DCH1CONbits.CHEN = 1; // Channel is enabled
        }
        else
        {
            DCH1CONbits.CHEN = 0; // Channel is disabled
        }
    }
    else if(channel == 2)
    {
        status = DCH2CONbits.CHEN;
        if(state)
        {
            DCH2CONbits.CHEN = 1; // Channel is enabled
        }
        else
        {
            DCH2CONbits.CHEN = 0; // Channel is disabled
        }
    }
    else
    {
        status = DCH3CONbits.CHEN;
        if(state)
        {
            DCH3CONbits.CHEN = 1; // Channel is enabled
        }
        else
        {
            DCH3CONbits.CHEN = 0; // Channel is disabled
        }  
    }
    return status;
}

/******************************************************************************/
/* DMA_ChannelEnable
 *
 * The function enables/disables the automatic enable for a channel.
/******************************************************************************/
unsigned char DMA_ChannelAutoEnable(unsigned char channel, unsigned char state)
{
    unsigned char status;
    
    if(channel == 0)
    {
        status = DCH0CONbits.CHAEN;
        if(state)
        {
            DCH0CONbits.CHAEN = 1; // Channel is continuously enabled, and not automatically disabled after a block transfer is complete
        }
        else
        {
            DCH0CONbits.CHAEN = 0; // Channel is disabled on block transfer complete
        }
    }
    else if(channel == 1)
    {
        status = DCH1CONbits.CHAEN;
        if(state)
        {
            DCH1CONbits.CHAEN = 1; // Channel is continuously enabled, and not automatically disabled after a block transfer is complete
        }
        else
        {
            DCH1CONbits.CHAEN = 0; // Channel is disabled on block transfer complete
        }
    }
    else if(channel == 2)
    {
        status = DCH2CONbits.CHAEN;
        if(state)
        {
            DCH2CONbits.CHAEN = 1; // Channel is continuously enabled, and not automatically disabled after a block transfer is complete
        }
        else
        {
            DCH2CONbits.CHAEN = 0; // Channel is disabled on block transfer complete
        }
    }
    else
    {
        status = DCH3CONbits.CHAEN;
        if(state)
        {
            DCH3CONbits.CHAEN = 1; // Channel is continuously enabled, and not automatically disabled after a block transfer is complete
        }
        else
        {
            DCH3CONbits.CHAEN = 0; // Channel is disabled on block transfer complete
        }
    }
    return status;
}

/******************************************************************************/
/* DMA_ChannelChain
 *
 * The function enables/disables the channel chaining feature.
/******************************************************************************/
unsigned char DMA_ChannelChain(unsigned char channel, unsigned char chained, unsigned char ChainPriority)
{
    unsigned char status;
    
    if(channel == 0)
    {
        status = DCH0CONbits.CHCHN;
        if(chained)
        {
            DCH0CONbits.CHCHN = 1; // Allow channel to be chained
        }
        else
        {
            DCH0CONbits.CHCHN = 0; // Do not allow channel to be chained
        }
        if(ChainPriority)
        {
            DCH0CONbits.CHCHNS = 1; // Chain to channel lower in natural priority (CH1 will be enabled by CH2 transfer complete)
        }
        else
        {
            DCH0CONbits.CHCHNS = 0; //Chain to channel higher in natural priority (CH1 will be enabled by CH0 transfer complete)
        }
    }
    else if(channel == 1)
    {
        status = DCH1CONbits.CHCHN;
        if(chained)
        {
            DCH1CONbits.CHCHN = 1; // Allow channel to be chained
        }
        else
        {
            DCH1CONbits.CHCHN = 0; // Do not allow channel to be chained
        }
        if(ChainPriority)
        {
            DCH1CONbits.CHCHNS = 1; // Chain to channel lower in natural priority (CH1 will be enabled by CH2 transfer complete)
        }
        else
        {
            DCH1CONbits.CHCHNS = 0; //Chain to channel higher in natural priority (CH1 will be enabled by CH0 transfer complete)
        }
    }
    else if(channel == 2)
    {
        status = DCH2CONbits.CHCHN;
        if(chained)
        {
            DCH2CONbits.CHCHN = 1; // Allow channel to be chained
        }
        else
        {
            DCH2CONbits.CHCHN = 0; // Do not allow channel to be chained
        }
        if(ChainPriority)
        {
            DCH2CONbits.CHCHNS = 1; // Chain to channel lower in natural priority (CH1 will be enabled by CH2 transfer complete)
        }
        else
        {
            DCH2CONbits.CHCHNS = 0; //Chain to channel higher in natural priority (CH1 will be enabled by CH0 transfer complete)
        }
    }
    else
    {
        status = DCH3CONbits.CHCHN;
        if(chained)
        {
            DCH3CONbits.CHCHN = 1; // Allow channel to be chained
        }
        else
        {
            DCH3CONbits.CHCHN = 0; // Do not allow channel to be chained
        }
        if(ChainPriority)
        {
            DCH3CONbits.CHCHNS = 1; // Chain to channel lower in natural priority (CH1 will be enabled by CH2 transfer complete)
        }
        else
        {
            DCH3CONbits.CHCHNS = 0; //Chain to channel higher in natural priority (CH1 will be enabled by CH0 transfer complete)
        }
    }
    return status;
}

/******************************************************************************/
/* DMA_ChannelPriority
 *
 * The function sets the channel priority.
/******************************************************************************/
unsigned char DMA_ChannelPriority(unsigned char channel, unsigned char priority)
{
    unsigned char status;
    if(priority > 3)
    {
       priority = 3;
    }
    
    if(channel == 0)
    {
        status = DCH0CONbits.CHPRI;
        DCH0CONbits.CHPRI = priority; // channel priority (3 is highest 0 is lowest)
    }
    else if(channel == 1)
    {
        status = DCH1CONbits.CHPRI;
        DCH1CONbits.CHPRI = priority; // channel priority (3 is highest 0 is lowest)
    }
    else if(channel == 2)
    {
        status = DCH2CONbits.CHPRI;
        DCH2CONbits.CHPRI = priority; // channel priority (3 is highest 0 is lowest)
    }
    else
    {
        status = DCH3CONbits.CHPRI;
        DCH3CONbits.CHPRI = priority; // channel priority (3 is highest 0 is lowest)
    }
    return status;    
}

/******************************************************************************/
/* DMA_ChannelTransferSource
 *
 * The function sets the channel transfer source.
/******************************************************************************/
unsigned char DMA_ChannelTransferSource(unsigned char channel, unsigned char IRQsource, unsigned char enable)
{
    unsigned char status;
    
    if(channel == 0)
    {
        status = DCH0ECONbits.SIRQEN;
        DCH0ECONbits.CHSIRQ = IRQsource;
        if(enable)
        {
            DCH0ECONbits.SIRQEN = 1; // Start channel cell transfer if an interrupt matching CHSIRQ occurs
        }
        else
        {
            DCH0ECONbits.SIRQEN = 0; // Interrupt number CHSIRQ is ignored and does not start a transfer
        }
    }
    else if(channel == 1)
    {
        status = DCH1ECONbits.SIRQEN;
        DCH1ECONbits.CHSIRQ = IRQsource;
        if(enable)
        {
            DCH1ECONbits.SIRQEN = 1; // Start channel cell transfer if an interrupt matching CHSIRQ occurs
        }
        else
        {
            DCH1ECONbits.SIRQEN = 0; // Interrupt number CHSIRQ is ignored and does not start a transfer
        }
    }
    else if(channel == 2)
    {
        status = DCH2ECONbits.SIRQEN;
        DCH2ECONbits.CHSIRQ = IRQsource;
        if(enable)
        {
            DCH2ECONbits.SIRQEN = 1; // Start channel cell transfer if an interrupt matching CHSIRQ occurs
        }
        else
        {
            DCH2ECONbits.SIRQEN = 0; // Interrupt number CHSIRQ is ignored and does not start a transfer
        }
    }
    else
    {
        status = DCH3ECONbits.SIRQEN;
        DCH3ECONbits.CHSIRQ = IRQsource;
        if(enable)
        {
            DCH3ECONbits.SIRQEN = 1; // Start channel cell transfer if an interrupt matching CHSIRQ occurs
        }
        else
        {
            DCH3ECONbits.SIRQEN = 0; // Interrupt number CHSIRQ is ignored and does not start a transfer
        }
    }
    return status;
}   

/******************************************************************************/
/* DMA_ChannelAbortSource
 *
 * The function sets the channel abort source.
/******************************************************************************/
unsigned char DMA_ChannelAbortSource(unsigned char channel, unsigned char source, unsigned char enable)
{
    unsigned char status;
    
    if(channel == 0)
    {
        status = DCH0ECONbits.AIRQEN;
        DCH0ECONbits.CHAIRQ = source;
        if(enable)
        {
            DCH0ECONbits.AIRQEN = 1; // Channel transfer is aborted if an interrupt matching CHAIRQ occurs
        }
        else
        {
            DCH0ECONbits.AIRQEN = 0; // Interrupt number CHAIRQ is ignored and does not terminate a transfer
        }
    }
    else if(channel == 1)
    {
        status = DCH1ECONbits.AIRQEN;
        DCH1ECONbits.CHAIRQ = source;
        if(enable)
        {
            DCH1ECONbits.AIRQEN = 1; // Channel transfer is aborted if an interrupt matching CHAIRQ occurs
        }
        else
        {
            DCH1ECONbits.AIRQEN = 0; // Interrupt number CHAIRQ is ignored and does not terminate a transfer
        }
    }
    else if(channel == 2)
    {
        status = DCH2ECONbits.AIRQEN;
        DCH2ECONbits.CHAIRQ = source;
        if(enable)
        {
            DCH2ECONbits.AIRQEN = 1; // Channel transfer is aborted if an interrupt matching CHAIRQ occurs
        }
        else
        {
            DCH2ECONbits.AIRQEN = 0; // Interrupt number CHAIRQ is ignored and does not terminate a transfer
        }
    }
    else
    {
        status = DCH3ECONbits.AIRQEN;
        DCH3ECONbits.CHAIRQ = source;
        if(enable)
        {
            DCH3ECONbits.AIRQEN = 1; // Channel transfer is aborted if an interrupt matching CHAIRQ occurs
        }
        else
        {
            DCH3ECONbits.AIRQEN = 0; // Interrupt number CHAIRQ is ignored and does not terminate a transfer
        }
    }
    return status;
} 

/******************************************************************************/
/* DMA_ChannelInterrupt
 *
 * The function sets up the DMA interrupt.
/******************************************************************************/
unsigned char DMA_ChannelInterrupt(unsigned char channel, unsigned long Interruptbits, unsigned char Enablebits, unsigned char MasterEnable)
{
    unsigned char status;
    
    if(channel == 0)
    {
        if(Interruptbits != DMA_INT_NoChange)
        {
            if(Enablebits)
            {
                DCH0INT |= Interruptbits; // enable the specific interrupt
            }
            else
            {
                DCH0INT &= ~Interruptbits; // disable the specific interrupt
            }
        }
        status = IEC2bits.DMA0IE; 
        if(MasterEnable)
        {
            IEC2bits.DMA0IE = 1; // enable interrupt
        }
        else
        {
            IEC2bits.DMA0IE = 0; // disable interrupt
        }
    }
    else if(channel == 1)
    {
        if(Interruptbits != DMA_INT_NoChange)
        {
            if(Enablebits)
            {
                DCH1INT |= Interruptbits; // enable the specific interrupt
            }
            else
            {
                DCH1INT &= ~Interruptbits; // disable the specific interrupt
            }
        }
        status = IEC2bits.DMA1IE; 
        if(MasterEnable)
        {
            IEC2bits.DMA1IE = 1; // enable interrupt
        }
        else
        {
            IEC2bits.DMA1IE = 0; // disable interrupt
        }
    }
    else if(channel == 2)
    {
        if(Interruptbits != DMA_INT_NoChange)
        {
            if(Enablebits)
            {
                DCH2INT |= Interruptbits; // enable the specific interrupt
            }
            else
            {
                DCH2INT &= ~Interruptbits; // disable the specific interrupt
            }
        }
        status = IEC2bits.DMA2IE; 
        if(MasterEnable)
        {
            IEC2bits.DMA2IE = 1; // enable interrupt
        }
        else
        {
            IEC2bits.DMA2IE = 0; // disable interrupt
        }
    }
    else
    {
        if(Interruptbits != DMA_INT_NoChange)
        {
            if(Enablebits)
            {
                DCH3INT |= Interruptbits; // enable the specific interrupt
            }
            else
            {
                DCH3INT &= ~Interruptbits; // disable the specific interrupt
            }
        }
        status = IEC2bits.DMA3IE; 
        if(MasterEnable)
        {
            IEC2bits.DMA3IE = 1; // enable interrupt
        }
        else
        {
            IEC2bits.DMA3IE = 0; // disable interrupt
        }
    }
    return status;
}

/******************************************************************************/
/* DMA_ChannelSource
 *
 * The function sets source address and size of the source.
/******************************************************************************/
void DMA_ChannelSource(unsigned char channel, unsigned long StartAddress, unsigned short size)
{
    if(channel == 0)
    {
        DCH0SSA = StartAddress;
        DCH0SSIZ = size;
    }
    else if(channel == 1)
    {
        DCH1SSA = StartAddress;
        DCH1SSIZ = size;
    }
    else if(channel == 2)
    {
        DCH2SSA = StartAddress;
        DCH2SSIZ = size;
    }
    else
    {
        DCH3SSA = StartAddress;
        DCH3SSIZ = size;
    }
}

/******************************************************************************/
/* DMA_ChannelDestination
 *
 * The function sets destination address and size of the destination.
/******************************************************************************/
void DMA_ChannelDestination(unsigned char channel, unsigned long StartAddress, unsigned short size)
{
    if(channel == 0)
    {
        DCH0DSA = StartAddress;
        DCH0DSIZ = size;
    }
    else if(channel == 1)
    {
        DCH1DSA = StartAddress;
        DCH1DSIZ = size;
    }
    else if(channel == 2)
    {
        DCH2DSA = StartAddress;
        DCH2DSIZ = size;
    }
    else
    {
        DCH3DSA = StartAddress;
        DCH3DSIZ = size;
    }
}

/******************************************************************************/
/* DMA_ChannelTransferSize
 *
 * The function sets the size of a transfer.
/******************************************************************************/
void DMA_ChannelTransferSize(unsigned char channel, unsigned short size)
{
    if(channel == 0)
    {
        DCH0CSIZ = size;
    }
    else if(channel == 1)
    {
        DCH1CSIZ = size;
    }
    else if(channel == 2)
    {
        DCH2CSIZ = size;
    }
    else
    {
        DCH3CSIZ = size;
    }   
}

/******************************************************************************/
/* DMA_RBG_Configure
 *
 * The function sets up DMA transfer of the PWM values for the RGB LED.
/******************************************************************************/
void DMA_RBG_Configure(void)
{
    /* set up DMA channel 0 for the Red LED */
    DMA_ChannelPriority(0,2);
    DMA_ChannelDestination(0,DMA_VirtToPhys(&OC3RS),4);
    DMA_ChannelSource(0,DMA_VirtToPhys(&Red_Duty),4);
    DMA_ChannelTransferSize(0,4);
    DMA_ChannelTransferSource(0,_TIMER_2_IRQ,ON);
    DMA_ChannelAutoEnable(0,ON);
    
    /* set up DMA channel 1 for the Green LED */
    DMA_ChannelPriority(1,1);
    DMA_ChannelDestination(1,DMA_VirtToPhys(&OC2RS),4);
    DMA_ChannelSource(1,DMA_VirtToPhys(&Green_Duty),4);
    DMA_ChannelTransferSize(1,4);
    DMA_ChannelTransferSource(1,_TIMER_2_IRQ,ON);
    DMA_ChannelAutoEnable(1,ON);
    
    /* set up DMA channel 2 for the Blue LED */
    DMA_ChannelPriority(2,0);
    DMA_ChannelDestination(2,DMA_VirtToPhys(&OC1RS),4);
    DMA_ChannelSource(2,DMA_VirtToPhys(&Blue_Duty),4);
    DMA_ChannelTransferSize(2,4);
    DMA_ChannelTransferSource(2,_TIMER_2_IRQ,ON);
    DMA_ChannelAutoEnable(2,ON);
    
    TMR_InterruptTimer2(OFF);
    
    DMA_ChannelEnable(0,ON);
    DMA_ChannelEnable(1,ON);
    DMA_ChannelEnable(2,ON);
}

/******************************************************************************/
/* DMA_BufferCopier_Configure
 *
 * The function sets up DMA channel 3 as an all-purpose DMA buffer copier.
/******************************************************************************/
void DMA_BufferCopier_Configure(void)
{
    DMA_ChannelPriority(3,3);
    DMA_ChannelTransferSource(3,0,OFF); // do not start on an interrupt
    DMA_ChannelAutoEnable(3,OFF);       // do not auto enable
    DMA_ChannelInterrupt(3, DMA_INT_SourceDone, TRUE, TRUE);
}

/******************************************************************************/
/* DMA_BufferCopy
 *
 * The function sets up DMA channel 3 as an all-purpose DMA buffer copier.
/******************************************************************************/
void DMA_BufferCopy(unsigned long FromAddress, unsigned long ToAddress, unsigned long amount, unsigned char type)
{
    DMA_ChannelDestination(3,DMA_VirtToPhys(ToAddress),amount);
    DMA_ChannelSource(3,DMA_VirtToPhys(FromAddress),amount);
    DMA_ChannelTransferSize(3,amount);
    DMA_TransferType = type;
    DMA_ChannelEnable(3,ON);
    DMA_BufferCopierComplete = FALSE;
    DMA_Force(3); // start the transfer
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/