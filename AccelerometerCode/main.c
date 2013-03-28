#include <msp430fr5969.h>
#include "accelerometer.h"

/*
 * main.c
 */

void main() {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	//To Know that program has started running
	P1DIR |= BIT0;                            // Set P1.0/LED to output
	P1OUT |= BIT0;                           // Clear LED to start

	P4DIR |= BIT3 | BIT4;							 // set p4.3 and p4.4 as output

	initAcc();
	accStartST();

	__bis_SR_register(GIE);
	while (1) {
		getSamples();
	}
}

