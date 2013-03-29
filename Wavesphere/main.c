/**
 * main.c
 * Author: Samuel
 *
 * This is the main entry point of the Wavesphere embedded application.
 * Initial version contains blinking LED example.
 * See System Flowchart.
 */

#include <msp430fr5969.h>
#include "main.h"

#ifndef DEBUG
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	P1DIR |= 0x01;					// Set P1.0 to output direction

	for(;;) {
		volatile unsigned int i;	// volatile to prevent optimization

		P1OUT ^= 0x01;				// Toggle P1.0 using exclusive-OR

		i = 10000;					// SW Delay
		do i--;
		while(i != 0);
	}
	
	return 0;
}
#endif
