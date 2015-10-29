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
/* Contains functions for the IR LED expander card. 
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
#include "EXPAND_IR.h"
#include "SYSTEM.h"
#include "TIMERS.h"
#include "USER.h"

/******************************************************************************/
/* IR codes                          
 *          
 * This is the NEC codes that we send.
 *                              
/******************************************************************************/
/*~~~~~~~~~~~~~~ Sanyo TV ~~~~~~~~~~~~~~~~~~~~~~~~*/
const unsigned long Sanyo_Power         = 0x1CE348B7;
const unsigned long Sanyo_Sleep         = 0x1CE3B04F;
const unsigned long Sanyo_Menu          = 0x1CE3E817;
const unsigned long Sanyo_Volume_Up     = 0x1CE3708F;
const unsigned long Sanyo_Volume_Down   = 0x1CE3F00F;
const unsigned long Sanyo_Channel_Up    = 0x1CE350AF;
const unsigned long Sanyo_Channel_Down  = 0x1CE3D02F;
const unsigned long Sanyo_Mute          = 0x1CE318E7;
const unsigned long Sanyo_Back          = 0x1CE39867;
const unsigned long Sanyo_1             = 0x1CE3807F;
const unsigned long Sanyo_2             = 0x1CE340BF;
const unsigned long Sanyo_3             = 0x1CE3C03F;
const unsigned long Sanyo_4             = 0x1CE320DF;
const unsigned long Sanyo_5             = 0x1CE3A05F;
const unsigned long Sanyo_6             = 0x1CE3609F;
const unsigned long Sanyo_7             = 0x1CE3E01F;
const unsigned long Sanyo_8             = 0x1CE310EF;
const unsigned long Sanyo_9             = 0x1CE3906F;
const unsigned long Sanyo_0             = 0x1CE300FF;
const unsigned long Sanyo_Enter         = 0x1CE32AD5;
const unsigned long Sanyo_Source        = 0x1CE3C837;
const unsigned long Sanyo_Rewind        = 0x1CE3946B;
const unsigned long Sanyo_Play          = 0x1CE324DB;
const unsigned long Sanyo_Forward       = 0x1CE314EB;
const unsigned long Sanyo_Record        = 0xFFFFFFFF;
const unsigned long Sanyo_Pause         = 0x1CE3649B;
const unsigned long Sanyo_Stop          = 0x1CE3A45B;

/*~~~~~~~~~~~~~~ Vizio TV ~~~~~~~~~~~~~~~~~~~~~~~~*/
const unsigned long Vizio_Power         = 0x20DF10EF;
const unsigned long Vizio_Sleep         = 0x20DF708F;
const unsigned long Vizio_Menu          = 0x20DFF20D;
const unsigned long Vizio_Volume_Up     = 0x20DF40BF;
const unsigned long Vizio_Volume_Down   = 0x20DFC03F;

/*~~~~~~~~~~~~~~ Idylis AC unit ~~~~~~~~~~~~~~~~~~~~~~~~*/
const unsigned long Idylis_Power        = 0x20DF10EF;

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
unsigned short IR_PWM50;
unsigned char IR_state = OFF;

/******************************************************************************/
/* Inline Functions
/******************************************************************************/
/******************************************************************************/
/* IR_LEDUse
 *
 * The function sets the IR LED to PWM mode.
/******************************************************************************/
inline unsigned char IR_LEDUse(unsigned char state)
{
    unsigned char status = FALSE;
    
    if(RPG0R)
    {
        status = TRUE;
    }
    
    if(state)
    {
        /* Map the remappable pin */ 

        RPG0R    = IR_LED1_Module;    // RG0 = OC4 aka IR LED modulation signal
    }
    else
    {
        LATD &= ~IR_LED1;
        RPG0R = 0;
    }

    return status;
}

/******************************************************************************/
/* IR_LEDTest
 *
 * The function tests the IR LED.
/******************************************************************************/
inline void IR_LEDTest(void)
{
    IR_LEDUse(OFF);
    while(1)
    {
        LATG &= ~IR_LED1;
        MSC_DelayUS(1000);
        LATG |= IR_LED1; 
        MSC_DelayUS(1000);
    }
}

/******************************************************************************/
/* IR_Module
 *
 * This function controls the IR PWM module.
/******************************************************************************/
void IR_Module(unsigned char state)
{
    
#undef ON
    
    if(state)
    {  
        OC4CONbits.ON = TRUE;     // Output Compare peripheral is enabled
    }
    else
    {
        OC4CONbits.ON = FALSE;    // Turn off output compare peripheral
    }
#define ON 1
}

/******************************************************************************/
/* Functions
/******************************************************************************/

/******************************************************************************/
/* InitIR
 *
 * The function initializes the IR LED transmitter.
/******************************************************************************/
void InitIR(void)
{   
    // Initialize Output Compare Module
    OC4CONbits.SIDL = 0; // Continue operation in Idle mode
    
    OC4CONbits.OCM = 0b110; // PWM mode on OC4; Fault pin disabled
    
    OC4R = 0; // Write the duty cycle for the first PWM pulse of IR LED
    OC4RS = 0; // Write the duty cycle for the second PWM pulse
    
    OC4CONbits.OCTSEL = 1; // Select Timer 3 as output compare time base
    
    InitTIMER3(38000);
    IR_Module(ON);
}

/******************************************************************************/
/* IR_SendNECRepeat
 *
 * The function sends the NEC repeat code.
/******************************************************************************/
void IR_SendNECRepeat(void)
{
    IR_state = OFF;
    TMR_ResetTimer3();
    IFS0bits.T3IF = 0; // Clear Timer 2 Interrupt Flag
    TMR_InterruptTimer3(ON);
    TMR_EnableTimer3(ON);
    IR_LEDUse(TRUE);
    
    /* send repeat */
    IR_state = ON;
    MSC_DelayUS(9000);
    IR_state = OFF;
    MSC_DelayUS(2250);
    
    IR_state = ON;
    MSC_DelayUS(563);
    IR_state = OFF;
    MSC_DelayUS(96875);
            
    IR_state = OFF;
    TMR_EnableTimer3(OFF);
    IR_LEDUse(FALSE);
    TMR_ResetTimer3();
    TMR_InterruptTimer3(OFF); 
}

/******************************************************************************/
/* IR_SendNEC
 *
 * The function sends the NEC code.
/******************************************************************************/
void IR_SendNEC(unsigned long* NEC)
{
    unsigned char i;
    unsigned long code;
    
    code = *NEC;
    code = MSC_ReverseLong(code);
    IR_state = OFF;
    TMR_ResetTimer3();
    IFS0bits.T3IF = 0; // Clear Timer 2 Interrupt Flag
    TMR_InterruptTimer3(ON);
    TMR_EnableTimer3(ON);
    IR_LEDUse(TRUE);
    
    /* send header sync pulse */
    IR_state = ON;
    MSC_DelayUS(9000);
    IR_state = OFF;
    MSC_DelayUS(4500);
    for(i=0; i<32;i++)
    {
        if(code & 0x00000001)
        {
            /* send logical 1 */
            IR_state = ON;
            MSC_DelayUS(563);
            IR_state = OFF;
            MSC_DelayUS(1686);
        }
        else
        {
            /* send logical 0 */
            IR_state = ON;
            MSC_DelayUS(563);
            IR_state = OFF;
            MSC_DelayUS(563);
        }
        code >>= 1;
    }
    /* final burst */
    IR_state = ON;
    MSC_DelayUS(563);
    IR_state = OFF;
    MSC_DelayUS(40500);
    
    IR_state = OFF;
    TMR_EnableTimer3(OFF);
    IR_LEDUse(FALSE);
    TMR_ResetTimer3();
    TMR_InterruptTimer3(OFF);    
}

/******************************************************************************/
/* IR_SendNEC
 *
 * The function sends the NEC code and a repeat.
/******************************************************************************/
void IR_SendNEC_Repeat(unsigned long* NEC)
{
    IR_SendNEC(NEC);
    IR_SendNECRepeat();
}

/******************************************************************************/
/* IR_SendNEC_Repeat_CMD
 *
 * The function sends the NEC code and a repeat from the global CommandData.
/******************************************************************************/
void IR_SendNEC_Repeat_CMD(void)
{
    IR_SendNEC(CommandDataPointer);
    IR_SendNECRepeat();
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/