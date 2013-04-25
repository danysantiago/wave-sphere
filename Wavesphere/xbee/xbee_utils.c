/*
 * xbee_utils.c
 *
 *  Created on: Apr 24, 2013
 *      Author: Samuel
 */

#include <msp430fr5969.h>

void shutdown_xbee(void) {
	// poner pin 4.6 en high
	P4OUT |= BIT6;
	UCA1CTL1 |= UCSWRST;                 // Turn off state machine
	UCA1IE &= ~UCRXIE;					// Disable interrupts
	return;
}

void power_on_xbee(void) {
	P4OUT &= ~BIT6;
	UCA1CTL1 &= ~UCSWRST;                 // Turn off state machine
	UCA1IE |= UCRXIE;
	return;
}

void initialize_xbee(void) {
	P4DIR |= BIT6;
	P4OUT &= ~BIT6;

	// Configure UART pins
	P2REN &= ~(BIT5 + BIT6);
	P2SEL1 |= BIT5 + BIT6;
	P2SEL0 &= ~(BIT5 + BIT6);
	P2DIR |= BIT6; // TX as output

	// Configure UART
	UCA1CTL1 |= UCSWRST;                      // Put eUSCI in reset
	UCA1CTL1 |= UCSSEL__SMCLK;                // CLK = SMCLK

	// Baud Rate calculation
	// 12000000/(16*9600) = 78.125
	// Fractional portion = 0.125
	// User's Guide Table 21-4: UCBRSx = 0x00, UCOS16 = 1
	// UCBRFx = int (0.125*16) = 2
	UCA1BR0 = 78;                             // 8000000/16/9600
	UCA1BR1 = 0x00;
	UCA1MCTLW |= UCOS16 | UCBRF_2;

	UCA1CTL1 &= ~UCSWRST;                     // Initialize eUSCI
	UCA1IE |= UCRXIE;                         // Enable USCI_A1 RX interrupt

	return;
}
