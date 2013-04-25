#include <msp430.h> 
#include "drivers/spi.h"
#include "demo/uart.h"
#include "sensors/accelerometer.h"
#include "sensors/magnetometer.h"
#include "sensors/gyroscope.h"
#include "drivers/i2c.h"
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

volatile unsigned char data;
int gyroArr[3];
int accArr[3];
int magArr[3];
/*
 * main.c
 */

const unsigned int SPI_GYRO_INIT = (UCMSB + UCCKPL);

void main(void) {
	data = 0;
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	P1DIR |= BIT3;
	P1OUT &= ~BIT3;
	P1IE |= BIT2;

	P3DIR |= BIT4;
	P3SEL1 |= BIT4;
	P1OUT |= 0x01;                            // Set P1.0 for LED
	P1DIR |= 0x01;                            // Set P1.0-2 to output direction

	spiInit(SPI_GYRO_INIT);
	spiDeselect(ALL);
	i2c_initialize();

	initUART();


	//Init Sensors
	initAcc();
	initGyro();
	initMag();

	spiSelect(GYRO);

	accStartST();

	while (1) {

		spiSelect(GYRO);
		data = readByteSPI(0x8F);
		spiDeselect(GYRO);

		getGyroData(gyroArr);
		getAccData(accArr);
		getMagData(magArr);

		sendSensorDataUART(accArr, "");
		sendStringUART("\t");
		sendSensorDataUART(gyroArr, "");
		sendStringUART("\t");
		sendSensorDataUART(magArr, "");
		sendStringUART("\n");
		__delay_cycles(1500000);

	}

}

