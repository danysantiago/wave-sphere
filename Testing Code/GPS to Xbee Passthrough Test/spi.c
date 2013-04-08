/**
 * spi.c - Spi stuff
 * Author Samuel
 */

#include <msp430fr5969.h>
#include <stdint.h>
#include "spi.h"

#define SPI_MODE_0 UCMST + UCSYNC + UCCKPH + UCMSB; // THIS ONE, ucckph = 1, data is captured on first and changed on second edge,
													// msb first, synchronous, no ucckpl, which means inactive state is low, first edge is high
#define SPI_MODE_3 UCMST + UCSYNC + UCCKPL + UCMSB;

#define SELECT()	P3OUT &= ~BIT3				// CS = L
#define DESELECT()	P3OUT |= BIT3				//CS = H

/**
 * spi_initialize() - Initialize and enable the SPI module
 * P3.3 - CS (active low)
 * P1.5 - SCLK
 * P2.0 - UCA0SIMO
 * P2.1 - UCA0SOMI
 */

void spi_initialize(void)
{
	UCA0CTLW0 |= UCSWRST;			// Put state machine in reset
	UCA0CTLW0 |= SPI_MODE_0			// 3-pin, 8-bit SPI master
									// Clock polarity high, MSB
	UCA0CTLW0 |= UCSSEL_2;			// SMCLK
									// source USCI clock from SMCLK, put USCI in reset mode
									// SPI MODE 0 - CPOL=0 CPHA=0
									// note: UCCKPH is inverted from CPHA

	P1SEL1 |= BIT5;					// configure P1.5, P2.0 and P2.1 for SPI
	P2SEL1 |= BIT0 + BIT1;

	UCA0BR0 = LOBYTE(SPI_400kHz);	// set initial speed 16MHz/400000 = 400kHz as needed by SD card
	UCA0BR1 = HIBYTE(SPI_400kHz);

	DESELECT();
	P3DIR |= BIT3;					// CS configured as output

	UCA0CTLW0 &= ~UCSWRST;			// release for operation (init state machine)
}

/**
 * spi_send() - send a byte and receive response
 */
void spi_send(const uint8_t c)
{
	while (!(UCA0IFG & UCTXIFG)); // wait for previous tx to complete

	UCA0TXBUF = c; // setting TXBUF clears the TXIFG flag
	while(!(UCA0IFG & UCTXIFG)); // wait I guess
	__delay_cycles(25);
}

/**
 * spi_receive() - send dummy btye then recv response
 */
uint8_t spi_receive(void) {

	while (!(UCA0IFG & UCTXIFG))
		; // wait for any previous xmits to complete

	UCA0TXBUF = 0xFF; // Send dummy packet to get data back.

	while (!(UCA0IFG & UCRXIFG))
		; // wait to recv a character

	return UCA0RXBUF; // reading clears RXIFG flag
}

/**
 * spi_setspeed() - set new clock divider for USCI
 *
 * USCI speed is based on the SMCLK divided by BR0 and BR1
 * initially we start slow (400kHz) to conform to SDCard
 * specifications then we speed up once initialized (16Mhz)
 *
 */
void spi_set_divisor(const uint16_t clkdiv)
{
	UCA0CTL1 |= UCSWRST;		// go into reset state
	UCA0BR0 = LOBYTE(clkdiv);
	UCA0BR1 = HIBYTE(clkdiv);
	UCA0CTL1 &= ~UCSWRST;		// release for operation
}
