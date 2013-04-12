/*
 * spi.h
 *
 *  Created on: Apr 12, 2013
 *      Author: Samuel
 */

#ifndef SPI_H_
#define SPI_H_
#include <stdint.h>
// device identifier
#define GYRO_DEVICE 0
#define SD_DEVICE 1
#define RF_DEVICE 2

// device spi polarity mode
#define GYRO_SPI_MODE UCMST + UCSYNC + UCCKPL + UCMSB // UCCKPL = 1 means inactive state is high, UCCKPH = 0 means data is changed on first edge
#define RF_SPI_MODE UCMST + UCSYNC + UCMSB // UCCKPL = 0 means inactive state is low, UCCKPH = 0 means data is changed on first edge and captured on second edge (falling)
#define SD_SPI_MODE UCMST + UCSYNC + UCCKPH + UCMSB // MSB, Sync, UCKPL = 0 mean inactive state is low, UCCKPH = 1 means data is captured of first edge

// device chip select ports
#define GYRO_CS_PORT P3OUT
#define RF_CS_PORT P3OUT
#define SD_CS_PORT P3OUT

// device chip select pins
#define GYRO_CS_BIT BIT0
#define RF_CS_BIT BIT1
#define SD_CS_BIT BIT2

// device selects
#define GYRO_SELECT() GYRO_CS_PORT &= ~GYRO_CS_BIT
#define RF_SELECT() RF_CS_PORT |= RF_CS_BIT
#define SD_SELECT() SD_CS_PORT |= SD_CS_BIT

// device deselects
#define GYRO_DESELECT() GYRO_CS_PORT |= GYRO_CS_BIT
#define RF_DESELECT() RF_CS_PORT &= ~RF_CS_BIT
#define SD_DESELECT() SD_CS_PORT &= ~SD_CS_BIT

// SPI Speed Divisors base on the SMCLK
#define SPI_100kHz	120 // 12MHz / 100kHz
#define SPI_250kHz	48
#define SPI_300kHz	40
#define SPI_400kHz	30 // 12MHz / 400kHz
#define SPI_500kHz	24
#define SPI_1MHz	12
#define SPI_1.5MHz	8
#define SPI_3MHz	4
#define SPI_4MHz	3
#define SPI_6MHz	2
#define SPI_12MHz	1 // 12MHz / 12MHz

#define RF_SPI_1MHz 1 // DCO at 1MHz/1MHz (this is the special component that will run with SMCLK at 1MHz)

#define GYRO_SPI_DIVISOR SPI_1MHz
#define SD_SPI_DIVISOR SPI_250kHz
#define RF_SPI_DIVISOR RF_SPI_1MHz
#define SPI_DEF_SPEED SPI_4MHz // for sd card after initialization

// function declarations
void spiInit(const unsigned char device);
unsigned char readByteSPI(const unsigned char address);
void sendByteWithAddressSPI(const unsigned char address, const unsigned char data);
void sendByteSPI(const unsigned char data);
void readMultipleBytesSPI(const unsigned char address, const unsigned char n, unsigned char *arr);
void spi_deselect(const unsigned char device);
void spi_select(const unsigned char device);
void spi_set_mode(const unsigned char device);
void spi_set_device_divisor(const unsigned char device);
uint8_t spi_receive(void);
uint8_t spi_send(const uint8_t c);
void spi_set_divisor(const unsigned int clkdiv);


#endif /* SPI_H_ */
