/*
 * spi.c
 *
 *  Created on: Apr 7, 2013
 *      Author: Adrian & Dany & Samuel & Nelian
 */
#include <msp430fr5969.h>
#include "spi.h"


void spiInit(const unsigned int device) {
	P2SEL1 |= BIT0 + BIT1; //SPI
	P1SEL1 |= BIT5; // SPI

	P3DIR |= BIT0;

	/*Initialize SPI*/
	//UCA0 is SPI
	UCA0CTLW0 |= UCSWRST;                      // **Put state machine in reset**
	UCA0CTLW0 |= UCMST + UCSYNC;    		// 3-pin, 8-bit SPI master

	UCA0CTLW0 &= ~(UCMSB + UCCKPL);
	UCA0CTLW0 |= device;

	UCA0CTLW0 |= UCSSEL__SMCLK;                     // SMCLK
	UCA0BR0 = 0x04; // /4 ? ANTES ERA 0x02 Bitch, cuz we want slow clock to be cool
	UCA0BR1 = 0;                              //
	UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**

}

void sendByteWithAddressSPI(const unsigned char address, const unsigned char data) {

	while (!(UCA0IFG & UCTXIFG)); // UCA0 tx buffer ready?
	UCA0TXBUF = address;
	while (!(UCA0IFG & UCTXIFG)); // UCA0 tx buffer ready?
	UCA0TXBUF = data;
	while (!(UCA0IFG & UCTXIFG));
	__delay_cycles(25);
}

void sendByteSPI(const unsigned char data) {

	while (!(UCA0IFG & UCTXIFG)); // UCA0 tx buffer ready?
	UCA0TXBUF = data;
	while (!(UCA0IFG & UCTXIFG));
	__delay_cycles(25);
}

unsigned char readByteSPI(const unsigned char address) {
	sendByteWithAddressSPI(address, 0x00); // dummy byte

	// wait for receive byte
	while (!(UCA0IFG & UCRXIFG)); // wait for rx buffer
	return UCA0RXBUF;
}

void readMultipleBytesSPI(const unsigned char address, const unsigned char n, unsigned char *arr) {
	int i;
	// reads the amount of bytes given by n at the address
	while (!(UCA0IFG & UCTXIFG)); // UCA0 tx buffer ready?
	UCA0TXBUF = address; // send first address byte

	// now send n dummy bits
	for (i = 0; i < n; i++) {
		while (!(UCA0IFG & UCTXIFG));
		UCA0TXBUF = 0x00;

		while (!(UCA0IFG & UCRXIFG)); // wait for rx buffer
		arr[i] = UCA0RXBUF;
	}
	__delay_cycles(25);
}

void spiSelect(const unsigned char device) {
	switch(device) {
	case GYRO: //GYRO
		P3OUT &= ~BIT0;
		break;
	default:
		break;
	}

}

void spiDeselect(const unsigned char device) {
	switch(device) {
	case GYRO:
		P3OUT |= BIT0;
		break;
	case ALL:
		P3OUT |= BIT0;
		break;
	default:
		break;
	}

}


