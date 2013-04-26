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
	_nop();
	return;
}

/**
 * Store info on SD card.
 */
void sampling_service(void) {
	volatile unsigned char data = 0;
	int i;
	int gyroArr[3];
	int accArr[3];
	int magArr[3];
	char buffer[512];
	char buffer2[23];
	int sector_count = 0;

	buffer[0] = '\0';
	buffer2[0] = '\0';

	shutdown_xbee();

	spiInit(GYRO_DEVICE);
	spi_select(GYRO_DEVICE);
	data = readByteSPI(0x8F); // WHO_AM_I should return 0xD4, this is just a check
	spi_deselect(GYRO_DEVICE);

	_nop();
	// init i2c
	i2c_initialize();

	//Init Sensors
	initAcc();
	initGyro();
	initMag();


	for(i = 20; i > 0; i--) {
		spiInit(GYRO_DEVICE);
		getGyroData(gyroArr);
		getAccData(accArr);
		getMagData(magArr);

		if(fillbuffer(buffer, buffer2, gyroArr, false)) {
			dump_sd(buffer, buffer2, sector_count++);
		}
		if(fillbuffer(buffer, buffer2, accArr, false)) {
			dump_sd(buffer, buffer2, sector_count++);
		}
		if(fillbuffer(buffer, buffer2, magArr, true)) {
			dump_sd(buffer, buffer2, sector_count++);
		}

		/*
		init_sd(file_position);
		//mountFile = false;
		spi_select(SD_DEVICE);
		file_position += write_sensor_data_sd(accArr);
		file_position += write_sd("\t");
		file_position += write_sensor_data_sd(gyroArr);
		file_position += write_sd("\t");
		file_position += write_sensor_data_sd(magArr);
		file_position += write_sd("\r\n");

		spi_deselect(SD_DEVICE);
		 */

		__delay_cycles(200000);
	}

	dump_sd(buffer, buffer2, sector_count++);
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

/**
 * Send battery info to Xbee, or something.
 * Maybe signal strength too.
 */
void status_service() {
	_nop();
	return;
}
