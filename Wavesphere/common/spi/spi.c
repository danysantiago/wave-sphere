/*
 * spi.c
 *
 *  Created on: Apr 12, 2013
 *      Author: Samuel
 */
#include <msp430fr5969.h>
#include <stdint.h>
#include "spi.h"
#include "../../main.h"

void spiInit(const unsigned char device) {
	// Configure ports for SPI
	P2SEL1 |= BIT0 + BIT1; // data
	P1SEL1 |= BIT5; // clk
	//P3OUT &= ~(BIT0 + BIT1 + BIT2); // all chip selects LOW, does not mean it will be deselected (some act h, some l)
	P3DIR |= BIT0 + BIT1 + BIT2; // all chip selects as outputs

	// all chips deselected
	//spi_deselect(GYRO_DEVICE);
	spi_deselect(RF_DEVICE);
	spi_deselect(SD_DEVICE);

	UCA0CTLW0 |= UCSWRST; // put state machine in reset

	spi_set_mode(device);
	spi_set_device_divisor(device);

	UCA0CTLW0 &= ~UCSWRST; // **Initialize USCI state machine**
}

void sendByteWithAddressSPI(const unsigned char address, const unsigned char data) {

	while (!(UCA0IFG & UCTXIFG)); // UCA0 tx buffer ready?
	UCA0TXBUF = address;
	while (!(UCA0IFG & UCTXIFG)); // UCA0 tx buffer ready?
	UCA0TXBUF = data;
	while (!(UCA0IFG & UCTXIFG));
	__delay_cycles(100);
}

void sendByteSPI(const unsigned char data) {
	while (!(UCA0IFG & UCTXIFG)); // UCA0 tx buffer ready?
	UCA0TXBUF = data;
	while (!(UCA0IFG & UCTXIFG));
	__delay_cycles(100);
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

void spi_select(const unsigned char device) {
	switch(device) {
	case GYRO_DEVICE: //GYRO
		GYRO_SELECT();
		SD_DESELECT();
		RF_DESELECT();
		break;
	case SD_DEVICE:
		SD_SELECT();
		GYRO_DESELECT();
		RF_DESELECT();
		break;
	case RF_DEVICE:
		RF_SELECT();
		GYRO_DESELECT();
		SD_DESELECT();
		break;
	default:
		break;
	}
}

void spi_deselect(const unsigned char device) {
	switch(device) {
	case GYRO_DEVICE:
		GYRO_DESELECT();
		break;
	case SD_DEVICE:
		SD_DESELECT();
		break;
	case RF_DEVICE:
		RF_DESELECT();
		break;
	default:
		break;
	}

}

/**
 * spi_send, spi_receive and spi_set_divisor are mostly for original SD Card code
 * spi_send() - send byte
 */
uint8_t spi_send(const uint8_t c)
{
	while (!(UCA0IFG & UCTXIFG)); // wait for previous tx to complete
	UCA0TXBUF = c; // setting TXBUF clears the TXIFG flag
	while (!(UCA0IFG & UCRXIFG)); // wait for an rx character
	return UCA0RXBUF; // reading clears RXIFG flag
}

/**
 * spi_receive() - send dummy btye then recv response
 */
uint8_t spi_receive(void) {

	while (!(UCA0IFG & UCTXIFG)); // wait for any previous xmits to complete
	UCA0TXBUF = 0xFF; // Send dummy packet to get data back.
	while (!(UCA0IFG & UCRXIFG)); // wait to recv a character
	return UCA0RXBUF; // reading clears RXIFG flag
}

/**
 * spi_set_divisor() - set SPI speed using divisor. Reset state machine (for SD speed upgrading)
 */
void spi_set_divisor(const unsigned int clkdiv) {
	UCA0CTLW0 |= UCSWRST;
	UCA0BRW = clkdiv;
	UCA0CTLW0 &= ~UCSWRST;
}

void spi_set_device_divisor(const unsigned char device)
{
	switch (device) {
	case GYRO_DEVICE:
		UCA0BRW = GYRO_SPI_DIVISOR;
		break;
	case SD_DEVICE:
		UCA0BRW = SD_SPI_DIVISOR;
		break;
	case RF_DEVICE:
		UCA0BRW = RF_SPI_DIVISOR;
		break;
	default:
		break;
	}
}

void spi_set_mode(const unsigned char device) {
	switch (device) {
	case GYRO_DEVICE:
		UCA0CTLW0 = GYRO_SPI_MODE;
		break;
	case SD_DEVICE:
		UCA0CTLW0 = SD_SPI_MODE;
		break;
	case RF_DEVICE:
		UCA0CTLW0 = RF_SPI_MODE;
		break;
	default:
		break;
	}
	UCA0CTLW0 |= UCSSEL__SMCLK; // set SMCLK
}

