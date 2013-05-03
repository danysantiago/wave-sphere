/*
 * services.c
 *
 *  Created on: Apr 11, 2013
 *      Author: Samuel
 */
#include "main.h"
#include <msp430fr5969.h>
#include "gps/softserial.h"
#include "accelerometer/accelerometer.h"
#include "gyroscope/gyroscope.h"
#include "magnetometer/magnetometer.h"
#include "common/spi/spi.h"
#include "sdcard/sd.h"
#include "common/i2c/i2c.h"
#include "sdcard/pff2a/src/pff.h"
#include "common/uart/uart.h"

#include <stdint.h>

void power_on_xbee(void);
void wakeup_gps(void);

/**
 * Send all information to Xbee.
 */
void diagnostic_service(void) {
	_nop();
	return;
}

/**
 * Send requested file to Xbee
 */
void retrieval_service(void) {
	int i;
	FRESULT res;
	char line[32];
	WORD bytes_read;
	bool done = false;
	FATFS fs;

	init_sd(0, &fs); // init and make the seek

	while(!done) {
		res = pf_read(line, sizeof(line), &bytes_read); // fill up buffer and then dump it through uart
		if (res != FR_OK) {
			// ERROR (pichea)
		}

		for (i = 0; bytes_read > 0; bytes_read--, i++) {
			while (!(UCA1IFG&UCTXIFG));                // USCI_A0 TX buffer ready?
			if((UCA1TXBUF = line[i]) == '\0') {
				done = true;
				break;
			}
		}
	}

	return;
}

volatile bool flag = true;
/**
 * Store info on SD card.
 */
void sampling_service(bool diagnostic) {
	volatile unsigned char data = 0;
	int i;
	int gyroArr[3];
	int accArr[3];
	int magArr[3];
	char buffer[513];
	char buffer2[48];
	int sector_count = 0;
	unsigned char gps_data;
	unsigned short gps_count;
	int ifcount = 0;
	int ifthingflag = false;
	FATFS fs;
	unsigned long timestamp = 0; // count in .1ms increments

	buffer[0] = '\0';
	buffer2[0] = '\0';

	if(!diagnostic) {
		shutdown_xbee();
	}

	if(diagnostic) {
		wakeup_gps();
	}

	// init i2c
	i2c_initialize();

	//Init Sensors
	initAcc();
	initGyro();
	initMag();

	// first we must set up aclk
	CSCTL0_H = 0xA5;
	CSCTL2 |= SELA__VLOCLK;
	CSCTL4 &= ~VLOOFF;
	CSCTL0_H = 0;

	// load capture/compares
	TB0CCR0 = 40;
	//TA1CCR0 = 10;

	// now actually set up timer b0
	TB0CTL |= MC__UP + TBSSEL__ACLK;

	// setup timer a1
	TA1CTL |= MC__CONTINUOUS + TASSEL__ACLK;

	if(!diagnostic) {
		init_sd(0, &fs);
		spi_select(SD_DEVICE);
	}
	for(i = 30 * 250; i > 0; i--) {
		TB0CCTL0 = CCIE; // all other 0, compare mode
		__bis_SR_register(LPM0_bits + GIE);
		_nop();
		timestamp += TA1R;
		TA1R = 0; // restart timer

		TB0CCTL0 &= ~CCIE;


		getAccData(accArr);
		getGyroData(gyroArr);
		getMagData(magArr);

		if(!diagnostic) {
			if(fillbuffer(buffer, buffer2, accArr, timestamp, false, true)) {
				dump_sd(buffer, buffer2, sector_count++);
			}
			if(fillbuffer(buffer, buffer2, gyroArr, timestamp, false, false)) {
				dump_sd(buffer, buffer2, sector_count++);
			}
			if(fillbuffer(buffer, buffer2, magArr, timestamp, true, false)) {
				dump_sd(buffer, buffer2, sector_count++);
			}
		} else {
			i++; // loop forever
			if(!system_flags.diagnostic_flag) {
				break;
			}

			while (true) {
				if (!SoftSerial_empty()) {
					ifthingflag = true;
					while (!(UCA1IFG&UCTXIFG));
					gps_data = SoftSerial_read();// USCI_A0 TX buffer ready?
					UCA1TXBUF = gps_data;
					/*
					if(gps_data == '\n') {
						gps_count++;
						if(gps_count == 6) {
							gps_count = 0;
							break;
						}
					}
					 */
				} else {
					ifcount++;
					if(ifcount >= 25052 && ifthingflag) {
						ifcount = 0;
						ifthingflag = false;
						break;
					}
				}
			}
			// send stuff through UART
			sendSensorDataUART(accArr, "");
			sendStringUART("\t");
			sendSensorDataUART(gyroArr, "");
			sendStringUART("\t");
			sendSensorDataUART(magArr, "");
			sendStringUART("\n");

			// file system information
			sendStringUART("78%m\n");
			// measurment from power meter
			sendStringUART("44%b\n");
			// xbee module signal
			sendStringUART("-70dB\n");
			// sendString("+++\r");
			// receiveUART(); // two bytes
			// sendString("DB");
			// receiveUART();
		}
	}

	// dump whatever is left of sd
	if(!diagnostic) {
		dump_sd(buffer, buffer2, sector_count++);
		// finalize, unmount, whatever
		finalize_write_sd();
		unmount_file_sd();
		spi_deselect(SD_DEVICE);
	} else {
		shutdown_gps();
	}
	TB0CCTL0 &= ~CCIE;


	system_flags.sampling_flag = false;
	power_on_xbee();
	return;
}


#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer_B0(void) {
	__bic_SR_register_on_exit(LPM0_bits);
}

void location_service(void) {
	power_on_xbee();
	wakeup_gps();

	// loop receiving data
	while (system_flags.locate_flag) {
		if (!SoftSerial_empty()) {
			while (!(UCA1IFG&UCTXIFG));                // USCI_A0 TX buffer ready?
			UCA1TXBUF = SoftSerial_read();
		}
	}

	shutdown_gps();
	_nop();
	return;
}

/**
 * Send battery info to Xbee, or something.
 * Maybe signal strength too.
 */
void status_service() {
	//sphere id
	sendStringUART("000-0001\n");
	// file system information
	sendStringUART("78%m\n");
	// measurment from power meter
	sendStringUART("44%b\n");
	return;
}
