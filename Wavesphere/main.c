/**
 * main.c
 * Author: Samuel
 *
 * This is the main entry point of the Wavesphere embedded application.
 * Initial version contains blinking LED example.
 * See System Flowchart.
 */

#include <msp430fr5969.h>
#include <stdbool.h>
#include "main.h"

/**
 * System Flags Structure
 * Controls the system flow
 */
volatile struct SYSTEM_FLAG system_flags = {
		false, // .diagnostic_flag = false,
		false, //.retreival_flag = false,
		false, //.sampling_flag = false,
		false //.shutdown_flag = false
};

/**
 * Initially sets up system
 */
void setup_clocks() {
	_nop();
	return;
}

#ifndef DEBUG
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

	// prepare clocks with 12MHz crystal
	setup_clocks();
	_enable_interrupts(); // enable global interrupts

	for(;;) {
		// shutdown system
		// clear all flags (in case)
		// enter low power mode

		// when low power mode is exited
		for(;;) {
			if (system_flags.shutdown_flag) {
				break;
			}
			else if (system_flags.diagnostic_flag) {
				diagnostic_service(); // will send all info through the xbee once.
			}
			else if (system_flags.retrieval_flag) {
				system_flags.retrieval_flag = false;
				retrieval_service();
			}
			else if (system_flags.sampling_flag) {
				// sampling flag is cleared when the device is found
				sampling_service();
				location_service();
			} else {
				// status mode thingy...
				status_service(); // will send status info through the xbee once.
			}
		}
	}
}

#endif

#pragma vector=USCI_A1_VECTOR
__interrupt void XBee_ISR(void)
{
	unsigned char received_char;
	switch(__even_in_range(UCA1IV,USCI_UART_UCTXCPTIFG))
	{
	case USCI_NONE: break;
	case USCI_UART_UCRXIFG:
		// This is the RX case, needed mostly for the main system flowchart.
		received_char = UCA1RXBUF;
		switch(received_char) {
		case M_DIAGNOSTIC_CLEAR:
			system_flags.diagnostic_flag = false;
			break;
		case M_DIAGNOSTIC_SET:
			system_flags.diagnostic_flag = true;
			break;
		case M_RETRIEVAL_SET:
			system_flags.retrieval_flag = true;
			break;
		case M_SAMPLING_SET:
			system_flags.sampling_flag = true;
			break;
		case M_SAMPLING_CLEAR:
			system_flags.sampling_flag = false;
			break;
		case M_SHUTDOWN_SET:
			system_flags.shutdown_flag = true;
			break;
		default:
			// break power mode
			break;

		}
		break;

		case USCI_UART_UCTXIFG: break;
		case USCI_UART_UCSTTIFG: break;
		case USCI_UART_UCTXCPTIFG: break;
	}
}
