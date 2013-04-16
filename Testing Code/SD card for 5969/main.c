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

/**
 * Seup - initialize timers and clocks
 */
void setup() {
	WDTCTL = WDTPW | WDTHOLD;                 // Stop Watchdog

	// Startup clock system with max DCO setting ~8MHz
	CSCTL0 = CSKEY;                           // Unlock clock registers
	CSCTL1 = DCOFSEL_3 | DCORSEL;             // DCO - 8Mhz
	CSCTL2 = SELA__VLOCLK + SELS__DCOCLK + SELM__DCOCLK;
	CSCTL3 = DIVA__1 + DIVS__1 + DIVM__1;     // set all dividers
	// Setup LED Pin
	P1OUT |= BIT3;
	P1DIR |= BIT3;

	// Configure UART pins
	P2REN &= ~(BIT5 + BIT6);
	P2SEL1 |= BIT5 + BIT6;
	P2SEL0 &= ~(BIT5 + BIT6);
	P2DIR |= BIT6; // TX as output

	// Configure UART
	UCA1CTL1 |= UCSWRST;                      // Put eUSCI in reset
	UCA1CTL1 |= UCSSEL__SMCLK;                // CLK = SMCLK

	// Baud Rate calculation
	// 8000000/(16*9600) = 52.083
	// Fractional portion = 0.083
	// User's Guide Table 21-4: UCBRSx = 0x04
	// UCBRFx = int ( (52.083-52)*16) = 1
	UCA1BR0 = 52;                             // 8000000/16/9600
	UCA1BR1 = 0x00;
	UCA1MCTLW |= UCOS16 | UCBRF_1;

	UCA1CTL1 &= ~UCSWRST;                     // Initialize eUSCI
	UCA1IE |= UCRXIE;                         // Enable USCI_A1 RX interrupt

	_enable_interrupts();
	_nop();                         // For debugger

	P3DIR |= BIT4;	// CS initially disabled (it is active low)
	P3SEL1 |= BIT4; // output SMCLK for measurement on P3.4
}

char line[64]; // serial and general buffer

void main (void)
{
	FRESULT res;
	FATFS fs; // File system object
	WORD s1; // unsigned short
	unsigned char i;

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


	pf_lseek(0); // seek to start of file

	res = pf_read(line, sizeof(line), &s1); // fill up buffer and then dump it through uart
	if (res != FR_OK) {
		// ERROR
	}

	for (i = 0; s1 > 0; s1--, i++) {
		// send line[i] through uart
		while (!(UCA1IFG&UCTXIFG));                // USCI_A0 TX buffer ready?
		UCA1TXBUF = line[i];
	}


	while(1);
}
