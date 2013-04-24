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
 * Sets up clock system
 */
void setup_crystal() {
	// Set PJ for HF Crystal use
	PJSEL0 |= BIT6;
	PJSEL1 &= ~BIT6;

	// Configure CS module
	CSCTL0_H = 0xA5; 							// Write password to unlock clock registers
	CSCTL1 = DCOFSEL0; 							// Set DCO to lowest speed (about 1MHz)
	CSCTL1 &= ~DCORSEL;
	CSCTL2 = SELS__HFXTCLK + SELM__HFXTCLK; 	// set SMCLK and MCLK to HFXTCLK 12MHz crystal oscillator
	CSCTL3 = DIVS__1; 							// set SMCLK divisor to 1, SMCLK = 12MHz
	CSCTL4 = HFFREQ_2;							// set HFX frequency to 8-16MHz, HFXTOFF = 0, turn it on
	CSCTL5 |= ENSTFCNT2;						// enable HF counter
	//CSCTL0_H = 0; 								// reset password to lock clock registers

	P3SEL1 |= BIT4;
	P3DIR |= BIT4; // output SMCLK on P3.4

	do {
		CSCTL5 &= ~HFXTOFFG;   					// Clear XT1 fault flag
		SFRIFG1 &= ~OFIFG;
	} while (SFRIFG1&OFIFG);					// Test oscillator fault flag

	SFRIE1 |= OFIE; // enable oscillator fault interrupt. If there is a fault.... then what?
	return;
}

void setup_rfwakeup(void) {
	volatile unsigned char data;
	data = 0;
	P4IE |= BIT5; // enable interrupts on P4.5
	spiInit(RF_DEVICE);

	// R0
	spi_select(RF_DEVICE);
	sendByteWithAddressSPI(0x00, 0x0E);
	spi_deselect(RF_DEVICE);

	// R1
	spi_select(RF_DEVICE);
	sendByteWithAddressSPI(0x01, 0x30); // enable antenna damper
	spi_deselect(RF_DEVICE);

	// R4
	spi_select(RF_DEVICE);
	sendByteWithAddressSPI(0x04, 0x3F); // max damp resistance and max gain reduction ? (seems to not do anything)
	spi_deselect(RF_DEVICE);

	// R7
	spi_select(RF_DEVICE);
	sendByteWithAddressSPI(0x07, 0xEB); // 350msec in R7 timeout
	spi_deselect(RF_DEVICE);

	// read the data here
	_nop();

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
	CSCTL5 &= ~ENSTFCNT2;					// disable HF counter
	CSCTL0_H = 0; 								// reset password to lock clock registers

	return;
}

void shutdown_components(void) {
	// shutdown xbee, sensors, etc.
	_nop();
	return;
}

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	default_clock_system();

	for(;;) {
		setup_rfwakeup();
		// at this point, the system is shut down with RF wakeup interrupts enabled.
		__bis_SR_register(LPM4_bits+GIE); // rf wakeup will take it out of this very low power mode
		for(;;) {
			__bis_SR_register(LPM0_bits+GIE);	// LPM0. XBee interrupt will take it out of this low power mode. XT on.
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

		// shutdown system
		default_clock_system();
		shutdown_components();

		// clear all flags
		system_flags.diagnostic_flag = false;
		system_flags.retrieval_flag = false;
		system_flags.sampling_flag = false;
		system_flags.shutdown_flag = false;
	}
}

void turn_on_led_of_happiness(void) {
	P2DIR |= BIT7;
	P2OUT |= BIT7;
	_nop();
}

#pragma vector=PORT4_VECTOR
__interrupt void RF_Wakeup_ISR(void) {
	// RF wakeup is in port 4.5 and it is the only pin that could cause interrupts
	turn_on_led_of_happiness();
	_nop();
	while(1);
	setup_crystal(); // turn on 12MHz crystal and switch clocks to use it

	// turn off interrupt flag...
	P4IFG &= ~BIT5;

	//TODO CLEAR WAKE, SEND SPI COMMANDS TO DO THIS

	// wake up CPU on exit
	__bic_SR_register_on_exit(LPM4_bits);
	_nop(); // YOU MUST HAVE NOP HERE
}

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

