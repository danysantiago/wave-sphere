#include <msp430.h> 
#include "spi.h"
#include "uartDemo.h"
#include "accelerometer.h"
#include "i2c.h"

volatile unsigned char data;
unsigned char gyroArr[6];
int accArr[3];
unsigned char magArr[6];
/*
 * main.c
 */

const unsigned int SPI_GYRO_INIT = (UCMSB + UCCKPL);

void main(void) {
	data = 0;
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	P1OUT |= 0x01;                            // Set P1.0 for LED
	P1DIR |= 0x01;                            // Set P1.0-2 to output direction

	initAcc();
	spiInit(SPI_GYRO_INIT);
	spiDeselect(ALL);
	i2c_initialize();

	initTestUART();

	//Init Gyro
	spiSelect(GYRO);
	sendByteWithAddressSPI(0x20, 0x0F);
	spiDeselect(GYRO);

	//__delay_cycles(5000);

	spiSelect(GYRO);
	data = readByteSPI(0x8F); // should return 0xD4
	spiDeselect(GYRO);
	__no_operation();
	__delay_cycles(5000);
	while (1) {

		//Get Gyro Measurements
		spiSelect(GYRO);
		readMultipleBytesSPI(0xE8, 6, gyroArr);
		spiDeselect(GYRO);


		//Get Acc Measurements

		getAccSamples(accArr);


		//Get Mag Data
		i2c_read_mult_bytes(0x1E, 0x03, 6, magArr);

		sendGyroDataUART(gyroArr);
		sendAccDataUART(accArr);
		sendMagDataUART(magArr);

		__delay_cycles(1500000);
	}

}
