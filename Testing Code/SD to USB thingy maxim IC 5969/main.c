/**
 * SDcardTest.c - For testing SD card functions.
 * Author: Samuel
 */
#include <msp430fr5969.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "pff2a/src/diskio.h"
#include "pff2a/src/pff.h"
#include "drivers/spi.h"

#define DESELECT_MODE()	P1OUT &= ~BIT3
#define SELECT_MODE()	P1OUT |= BIT3
#define TOGGLE_MODE()	P1OUT ^= BIT3
/**
 * Seup - initialize timers and clocks
 */
void setup() {
	WDTCTL = WDTPW + WDTHOLD;       // Stop the watchdog timer.

	CSCTL0_H = 0xA5; 				// Set password to write clock registers.
	CSCTL1 = DCOFSEL_4 | DCORSEL; 	// Set DCOCLK to 16MHz
	CSCTL2 = SELM__DCOCLK + SELS__DCOCLK; // Set MCLK and SMCLK to DCO
	CSCTL0_H = 0; 					// Lock clock registers

	P3DIR |= BIT4;	// CS initially disabled (it is active low)
	P3SEL1 |= BIT4; // output SMCLK for measurement on P3.4
	// GPIO sd mode, puerto 1.3
	DESELECT_MODE();
	P1DIR |= BIT3; // as output
	P1DIR &= ~BIT1;
	P1IE |= BIT1; // interrupt de boton en P1.1 para hacer toggle del mode
	P2DIR &= ~BIT3; // P2.3 as input
}

char line[64]; // serial and general buffer

void main (void)
{
	FRESULT res;
	FATFS fs; // File system object

	setup();
	spi_initialize();
	_enable_interrupt(); // enable timers for serial isr handlers

	for(;;) {
		res = (FRESULT) disk_initialize();
		if (res == FR_OK) {
			break;
		}
	}

	res = pf_mount(&fs); // check output of function, fi, mount
	res = pf_open("WRITE.TXT");

	// write 100 lines to file
	if (res == FR_OK) {
		WORD bw;
		WORD n = 0;
		do {
			ltoa(++n, line);
			strcat(line," The quick brown fox jumps over the lazy dog. 1234567890\r\n");
			res = pf_write(line, strlen(line), &bw);
			if (res != FR_OK) {
				break;
			}
		} while(n < 100);

		if (res == FR_OK) {
			res = pf_write(0, 0, &bw); // finalize the write process
			if (res != FR_OK) {
				// error
			}
		}
	}

	while(1);
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void) {
	// toggle mode
	TOGGLE_MODE();
	__no_operation();
}
