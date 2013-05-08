/*
 * rfwakeup_setup.c
 *
 *  Created on: Apr 24, 2013
 *      Author: Samuel
 */
#include <msp430fr5969.h>
#include "../common/spi/spi.h"

/**
 * Sets up the rf wakeup module.
 */
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
	sendByteWithAddressSPI(0x04, 0x3F); // max damp resistance and max gain reduction
	spi_deselect(RF_DEVICE);

	// R7
	spi_select(RF_DEVICE);
	sendByteWithAddressSPI(0x07, 0xEB); // 350msec in R7 timeout
	spi_deselect(RF_DEVICE);

	return;
}

/**
 * Sends command to clear rf wake signal. If you are running at 12MHz, make sure you set the necessary dividers
 */
void rf_clear_wake(void) {
	// spi should already be set up for the rf wakeup.
	spi_select(RF_DEVICE);
	sendByteSPI(0xC0);
	spi_deselect(RF_DEVICE);

	return;
}

