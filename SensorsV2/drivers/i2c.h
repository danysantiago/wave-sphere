/*
 * i2c.h
 *
 *  Created on: Apr 5, 2013
 *      Author: Adrian
 */

#ifndef I2C_H_
#define I2C_H_

/** TODO
 * If SMCLK is NOT 16MHZ, then we must change this!!!
 */

/**
 * SMCLK divider arguments for spi_setspeed
 * change if your SMCLK isn't 16MHz
 */
#define SPI_250kHz 64 // 16MHz/250000
#define SPI_400kHz 40 // 16MHz/400000
#define SPI_1MHz   16 // 16MHz/1MHz
#define SPI_2MHz    8 // 16MHz/2MHz
#define SPI_4MHz    4 // 16MHz/4MHz
#define SPI_8MHz    2 // 16MHz/8MHz
#define SPI_16MHz   1 // 16MHz/16Mhz

#define SPI_DEF_SPEED SPI_4MHz

/**
 * Utility macros for word argument
 */
#ifndef LOBYTE
#define LOBYTE(w) ((w)&0xFF)
#define HIBYTE(w) ((w)>>8)
#endif

void i2c_initialize(void);
unsigned char i2c_read_byte(const unsigned char, const unsigned char);
void i2c_write_byte(const unsigned char, const unsigned char, const unsigned char);
void i2c_read_mult_bytes(const unsigned char slaveAddr, const unsigned char byteAddr, const unsigned char n, unsigned char *arr);
//void spi_set_divisor(const uint16_t clkdivider);




#endif /* I2C_H_ */
