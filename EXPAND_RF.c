/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 10/26/15     10.0_DW0b    Initial project make.
/******************************************************************************/

/******************************************************************************/
/* Contains functions for the RF expander card.
 * For use with PCBs:
 * 1. Catalyst_Expander_IR_RF revA
 *
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "CMD.h"
#include "EXPAND_RF.h"
#include "MISC.h"
#include "USER.h"

/******************************************************************************/
/* RF codes                          
 *          
 * The code consists of state switch timing in micro seconds.
 *    
 * Config 1:       
 * RF bit Timing for PT2260a
 * f = short long long short
 * 0 = short long short long
 * 1 = long short long short
 * 
 * Config 2:
 * these have different timing then the one above
 * zero is 650uS high followed by 1.75mS low
 * full pulse is 2.42mS
 * one is 1.75mS high followed by 650uS high
 * sync is short high followed by 11.89 mS low
/******************************************************************************/
/*~~~~~~~~~~~~~~ Livingroom light ~~~~~~~~~~~~~~~~~~~~~~~~*/
/* config 1 (RFA108) channel D */
const unsigned char Conf1_ChannelD[] = {f,f,f,f,f,f,0,0,1,0,0,0};//(A1 A0 x x x x x x)
long LivingroomLight[RF_CODE_SIZE] = {CODE_END};

/*~~~~~~~~~~~~~~ Livingroom fan ~~~~~~~~~~~~~~~~~~~~~~~~*/
// config 2 Channel H 2
const unsigned char Conf2_ChannelH_2_ON[] =  {0,1,1,0,1,0,0,0,0,0,1,0,0,0,0,0};
const unsigned char Conf2_ChannelH_2_OFF[] = {0,1,1,0,1,0,0,0,0,0,0,1,0,0,0,0};
long LivingroomFanOn[RF_CODE_SIZE] = {CODE_END};
long LivingroomFanOff[RF_CODE_SIZE] = {CODE_END};

/*~~~~~~~~~~~~~~ Christmas tree white lights ~~~~~~~~~~~~~~~~~~~~~~~~*/
/* config 2 Channel D */
const unsigned char Conf2_ChannelD_ON[] =  {0,1,1,0,1,0,0,0,0,0,1,0,0,0,0,1};
const unsigned char Conf2_ChannelD_OFF[] = {0,1,1,0,1,0,0,0,0,0,0,1,0,0,0,1};
long ChristmasTreeWhiteOn[RF_CODE_SIZE] = {CODE_END};
long ChristmasTreeWhiteOff[RF_CODE_SIZE] = {CODE_END};

/*~~~~~~~~~~~~~~ Christmas tree colored lights ~~~~~~~~~~~~~~~~~~~~~~~~*/
/* config 1 (RFA108) channel F */
const unsigned char Conf1_ChannelF[] = {f,0,f,f,f,f,0,0,1,0,0,0};//(A1 A0 x x x x x x)
long ChristmasTreeColor[RF_CODE_SIZE] = {CODE_END};

/*~~~~~~~~~~~~~~ Bedroom light ~~~~~~~~~~~~~~~~~~~~~~~~*/
/* config 1 (RFA108) channel E */
const unsigned char Conf1_ChannelE[] = {0,f,f,f,f,f,0,0,1,0,0,0};//(A1 A0 x x x x x x)
long BedroomLight[RF_CODE_SIZE] = {CODE_END};

/*~~~~~~~~~~~~~~ Bedroom fan ~~~~~~~~~~~~~~~~~~~~~~~~*/
/* config 2 Channel H 3 */
const unsigned char Conf2_ChannelH_3_ON[] =  {0,1,1,0,1,0,0,1,0,0,0,0,0,0,0,0};
const unsigned char Conf2_ChannelH_3_OFF[] = {0,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0};
long BedroomFanOn[RF_CODE_SIZE] = {CODE_END};
long BedroomFanOff[RF_CODE_SIZE] = {CODE_END};

/*~~~~~~~~~~~~~~ Bedroom bed shaker ~~~~~~~~~~~~~~~~~~~~~~~~*/
/* config 2 Channel H 1 */
const unsigned char Conf2_ChannelH_1_ON[] =  {0,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0};
const unsigned char Conf2_ChannelH_1_OFF[] = {0,1,1,0,1,0,0,0,0,1,0,0,0,0,0,0};
long BedroomBedOn[RF_CODE_SIZE] = {CODE_END};
long BedroomBedOff[RF_CODE_SIZE] = {CODE_END};

/*~~~~~~~~~~~~~~ Joes room light ~~~~~~~~~~~~~~~~~~~~~~~~*/
/* config 2 Channel B */
const unsigned char Conf2_ChannelB_ON[] =  {0,1,1,0,1,0,0,0,0,0,1,0,0,1,0,0};
const unsigned char Conf2_ChannelB_OFF[] = {0,1,1,0,1,0,0,0,0,0,0,1,0,1,0,0};
long JoesroomLightOn[RF_CODE_SIZE] = {CODE_END};
long JoesroomLightOff[RF_CODE_SIZE] = {CODE_END};

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
unsigned char LivingroomFanLast = ON;
unsigned char ChristmasTreeWhiteLast = ON;
unsigned char BedroomFanLast = ON;
unsigned char BedroomBedLast = ON;
unsigned char JoesroomLightLast = ON;

/******************************************************************************/
/* Inline Functions
/******************************************************************************/

/******************************************************************************/
/* RF_SetState
 *
 * The function controls the RF transmitter or turns it off. The transmitter is
 *  pulled up to 5 volts with an external resistor so to turn it on we just
 *  set the direction to input. To turn off the transmitter, pull the line low.
/******************************************************************************/
inline unsigned char RF_SetState(unsigned char state)
{
    unsigned char status = FALSE;
    
    if(RF_TRANSMIT_Tris == INPUT)
    {
        status = TRUE;
    }
    
    if(state)
    {
        RF_TRANSMIT_Tris = INPUT;
    }
    else
    {
        LATF &= ~RF_TRANSMIT;
        RF_TRANSMIT_Tris = OUTPUT;
    }
    return status;
}

/******************************************************************************/
/* RF_GetState
 *
 * The function returns the status of the RF transmitter (ON or OFF).
/******************************************************************************/
inline unsigned char RF_GetState(void)
{
    if(RF_TRANSMIT_Tris == INPUT)
    {
        return TRUE;
    }
    return FALSE;
}

/******************************************************************************/
/* Functions
/******************************************************************************/

/******************************************************************************/
/* InitRF
 *
 * The function initializes the RF transmitter.
/******************************************************************************/
void InitRF(void)
{
    RF_SetState(OFF);
    RF_CalculateCodes();
}

/******************************************************************************/
/* RF_CalculateConf1Codes
 *
 * The function calculates the timing for Configuration 1 codes.
/******************************************************************************/
void RF_CalculateConf1Codes(const unsigned char* code, long *timing)
{
    unsigned char i;
    unsigned char place = 0;
    
    /* 1 bits is 1024*a and the oscope shows that 1 bits is 1680 uS */
    double a = 1680.0 / 1024.0;
    
    /* the code is 8 address bits followed by 4 data bits followed by a sync bit 
     * 
     * 0 bit: 128a on, 384a off, 128a on, 384a off
     * 1 bit: 384a on, 128a off, 384a on, 128a off
     * f bit: 128a on, 384a off, 384a on, 128a off
     * sync bit: 128a on then 4096a off
     * 
     */
    
        place = 0;
    for(i=0;i<CONF1_SIZE;i++)
    {
        if(code[i] == 0)
        {
            timing[place++] = (long) MSC_Round(128.0 * a);
            timing[place++] = (long) MSC_Round(384.0 * a);
            timing[place++] = (long) MSC_Round(128.0 * a);
            timing[place++] = (long) MSC_Round(384.0 * a);
        }
        else if(code[i] == 1)
        {
            timing[place++] = (long) MSC_Round(384.0 * a);
            timing[place++] = (long) MSC_Round(128.0 * a);
            timing[place++] = (long) MSC_Round(384.0 * a);
            timing[place++] = (long) MSC_Round(128.0 * a);
        }
        else
        {
            /* f for floating */
            timing[place++] = (long) MSC_Round(128.0 * a);
            timing[place++] = (long) MSC_Round(384.0 * a);
            timing[place++] = (long) MSC_Round(384.0 * a);
            timing[place++] = (long) MSC_Round(128.0 * a);
        }
    }
        
    /* sync bit */
    timing[place++] = (long) MSC_Round(128.0 * a);
    timing[place++] = (long) MSC_Round(4096.0 * a);
            
    timing[place++] = CODE_END;
}

/******************************************************************************/
/* RF_CalculateConf2Codes
 *
 * The function calculates the timing for Configuration 2 codes.
/******************************************************************************/
void RF_CalculateConf2Codes(const unsigned char* code, long *timing)
{
    char i;
    char place = 0;
    
    /*
     * zero is 650uS high followed by 1.75mS low
     * full pulse is 2.42mS
     * one is 1.75mS high followed by 650uS high
     * sync is short high followed by 11.89 mS low
     */
    
    place = 0;
    for(i=0;i<CONF2_SIZE;i++)
    {
        if(code[i] == 0)
        {
            timing[place++] = 650;
            timing[place++] = 1750;
        }
        else
        {
            /* 1 */
            timing[place++] = 1750;
            timing[place++] = 650;
        }
    }
        
    /* sync bit */
    timing[place++] = (long) MSC_Round(650);
    timing[place++] = (long) MSC_Round(11890);
            
    timing[place++] = CODE_END;
}

/******************************************************************************/
/* RF_CalculateCodes
 *
 * The function calculates the timing for all of the codes.
/******************************************************************************/
void RF_CalculateCodes(void)
{

    /* configuration 1 (PT2260)
     * 
     * Livingroom light
     * Bedroom light 
     * christmas tree colored lights
     */
    
    /* Livingroom code */
    RF_CalculateConf1Codes(Conf1_ChannelD,LivingroomLight);
    
    /* Bedroom code */
    RF_CalculateConf1Codes(Conf1_ChannelE,BedroomLight);
    
    /* christmas tree colored lights */
    RF_CalculateConf1Codes(Conf1_ChannelF,ChristmasTreeColor);
    
    /* configuration 2
     * 
     * Livingroom fan
     * Christmas tree white lights
     * Bedroom fan
     * Bedroom bed shaker
     * Joes room light
     */
    
    /* Livingroom fan */
    RF_CalculateConf2Codes(Conf2_ChannelH_2_ON,LivingroomFanOn);
    RF_CalculateConf2Codes(Conf2_ChannelH_2_OFF,LivingroomFanOff);
    
    /* Christmas tree white lights*/
    RF_CalculateConf2Codes(Conf2_ChannelD_ON,ChristmasTreeWhiteOn);
    RF_CalculateConf2Codes(Conf2_ChannelD_OFF,ChristmasTreeWhiteOff);

    /* Bedroom fan */
    RF_CalculateConf2Codes(Conf2_ChannelH_3_ON,BedroomFanOn);
    RF_CalculateConf2Codes(Conf2_ChannelH_3_OFF,BedroomFanOff);
    
    /* Bedroom bed shaker */
    RF_CalculateConf2Codes(Conf2_ChannelH_1_ON,BedroomBedOn);
    RF_CalculateConf2Codes(Conf2_ChannelH_1_OFF,BedroomBedOff);
    
    /* Joes room light */
    RF_CalculateConf2Codes(Conf2_ChannelB_ON,JoesroomLightOn);
    RF_CalculateConf2Codes(Conf2_ChannelB_OFF,JoesroomLightOff);      
}

/******************************************************************************/
/* RF_SendCode
 *
 * The function sends an arbitrary code.
/******************************************************************************/
void RF_SendCode(unsigned long *code)
{
    unsigned char i,RF_Place;
    unsigned long* pointer = code;
    
    RF_SetState(OFF);
    for(i=0;i<RF_TRANSMIT_REPEAT;i++)
    {
        pointer = code;
        RF_Place = 0;
        while(*pointer != CODE_END && RF_Place < RF_CODE_SIZE)
        {
            if(RF_GetState())
            {
                RF_SetState(OFF);
            }
            else
            {
                RF_SetState(ON);
            }
            MSC_DelayUS(*pointer);
            pointer++;
            RF_Place++;
        }
    }
    RF_SetState(OFF);
}

/******************************************************************************/
/* RF_SendCode_CMD
 *
 * The function sends an RF code from the global CommandData.
/******************************************************************************/
void RF_SendCode_CMD(void)
{
    if(CommandDataPointer == LivingroomFanOn || CommandDataPointer == LivingroomFanOff)
    {
        /* Livingroom fan toggle */
        if(LivingroomFanLast == ON)
        {
            CommandDataPointer = LivingroomFanOff;
            LivingroomFanLast = OFF;
        }
        else
        {
            CommandDataPointer = LivingroomFanOn;
            LivingroomFanLast = ON;
        }
    }
    else if(CommandDataPointer == BedroomFanOn || CommandDataPointer == BedroomFanOff)
    {
        /* Bedroom fan toggle */
        if(BedroomFanLast == ON)
        {
            CommandDataPointer = BedroomFanOff;
            BedroomFanLast = OFF;
        }
        else
        {
            CommandDataPointer = BedroomFanOn;
            BedroomFanLast = ON;
        }
    }
    else if(CommandDataPointer == ChristmasTreeWhiteOn || CommandDataPointer == ChristmasTreeWhiteOff)
    {
        /* Christmas tree white lights toggle */
        if(ChristmasTreeWhiteLast == ON)
        {
            CommandDataPointer = ChristmasTreeWhiteOff;
            ChristmasTreeWhiteLast = OFF;
        }
        else
        {
            CommandDataPointer = ChristmasTreeWhiteOn;
            ChristmasTreeWhiteLast = ON;
        }
    }
    else if(CommandDataPointer == BedroomBedOn || CommandDataPointer == BedroomBedOff)
    {
        /* Bedroom bed shaker toggle */
        if(BedroomBedLast == ON)
        {
            CommandDataPointer = BedroomBedOff;
            BedroomBedLast = OFF;
        }
        else
        {
            CommandDataPointer = BedroomBedOn;
            BedroomBedLast = ON;
        }
    }
    else if(CommandDataPointer == JoesroomLightOn || CommandDataPointer == JoesroomLightOff)
    {
        /* Joes room light toggle */
        if(JoesroomLightLast == ON)
        {
            CommandDataPointer = JoesroomLightOff;
            JoesroomLightLast = OFF;
        }
        else
        {
            CommandDataPointer = JoesroomLightOn;
            JoesroomLightLast = ON;
        }
    }          
                       
    RF_SendCode(CommandDataPointer);
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/