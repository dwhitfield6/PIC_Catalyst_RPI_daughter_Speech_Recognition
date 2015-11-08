/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 10/29/15     10.0_DW0b   Initial project make.
/******************************************************************************/

/******************************************************************************/
/* Contains functions for the UART commands.
 *
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "CMD.h"
#include "EXPAND_IR.h"
#include "EXPAND_RF.h"
#include "UART.h"
#include "USER.h"
    
/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
COMMANDTYPE COMMANDS[NUMBER_OF_COMMANDS]= {
    {"\r\nTELEVISION POWER\r\n", IR_SendNEC_Repeat_CMD,&Sanyo_Power},
    {"\r\nVOLUME LOUD\r\n", IR_SendNEC_Repeat_Multiple_CMD,&Sanyo_Volume_Up},
    {"\r\nVOLUME QUIET\r\n", IR_SendNEC_Repeat_Multiple_CMD,&Sanyo_Volume_Down},
    {"\r\nLIVINGROOM LIGHT\r\n", RF_SendCode_CMD,LivingroomLight},
    {"\r\nLIVINGROOM FAN\r\n", RF_SendCode_CMD,LivingroomFanOn},
    {"\r\nBEDROOM LIGHT\r\n", RF_SendCode_CMD,BedroomLight},
    {"\r\nBEDROOM FAN\r\n", RF_SendCode_CMD,BedroomFanOn},
    {"\r\nCHRISTMAS BEAUTIFUL\r\n", RF_SendCode_CMD,ChristmasTreeColor},
    {"\r\nCHRISTMAS UGLY\r\n", RF_SendCode_CMD,ChristmasTreeWhiteOn},
};

long* CommandDataPointer;
unsigned char PhraseSearchFind[PHRASE_CHANNELS];
unsigned char SearchPhrase[PHRASE_CHANNELS][MAX_PHRASE_LENGTH];
unsigned char CheckPhrase = FALSE;
unsigned long PhraseIndex[PHRASE_CHANNELS];

/******************************************************************************/
/* Inline Functions
/******************************************************************************/

/******************************************************************************/
/* Functions
/******************************************************************************/

/******************************************************************************/
/* InitCMD
 *
 * The function initializes the command and phrase searching.
/******************************************************************************/
void InitCMD(void)
{
    unsigned char i;
    
    for(i=0; i<PHRASE_CHANNELS;i++)
    {
        CMD_PhraseCheckingClear(0);
        CMD_PhraseCheckingReset(0);
    }
}

/******************************************************************************/
/* CMD_Match
 *
 * The function searches through a buffer to look for matches and if one is
 *  found then it jumps to the function.
/******************************************************************************/
unsigned char CMD_Match(unsigned char* buffer, COMMANDTYPE* commands, unsigned char* Match_Index)
{
    unsigned char i;
    
    for(i=0;i< NUMBER_OF_COMMANDS;i++)
    {
        /* start the look over */
        commands[i].index = 0;
    }

    while(*buffer != 0)
    {
        /* search whole buffer */
        for(i=0;i< NUMBER_OF_COMMANDS;i++)
        {
            /* search for letter in all commands */
            if(commands[i].command[commands[i].index] == 0)
            {
                /* the command has been found */
                CommandDataPointer = commands[i].pValue;
                commands[i].Function();
                *Match_Index = i;
                return TRUE;
            }
            if(commands[i].command[commands[i].index] == *buffer)
            {
                /* the letter matches the command */
                commands[i].index++;
            }
            else
            {
                /* the letter does not match the command */
                commands[i].index = 0;
            }
        }
        buffer++;
    }
    return FALSE;
}

/******************************************************************************/
/* CMD_StreamingPhraseFound
 *
 * The function checks if the phrase was found.
/******************************************************************************/
unsigned char CMD_StreamingPhraseFound(unsigned char channel)
{
    if(PhraseSearchFind[channel])
    {
        return TRUE;
    }
    return FALSE;
}

/******************************************************************************/
/* CMD_StreamingPhraseSet
 *
 * The function sets the phrase to search for.
/******************************************************************************/
void CMD_StreamingPhraseSet(unsigned char* phrase, unsigned char channel)
{
    unsigned char status;
    status = CMD_PhraseChecking(OFF);
    CMD_PhraseCheckingClear(channel);
    MSC_StringCopy(phrase, &SearchPhrase[channel][0]);
    if(status)
    {
        CMD_PhraseChecking(ON);
    }
}

/******************************************************************************/
/* CMD_PhraseChecking
 *
 * The function controls phase checking.
/******************************************************************************/
unsigned char CMD_PhraseChecking(unsigned char state)
{
    unsigned char status = CheckPhrase;
    if(state)
    {
        CheckPhrase = TRUE;
    }
    else
    {
        CheckPhrase = FALSE;
    }
    return status;
}

/******************************************************************************/
/* CMD_PhraseCheckingClear
 *
 * The function resets phase checking algorithm.
/******************************************************************************/
void CMD_PhraseCheckingClear(unsigned char channel)
{
    PhraseIndex[channel] = 0;
    PhraseSearchFind[channel] = 0;
}

/******************************************************************************/
/* CMD_PhraseCheckingReset
 *
 * The function resets phase checking algorithm and clears the buffer.
/******************************************************************************/
void CMD_PhraseCheckingReset(unsigned char channel)
{
    unsigned char status;
    
    status = CMD_PhraseChecking(OFF);
    PhraseSearchFind[channel] = FALSE;         
    CMD_PhraseCheckingClear(channel);
    MSC_CleanBuffer(RX1_Buffer, UART1_RECEIVE_SIZE);
    UART_CleanReceive1();
    if(status)
    {
        CMD_PhraseChecking(ON);
    }
}

            
/******************************************************************************/
/* CMD_StreamingPhraseSearch
 *
 * The function checks for a phrase live when data is streaming.
/******************************************************************************/
unsigned char CMD_StreamingPhraseSearch(unsigned char data,unsigned char* phrase, unsigned char channel)
{  
    if(CheckPhrase)
    {
        if(phrase[PhraseIndex[channel]] == 0)
        {
            /* start over if we are already pointing at a null */
            PhraseIndex[channel] = 0;
        }

        if(data == phrase[PhraseIndex[channel]])
        {
            PhraseIndex[channel]++;
            if(phrase[PhraseIndex[channel]] == 0)
            {
                PhraseSearchFind[channel] = TRUE;
                return TRUE;
            }
        }
        else
        {
            /* start over since the data does not match the phrase */
            PhraseIndex[channel] = 0;
        }
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/