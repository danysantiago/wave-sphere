/*
 * services.c
 *
 *  Created on: Apr 11, 2013
 *      Author: Samuel
 */
#include "main.h"
#include <msp430fr5969.h>
#include "gps/softserial.h"
#include <stdint.h>

void power_on_xbee(void);
void wakeup_gps(void);

void diagnostic_service(void) {
	_nop();
	return;
}

void retrieval_service(void) {
	_nop();
	return;
}

void sampling_service(void) {
	shutdown_xbee();
	_nop();
	return;
}

void location_service(void) {
	power_on_xbee();
	wakeup_gps();

	// loop receiving data
	while (system_flags.sampling_flag) {
		if (!SoftSerial_empty()) {
			while (!(UCA1IFG&UCTXIFG));                // USCI_A0 TX buffer ready?
			UCA1TXBUF = SoftSerial_read();
		}
	}

	shutdown_gps();
	_nop();
	return;
}

void status_service() {
	_nop();
	return;
}
