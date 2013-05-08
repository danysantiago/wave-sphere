/*
 * i2c.c
 *
 *  Created on: Apr 5, 2013
 *      Author: Adrian
 */

#include <msp430fr5969.h>
#include <stdint.h>
#include "i2c.h"


/**
 * spi_initialize() - Initialize and enable the SPI module
 * P3.2 - CS (active low)
 * P1.5 - SCLK
 * P2.0 - UCA0SIMO
 * P2.1 - UCA0SOMI
 * P2.2 - Card detect (for SD card)
 */

void i2c_initialize(void) {
	P1SEL1 |= BIT6 + BIT7;                         // Assign I2C pins to USCI_B0
	P1SEL0 &= ~(BIT6 + BIT7);

	UCB0CTLW0 |= UCSWRST;                      // Enable SW reset (reset state machine)
	//UCB0CTL1 |= UCSWRST;
	UCB0CTLW0 |= UCMST + UCMODE_3;     // I2C Master, synchronous mode
	UCB0CTLW0 |= UCSSEL_3;            // Use SMCLK, keep SW reset
	UCB0BRW = 35;                             // fSCL = 12M/400k = 30
	//UCB0BR1 = 0;
	//UCTBCNT0 = 1;
	//UCB0CTLW1 |= UCASTP0_2;
	UCB0CTLW0 &= ~UCSWRST;                    // Clear SW reset, resume operation
	//UCB0CTLW1 |= UCASTP_2;
	//UCB0TBCNT = 1;

	//UCB0CTL1 &= ~UCSWRST;
	UCB0IE |= UCNACKIE;//UCTXIE0 + UCRXIE0;                // Enable TX and RX interrupt

}

/**
 * spi_send() - send a byte and receive response
 */
unsigned char i2c_read_byte(const unsigned char slaveAddr, const unsigned char byteAddr) {
	unsigned char data = 0;
	UCB0I2CSA = slaveAddr; // set slave address

	UCB0CTLW0 |= UCTR + UCTXSTT;	// put in transmitter mode and send start bit

	while (!(UCB0IFG & UCTXIFG0) | (UCB0CTLW0 & UCTXSTT)); // wait for previous tx to complete
	UCB0TXBUF = byteAddr; // setting TXBUF clears the TXIFG flag

	while (!(UCB0IFG & UCTXIFG0) | (UCB0CTLW0 & UCTXSTT)); // wait for previous tx to complete
	UCB0CTLW0 &= ~UCTR;				//change to receiver mode
	UCB0CTLW0 |= UCTXSTT;			// send start bit

	while (!(UCB0IFG & UCRXIFG0))//{
		// wait for an rx character
//	}
		UCB0CTLW0 |= UCTXSTP; 				// I2C stop condition
	//receivedByte = UCB0RXBUF;

	data = UCB0RXBUF;
	while (UCB0CTLW0 & UCTXSTP);
	//UCB0IFG &= ~(UCSTPIFG + UCBIT9IFG);

	return data; // reading clears RXIFG flag
}

/**
 * spi_receive() - send dummy byte then recv response
 */
void i2c_write_byte(const unsigned char slaveAddr, const unsigned char byteAddr, const unsigned char data) {
	UCB0I2CSA = slaveAddr; // set slave address

	UCB0CTLW0 |= UCTR + UCTXSTT;	// put in transmitter mode and send start bit

	while (!(UCB0IFG & UCTXIFG0) | (UCB0CTLW0 & UCTXSTT)); // wait for previous tx to complete
	UCB0TXBUF = byteAddr; // setting TXBUF clears the TXIFG flag

	while (!(UCB0IFG & UCTXIFG0)); // wait for previous tx to complete
	UCB0TXBUF = data; // setting TXBUF clears the TXIFG flag

	while (!(UCB0IFG & UCTXIFG0)); // wait for previous tx to complete
	//UCB0CTL1 |= UCTXSTT;			// send start bit
	//while (!(UCB0IFG & UCRXIFG)); // wait for an rx character
	UCB0CTLW0 |= UCTXSTP; 				// I2C stop condition
	while (UCB0CTLW0 & UCTXSTP);
	//receivedByte = UCB0RXBUF;
}

void i2c_read_mult_bytes(const unsigned char slaveAddr, const unsigned char byteAddr, const unsigned char n, unsigned char *arr) {

	int i;
	UCB0I2CSA = slaveAddr; // set slave address

	UCB0CTLW0 |= UCTR + UCTXSTT;	// put in transmitter mode and send start bit

	while (!(UCB0IFG & UCTXIFG0) | (UCB0CTLW0 & UCTXSTT)); // wait for previous tx to complete
	UCB0TXBUF = byteAddr | BIT7; // setting TXBUF clears the TXIFG flag

	while (!(UCB0IFG & UCTXIFG0) | (UCB0CTLW0 & UCTXSTT)); // wait for previous tx to complete
	UCB0CTLW0 &= ~UCTR;				//change to receiver mode
	UCB0CTLW0 |= UCTXSTT;			// send start bit

	for (i = 0; i < n; i++) {

		while (!(UCB0IFG & UCRXIFG0)); // wait for rx buffer
		if(i == n - 1) {
			UCB0CTLW0 |= UCTXSTP; 				// I2C stop condition
		}
		arr[i] = UCB0RXBUF;
	}


	while (UCB0CTLW0 & UCTXSTP);

}
