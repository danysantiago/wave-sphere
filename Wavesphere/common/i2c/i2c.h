/*
 * i2c.h
 *
 *  Created on: Apr 5, 2013
 *      Author: Adrian
 */

#ifndef I2C_H_
#define I2C_H_

void i2c_initialize(void);
unsigned char i2c_read_byte(const unsigned char, const unsigned char);
void i2c_write_byte(const unsigned char, const unsigned char, const unsigned char);
void i2c_read_mult_bytes(const unsigned char slaveAddr, const unsigned char byteAddr,
							const unsigned char n, unsigned char *arr);

#endif /* I2C_H_ */
