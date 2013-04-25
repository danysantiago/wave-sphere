/*
 * gps_utils.c
 *
 *  Created on: Apr 24, 2013
 *      Author: Samuel
 */

#include <msp430fr5969.h>
#include "../common/spi/spi.h"
#include "config.h"
#include <stdint.h>

void SoftSerial_xmit(uint8_t byte);
void SoftSerial_init(void);

void init_gps_coms(void) {
	SoftSerial_init();
}

void send_string_gps(char *string) {
	while(*string) {
		SoftSerial_xmit(*string);
		string++;
	}
}

/**
 * Shutdown the GPS with the command.
 */
void shutdown_gps(void) {
	SoftSerial_init();
	__delay_cycles(100001);
	send_string_gps("$PMTK161,0*28\r\n");
}

/**
 * Send anything through software UART and the GPS will turn on
 */
void wakeup_gps(void) {
	SoftSerial_xmit(0xAA);
}

