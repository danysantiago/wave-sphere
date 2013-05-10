/**
 * main.c
 * Author: Samuel
 *
 * This is the main entry point of the Wavesphere embedded application.
 * See System Flowchart.
 */

#include <msp430fr5969.h>
#include <stdbool.h>
#include <stdint.h>
#include "main.h"
#include "common/spi/spi.h"
#include <string.h>

/**
 * System Flags Structure
 * Controls the system flow
 */
volatile struct SYSTEM_FLAG system_flags = {
		false, // .diagnostic_flag = false,
		false, //.retreival_flag = false,
		false, //.sampling_flag = false,
		false, //.shutdown_flag = false,
		false  //.locate_flag = false
};

const char id[9] = "000-0001";

/**
 * Utility function that sets up a fake DCO 12MHz crystal.
 * Used for when the crystal is acting up.
 */
void setup_fcrystal() {
	CSCTL0_H = 0xA5; 							// Write password to unlock clock registers
	CSCTL1 = DCOFSEL_6; 						// Set DCO to 24MHz
	CSCTL1 |= DCORSEL;
	CSCTL2 = SELS__DCOCLK + SELM__DCOCLK; 		// set SMCLK and MCLK to HFXTCLK 12MHz crystal oscillator
	CSCTL3 = DIVS__2 + DIVM__2; 				// set SMCLK divisor to 1, SMCLK = 12MHz

	P3SEL1 |= BIT4;
	P3DIR |= BIT4; // output SMCLK on P3.4
	CSCTL0_H = 0;
	return;
}

/**
 * Sets up clock system with 12MHz crystal.
 */
void setup_crystal() {
	// Set PJ for HF Crystal use
	PJSEL0 |= BIT6;
	PJSEL1 &= ~BIT6;

	// Configure CS module
	CSCTL0_H = 0xA5; 							// Write password to unlock clock registers
	CSCTL1 = DCOFSEL_0; 						// Set DCO to 1MHz
	CSCTL1 &= ~DCORSEL;
	CSCTL2 = SELS__HFXTCLK + SELM__HFXTCLK; 	// set SMCLK and MCLK to HFXTCLK 12MHz crystal oscillator
	CSCTL3 = DIVS__1 + DIVM__1; 				// set SMCLK divisor to 1, SMCLK = 12MHz
	CSCTL4 |= HFFREQ_2;							// set HFX frequency to 8-16MHz, HFXTOFF = 0, turn it on
	CSCTL4 &= ~HFXTOFF;
	CSCTL5 |= ENSTFCNT2;						// enable HF counter

	// output SMCLK on P3.4
	P3SEL1 |= BIT4;
	P3DIR |= BIT4;

	do {
		CSCTL5 &= ~HFXTOFFG;   					// Clear XT1 fault flag
		SFRIFG1 &= ~OFIFG;
	} while (SFRIFG1&OFIFG);					// Test oscillator fault flag

	CSCTL0_H = 0; 								// reset password to lock clock registers

	SFRIE1 |= OFIE;
	return;
}

void setup_fake_crystal(void) {
	// disable XF crystal
	// put DCO in 1MHz, etc.
	CSCTL0_H = 0xA5;						// Password
	CSCTL1 = DCOFSEL_6;						// Set DCO = 24MHz
	CSCTL1 |= DCORSEL;
	CSCTL2 = SELS__DCOCLK + SELM__DCOCLK;
	CSCTL3 = DIVS__2 + DIVM__2;				// set dividers
	CSCTL4 |= HFXTOFF;						// make sure crystal is off
	CSCTL0_H = 0; 							// reset password to lock clock registers

	// output SMCLK on P3.4
	P3SEL1 |= BIT4;
	P3DIR |= BIT4;
	return;
}

void default_clock_system(void) {
	// disable XF crystal
	// put DCO in 1MHz, etc.
	CSCTL0_H = 0xA5;						// Password
	CSCTL1 = DCOFSEL_0;						// Set DCO = 1MHz
	CSCTL1 &= ~DCORSEL;
	CSCTL2 = SELS__DCOCLK + SELM__DCOCLK;
	CSCTL3 = DIVS__1 + DIVM__1;				// set dividers
	CSCTL4 |= HFXTOFF;						// make sure crystal is off
	CSCTL0_H = 0; 							// reset password to lock clock registers

	// output SMCLK on P3.4
	P3SEL1 |= BIT4;
	P3DIR |= BIT4;
	return;
}

void shutdown_components(void) {
	// shutdown xbee, sensors, etc.
	shutdown_xbee();
	shutdown_gps();
	return;
}

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	__delay_cycles(1000000);
	P2DIR |= BIT7;
	P2OUT &= ~BIT7;
	setup_fake_crystal();

	shutdown_xbee();
	_enable_interrupts();
	shutdown_gps();
	__delay_cycles(100001);
	default_clock_system();

	for(;;) {
		setup_rfwakeup();
		// at this point, the system is shut down with RF wakeup interrupts enabled.
		__bis_SR_register(LPM4_bits+GIE); // rf wakeup will take it out of this very low power mode
		P4IE &= ~BIT5; //Disable rf wakeup interrupt
		P2OUT |= BIT7;
		initialize_xbee(); // xbee must be turned on after the rf wakes up the system

		for(;;) {
			status_service();
			__bis_SR_register(LPM0_bits+GIE);	// LPM0. XBee interrupt will take it out of this low power mode. XT on.
			if (system_flags.shutdown_flag) {
				system_flags.shutdown_flag = false;
				break;
			}
			else if (system_flags.diagnostic_flag) {
				sampling_service(true); // will send all info through the xbee once.
			}
			else if (system_flags.retrieval_flag) {
				system_flags.retrieval_flag = false;
				retrieval_service();
			}
			else if (system_flags.sampling_flag) {
				// sampling flag is cleared when the device is found
				sampling_service(false);
				//location_service();
			}
			else if (system_flags.locate_flag) {
				location_service();
				system_flags.locate_flag = false;
			}
			else {
				// status mode thingy...
				status_service(); // will send status info through the xbee once.
			}
		}

		// shutdown system

		shutdown_components();
		__delay_cycles(100001);
		default_clock_system();
		P2OUT &= ~BIT7;

		// clear all flags
		system_flags.diagnostic_flag = false;
		system_flags.retrieval_flag = false;
		system_flags.sampling_flag = false;
		system_flags.shutdown_flag = false;
	}
}

#pragma vector=PORT4_VECTOR
__interrupt void RF_Wakeup_ISR(void) {
	// RF wakeup is in port 4.5 and it is the only pin in that port that could cause interrupts
	setup_fake_crystal(); // turn on 12MHz crystal and switch clocks to use it

	// turn off interrupt flag
	P4IFG &= ~BIT5;

	// wake up CPU on exit
	__bic_SR_register_on_exit(LPM4_bits);
	_nop(); // YOU MUST HAVE NOP HERE
}

#pragma vector=USCI_A1_VECTOR
__interrupt void XBee_ISR(void)
{
	unsigned char received_char;
	char receivedId[9];
	int i = 0;

	switch(__even_in_range(UCA1IV,USCI_UART_UCTXCPTIFG))
	{
	case USCI_NONE: break;
	case USCI_UART_UCRXIFG:
		// This is the RX case, needed mostly for the main system flowchart.
		// will receive ID command
		// XXX-XXXX 3, for example
		// receive 8 bytes, confirm 9th byte is a space, then receive command

		receivedId[0] = UCA1RXBUF;
		// receive next 7 bytes
		for(i = 1; i < 8; i++) {
			while (!(UCA1IFG & UCRXIFG));
			receivedId[i] = UCA1RXBUF;
		}
		receivedId[8] = '\0';

		// confirm space
		while (!(UCA1IFG & UCRXIFG));
		if(UCA1RXBUF == ' ') {
			_nop(); // do something?
		}

		// get received char
		while (!(UCA1IFG & UCRXIFG));


		received_char = UCA1RXBUF;

		if(strcmp(id, receivedId)) {
			// strings are not the same...
			return;
		}

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
			system_flags.locate_flag = false;
			break;
		case M_SHUTDOWN_SET:
			system_flags.shutdown_flag = true;
			break;
		case M_LOCATE_SET:
			system_flags.locate_flag = true;
			break;
		default:
			break;

		}
		break;

		case USCI_UART_UCTXIFG: break;
		case USCI_UART_UCSTTIFG: break;
		case USCI_UART_UCTXCPTIFG: break;
	}

	__bic_SR_register_on_exit(LPM0_bits); // wakeup cpu on exit
	_nop(); // THIS NOP MUST BE HERE!!!
}

