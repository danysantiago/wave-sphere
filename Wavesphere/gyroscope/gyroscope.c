/*
 * gyroscope.c
 *
 *  Created on: Apr 12, 2013
 *      Author: Adrian
 */

#include "../common/i2c/i2c.h"

void initGyro() {
	i2c_write_byte(0x6A, 0x20, 0x0F);
}

void getGyroData(int *dataArray) {
	unsigned char gyroArr[6];

	i2c_read_mult_bytes(0x6A, 0xE8, 6, gyroArr);

	dataArray[0] = (int) ((gyroArr[0] << 8) | gyroArr[1]);
	dataArray[1] = (int) ((gyroArr[2] << 8) | gyroArr[3]);
	dataArray[2] = (int) ((gyroArr[4] << 8) | gyroArr[5]);
}

