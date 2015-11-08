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

#include "UART.h"
#include "USER.h"

/******************************************************************************/
/* check phrase
 *
 * This is the phase that triggers the system to check for the key command
 *  phrases.
/******************************************************************************/
#define CHECK_PHRASE   "READY...."

/******************************************************************************/
/* NUMBER_OF_COMMANDS
 *
 * This is the number of commands.
/******************************************************************************/
#define NUMBER_OF_COMMANDS 9

/******************************************************************************/
/* PHRASE_CHANNELS
 *
 * This is the number of phrases that we can search for at the same time.
/******************************************************************************/
#define PHRASE_CHANNELS 10

/******************************************************************************/
/* MAX_PHRASE_LENGTH
 *
 * This is the maximum phrase length.
/******************************************************************************/
#define MAX_PHRASE_LENGTH 50

/******************************************************************************/
/* Structures                                                                 */
/******************************************************************************/
typedef void (*pFunction)(void); // pointer to a function

typedef struct commands
{
	unsigned char command[80]; // command phrase to check for
	pFunction Function;        // function to call when phrase is found
    long* pValue;      // pointer to the value to pass into the function
    unsigned long index;       // place in string where match function is currently looking
}COMMANDTYPE;
   
/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
extern COMMANDTYPE COMMANDS[NUMBER_OF_COMMANDS];
extern long* CommandDataPointer;
extern unsigned char PhraseSearchFind[PHRASE_CHANNELS];
extern unsigned char SearchPhrase[PHRASE_CHANNELS][MAX_PHRASE_LENGTH];
extern unsigned char CheckPhrase;
extern unsigned long PhraseIndex[PHRASE_CHANNELS];

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

/******************************************************************************/
/* Macro Functions                                                            */
/******************************************************************************/

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
void InitCMD(void);
unsigned char CMD_Match(unsigned char* buffer, COMMANDTYPE* commands, unsigned char* Match_Index);
unsigned char CMD_StreamingPhraseFound(unsigned char channel);
void CMD_StreamingPhraseSet(unsigned char* phrase, unsigned char channel);
unsigned char CMD_PhraseChecking(unsigned char state);
void CMD_PhraseCheckingClear(unsigned char channel);
void CMD_PhraseCheckingReset(unsigned char channel);
unsigned char CMD_StreamingPhraseSearch(unsigned char data,unsigned char* phrase, unsigned char channel);

#endif	/* CMD_H */