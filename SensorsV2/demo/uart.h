/*
 * uartDemo.h
 *
 *  Created on: Apr 7, 2013
 *      Author: Adrian
 */

#ifndef UARTDEMO_H_
#define UARTDEMO_H_

void initUART();
void sendByteUART(unsigned char byte);
void sendSensorDataUART(int *arr, char *dataLabel);
void sendStringUART(char *string);

#endif /* UARTDEMO_H_ */
