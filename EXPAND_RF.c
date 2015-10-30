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
#include "USER.h"

/******************************************************************************/
/* RF codes                          
 *          
 * The code consists of state switch timing in micro seconds.
 *                               
/******************************************************************************/
/*~~~~~~~~~~~~~~ Livingroom light ~~~~~~~~~~~~~~~~~~~~~~~~*/
const long LivingroomLight[RF_CODE_SIZE] = {CODE_END};

/*~~~~~~~~~~~~~~ Livingroom fan ~~~~~~~~~~~~~~~~~~~~~~~~*/
const long LivingroomFan[RF_CODE_SIZE] = {CODE_END};

/*~~~~~~~~~~~~~~ Christmas tree white lights ~~~~~~~~~~~~~~~~~~~~~~~~*/
const long ChristmasTreeWhite[RF_CODE_SIZE] = {CODE_END};

/*~~~~~~~~~~~~~~ Christmas tree colored lights ~~~~~~~~~~~~~~~~~~~~~~~~*/
const long ChristmasTreeColor[RF_CODE_SIZE] = {CODE_END};

/*~~~~~~~~~~~~~~ Bedroom light ~~~~~~~~~~~~~~~~~~~~~~~~*/
/* RFA108 channel E */
const long BedroomLight[RF_CODE_SIZE] = {CODE_END};

/*~~~~~~~~~~~~~~ Bedroom fan ~~~~~~~~~~~~~~~~~~~~~~~~*/
const long BedroomFan[RF_CODE_SIZE] = {CODE_END};

/*~~~~~~~~~~~~~~ Joes room light ~~~~~~~~~~~~~~~~~~~~~~~~*/
const long JoesroomLight[RF_CODE_SIZE] = {CODE_END};

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

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
    
    for(i=0;i<RF_TRANSMIT_REPEAT;i++)
    {
        pointer = code;
        RF_Place = 0;
        while(*pointer != CODE_END && RF_Place < RF_CODE_SIZE)
        {
            if(RF_GetState)
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
}

/******************************************************************************/
/* RF_SendCode_CMD
 *
 * The function sends an arbitrary code from the global CommandData.
/******************************************************************************/
void RF_SendCode_CMD(void)
{
    RF_SendCode(CommandDataPointer);
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/