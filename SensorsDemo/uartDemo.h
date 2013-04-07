/*
 * uartDemo.h
 *
 *  Created on: Apr 7, 2013
 *      Author: Adrian
 */

#ifndef UARTDEMO_H_
#define UARTDEMO_H_

void initTestUART();
void sendByteUART(unsigned char byte);
void sendGyroDataUART(unsigned char *arr);
void sendAccDataUART(int *arr);
void sendMagDataUART(unsigned char *arr);
void sendStringUART(char *string);

#endif /* UARTDEMO_H_ */
