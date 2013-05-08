/*
 * uart.h
 *
 *  Created on: Apr 26, 2013
 *      Author: Samuel
 */

#ifndef UART_H_
#define UART_H_

void sendSensorDataUART(int *arr, char *dataLabel);
void sendByteUART(unsigned char byte);
void sendStringUART(char *string);
void sendWordUART(int data);


#endif /* UART_H_ */
