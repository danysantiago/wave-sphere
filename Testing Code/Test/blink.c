//*******************************************************************************
//  MSP-MMC Demo - Demo to show how to use the MMC library
//
//  Description; This example shows the correct setup and usage of the MMC
//  Library
//
//
//  S. Schauer
//  Texas Instruments, Inc
//  March 2008
//  V1.1: Updated for IAR Embedded Workbench Version: 5.10
//  V1.2: fixed fill of buffer (buffer size overrun)
//******************************************************************************

/* ***********************************************************
* THIS PROGRAM IS PROVIDED "AS IS". TI MAKES NO WARRANTIES OR
* REPRESENTATIONS, EITHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR
* COMPLETENESS OF RESPONSES, RESULTS AND LACK OF NEGLIGENCE.
* TI DISCLAIMS ANY WARRANTY OF TITLE, QUIET ENJOYMENT, QUIET
* POSSESSION, AND NON-INFRINGEMENT OF ANY THIRD PARTY
* INTELLECTUAL PROPERTY RIGHTS WITH REGARD TO THE PROGRAM OR
* YOUR USE OF THE PROGRAM.
*
* IN NO EVENT SHALL TI BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
* CONSEQUENTIAL OR INDIRECT DAMAGES, HOWEVER CAUSED, ON ANY
* THEORY OF LIABILITY AND WHETHER OR NOT TI HAS BEEN ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGES, ARISING IN ANY WAY OUT
* OF THIS AGREEMENT, THE PROGRAM, OR YOUR USE OF THE PROGRAM.
* EXCLUDED DAMAGES INCLUDE, BUT ARE NOT LIMITED TO, COST OF
* REMOVAL OR REINSTALLATION, COMPUTER TIME, LABOR COSTS, LOSS
* OF GOODWILL, LOSS OF PROFITS, LOSS OF SAVINGS, OR LOSS OF
* USE OR INTERRUPTION OF BUSINESS. IN NO EVENT WILL TI'S
* AGGREGATE LIABILITY UNDER THIS AGREEMENT OR ARISING OUT OF
* YOUR USE OF THE PROGRAM EXCEED FIVE HUNDRED DOLLARS
* (U.S.$500).
*
* Unless otherwise stated, the Program written and copyrighted
* by Texas Instruments is distributed as "freeware".  You may,
* only under TI's copyright in the Program, use and modify the
* Program without any charge or restriction.  You may
* distribute to third parties, provided that you transfer a
* copy of this license to the third party and the third party
* agrees to these terms by its first use of the Program. You
* must reproduce the copyright notice and any other legend of
* ownership on each copy or partial copy, of the Program.
*
* You acknowledge and agree that the Program contains
* copyrighted material, trade secrets and other TI proprietary
* information and is protected by copyright laws,
* international copyright treaties, and trade secret laws, as
* well as other intellectual property laws.  To protect TI's
* rights in the Program, you agree not to decompile, reverse
* engineer, disassemble or otherwise translate any object code
* versions of the Program to a human-readable form.  You agree
* that in no event will you alter, remove or destroy any
* copyright notice included in the Program.  TI reserves all
* rights not specifically granted under this license. Except
* as specifically provided herein, nothing in this agreement
* shall be construed as conferring by implication, estoppel,
* or otherwise, upon you, any license or other right under any
* TI patents, copyrights or trade secrets.
*
* You may not use the Program in non-TI devices.
* ********************************************************* */



#include <msp430fr5969.h>
#include "hal_hardware_board.h"
#include "mmc.h"
#include <stdio.h>

unsigned long cardSize = 0;
unsigned char status = 1;
unsigned int timeout = 0;
int i = 0;

unsigned char buffer[512];

int main( void )
{
  WDTCTL = WDTPW + WDTHOLD;

  //Initialisation of the MMC/SD-card
  while (status != 0)                       // if return in not NULL an error did occur and the
                                            // MMC/SD-card will be initialized again
  {
    status = mmcInit();
    timeout++;
    if (timeout == 150)                      // Try 50 times till error
    {
      //printf ("No MMC/SD-card found!! %x\n", status);
      break;
    }
  }

  while ((mmcPing() != MMC_SUCCESS));      // Wait till card is inserted

  // Read the Card Size from the CSD Register
  cardSize =  mmcReadCardSize();

// Clear Sectors on MMC
  for (i = 0; i < 512; i++) buffer[i] = 0;
  mmcWriteSector(0, buffer);                // write a 512 Byte big block beginning at the (aligned) adress

  for (i = 0; i < 512; i++) buffer[i] = 0;
  mmcWriteSector(1, buffer);                // write a 512 Byte big block beginning at the (aligned) adress

  mmcReadSector(0, buffer);                 // read a size Byte big block beginning at the address.
  for (i = 0; i < 512; i++) if(buffer[i] != 0) P1OUT |= 0x01;

  mmcReadSector(1, buffer);                 // read a size Byte big block beginning at the address.
  for (i = 0; i < 512; i++) if(buffer[i] != 0) P1OUT |= 0x02;


// Write Data to MMC
  for (i = 0; i < 512; i++) buffer[i] = i;
  mmcWriteSector(0, buffer);                // write a 512 Byte big block beginning at the (aligned) adress

  for (i = 0; i < 512; i++) buffer[i] = i+64;
  mmcWriteSector(1, buffer);                // write a 512 Byte big block beginning at the (aligned) adress

  mmcReadSector(0, buffer);                 // read a size Byte big block beginning at the address.
  for (i = 0; i < 512; i++) if(buffer[i] != (unsigned char)i) P1OUT |= 0x04;

  mmcReadSector(1, buffer);                 // read a size Byte big block beginning at the address.
  for (i = 0; i < 512; i++) if(buffer[i] != (unsigned char)(i+64)) P1OUT |= 0x08;

  for (i = 0; i < 512; i++)
    mmcReadSector(i, buffer);               // read a size Byte big block beginning at the address.

  mmcGoIdle();                              // set MMC in Idle mode

  while (1);
}
