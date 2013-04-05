/**
 * main.c - For testing RTC thingy
 * Author: Samuel
 */
#include <msp430fr5969.h>

#define DESELECT()	P3OUT &= ~BIT3				// CS = L
#define SELECT()	P3OUT |= BIT3				//CS = H

/**
 * SMCLK divider arguments for spi speed, SMCLK at 16MHz
 */
#define SPI_250kHz 64 // 16MHz/250000
#define SPI_400kHz 40 // 16MHz/400000
#define SPI_1MHz   16 // 16MHz/1MHz
#define SPI_2MHz    8 // 16MHz/2MHz
#define SPI_4MHz    4 // 16MHz/4MHz
#define SPI_8MHz    2 // 16MHz/8MHz
#define SPI_16MHz   1 // 16MHz/16Mhz

/**
 * Utility macros for word argument
 */
#ifndef LOBYTE
#define LOBYTE(w) ((w)&0xFF)
#define HIBYTE(w) ((w)>>8)
#endif

/**
 * Seup - initialize timers and clocks
 */
void setup() {
	WDTCTL = WDTPW + WDTHOLD;       // Stop the watchdog timer.

	CSCTL0_H = 0xA5; 				// Set password to write clock registers.
	CSCTL1 = DCOFSEL_4 | DCORSEL; 	// Set DCOCLK to 16MHz
	CSCTL2 = SELM__DCOCLK + SELS__DCOCLK; // Set MCLK and SMCLK to DCO
	CSCTL0_H = 0; 					// Lock clock registers

	P3OUT &= ~BIT3;  // CE initially disabled for RTC clock at P3.3
	P3DIR |= BIT3;
}

void spi_initialize(void)
{
	UCA0CTLW0 |= UCSWRST;			// Put state machine in reset
	UCA0CTLW0 |= UCMST + UCSYNC + UCCKPH + UCCKPL + UCMSB + UCMODE0; // 3-pin, 8-bit SPI master
	UCA0CTLW0 &= ~UC7BIT;
									// Clock polarity high, MSB
	UCA0CTLW0 |= UCSSEL_2;			// SMCLK
									// source USCI clock from SMCLK, put USCI in reset mode
									// SPI MODE 0 - CPOL=0 CPHA=0
									// note: UCCKPH is inverted from CPHA

	P1SEL1 |= BIT5;					// configure P1.5, P2.0 and P2.1 for SPI
	P2SEL1 |= BIT0 + BIT1;

	UCA0BR0 = LOBYTE(SPI_1MHz);	// set initial speed 16MHz/400000 = 400kHz (kinda slow speed for the RTC, in case)
	UCA0BR1 = HIBYTE(SPI_1MHz);

	P3OUT &= ~BIT3;					// CS on P3.3. start out disabled (it is active high)
	P3DIR |= BIT3;					// CS configured as output

	UCA0CTLW0 &= ~UCSWRST;			// release for operation, initialize state machine
}

/**
 * spi_send_address_data() - send an address followed by a byte of data
 */
void spi_send_address_data(const unsigned char address, const unsigned char data) {
		while (!(UCA0IFG & UCTXIFG))
			; // wait for a previous tx to complete

		UCA0TXBUF = address; // setting TXBUF clears the TXIFG flag

		while (!(UCA0IFG & UCTXIFG))
			; // wait for the address tx to complete

		UCA0TXBUF = data;

		return;
}

/**
 * spi_send() - send a byte and receive response
 */
unsigned char spi_send(const unsigned char c)
{
	while (!(UCA0IFG & UCTXIFG))
		; // wait for previous tx to complete

	UCA0TXBUF = c; // setting TXBUF clears the TXIFG flag

	while (!(UCA0IFG & UCTXIFG))
			; // wait for previous tx to complete

	UCA0TXBUF = 0x00; // send nothing, just to receive a byte back

	while (!(UCA0IFG & UCRXIFG))
		; // wait for an rx character

	return UCA0RXBUF; // reading clears RXIFG flag
}

/**
 * spi_receive() - send dummy byte then recv response
 */
unsigned char spi_receive(void) {

	while (!(UCA0IFG & UCTXIFG))
		; // wait for any previous xmits to complete

	UCA0TXBUF = 0xFF; // Send dummy packet to get data back.

	while (!(UCA0IFG & UCRXIFG))
		; // wait to recv a character

	return UCA0RXBUF; // reading clears RXIFG flag
}

void main (void)
{
	register volatile unsigned char received;
	setup();
	spi_initialize();

	SELECT();
	spi_send_address_data(0x8F, 0x00);
	DESELECT();
	__delay_cycles(10000);
	SELECT();
	received = spi_send(0x00);
	DESELECT();
	__delay_cycles(10000);

	while(1) {
		SELECT();
		received = spi_send(0x00);
		DESELECT();
	}
}

