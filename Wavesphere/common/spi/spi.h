/*
 * spi.h
 *
 *  Created on: Apr 12, 2013
 *      Author: Samuel
 */

#ifndef SPI_H_
#define SPI_H_


/**
 * Utility macros for word argument
 */
#ifndef LOBYTE
#define LOBYTE(w) ((w)&0xFF)
#define HIBYTE(w) ((w)>>8)
#endif

#define GYRO 0
#define SD 1
#define ALL 2

// SPI Speed Divisors base on the SMCLK
#define SPI_100kHz	30 // 3MHz/100kHz
#define SPI_250kHz	12 // 3MHz/250kHz
#define	SPI_300kHz	10 // 3MHz/100kHz
#define SPI_500KHz	6  // 3MHz/500kHz
#define SPI_1MHz	3  // 3MHz/1MHz
#define SPI_1.5MHz	2  // 3MHz/1.5MHz
#define SPI_3MHz   	1  // 3MHz/3Mhz

void spiInit(const unsigned char device);
unsigned char readByteSPI(const unsigned char address);
void sendByteWithAddressSPI(const unsigned char address, const unsigned char data);
void sendByteSPI(const unsigned char data);
void readMultipleBytesSPI(const unsigned char address, const unsigned char n, unsigned char *arr);
void spiDeselect(const unsigned char device);
void spiSelect(const unsigned char device);


#endif /* SPI_H_ */
