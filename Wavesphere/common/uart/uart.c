/*
 * uart.c
 *
 *  Created on: Apr 26, 2013
 *      Author: Samuel
 */

#include <stdio.h>
#include <msp430fr5969.h>
#include "uart.h"

void sendSensorDataUART(int *arr, char *dataLabel) {
	char string[30];

	sprintf(string, dataLabel);

	sendStringUART(string);

	sprintf(string, "%d, %d, %d", arr[0], arr[1], arr[2]);

	sendStringUART(string);
}

void sendStringUART(char *string) {
	while (*string) {
		sendByteUART(*string);
		string++;
	}
}

void sendWordUART(int data) {
	while (!(UCA1IFG & UCTXIFG));
	UCA1TXBUF = data & 0xFF00;
	while (!(UCA1IFG & UCTXIFG));
	UCA1TXBUF = data & 0x00FF;
}

void sendByteUART(unsigned char byte) {
	while (!(UCA1IFG & UCTXIFG)); // wait for buffer availability (TX)
	UCA1TXBUF = byte;
}


