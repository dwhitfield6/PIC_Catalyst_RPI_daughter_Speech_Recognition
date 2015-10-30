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
    {"Television", IR_SendNEC_Repeat_CMD,&Sanyo_Power},
    {"Livingroom Light", RF_SendCode_CMD,LivingroomLight},
    {"Livingroom Fan", RF_SendCode_CMD,LivingroomFan},
    {"Bedroom Light", RF_SendCode_CMD,BedroomLight},
    {"Bedroom Fan", RF_SendCode_CMD,BedroomFan},
    {"Christmas Tree pretty", RF_SendCode_CMD,ChristmasTreeColor},
    {"Christmas Tree ugly", RF_SendCode_CMD,ChristmasTreeWhite},
};

unsigned long* CommandDataPointer;

/******************************************************************************/
/* Inline Functions
/******************************************************************************/

/******************************************************************************/
/* Functions
/******************************************************************************/

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

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/