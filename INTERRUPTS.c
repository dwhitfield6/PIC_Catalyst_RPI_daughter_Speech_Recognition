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
/* Interrupt Vector Options
 *
 * VECTOR NAMES:
 *
 * _CORE_TIMER_VECTOR          _COMPARATOR_2_VECTOR
 * _CORE_SOFTWARE_0_VECTOR     _UART_2A_VECTOR
 * _CORE_SOFTWARE_1_VECTOR     _I2C_2A_VECTOR
 * _EXTERNAL_0_VECTOR          _SPI_2_VECTOR
 * _TIMER_1_VECTOR             _SPI_2A_VECTOR
 * _INPUT_CAPTURE_1_VECTOR     _I2C_4_VECTOR
 * _OUTPUT_COMPARE_1_VECTOR    _UART_3_VECTOR
 * _EXTERNAL_1_VECTOR          _UART_2_VECTOR
 * _TIMER_2_VECTOR             _SPI_3A_VECTOR
 * _INPUT_CAPTURE_2_VECTOR     _I2C_3A_VECTOR
 * _OUTPUT_COMPARE_2_VECTOR    _UART_3A_VECTOR
 * _EXTERNAL_2_VECTOR          _SPI_4_VECTOR
 * _TIMER_3_VECTOR             _I2C_5_VECTOR
 * _INPUT_CAPTURE_3_VECTOR     _I2C_2_VECTOR
 * _OUTPUT_COMPARE_3_VECTOR    _FAIL_SAFE_MONITOR_VECTOR
 * _EXTERNAL_3_VECTOR          _RTCC_VECTOR
 * _TIMER_4_VECTOR             _DMA_0_VECTOR
 * _INPUT_CAPTURE_4_VECTOR     _DMA_1_VECTOR
 * _OUTPUT_COMPARE_4_VECTOR    _DMA_2_VECTOR
 * _EXTERNAL_4_VECTOR          _DMA_3_VECTOR
 * _TIMER_5_VECTOR             _DMA_4_VECTOR
 * _INPUT_CAPTURE_5_VECTOR     _DMA_5_VECTOR
 * _OUTPUT_COMPARE_5_VECTOR    _DMA_6_VECTOR
 * _SPI_1_VECTOR               _DMA_7_VECTOR
 * _I2C_3_VECTOR               _FCE_VECTOR
 * _UART_1A_VECTOR             _USB_1_VECTOR
 * _UART_1_VECTOR              _CAN_1_VECTOR
 * _SPI_1A_VECTOR              _CAN_2_VECTOR
 * _I2C_1A_VECTOR              _ETH_VECTOR
 * _SPI_3_VECTOR               _UART_4_VECTOR
 * _I2C_1_VECTOR               _UART_1B_VECTOR
 * _CHANGE_NOTICE_VECTOR       _UART_6_VECTOR
 * _ADC_VECTOR                 _UART_2B_VECTOR
 * _PMP_VECTOR                 _UART_5_VECTOR
 * _COMPARATOR_1_VECTOR        _UART_3B_VECTOR
 *
 * Refer to the device specific .h file in the C32 Compiler
 * pic32mx\include\proc directory for a complete Vector and IRQ mnemonic
 * listings for the PIC32 device.
 *
 * PRIORITY OPTIONS:
 *
 * (default) IPL0AUTO, IPL1, IPL2, ... IPL7 (highest)
 *
 * Example Shorthand Syntax
 *
 * void __ISR(<Vector Name>,<PRIORITY>) user_interrupt_routine_name(void)
 * {
 *     <Clear Interrupt Flag>
 * }
 *
 * For more interrupt macro examples refer to the C compiler User Guide in
 * the C compiler /doc directory.
 *
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>        /* XC8 General Include File */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <sys/attribs.h>     /* For __ISR definition */

#include "POWER.h"
#include "PWM.h"
#include "UART.h"
#include "RDI.h"
#include "RTCC.h"
#include "TIMERS.h"
#include "I2C.h"
#include "ADC.h"
#include "DMA.h"
#include "SPI.h"
#include "MISC.h"

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/******************************************************************************/
/* External interrupt 0 (Comparator VCAP_Warn1 warning that cap is under 4.1V)
/******************************************************************************/
void __ISR(_EXTERNAL_0_VECTOR , IPL7AUTO) INT0_IntHandler (void)
{
    PWR_Charge(TRUE);
    PWR_ReadComparator();
    IFS0bits.INT0IF = 0;
}

/******************************************************************************/
/* External interrupt 1 (Comparator VCAP_Warn3 warning that cap is over the
 *  voltage of the Potentiometer)
/******************************************************************************/
void __ISR(_EXTERNAL_1_VECTOR , IPL7AUTO) INT1_IntHandler (void)
{
    PWR_Charge(FALSE);
    PWR_ReadComparator();
    IFS0bits.INT1IF = 0;
}

/******************************************************************************/
/* External interrupt 2 (Comparator VIN_Warn warning that VIN power is below
 *  9 volts)
/******************************************************************************/
void __ISR(_EXTERNAL_2_VECTOR , IPL7AUTO) INT2_IntHandler (void)
{
    if(PWR_ExternalInterruptEdge(COMP_VIN_Warn_INT,FALLING) == RISING)
    {
        MSC_RedLEDON();
        PWR_Volts5(OFF);
        PWR_Charge(FALSE);
    }
    else
    {
        MSC_RedLEDOFF();
        PWR_ExternalInterruptEdge(COMP_VIN_Warn_INT,RISING); 
        PWR_Volts5(ON);
        PWR_Charge(TRUE);
    }
    PWR_ReadComparator();
    IFS0bits.INT2IF = 0;
    
}

/******************************************************************************/
/* External interrupt 3 (Comparator VCAP_Warn2 warning that cap is under 4.4V)
/******************************************************************************/
void __ISR(_EXTERNAL_3_VECTOR , IPL7AUTO) INT3_IntHandler (void)
{
    PWR_ReadComparator();
    IFS0bits.INT3IF = 0;
}

/******************************************************************************/
/* Timer 2 interrupt (PWM RGB LED)
/******************************************************************************/
void __ISR(_TIMER_2_VECTOR , IPL7AUTO) TMR2_IntHandler (void)
{
    /* Only used when DMA is disabled */
    
    OC3RS = Red_Duty; // Write Duty Cycle value for next PWM cycle
    OC2RS = Green_Duty; // Write Duty Cycle value for next PWM cycle
    OC1RS = Blue_Duty; // Write Duty Cycle value for next PWM cycle
    IFS0bits.T2IF = 0; // Clear Timer 2 interrupt flag
}

/******************************************************************************/
/* Timer 4 interrupt (RGB LED function change)
/******************************************************************************/
void __ISR(_TIMER_4_VECTOR , IPL7AUTO) TMR4_IntHandler (void)
{        
    if(RedAction)
    {
        if(RGB_Functions[0][PWM_Action][PWM_Place] != NMM)
        {
            Red_Duty = RGB_Functions[0][PWM_Action][PWM_Place];
        }        
    }
    if(GreenAction)
    {
        if(RGB_Functions[1][PWM_Action][PWM_Place] != NMM)
        {
            Green_Duty = RGB_Functions[1][PWM_Action][PWM_Place];
        }
    }
    if(BlueAction)
    {
        if(RGB_Functions[2][PWM_Action][PWM_Place] != NMM)
        {
            Blue_Duty = RGB_Functions[2][PWM_Action][PWM_Place];
        }
    }
    
    PWM_Place++;
    if(PWM_Place >= 25)
    {
        PWM_Place = 0;
    }
    IFS0bits.T4IF = 0; // Clear Timer 2 interrupt flag
}

/******************************************************************************/
/* Timer 5 interrupt (ADC channel switching and raspberry pi status monitoring)
/******************************************************************************/
void __ISR(_TIMER_5_VECTOR , IPL7AUTO) TMR5_IntHandler (void)
{  
    static unsigned char sample = 0;

    if(PWR_RASP_SPIReady())
    {
        SPI_ReceiverInterrupt(ON);
    }
    else
    {
        SPI_ReceiverInterrupt(OFF);
    }
    if(sample == 0)
    {
        /* set channel and start sampling */
        ADC_SetSample(CurrentChannel);
        PWR_StatusUpdate();
        sample++;
    }
    else
    {
        /* stop sampling and start conversion */
        IFS0bits.AD1IF = 0;
        ADC_Interrupt(ON);
        ADC_StopSample();
        sample = 0;
        TMR_EnableTimer5(OFF);
    }   
    IFS0bits.T5IF = 0; // Clear Timer 5 interrupt flag
}

/******************************************************************************/
/* ADC interrupt (ADC conversion is complete)
/******************************************************************************/
void __ISR(_ADC_VECTOR , IPL7AUTO) ADC_IntHandler (void)
{  
    unsigned short RawCounts;
    double Voltage;
    
    /* conversion is complete */
    
    RawCounts = ADC1BUF0;
    Voltage = ((double) RawCounts * VREF) / (1<<ADC_BITS);
    
    if(CurrentChannel == ADC_VIN_AN)
    {
        RailStatus.VIN = (Voltage * (R28 + R29)) / R29;
        CurrentChannel = ADC_Volt5_0_AN;
    }
    else if(CurrentChannel == ADC_Volt5_0_AN)
    {
        RailStatus.Volt5_0 = (Voltage * (R26 + R27)) / R27;
        CurrentChannel = ADC_Volt3_3_AN;
    }
    else if(CurrentChannel == ADC_Volt3_3_AN)
    {
        RailStatus.Volt3_3 = (Voltage * (R30 + R31)) / R31;
        CurrentChannel = ADC_Volt4_1_AN;
    }
    else if(CurrentChannel == ADC_Volt4_1_AN)
    {
        RailStatus.Volt4_1 = (Voltage * (R32 + R33)) / R33;
        CurrentChannel = ADC_POT_AN;
    }
    else if(CurrentChannel == ADC_POT_AN)
    {
        RailStatus.POT = Voltage;
        CurrentChannel = ADC_VCAP_AN;
    }
    else
    {
        RailStatus.VCAP = (Voltage * (R34 + R35)) / R35;
        CurrentChannel = ADC_VIN_AN;
        ADC_ReadRailStatus = TRUE;
    }       
    TMR_EnableTimer5(ON);
    ADC_Interrupt(OFF);
    IFS0bits.AD1IF = 0;
}

/******************************************************************************/
/* SPI interrupt (bus to raspberry pi)
/******************************************************************************/
void __ISR(_SPI_2_VECTOR , IPL7AUTO) SPI2_IntHandler (void)
{
    if(IFS1bits.SPI2RXIF)
    {
        /* there are no more word in the receive buffer */
        if(SPI_ReceivePlace < RECEIVE_BUFFER_SIZE)
        {
            SPI_ReceiveBuffer[SPI_ReceivePlace] = SPI2BUF;
            SPI_ReceivePlace++;
            SPI_ReceiveFull = FALSE;
        }
        else
        {
            SPI_ReceiveFull = TRUE;
        }
    }
    if(IFS1bits.SPI2TXIF)
    {
        /* there are no more characters in the transmit buffer */
        if(SPI_TransmitBufferAmount > 0)
        {
            MSC_RedLEDTOGGLE();
            SPI2BUF = SPI_TransmitBuffer[SPI_TransmitPlace];
            SPI_TransmitPlace++;
            SPI_TransmitBufferAmount--;
        }
        else
        {
            RDI_RequestRaspberryPiSPI(FALSE);
            SPI_TransmitterInterrupt(OFF);
            SPI_Transfering = FALSE;
        }
    }
    IFS1bits.SPI2RXIF = 0;            // clear interrupt
    IFS1bits.SPI2TXIF = 0;            // clear interrupt
}

/******************************************************************************/
/* UART 1 Interrupt (Raspberry pi debug)
/******************************************************************************/
void __ISR(_UART_1_VECTOR , IPL7AUTO) UART1_IntHandler (void)
{
    unsigned char data;
    
    if(IFS1bits.U1RXIF)
    {
        /* receive interrupt */        
        if(U1STAbits.FERR)
        {
            /* 
             * Receive error. This could be from a break or incorrect
             *  baud rate 
             */
            while(U1STAbits.URXDA)
            {
                data = U1RXREG;
                if(!data)
                {
                    /* There was a break */
                    Nop();
                }
            }
        }
        else
        {
            while(U1STAbits.URXDA)
            {
                /* receive buffer has data */
                data = U1RXREG;
                if(RX1_Buffer_Place < UART1_RECEIVE_SIZE)
                {
                    RX1_Buffer[RX1_Buffer_Place] = data;
                    RX1_Buffer_Place++;
                }
                else
                {
                    /* Overflow */
                    UART_CleanReceive1();
                }
            }
        }
    }
    if(IFS1bits.U1TXIF)
    {
        if(TX1_Buffer_REMOVE_Place != TX1_Buffer_ADD_Place)
        {
            U1TXREG = TX1_Buffer[TX1_Buffer_REMOVE_Place];
            TX1_Buffer_REMOVE_Place++;
            if(TX1_Buffer_REMOVE_Place >= UART1_TRANSMIT_SIZE)
            {
                TX1_Buffer_REMOVE_Place = 0;
            }
        }
        else
        {
            UART_TransmitterInterrupt1(OFF);
        }
    }
    IFS1bits.U1RXIF = 0;
    IFS1bits.U1TXIF = 0;
}

/******************************************************************************/
/* UART 2 Interrupt (Male rs232 DB9 connector)
/******************************************************************************/
void __ISR(_UART_2_VECTOR , IPL7AUTO) UART2_IntHandler (void)
{
    unsigned char data;
    
    if(IFS1bits.U2RXIF)
    {
        /* receive interrupt */        
        if(U2STAbits.FERR)
        {
            /* 
             * Receive error. This could be from a break or incorrect
             *  baud rate 
             */
            while(U2STAbits.URXDA)
            {
                data = U2RXREG;
                if(!data)
                {
                    /* There was a break */
                    Nop();
                }
            }
        }
        else
        {
            while(U2STAbits.URXDA)
            {
                /* receive buffer has data */
                if(!GetProduct)
                {
                    data = U2RXREG;
                    UART_RS232_FemaleSendChar(data);
                    if(RX2_Buffer_Place < UART2_RECEIVE_SIZE)
                    {
                        RX2_Buffer[RX2_Buffer_Place] = data;
                        RX2_Buffer_Place++;
                    }
                    else
                    {
                        /* Overflow */
                        UART_CleanReceive2();
                    }    
                    if(UserSentBreak && RDI_product)
                    {
                        /* 
                         * The user sent a break and we have a valid product
                         *  so insert the extra catalyst message in the banner 
                         */
                        if(data == Workhorse_H_ADCP[Banner_Correct_place])
                        {  
                            Banner_Correct_place++;
                            if(Workhorse_H_ADCP[Banner_Correct_place] == 0)
                            {
                                /* banner match for Workhorse HADCP */
                                RDI_PrintBannerExtention();
                                UserSentBreak = FALSE;
                            }
                        }   
                    }                
                }
                else
                {
                    data = U2RXREG;
                    if(Banner_Buffer_Place < Banner_RECEIVE_SIZE)
                    {
                        Banner_Buffer[Banner_Buffer_Place] = data;
                        Banner_Buffer_Place++;
                        if(data == Workhorse_H_ADCP[Banner_Correct_place])
                        {  
                            Banner_Correct_place++;
                            if(Workhorse_H_ADCP[Banner_Correct_place] == 0)
                            {
                                RDI_product = PROD_WORKHORSE_HADCP;
                            }
                        }
                        else
                        {
                            Banner_Correct_place = 0;
                        }                        
                    }
                    if(data == '>')
                    {
                        BannerFinished = TRUE;
                    }
                }
            }
        }
    }
    if(IFS1bits.U2TXIF)
    {
        if(TX2_Buffer_REMOVE_Place != TX2_Buffer_ADD_Place)
        {
            U2TXREG = TX2_Buffer[TX2_Buffer_REMOVE_Place];
            TX2_Buffer_REMOVE_Place++;
            if(TX2_Buffer_REMOVE_Place >= UART2_TRANSMIT_SIZE)
            {
                TX2_Buffer_REMOVE_Place = 0;
            }
        }
        else
        {
            UART_TransmitterInterrupt2(OFF);
        }
    }
    IFS1bits.U2RXIF = 0;
    IFS1bits.U2TXIF = 0; 
}

/******************************************************************************/
/* UART 4 Interrupt (Female rs232 DB9 connector)
/******************************************************************************/
void __ISR(_UART_4_VECTOR , IPL7AUTO) UART4_IntHandler (void)
{
    unsigned char data;
 
    if(IFS2bits.U4RXIF)
    {
        /* receive interrupt */        
        if(U4STAbits.FERR)
        {
            /* 
             * Receive error. This could be from a break or incorrect
             *  baud rate 
             */
            while(U4STAbits.URXDA)
            {
                data = U4RXREG;
                if(!data)
                {
                    /* There was a break */
                    UserSentBreak = TRUE;
                    Banner_Correct_place = 0;
                }
            }
            if(UserSentBreak)
            {
                UART_SendLongBreak2();
            }
        }
        else
        {
            while(U4STAbits.URXDA)
            {
                /* receive buffer has data */
                data = U4RXREG;
                UART_RS232_MaleSendChar(data);
                if(RX4_Buffer_Place < UART4_RECEIVE_SIZE)
                {
                    RX4_Buffer[RX4_Buffer_Place] = data;
                    RX4_Buffer_Place++;
                }
                else
                {
                    /* Overflow */
                    UART_CleanReceive4();
                }
            }
        }
    }
    if(IFS2bits.U4TXIF)
    {
        if(TX4_Buffer_REMOVE_Place != TX4_Buffer_ADD_Place)
        {
            U4TXREG = TX4_Buffer[TX4_Buffer_REMOVE_Place];
            TX4_Buffer_REMOVE_Place++;
            if(TX4_Buffer_REMOVE_Place >= UART4_TRANSMIT_SIZE)
            {
                TX4_Buffer_REMOVE_Place = 0;
            }
        }
        else
        {
            UART_TransmitterInterrupt4(OFF);
        }
    }
    IFS2bits.U4RXIF = 0;
    IFS2bits.U4TXIF = 0;
}

/******************************************************************************/
/* Real Time Clock Alarm Interrupt
/******************************************************************************/
void __ISR(_RTCC_VECTOR , IPL7AUTO) RTCC_IntHandler (void)
{
    /* Alarm went off */
    if(RTCC_CheckAlarmYear())
    {
        /* Alarm is valid */
    }
    else
    {
        /* Year is wrong */
    }
    IFS0bits.RTCCIF = 0;
}

/******************************************************************************/
/* I2C module 1 Interrupt
/******************************************************************************/
void __ISR(_I2C_1_VECTOR , IPL7AUTO) I2C_1_IntHandler (void)
{   
    if(IFS1bits.I2C1MIF)
    {
        /* I2C bus master event */
    }
    if(IFS1bits.I2C1BIF)
    {
        /* I2C bus collision event */
        I2C1STATbits.BCL = 0; // clear I2C collide status
        I2C1STATbits.IWCOL = 0; // clear I2C collide status
    }
    IFS1bits.I2C1MIF = 0;
    IFS1bits.I2C1BIF = 0;
    IFS1bits.I2C1SIF = 0;
}

/******************************************************************************/
/* I2C module 2 Interrupt
/******************************************************************************/
void __ISR(_I2C_2_VECTOR , IPL7AUTO) I2C_2_IntHandler (void)
{   
    if(IFS1bits.I2C2MIF)
    {
        /* I2C bus master event */
    }
    if(IFS1bits.I2C2BIF)
    {
        /* I2C bus collision event */
        I2C2STATbits.BCL = 0; // clear I2C collide status
        I2C2STATbits.IWCOL = 0; // clear I2C collide status
    }
    IFS1bits.I2C2MIF = 0;
    IFS1bits.I2C2BIF = 0;
    IFS1bits.I2C2SIF = 0;
}

/******************************************************************************/
/* DMA0 Interrupt (RGB Red LED PWM)
/******************************************************************************/
void __ISR(_DMA_0_VECTOR , IPL7AUTO) DMA_0_IntHandler (void)
{   
    if(DCH0INTbits.CHSDIF)
    {
        /* Channel Source Pointer has reached end of source (CHSPTR = CHSSIZ) */
        DCH0INTbits.CHSDIF = 0;
    }
    if(DCH0INTbits.CHSHIF)
    {
        /* 
         * Channel Source Pointer has reached midpoint of source 
         * (CHSPTR = CHSSIZ/2) 
         */
        DCH0INTbits.CHSHIF = 0;
    }
    if(DCH0INTbits.CHDHIF)
    {
        /*  
         * Channel Destination Pointer has reached midpoint of destination
         *  (CHDPTR = CHDSIZ/2) 
         */
        DCH0INTbits.CHDHIF = 0;
    }
    if(DCH0INTbits.CHBCIF)
    {
        /* 
         * A block transfer has been completed (the larger of CHSSIZ/CHDSIZ
         *  bytes has been transferred), or a pattern match event occurs  
         */
        DCH0INTbits.CHBCIF = 0;
    }
    if(DCH0INTbits.CHCCIF)
    {
        /* 
         * A cell transfer has been completed (CHCSIZ bytes have been
         *  transferred) 
         */
        DCH0INTbits.CHCCIF = 0;
    }
    if(DCH0INTbits.CHTAIF)
    {
        /* 
         * An interrupt matching CHAIRQ has been detected and the DMA transfer
         *  has been aborted 
         */
        DCH0INTbits.CHTAIF = 0;
    }
    if(DCH0INTbits.CHERIF)
    {
        /* 
         * A channel address error has been detected. Either the source or the
         *  destination address is invalid.
         */
        DCH0INTbits.CHERIF = 0;
    }
    IFS2bits.DMA0IF = 0;
}

/******************************************************************************/
/* DMA1 Interrupt (RGB Green LED PWM)
/******************************************************************************/
void __ISR(_DMA_1_VECTOR , IPL7AUTO) DMA_1_IntHandler (void)
{   
    if(DCH1INTbits.CHSDIF)
    {
        /* Channel Source Pointer has reached end of source (CHSPTR = CHSSIZ) */
        DCH1INTbits.CHSDIF = 0;
    }
    if(DCH1INTbits.CHSHIF)
    {
        /* 
         * Channel Source Pointer has reached midpoint of source 
         * (CHSPTR = CHSSIZ/2) 
         */
        DCH1INTbits.CHSHIF = 0;
    }
    if(DCH1INTbits.CHDHIF)
    {
        /*  
         * Channel Destination Pointer has reached midpoint of destination
         *  (CHDPTR = CHDSIZ/2) 
         */
        DCH1INTbits.CHDHIF = 0;
    }
    if(DCH1INTbits.CHBCIF)
    {
        /* 
         * A block transfer has been completed (the larger of CHSSIZ/CHDSIZ
         *  bytes has been transferred), or a pattern match event occurs  
         */
        DCH1INTbits.CHBCIF = 0;
    }
    if(DCH1INTbits.CHCCIF)
    {
        /* 
         * A cell transfer has been completed (CHCSIZ bytes have been
         *  transferred) 
         */
        DCH1INTbits.CHCCIF = 0;
    }
    if(DCH1INTbits.CHTAIF)
    {
        /* 
         * An interrupt matching CHAIRQ has been detected and the DMA transfer
         *  has been aborted 
         */
        DCH1INTbits.CHTAIF = 0;
    }
    if(DCH1INTbits.CHERIF)
    {
        /* 
         * A channel address error has been detected. Either the source or the
         *  destination address is invalid.
         */
        DCH1INTbits.CHERIF = 0;
    }
    IFS2bits.DMA1IF = 0;
}

/******************************************************************************/
/* DMA2 Interrupt (RGB Blue LED PWM)
/******************************************************************************/
void __ISR(_DMA_2_VECTOR , IPL7AUTO) DMA_2_IntHandler (void)
{   
    if(DCH2INTbits.CHSDIF)
    {
        /* Channel Source Pointer has reached end of source (CHSPTR = CHSSIZ) */
        DCH2INTbits.CHSDIF = 0;
    }
    if(DCH2INTbits.CHSHIF)
    {
        /* 
         * Channel Source Pointer has reached midpoint of source 
         * (CHSPTR = CHSSIZ/2) 
         */
        DCH2INTbits.CHSHIF = 0;
    }
    if(DCH2INTbits.CHDHIF)
    {
        /*  
         * Channel Destination Pointer has reached midpoint of destination
         *  (CHDPTR = CHDSIZ/2) 
         */
        DCH2INTbits.CHDHIF = 0;
    }
    if(DCH2INTbits.CHBCIF)
    {
        /* 
         * A block transfer has been completed (the larger of CHSSIZ/CHDSIZ
         *  bytes has been transferred), or a pattern match event occurs  
         */
        DCH2INTbits.CHBCIF = 0;
    }
    if(DCH2INTbits.CHCCIF)
    {
        /* 
         * A cell transfer has been completed (CHCSIZ bytes have been
         *  transferred) 
         */
        DCH2INTbits.CHCCIF = 0;
    }
    if(DCH2INTbits.CHTAIF)
    {
        /* 
         * An interrupt matching CHAIRQ has been detected and the DMA transfer
         *  has been aborted 
         */
        DCH2INTbits.CHTAIF = 0;
    }
    if(DCH2INTbits.CHERIF)
    {
        /* 
         * A channel address error has been detected. Either the source or the
         *  destination address is invalid.
         */
        DCH2INTbits.CHERIF = 0;
    }
    IFS2bits.DMA2IF = 0;
}

/******************************************************************************/
/* DMA3 Interrupt (Buffer copier)
/******************************************************************************/
void __ISR(_DMA_3_VECTOR , IPL7AUTO) DMA_3_IntHandler (void)
{   
    if(DCH3INTbits.CHSDIF)
    {
        /* Channel Source Pointer has reached end of source (CHSPTR = CHSSIZ) */
        if(DMA_TransferType)
        {
            if(DMA_TransferType == DMA_SPI_TRANSMIT)
            {
                /* transmit buffer has been copied so start transmitting */
                RDI_SPI_TransferToRaspberry(DMA_TransferAmount);
            }
        }
        DMA_BufferCopierComplete = TRUE;
        DCH3INTbits.CHSDIF = 0;
    }
    if(DCH3INTbits.CHSHIF)
    {
        /* 
         * Channel Source Pointer has reached midpoint of source 
         * (CHSPTR = CHSSIZ/2) 
         */
        DCH3INTbits.CHSHIF = 0;
    }
    if(DCH3INTbits.CHDHIF)
    {
        /*  
         * Channel Destination Pointer has reached midpoint of destination
         *  (CHDPTR = CHDSIZ/2) 
         */
        DCH3INTbits.CHDHIF = 0;
    }
    if(DCH3INTbits.CHBCIF)
    {
        /* 
         * A block transfer has been completed (the larger of CHSSIZ/CHDSIZ
         *  bytes has been transferred), or a pattern match event occurs  
         */
        DCH3INTbits.CHBCIF = 0;
    }
    if(DCH3INTbits.CHCCIF)
    {
        /* 
         * A cell transfer has been completed (CHCSIZ bytes have been
         *  transferred) 
         */
        DCH3INTbits.CHCCIF = 0;
    }
    if(DCH3INTbits.CHTAIF)
    {
        /* 
         * An interrupt matching CHAIRQ has been detected and the DMA transfer
         *  has been aborted 
         */
        DCH3INTbits.CHTAIF = 0;
    }
    if(DCH3INTbits.CHERIF)
    {
        /* 
         * A channel address error has been detected. Either the source or the
         *  destination address is invalid.
         */
        DCH3INTbits.CHERIF = 0;
    }
    IFS2bits.DMA3IF = 0;
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/