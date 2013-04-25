/*
 * gyroscope.c
 *
 *  Created on: Apr 12, 2013
 *      Author: Adrian
 */

#include "../drivers/spi.h"

void initGyro() {
	spiSelect(GYRO);
	sendByteWithAddressSPI(0x20, 0x0F);
	spiDeselect(GYRO);
}

void getGyroData(int *dataArray) {
	unsigned char gyroArr[6];

	spiSelect(GYRO);
	readMultipleBytesSPI(0xE8, 6, gyroArr);
	spiDeselect(GYRO);

	dataArray[0] = (int) ((gyroArr[0] << 8) | gyroArr[1]);
	dataArray[1] = (int) ((gyroArr[2] << 8) | gyroArr[3]);
	dataArray[2] = (int) ((gyroArr[4] << 8) | gyroArr[5]);

	/*
	dataArray[0] = (int) ((gyroArr[1] << 8) | gyroArr[0]);
	dataArray[1] = (int) ((gyroArr[3] << 8) | gyroArr[2]);
	dataArray[2] = (int) ((gyroArr[5] << 8) | gyroArr[4]);
	*/
}

