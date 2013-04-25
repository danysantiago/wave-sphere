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
	volatile unsigned char data = 0;
	int gyroArr[3];
	int accArr[3];
	int magArr[3];

	shutdown_xbee();

	//Init Sensors
		initAcc();
		initGyro();
		initMag();

		spi_select(GYRO_DEVICE);

		accStartST();

		/*while(1)*/ {

			spi_select(GYRO_DEVICE);
			data = readByteSPI(0x8F);
			spi_deselect(GYRO_DEVICE);

			getGyroData(gyroArr);
			getAccData(accArr);
			getMagData(magArr);

			/*
			sendSensorDataUART(accArr, "");
			sendStringUART("\t");
			sendSensorDataUART(gyroArr, "");
			sendStringUART("\t");
			sendSensorDataUART(magArr, "");
			sendStringUART("\n");
			__delay_cycles(1500000);
			*/

		}

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
