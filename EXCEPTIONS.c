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
/* Contains the exception handler.
 *
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>

#include "USER.h"
#include "MISC.h"
#include "EXCEPTIONS.h"
#include "I2C.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
FAULTTYPE Fault;

/******************************************************************************/
/* Functions
/******************************************************************************/

/******************************************************************************/
/* InitException
 *
 * The function initializes the exception and fault parameters.
/******************************************************************************/
void InitException(void)
{    
    unsigned char string[] = "FaultLog";
    if(!I2C_RASP_GEN_EEPROMReadFaultLog())
    {
        EXC_ClearFaults();
        return;
    }
    if(Fault.LogCreated != TRUE || !MSC_StringMatch(Fault.LogCreatedString,string))
    {
        EXC_ClearFaults();
        Fault.LogCreated = TRUE;
        MSC_StringCopy(string, Fault.LogCreatedString);
    }
    I2C_RASP_GEN_EEPROMWriteFaultLog();
    I2C_RASP_GEN_EEPROMReadFaultLog();
}

/******************************************************************************/
/* void EXC_ClearFaults(void)

 *
 * This function clears the fault parameters.
/******************************************************************************/
void EXC_ClearFaults(void)
{
    unsigned short size;
    
    size = sizeof(Fault);
    MSC_CleanBuffer(&Fault,size);   
}

/******************************************************************************/
/* general_Exception_Handler
 *
 * This function overrides the normal _weak_ generic handler.
/******************************************************************************/
void general_Exception_Handler(void)
{
    unsigned long code;
    unsigned long address;
    
    /* an exception happened */
    asm volatile("mfc0 %0,$13" : "=r" (code));
    asm volatile("mfc0 %0,$14" : "=r" (address));

    code = (code & 0x0000007C) >> 2;
    Fault.Exception = TRUE;
    Fault.ExceptionAddress = address;
    Fault.ExceptionCode = code;
    I2C_RASP_GEN_EEPROMWriteFaultLog();
}
