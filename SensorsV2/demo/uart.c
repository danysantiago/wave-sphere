/*
 * uartDemo.c
 *
 *  Created on: Apr 7, 2013
 *      Author: Adrian & Dany & Samuel & Nelian
 */


#include <msp430fr5969.h>
#include <stdio.h>
void initUART() {
	P2SEL1 |= BIT5+BIT6; // UART
	//UCA1 is UART
	UCA1CTLW0 |= UCSWRST;                      // **Put state machine in reset**
	UCA1CTLW0 |= UCSSEL_2;                    // SMCLK

	//UBR00=0x66; UBR10=0x00; UMCTL0=0xB5; /* uart0 985000Hz 9600bps */
	//UBR01=0x66; UBR11=0x00; UMCTL1=0xB5; /* uart1 985000Hz 9600bps */
	UCA1BRW = 0x0066;                            // 1MHz 9600 (see User's Guide)
	//UCA1BR1 = 0;                              // 1MHz 9600
	UCA1MCTLW |= 0xB500;

	// Modulation UCBRSx=2, UCBRFx=0 *MIGHT BLOW UP*
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

void sendSensorDataUART(int *arr, char *dataLabel) {
	char string[30];

	sprintf(string, dataLabel);

	sendStringUART(string);

	sprintf(string, "%d, %d, %d", arr[0], arr[1], arr[2]);

	sendStringUART(string);
}

void endDataTransmission() {
	sendStringUART("\0");
}
/*
void sendAccDataUART(int *arr) {
	unsigned int i;
	char axis[3] = { 'x', 'y', 'z' };
	char string[30];
	sprintf(string, "Accelerometer\n");
	sendStringUART(string);
	for (i = 0; i < 3; i++) {
		sprintf(string, "%c: 0x%x %c\n", axis[i], arr[i], (i == 2) ? '\n' : ' ');
		sendStringUART(string);
	}
}

void sendMagDataUART(unsigned char *arr) {
	unsigned int i;
	char axis[3] = { 'x', 'z', 'y' };
	char string[30];
	sprintf(string, "Magnetometer\n");
	sendStringUART(string);
	for (i = 0; i < 3; i++) {
		sprintf(string, "%c: 0x%x %c\n", axis[i],
				(arr[i * 2 + 1] + (arr[i * 2] << 8)), (i == 2) ? '\n' : ' ');
		sendStringUART(string);
	}
}
*/
