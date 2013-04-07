/*
 * uartDemo.c
 *
 *  Created on: Apr 7, 2013
 *      Author: Adrian & Dany & Samuel & Nelian
 */


#include <msp430fr5969.h>
#include <stdio.h>
void initTestUART() {
	P2SEL1 |= BIT5+BIT6; // UART
	//UCA1 is UART
	UCA1CTLW0 |= UCSWRST;                      // **Put state machine in reset**
	UCA1CTLW0 |= UCSSEL__SMCLK;                     // SMCLK


	UCA1BR0 = 109;                            // 1MHz 9600 (see User's Guide)
	UCA1BR1 = 0;                              // 1MHz 9600
	UCA1MCTLW |= 0x0400 + UCBRF_0; // Modulation UCBRSx=2, UCBRFx=0 *MIGHT BLOW UP*
	UCA1CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**

}

void sendByteUART(unsigned char byte) {
	while (!(UCA1IFG & UCTXIFG)); // wait for buffer availability (TX)
	UCA1TXBUF = byte;
}

void sendStringUART(char *string) {
	while (*string) {
		sendByteUART(*string);
		string++;
	}
}

void sendGyroDataUART(unsigned char *arr) {
	unsigned int i;
	char axis[3] = { 'x', 'y', 'z' };
	char string[30];
	sprintf(string, "Gyroscope\n");
	sendStringUART(string);
	for (i = 0; i < 3; i++) {
		sprintf(string, "%c: %d%c\n", axis[i],
				arr[i * 2] + (arr[i * 2 + 1] << 8), (i == 2) ? '\n' : ' ');
		sendStringUART(string);
	}
}

void sendAccDataUART(int *arr) {
	unsigned int i;
	char axis[3] = { 'x', 'y', 'z' };
	char string[30];
	sprintf(string, "Accelerometer\n");
	sendStringUART(string);
	for (i = 0; i < 3; i++) {
		sprintf(string, "%c: %d%c\n", axis[i], arr[i], (i == 2) ? '\n' : ' ');
		sendStringUART(string);
	}
}

void sendMagDataUART(unsigned char *arr) {
	unsigned int i;
	char axis[3] = { 'x', 'y', 'z' };
	char string[30];
	sprintf(string, "Magnetometer\n");
	sendStringUART(string);
	for (i = 0; i < 3; i++) {
		sprintf(string, "%c: %d%c\n", axis[i], arr[i], (i == 2) ? '\n' : ' ');
		sendStringUART(string);
	}
}
