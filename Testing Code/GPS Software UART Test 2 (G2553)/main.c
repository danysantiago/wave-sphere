#include <msp430.h>
#include <stdint.h>
#include "config.h"
#include "softserial.h"


/*
 * main.c
 */
int main(void) {
	unsigned char c;
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	if (CALBC1_1MHZ==0xFF)					// If calibration constant erased
	{
		while(1);                               // do not load, trap CPU!!
	}
	DCOCTL = 0;                               // Select lowest DCOx and MODx settings
	BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
	DCOCTL = CALDCO_1MHZ;
	P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
	P1SEL2 = BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 = 104;                            // 1MHz 9600
	UCA0BR1 = 0;                              // 1MHz 9600
	UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

	_enable_interrupts();
	// get data from GPS
	SoftSerial_init();

	for (;;) {
		if (!SoftSerial_empty()) {
			c = SoftSerial_read();
			while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
			UCA0TXBUF = c;
		}
	}

}
