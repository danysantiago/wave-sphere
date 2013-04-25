/*
 * magnetometer.c
 *
 *  Created on: Apr 11, 2013
 *      Author: Adrian
 */

#include "../common/i2c/i2c.h"

unsigned char sensitivity;

void initMag() {
	i2c_write_byte(0x1E, 0x02, 0x00);
	i2c_write_byte(0x1E, 0x01, 0xC0);
}

void initMagAcc() {
	//Sets ODR at 400 Hz and enables x, y and z
	i2c_write_byte(0x1E, 0x20, 0x77);
}

void getMagData(int *dataArray) {
	unsigned char magArr[6];
	i2c_read_mult_bytes(0x1E, 0x03, 6, magArr);

	dataArray[0] = (int) (magArr[1] << 8) | magArr[0];
	dataArray[2] = (int) (magArr[3] << 8) | magArr[1];
	dataArray[1] = (int) (magArr[5] << 8) | magArr[4];

}

void getMagAccData(int *dataArray) {
	unsigned char accArr[6];
	i2c_read_mult_bytes(0x1E, 0x28, 6, accArr);

	dataArray[0] = (int) (accArr[0] << 8) + accArr[1];
	dataArray[1] = (int) (accArr[2] << 8) + accArr[3];
	dataArray[2] = (int) (accArr[4] << 8) + accArr[5];

}

