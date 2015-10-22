/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 09/12/15     1.0_DW0a    Initial project make.
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#ifndef RDI_H
#define	RDI_H

#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "USER.h"

/******************************************************************************/
/* Banner_RECEIVE_SIZE
 *
 * This is the number of maximum number of characters that make up an RDI
 *  instrument banner.
/******************************************************************************/
#define Banner_RECEIVE_SIZE 250

/******************************************************************************/
/* Banner_TIMEOUT
 *
 * This is the number of milliseconds to wait between a break and a banner to
 *  be printed.
/******************************************************************************/
#define Banner_TIMEOUT 2000

/******************************************************************************/
/* RDI catalyst compatible products
 *
 * This is the RDI products that work with the catalyst.
/******************************************************************************/
#define PROD_WORKHORSE_HADCP  1

/******************************************************************************/
/* Catalyst strings
 *
 * This is the banner extention string for a product banner
/******************************************************************************/
extern const unsigned char Workhorse_H_ADCP[];
extern const unsigned char Catalyst_PIC_only_String[];
extern const unsigned char Catalyst_String[];

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
extern unsigned short RDI_product;
extern unsigned short Banner_Buffer_Place;
extern unsigned char Banner_Buffer[Banner_RECEIVE_SIZE];
extern unsigned char GetProduct;
extern volatile unsigned char BannerFinished;
extern volatile unsigned char Banner_Correct_place;

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
void InitRDI(void);
void RDI_GetProduct(void);
void RDI_ClearProduct(void);
void RDI_PrintBanner(void);
void RDI_PrintBannerExtention(void);
unsigned char RDI_RequestRaspberryPiSPI(unsigned char state);
unsigned char RDI_SendToRaspberry(unsigned long address, unsigned short amount);
void RDI_SPI_TransferToRaspberry(unsigned short amount);

#endif	/* RDI_H */