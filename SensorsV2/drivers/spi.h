/*
 * spi.h
 *
 *  Created on: Apr 7, 2013
 *      Author: Adrian
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

//#define GYRO 0;
enum devices{GYRO, SD, ALL};

void spiInit(const unsigned int device);
unsigned char readByteSPI(const unsigned char address);
void sendByteWithAddressSPI(const unsigned char address, const unsigned char data);
void sendByteSPI(const unsigned char data);
void readMultipleBytesSPI(const unsigned char address, const unsigned char n, unsigned char *arr);
void spiDeselect(const unsigned char device);
void spiSelect(const unsigned char device);





#endif /* SPI_H_ */
