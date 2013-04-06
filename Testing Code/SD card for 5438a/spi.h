/*
 * spi.h
 *
 *  Created on: Mar 27, 2013
 *      Author: Samuel
 */

#ifndef SPI_H_
#define SPI_H_

#include <msp430f5438a.h>
void spi_initialize();
void spi_set_divisor(unsigned int divisor);
void spi_cs_assert();
void spi_cs_deassert();
void spi_send_byte(unsigned char input);
unsigned char spi_rcv_byte();
void spi_enable();
void spi_disable();


#endif /* SPI_H_ */
