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
/* Files to Include                                                           */
/******************************************************************************/
#ifndef CMD_H
#define	CMD_H

#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "USER.h"

/******************************************************************************/
/* Structures                                                                 */
/******************************************************************************/
typedef void (*pFunction)(void); // pointer to a function

typedef struct commands
{
	unsigned char command[80]; // command phrase to check for
	pFunction Function;        // function to call when phrase is found
    unsigned long* Value;      // value to pass into the function
    unsigned long index;       // place in string where match function is currently looking
}COMMANDTYPE;
    
/******************************************************************************/
/* NUMBER_OF_COMMANDS
 *
 * This is the number of commands.
/******************************************************************************/
#define NUMBER_OF_COMMANDS 7

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
extern const COMMANDTYPE COMMANDS[NUMBER_OF_COMMANDS];
extern unsigned long* CommandDataPointer;

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

/******************************************************************************/
/* Macro Functions                                                            */
/******************************************************************************/

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
unsigned char CMD_Match(unsigned char* buffer, COMMANDTYPE* commands, unsigned char* Match_Index);

#endif	/* CMD_H */